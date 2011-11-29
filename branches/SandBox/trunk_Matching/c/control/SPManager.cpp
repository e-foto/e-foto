/**************************************************************************
                                                           SPManager.cpp
**************************************************************************/

#include "SPManager.h"
#include "EFotoManager.h"
#include "SPUserInterface_Qt.h"


// Constructors and destructors
//

SPManager::SPManager()
{
    started = false;
    status = false;
}

SPManager::SPManager(EFotoManager* manager, deque<Image*>images, deque<ExteriorOrientation*> eos)
{
    this->manager = manager;
    started = false;
    status = false;
    listAllImages = images;
    listEOs = eos;
    //grid = NULL;
    //setListPoint();
}

SPManager::~SPManager()
{
    //if (grid != NULL)
        //delete grid;
}

// Association Methods
//

void SPManager::setInterface(SPUserInterface* newInterface)
{
    myInterface = newInterface;
}

SPUserInterface* SPManager::getInterface()
{
    return myInterface;
}

// Other Methods
//

bool SPManager::exec()
{
    if (manager != NULL)
    {
        if (manager->getInterfaceType().compare("Qt") == 0)
        {
            myInterface = SPUserInterface_Qt::instance(this);
        }
        //connectImagePoints();
        started = true;
        if (myInterface != NULL)
        {
            myInterface->exec();
            //getPairs();
            //createInitialSeeds();
        }
    }
    return status;
}

void SPManager::returnProject()
{
    manager->reloadProject();
}
