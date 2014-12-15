/* ============================================================================
 * Copyright (c) 2011, Michael A. Jackson (BlueQuartz Software)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of Michael A. Jackson nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "VtkStructuredPointsReader.h"

#include <QtCore/QMap>
#include <QtCore/QFileInfo>

#include "DREAM3DLib/Common/Constants.h"
#include "DREAM3DLib/Utilities/DREAM3DEndian.h"

#include "IO/IOConstants.h"


#define kBufferSize 1024

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VtkStructuredPointsReader::VtkStructuredPointsReader() :
  AbstractFilter(),
  m_VolumeDataContainerName(DREAM3D::Defaults::VolumeDataContainerName),
  m_VertexDataContainerName(DREAM3D::Defaults::VertexDataContainerName),
  m_CellAttributeMatrixName(DREAM3D::Defaults::CellAttributeMatrixName),
  m_VertexAttributeMatrixName(DREAM3D::Defaults::VertexAttributeMatrixName),
  m_InputFile(""),
  m_Comment(""),
  m_DatasetType(""),
  m_ReadCellData(true),
  m_ReadPointData(true),
  m_FileIsBinary(true)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VtkStructuredPointsReader::~VtkStructuredPointsReader()
{
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VtkStructuredPointsReader::setupFilterParameters()
{
  FilterParameterVector parameters;
  parameters.push_back(FileSystemFilterParameter::New("Input Vtk File", "InputFile", FilterParameterWidgetType::InputFileWidget, getInputFile(), false));
  QStringList linkedProps;
  linkedProps << "VertexDataContainerName" << "VertexAttributeMatrixName";
  parameters.push_back(LinkedBooleanFilterParameter::New("Read Point Data", "ReadPointData", getReadPointData(), linkedProps, false));
  linkedProps.clear();
  linkedProps << "VolumeDataContainerName" << "CellAttributeMatrixName";
  parameters.push_back(LinkedBooleanFilterParameter::New("Read Cell Data", "ReadCellData", getReadCellData(), linkedProps, false));
  parameters.push_back(FilterParameter::New("Created Information", "", FilterParameterWidgetType::SeparatorWidget, "", true));
  parameters.push_back(FilterParameter::New("Vertex Data Container", "VertexDataContainerName", FilterParameterWidgetType::StringWidget, getVolumeDataContainerName(), true, ""));
  parameters.push_back(FilterParameter::New("Vertex Attribute Matrix", "VertexAttributeMatrixName", FilterParameterWidgetType::StringWidget, getVertexAttributeMatrixName(), true, ""));
  parameters.push_back(FilterParameter::New("Volume Data Container", "VolumeDataContainerName", FilterParameterWidgetType::StringWidget, getVertexDataContainerName(), true, ""));
  parameters.push_back(FilterParameter::New("Cell Attribute Matrix", "CellAttributeMatrixName", FilterParameterWidgetType::StringWidget, getCellAttributeMatrixName(), true, ""));
  setFilterParameters(parameters);
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VtkStructuredPointsReader::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setVolumeDataContainerName(reader->readString("VolumeDataContainerName", getVolumeDataContainerName() ) );
  setVertexDataContainerName(reader->readString("VertexDataContainerName", getVertexDataContainerName() ) );
  setCellAttributeMatrixName(reader->readString("CellAttributeMatrixName", getCellAttributeMatrixName() ) );
  setVertexAttributeMatrixName(reader->readString("VertexAttributeMatrixName", getVertexAttributeMatrixName() ) );
  setInputFile( reader->readString( "InputFile", getInputFile() ) );
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int VtkStructuredPointsReader::writeFilterParameters(AbstractFilterParametersWriter* writer, int index)
{
  writer->openFilterGroup(this, index);
  DREAM3D_FILTER_WRITE_PARAMETER(InputFile)
  DREAM3D_FILTER_WRITE_PARAMETER(VolumeDataContainerName)
  DREAM3D_FILTER_WRITE_PARAMETER(VertexDataContainerName)
  DREAM3D_FILTER_WRITE_PARAMETER(CellAttributeMatrixName)
  DREAM3D_FILTER_WRITE_PARAMETER(VertexAttributeMatrixName)
  writer->closeFilterGroup();
  return ++index; // we want to return the next index that was just written to
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VtkStructuredPointsReader::dataCheck()
{
  DataArrayPath tempPath;

  setErrorCondition(0);
  if(m_ReadPointData == true)
  {
    VertexDataContainer* m = getDataContainerArray()->createNonPrereqDataContainer<VertexDataContainer, VtkStructuredPointsReader>(this, getVertexDataContainerName());
    if(getErrorCondition() < 0 && NULL == m) { return; }

    QVector<size_t> tDims(1, 0);
    AttributeMatrix::Pointer pointAttrMat = m->createNonPrereqAttributeMatrix<AbstractFilter>(this, getVertexAttributeMatrixName(), tDims, DREAM3D::AttributeMatrixType::Vertex);
    if(getErrorCondition() < 0) { return; }  
  }

  if(m_ReadCellData == true)
  {
    VolumeDataContainer* m = getDataContainerArray()->createNonPrereqDataContainer<VolumeDataContainer, VtkStructuredPointsReader>(this, getVolumeDataContainerName());
    if(getErrorCondition() < 0 && NULL == m) { return; }

    QVector<size_t> tDims(3, 0);
    AttributeMatrix::Pointer cellAttrMat = m->createNonPrereqAttributeMatrix<AbstractFilter>(this, getCellAttributeMatrixName(), tDims, DREAM3D::AttributeMatrixType::Cell);
    if(getErrorCondition() < 0) { return; }  
  }

  QFileInfo fi(getInputFile());
  if (getInputFile().isEmpty() == true)
  {
    QString ss = QObject::tr("%1 needs the Input File is empty").arg(ClassName());
    setErrorCondition(-387);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
  }
  else if (fi.exists() == false)
  {
    QString ss = QObject::tr("The input file does not exist.");
    setErrorCondition(-388);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
  }

  if(getErrorCondition() < 0) { return; }

  readFile();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VtkStructuredPointsReader::preflight()
{
  setInPreflight(true);
  emit preflightAboutToExecute();
  emit updateFilterParameters(this);
  dataCheck();
  emit preflightExecuted();
  setInPreflight(false);
}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VtkStructuredPointsReader::execute()
{
  setErrorCondition(0);

  // The dataCheck function will do everything based on boolean "InPreflight". If
  // the filter is preflighting then the filter will just gather all the scalar
  // and vector data set names and number of tuples from the file. If the filter
  // is executing then the file data will be read into the data arrays
  dataCheck();

  notifyStatusMessage(getHumanLabel(), "Complete");
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
size_t VtkStructuredPointsReader::parseByteSize(QString text)
{

  if (text.compare("unsigned_char") == 0) {return 1; }
  if (text.compare("char") == 0) {return 1; }
  if (text.compare("unsigned_short") == 0) {return 2; }
  if (text.compare("short") == 0) {return 2; }
  if (text.compare("unsigned_int") == 0) {return 4; }
  if (text.compare("int") == 0) {return 4; }
  if (text.compare("unsigned_long") == 0) {return 8; }
  if (text.compare("long") == 0) {return 8; }
  if (text.compare("float") == 0) {return 4; }
  if (text.compare("double") == 0) {return 8; }

  return 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
int skipVolume(QFile& inStream, bool binary, size_t totalSize)
{
  int err = 0;
  if(binary == true)
  {
    qint64 pos = inStream.pos();

    qint64 newPos = pos + totalSize * sizeof(T);

    inStream.seek(newPos);

  }
  else
  {
    T tmp;
    QDataStream in(&inStream);
    for (size_t z = 0; z < totalSize; ++z)
    {
      //for (size_t y = 0; y < yDim; ++y)
      {
        //for (size_t x = 0; x < xDim; ++x)
        {
          in >> tmp;
        }
      }
    }
  }
  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
template<typename T>
int readDataChunk(AttributeMatrix::Pointer attrMat, QFile &inStream, bool inPreflight, bool binary, const QString &scalarName, const QString &scalarType, const QString &scalarNumComp)
{
  QVector<size_t> amTDims = attrMat->getTupleDimensions();

  size_t numTuples = attrMat->getNumTuples();
  QVector<size_t> tDims = attrMat->getTupleDimensions();
  bool ok = false;
  int numComp = scalarNumComp.toInt(&ok, 10);
  QVector<size_t> cDims(1, numComp);


  typename DataArray<T>::Pointer data = DataArray<T>::CreateArray(tDims, cDims, scalarName, !inPreflight);
  attrMat->addAttributeArray(data->getName(), data);
  if(inPreflight == true) {
    // qDebug() << "Skipping Scalars " << scalarName;
    return skipVolume<T>(inStream, binary, numTuples*numComp);
  }
  //qDebug() << "Reading Scalars " << scalarName;


  if (binary == true)
  {
    size_t k_BufferSize = 8192; // Read the data in k_BufferSize chunks which should be reasonably optimal for most drives
    T* buffer = data->getPointer(0);
    char* ptr = reinterpret_cast<char*>(buffer);
    // Read all the data in one shot into a buffer
    qint64 totalSize = static_cast<qint64>(numTuples*numComp);
    for(qint64 i = 0; i < totalSize; i=i+k_BufferSize)
    {
      if(totalSize - i < k_BufferSize)
      {
        k_BufferSize = totalSize - i;
      }
      qint64 bytesRead = inStream.read(ptr, k_BufferSize);
      if(bytesRead != k_BufferSize)
      {
        qDebug() << "Reading Dataset: ERROR READING BINARY FILE. Bytes read was not the same as func->xDim *. " << bytesRead << " vs "
                 << k_BufferSize ;
        return -4002;
      }
      buffer = buffer + k_BufferSize;
    }

    //Vtk Binary files are written with BIG Endine byte order. If we are on a little
    // endian machine (Intel x86 or Intel x86_64 then we need to byte swap the elements in the array
    if(BIGENDIAN == 0) {data->byteSwapElements(); }
  }
  else // ASCII VTK File
  {
    T value = static_cast<T>(0.0);
    QDataStream in(&inStream);
    size_t totalSize = numTuples*numComp;
    for (size_t i = 0; i < totalSize; ++i)
    {
      in >> value;
      data->setValue(i, value);
    }
  }
  return 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int VtkStructuredPointsReader::readHeader()
{
  return 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int VtkStructuredPointsReader::readFile()
{
  int err = 0;

  QFile instream(getInputFile());

  if (!instream.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QString msg = QObject::tr("Vtk file could not be opened: %1").arg(getInputFile());
    setErrorCondition(-100);
    notifyErrorMessage(getHumanLabel(), "", getErrorCondition());
    return -100;
  }
  QByteArray buf = instream.readLine().trimmed(); // Read Line 1 - VTK Version Info
  buf = instream.readLine(); // Read Line 2 - User Comment
  setComment(QString(buf));
  buf = instream.readLine(); // Read Line 3 - BINARY or ASCII
  QString fileType(buf);
  if (fileType.startsWith("BINARY") == true)
  {
    setFileIsBinary(true);
  }
  else if (fileType.startsWith("ASCII") == true)
  {
    setFileIsBinary(false);
  }
  else
  {

    QString ss = QObject::tr("The file type of the VTK legacy file could not be determined. It should be ASCII' or 'BINARY' and should appear on line 3 of the file.");
    setErrorCondition(-51030);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return getErrorCondition();
  }

  // Read Line 4 - Type of Dataset
  buf = instream.readLine().trimmed();
  QList<QByteArray> words = buf.split(' ');
  if(words.size() != 2)
  {
    QString ss = QObject::tr("Error Reading the type of data set. Was expecting 2 words but got %1").arg(QString(buf));
    setErrorCondition(-51040);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return getErrorCondition();
  }
  QString dataset(words.at(1));
  dataset = dataset.trimmed();
  setDatasetType(dataset);


  bool ok = false;
  buf = instream.readLine().trimmed(); // Read Line 5 which is the Dimension values
  // But we need the 'extents' which is one less in all directions (unless dim=1)
  size_t dims[3];
  QList<QByteArray> tokens = buf.split(' ');
  dims[0] = tokens[1].toInt(&ok, 10);
  dims[1] = tokens[2].toInt(&ok, 10);
  dims[2] = tokens[3].toInt(&ok, 10);
  if(m_ReadCellData == true)
  {
    getDataContainerArray()->getDataContainerAs<VolumeDataContainer>(getVolumeDataContainerName())->setDimensions(dims);
    QVector<size_t> tDims(3, 0);
    tDims[0] = dims[0]-1;
    tDims[1] = dims[1]-1;
    tDims[2] = dims[2]-1;
    getDataContainerArray()->getDataContainer(getVolumeDataContainerName())->getAttributeMatrix(getCellAttributeMatrixName())->resizeAttributeArrays(tDims);
  }

  buf = instream.readLine().trimmed();// Read Line 7 which is the Scaling values
  tokens = buf.split(' ');
  float resolution[3];
  resolution[0] = tokens[1].toFloat(&ok);
  resolution[1] = tokens[2].toFloat(&ok);
  resolution[2] = tokens[3].toFloat(&ok);
  if(m_ReadCellData == true) getDataContainerArray()->getDataContainerAs<VolumeDataContainer>(getVolumeDataContainerName())->setResolution(resolution);

  buf = instream.readLine().trimmed(); // Read Line 6 which is the Origin values
  tokens = buf.split(' ');
  float origin[3];
  origin[0] = tokens[1].toFloat(&ok);
  origin[1] = tokens[2].toFloat(&ok);
  origin[2] = tokens[3].toFloat(&ok);
  if(m_ReadCellData == true) getDataContainerArray()->getDataContainerAs<VolumeDataContainer>(getVolumeDataContainerName())->setOrigin(origin);
  if(m_ReadPointData = true)
  {
    //create point and cell attribute matrices
    QVector<size_t> tDims(1, dims[0]*dims[1]*dims[2]);
    getDataContainerArray()->getDataContainer(getVertexDataContainerName())->getAttributeMatrix(getVertexAttributeMatrixName())->resizeAttributeArrays(tDims);
  }

  // Now parse through the file. If we are preflighting then we only construct the DataArrays with no allocation
  readData(instream);

  instream.close();
  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int VtkStructuredPointsReader::parseCoordinateLine(const char* input, size_t& value)
{
  char text[256];
  char text1[256];
  int i = 0;
  int n = sscanf(input, "%s %d %s", text, &i, text1);
  if (n != 3)
  {
    value = -1;
    return -1;
  }
  value = i;
  return 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VtkStructuredPointsReader::readData(QFile &instream)
{

  QByteArray buf ;
  QList<QByteArray> tokens;
  // QList<QByteArray> words;
  int err = 0;

  AttributeMatrix::Pointer attrMat;

  while(instream.atEnd() == false)
  {
    qint64 filePos = instream.pos();
    buf = instream.readLine().trimmed();
    // If we read an empty line, then we should drop into this while loop and start reading lines until
    // we find a line with something on it.
    while(buf.isEmpty() == true && instream.atEnd() == false)
    {
      buf = instream.readLine().trimmed();
    }
    // Check to make sure we didn't read to the end of the file
    if(instream.atEnd() == true)
    {
      return;
    }
    tokens = buf.split(' ');

    bool readNewSection = false;
    QString cellDataStr(tokens.at(0));
    if (cellDataStr.compare("POINT_DATA") == 0)
    {
      attrMat = getDataContainerArray()->getDataContainer(getVertexDataContainerName())->getAttributeMatrix(getVertexAttributeMatrixName());
      readNewSection = true;
    }
    else if (cellDataStr.compare("CELL_DATA") == 0)
    {
      attrMat = getDataContainerArray()->getDataContainer(getVolumeDataContainerName())->getAttributeMatrix(getCellAttributeMatrixName());
      readNewSection = true;
    }

    if(readNewSection == true)
    {
      // Read the SCALARS/VECTORS line which should be 3 or 4 words
      buf = instream.readLine().trimmed();
      // If we read an empty line, then we should drop into this while loop and start reading lines until
      // we find a line with something on it.
      while(buf.isEmpty() == true && instream.atEnd() == false)
      {
        buf = instream.readLine().trimmed();
      }
      // Check to make sure we didn't read to the end of the file
      if(instream.atEnd() == true)
      {
        return;
      }
      tokens = buf.split(' ');

      if (tokens.size() < 3 || tokens.size() > 4)
      {
        QString ss = QObject::tr("Error reading SCALARS header section of VTK file. 3 or 4 words are needed. Found %1. Read Line was\n  %2").arg(tokens.size()).arg(QString(buf));
        setErrorCondition(-1002);
        notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
        return;
      }

      QString scalarNumComps("1");
      QString scalarKeyWord = tokens[0];
      if(scalarKeyWord.compare("SCALARS") == 0)
      {
        buf = instream.readLine().trimmed(); // Read the LOOKUP_TABLE line
      }
      else if (scalarKeyWord.compare("VECTORS") == 0)
      {
        scalarNumComps = QString("3");
      }
      else {
        QString ss = QObject::tr("Error reading Dataset section. Unknown Keyword found. %1").arg(scalarKeyWord);
        setErrorCondition(-1003);
        notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
        return;
      }
      QString scalarName = tokens[1];
      scalarName = scalarName.replace("%20", " "); // Replace URL style encoding of string names. %20 is a Space.
      QString scalarType = tokens[2];

      if(tokens.size() == 4) {
        scalarNumComps = tokens[3];
      }

      if (scalarType.compare("unsigned_char") == 0) {
        err = readDataChunk<uint8_t>(attrMat, instream, getInPreflight(), getFileIsBinary(), scalarName, scalarType, scalarNumComps);
      }
      else if (scalarType.compare("char") == 0) {
        err = readDataChunk<int8_t>(attrMat, instream, getInPreflight(), getFileIsBinary(), scalarName, scalarType, scalarNumComps);
      }
      else if (scalarType.compare("unsigned_short") == 0) {
        err = readDataChunk<uint16_t>(attrMat, instream, getInPreflight(), getFileIsBinary(), scalarName, scalarType, scalarNumComps);
      }
      else if (scalarType.compare("short") == 0) {
        err = readDataChunk<int16_t>(attrMat, instream, getInPreflight(), getFileIsBinary(), scalarName, scalarType, scalarNumComps);
      }
      else if (scalarType.compare("unsigned_int") == 0) {
        err = readDataChunk<uint32_t>(attrMat, instream, getInPreflight(), getFileIsBinary(), scalarName, scalarType, scalarNumComps);
      }
      else if (scalarType.compare("int") == 0) {
        err = readDataChunk<int32_t>(attrMat, instream, getInPreflight(), getFileIsBinary(), scalarName, scalarType, scalarNumComps);
      }
      else if (scalarType.compare("unsigned_long") == 0) {
        err = readDataChunk<qint64>(attrMat, instream, getInPreflight(), getFileIsBinary(), scalarName, scalarType, scalarNumComps);
      }
      else if (scalarType.compare("long") == 0) {
        err = readDataChunk<quint64>(attrMat, instream, getInPreflight(), getFileIsBinary(), scalarName, scalarType, scalarNumComps);
      }
      else if (scalarType.compare("float") == 0) {
        err = readDataChunk<float>(attrMat, instream, getInPreflight(), getFileIsBinary(), scalarName, scalarType, scalarNumComps);
      }
      else if (scalarType.compare("double") == 0) {
        err = readDataChunk<double>(attrMat, instream, getInPreflight(), getFileIsBinary(), scalarName, scalarType, scalarNumComps);
      }

      if(err < 0) {
        QString ss = QObject::tr("Error Reading Dataset from VTK File. Dataset Type %1\n  DataSet Name %2\n  Numerical Type: %3\n  File Pos").arg(scalarKeyWord).arg(scalarKeyWord).arg(scalarType).arg(filePos);
        setErrorCondition(err);
        notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
        return;
      }
    
    }

  }

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer VtkStructuredPointsReader::newFilterInstance(bool copyFilterParameters)
{
  VtkStructuredPointsReader::Pointer filter = VtkStructuredPointsReader::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString VtkStructuredPointsReader::getCompiledLibraryName()
{ return IO::IOBaseName; }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString VtkStructuredPointsReader::getGroupName()
{ return DREAM3D::FilterGroups::IOFilters; }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString VtkStructuredPointsReader::getSubGroupName()
{ return DREAM3D::FilterSubGroups::InputFilters; }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString VtkStructuredPointsReader::getHumanLabel()
{ return "Vtk Structured Points Reader"; }

