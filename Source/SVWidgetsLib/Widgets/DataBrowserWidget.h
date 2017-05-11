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

#ifndef _dataBrowserWidget_h_
#define _dataBrowserWidget_h_

#include <QtCore/QUuid>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/AbstractFilter.h"
#include "SIMPLib/Common/IObserver.h"
#include "SIMPLib/Common/PipelineMessage.h"
#include "SIMPLib/DataContainers/DataContainerArray.h"

#include "SVWidgetsLib/SVWidgetsLib.h"

#include "ui_DataBrowserWidget.h"

class PipelineFilterObject;
class QStandardItem;

namespace Ui
{
class DataBrowserWidget;
}

class SVWidgetsLib_EXPORT DataBrowserWidget : public QWidget, public IObserver
{
  Q_OBJECT
public:
  DataBrowserWidget(QWidget* parent = nullptr);
  virtual ~DataBrowserWidget();

public slots:

  void updateDataContainerArray(DataContainerArray::Pointer dca);

  void filterObjectActivated(PipelineFilterObject* object);

  void handleFilterParameterChanged(PipelineFilterObject* obj);

  void dataBrowserTreeView_indexChanged(const QModelIndex& current, const QModelIndex& previous);

  void refreshData();

  void handleFilterRemoved(PipelineFilterObject* object);

protected:
  void setupGui();

  QStandardItem* findChildByName(QStandardItem* rootItem, const QString& name, int column);

  void removeNonexistingEntries(QStandardItem* rootItem, QList<QString> existing, int column);

private:
  DataContainerArray::Pointer  m_Dca = nullptr;
  Ui::DataBrowserWidget*       m_Ui = nullptr;

  DataBrowserWidget(const DataBrowserWidget&); // Copy Constructor Not Implemented
  void operator=(const DataBrowserWidget&);    // Operator '=' Not Implemented
};

#endif /* _dataBrowserWidget_h_   */
