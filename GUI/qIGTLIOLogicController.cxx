#include "qIGTLIOLogicController.h"
#include <QTimer>
#include <vtkIGTLIOLogic.h>

qIGTLIOLogicController::qIGTLIOLogicController()
{
  ImportDataAndEventsTimer = new QTimer(this);
  ImportDataAndEventsTimer->setInterval(5);
  connect(ImportDataAndEventsTimer, SIGNAL(timeout()),
          this, SLOT(importDataAndEvents()));
}

void qIGTLIOLogicController::setLogic(vtkIGTLIOLogicPointer logic)
{  
  foreach(int evendId, QList<int>()
          << vtkIGTLIOLogic::ConnectionAddedEvent
          << vtkIGTLIOLogic::ConnectionAboutToBeRemovedEvent)
    {
    qvtkReconnect(this->Logic, logic, evendId,
                  this, SLOT(onConnectionsChanged()));
    }

  this->Logic = logic;

  this->onConnectionsChanged();
}

void qIGTLIOLogicController::onConnectionsChanged()
{
  if (Logic->GetNumberOfConnectors() > 0)
    {
      if (!ImportDataAndEventsTimer->isActive())
        ImportDataAndEventsTimer->start();
    }
  else
    {
      ImportDataAndEventsTimer->stop();
    }
}

//-----------------------------------------------------------------------------
void qIGTLIOLogicController::importDataAndEvents()
{
  if (Logic)
    {
    Logic->PeriodicProcess();
    }
}
