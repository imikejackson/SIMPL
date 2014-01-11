#ifndef _EnsembleArrayComparisonSelectionWidget_H_
#define _EnsembleArrayComparisonSelectionWidget_H_




#include <QtCore/QObject>
#include <QtGui/QWidget>


#include "DREAM3DLib/Common/AbstractFilter.h"

#include "FilterWidgetsLib/QFilterParameterWidget.h"
#include "FilterWidgetsLib/FilterWidgetsLib.h"

#include "FilterWidgetsLib/ui_EnsembleArrayComparisonSelectionWidget.h"


/**
* @brief
* @author
* @version
*/
class FilterWidgetsLib_EXPORT EnsembleArrayComparisonSelectionWidget : public QWidget, private Ui::EnsembleArrayComparisonSelectionWidget
{
  Q_OBJECT

  public:
    /**
    * @brief Constructor
    * @param parameter The FilterParameter object that this widget represents
    * @param filter The instance of the filter that this parameter is a part of
    * @param parent The parent QWidget for this Widget
    */
    EnsembleArrayComparisonSelectionWidget(FilterParameter* parameter, AbstractFilter* filter = NULL, QWidget* parent = NULL);
    
    virtual ~EnsembleArrayComparisonSelectionWidget();
    
    /**
    * @brief This method does additional GUI widget connections
    */
    void setupGui();

  public slots:


  private:
    AbstractFilter*   m_Filter;
    FilterParameter*  m_FilterParameter;

    EnsembleArrayComparisonSelectionWidget(const EnsembleArrayComparisonSelectionWidget&); // Copy Constructor Not Implemented
    void operator=(const EnsembleArrayComparisonSelectionWidget&); // Operator '=' Not Implemented

};

#endif /* _EnsembleArrayComparisonSelectionWidget_H_ */

