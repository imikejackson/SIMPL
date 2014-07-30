/*
 * Your License or Copyright Information can go here
 */
#if (_MSC_VER)
#define _SCL_SECURE_NO_WARNINGS
#endif

#include "AlignSectionsPhaseCorrelation.h"

#include <QtCore/QString>

#include "ImageProcessing/ImageProcessingConstants.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AlignSectionsPhaseCorrelation::AlignSectionsPhaseCorrelation() :
  AbstractFilter()
/* DO NOT FORGET TO INITIALIZE ALL YOUR DREAM3D Filter Parameters HERE */
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AlignSectionsPhaseCorrelation::~AlignSectionsPhaseCorrelation()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AlignSectionsPhaseCorrelation::setupFilterParameters()
{
  FilterParameterVector parameters;
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AlignSectionsPhaseCorrelation::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  /*
   Place code in here that will read the parameters from a file
   setOutputFile( reader->readValue("OutputFile", getOutputFile() ) );
   */
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int AlignSectionsPhaseCorrelation::writeFilterParameters(AbstractFilterParametersWriter* writer, int index)
{
  writer->openFilterGroup(this, index);
  /* Place code that will write the inputs values into a file. reference the AbstractFilterParametersWriter class for the proper API to use. */
  /*  DREAM3D_FILTER_WRITE_PARAMETER(OutputFile) */
  writer->closeFilterGroup();
  return ++index; // we want to return the next index that was just written to
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AlignSectionsPhaseCorrelation::dataCheck()
{
  setErrorCondition(0);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AlignSectionsPhaseCorrelation::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true); // Set the fact that we are preflighting.
  emit preflightAboutToExecute(); // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  dataCheck(); // Run our DataCheck to make sure everthing is setup correctly
  emit preflightExecuted(); // We are done preflighting this filter
  setInPreflight(false); // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString AlignSectionsPhaseCorrelation::getCompiledLibraryName()
{
  return ImageProcessing::ImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString AlignSectionsPhaseCorrelation::getGroupName()
{
  return "ImageProcessing";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString AlignSectionsPhaseCorrelation::getHumanLabel()
{
  return "AlignSectionsPhaseCorrelation";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString AlignSectionsPhaseCorrelation::getSubGroupName()
{
  return "Misc";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AlignSectionsPhaseCorrelation::execute()
{
  int err = 0;
  // typically run your dataCheck function to make sure you can get that far and all your variables are initialized
  dataCheck();
  // Check to make sure you made it through the data check. Errors would have been reported already so if something
  // happens to fail in the dataCheck() then we simply return
  if(getErrorCondition() < 0) { return; }
  setErrorCondition(0);

  /* Place all your code to execute your filter here. */

  /* If some error occurs this code snippet can report the error up the call chain*/
  if (err < 0)
  {
    QString ss = QObject::tr("Some error message");
    setErrorCondition(-99999999);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  /* Let the GUI know we are done with this filter */
  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer AlignSectionsPhaseCorrelation::newFilterInstance(bool copyFilterParameters)
{
  /*
  * write code to optionally copy the filter parameters from the current filter into the new instance
  */
  AlignSectionsPhaseCorrelation::Pointer filter = AlignSectionsPhaseCorrelation::New();
  if(true == copyFilterParameters)
  {
    /* If the filter uses all the standard Filter Parameter Widgets you can probabaly get
     * away with using this method to copy the filter parameters from the current instance
     * into the new instance
     */
    copyFilterParameterInstanceVariables(filter.get());
    /* If your filter is using a lot of custom FilterParameterWidgets @see ReadH5Ebsd then you
     * may need to copy each filter parameter explicitly plus any other instance variables that
     * are needed into the new instance. Here is some example code from ReadH5Ebsd
     */
    //    DREAM3D_COPY_INSTANCEVAR(OutputFile)
    //    DREAM3D_COPY_INSTANCEVAR(ZStartIndex)
    //    DREAM3D_COPY_INSTANCEVAR(ZEndIndex)
    //    DREAM3D_COPY_INSTANCEVAR(ZResolution)
  }
  return filter;
}

