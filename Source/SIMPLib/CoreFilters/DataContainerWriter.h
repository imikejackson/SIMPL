/* ============================================================================
* Copyright (c) 2009-2016 BlueQuartz Software, LLC
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
* Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
* contributors may be used to endorse or promote products derived from this software
* without specific prior written permission.
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
*
* The code contained herein was partially funded by the followig contracts:
*    United States Air Force Prime Contract FA8650-07-D-5800
*    United States Air Force Prime Contract FA8650-10-D-5210
*    United States Prime Contract Navy N00173-07-C-2068
*
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


#pragma once

#include <memory>

#include <hdf5.h>

#include <QtCore/QTextStream>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Filtering/AbstractFilter.h"

/**
 * @brief The DataContainerWriter class. See [Filter documentation](@ref datacontainerwriter) for details.
 */
class SIMPLib_EXPORT DataContainerWriter : public AbstractFilter
{
    Q_OBJECT
    
    // This line MUST be first when exposing a class and properties to Python

    // Start Python bindings declarations
    PYB11_BEGIN_BINDINGS(DataContainerWriter SUPERCLASS AbstractFilter)
    PYB11_FILTER()
    PYB11_SHARED_POINTERS(DataContainerWriter)
    PYB11_FILTER_NEW_MACRO(DataContainerWriter)
    PYB11_PROPERTY(QString OutputFile READ getOutputFile WRITE setOutputFile)
    PYB11_PROPERTY(bool WriteXdmfFile READ getWriteXdmfFile WRITE setWriteXdmfFile)
    PYB11_PROPERTY(bool WriteTimeSeries READ getWriteTimeSeries WRITE setWriteTimeSeries)
    PYB11_END_BINDINGS()
    // End Python bindings declarations

  public:
    using Self = DataContainerWriter;
    using Pointer = std::shared_ptr<Self>;
    using ConstPointer = std::shared_ptr<const Self>;
    using WeakPointer = std::weak_ptr<Self>;
    using ConstWeakPointer = std::weak_ptr<const Self>;
    static Pointer NullPointer();

    static std::shared_ptr<DataContainerWriter> New();

    /**
     * @brief Returns the name of the class for DataContainerWriter
     */
    QString getNameOfClass() const override;
    /**
     * @brief Returns the name of the class for DataContainerWriter
     */
    static QString ClassName();

    ~DataContainerWriter() override;

    /**
     * @brief Setter property for OutputFile
     */
    void setOutputFile(const QString& value);
    /**
     * @brief Getter property for OutputFile
     * @return Value of OutputFile
     */
    QString getOutputFile() const;

    Q_PROPERTY(QString OutputFile READ getOutputFile WRITE setOutputFile)

    /**
     * @brief Setter property for WritePipeline
     */
    void setWritePipeline(bool value);
    /**
     * @brief Getter property for WritePipeline
     * @return Value of WritePipeline
     */
    bool getWritePipeline() const;

    /**
     * @brief Setter property for WriteXdmfFile
     */
    void setWriteXdmfFile(bool value);
    /**
     * @brief Getter property for WriteXdmfFile
     * @return Value of WriteXdmfFile
     */
    bool getWriteXdmfFile() const;

    Q_PROPERTY(bool WriteXdmfFile READ getWriteXdmfFile WRITE setWriteXdmfFile)

    /**
     * @brief Setter property for WriteTimeSeries
     */
    void setWriteTimeSeries(bool value);
    /**
     * @brief Getter property for WriteTimeSeries
     * @return Value of WriteTimeSeries
     */
    bool getWriteTimeSeries() const;

    Q_PROPERTY(bool WriteTimeSeries READ getWriteTimeSeries WRITE setWriteTimeSeries)

    /**
     * @brief Setter property for AppendToExisting
     */
    void setAppendToExisting(bool value);
    /**
     * @brief Getter property for AppendToExisting
     * @return Value of AppendToExisting
     */
    bool getAppendToExisting() const;

    /**
     * @brief getCompiledLibraryName Reimplemented from @see AbstractFilter class
     */
    QString getCompiledLibraryName() const override;

    /**
     * @brief getBrandingString Returns the branding string for the filter, which is a tag
     * used to denote the filter's association with specific plugins
     * @return Branding string
     */
    QString getBrandingString() const override;

    /**
     * @brief getFilterVersion Returns a version string for this filter. Default
     * value is an empty string.
     * @return
     */
    QString getFilterVersion() const override;

    /**
     * @brief newFilterInstance Reimplemented from @see AbstractFilter class
     */
    AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

    /**
     * @brief getGroupName Reimplemented from @see AbstractFilter class
     */
    QString getGroupName() const override;

    /**
     * @brief getSubGroupName Reimplemented from @see AbstractFilter class
     */
    QString getSubGroupName() const override;

    /**
     * @brief getUuid Return the unique identifier for this filter.
     * @return A QUuid object.
     */
    QUuid getUuid() const override;

    /**
     * @brief getHumanLabel Reimplemented from @see AbstractFilter class
     */
    QString getHumanLabel() const override;

    /**
     * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
     */
    void setupFilterParameters() override;

    /**
     * @brief readFilterParameters Reimplemented from @see AbstractFilter class
     */
    void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

    /**
     * @brief execute Reimplemented from @see AbstractFilter class
     */
    void execute() override;


  protected:
    DataContainerWriter();
    /**
     * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
     */
    void dataCheck() override;

    /**
     * @brief Initializes all the private instance variables.
     */
    void initialize();


    /**
     * @brief openFile Opens or creates an HDF5 file to write data into
     * @param append Should a new file be created or append data to a currently existing file
     * @return
     */
    hid_t openFile(bool append = false);

    /**
     * @brief closeFile Closes the currently open file
     * @return Integer error value
     */
    herr_t closeFile();

    /**
     * @brief writePipeline Writes the existing pipeline to the HDF5 file
     * @return
     */
    int writePipeline();

    /**
     * @brief writeDataContainerBundles Writes any existing DataContainerBundles to the HDF5 file
     * @param fileId Group Id for the DataContainerBundles
     * @return
     */
    int writeDataContainerBundles(hid_t fileId);

    /**
     * @brief writeMontages Writes any existing Montages to the HDF5 file
     * @param fileId Group Id for the Montages
     * @return
     */
    int writeMontages(hid_t fileId);

    /**
     * @brief writeXdmfHeader Writes the Xdmf header
     * @param out QTextStream for output
     */
    void writeXdmfHeader(QTextStream& out);

    /**
     * @brief writeXdmfFooter Writes the Xdmf footer
     * @param out QTextStream for output
     */
    void writeXdmfFooter(QTextStream& out);

  private:
    QString m_OutputFile = {};
    bool m_WritePipeline = {};
    bool m_WriteXdmfFile = {};
    bool m_WriteTimeSeries = {};
    bool m_AppendToExisting = {};

    hid_t m_FileId;

  public:
    DataContainerWriter(const DataContainerWriter&) = delete; // Copy Constructor Not Implemented
    DataContainerWriter(DataContainerWriter&&) = delete;      // Move Constructor Not Implemented
    DataContainerWriter& operator=(const DataContainerWriter&) = delete; // Copy Assignment Not Implemented
    DataContainerWriter& operator=(DataContainerWriter&&) = delete;      // Move Assignment Not Implemented
};

