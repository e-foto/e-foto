/**************************************************************************
                                                           DEMManager.cpp
**************************************************************************/

#include "DEMManager.h"
#include "EFotoManager.h"
#include "DEMUserInterface.h"
#include "DEMUserInterface_Qt.h"

// Constructors and destructors
//

DEMManager::DEMManager()
{
    started = false;
    status = false;
}

DEMManager::DEMManager(EFotoManager* manager)
{
        this->manager = manager;
	started = false;
	status = false;
}

DEMManager::~DEMManager()
{

}

// Association Methods
//

void DEMManager::setInterface(DEMUserInterface* newInterface)
{
	myInterface = newInterface;
}

DEMUserInterface* DEMManager::getInterface()
{
	return myInterface;
}

// Other Methods
//

bool DEMManager::exec()
{
        if (manager != NULL)
        {
		if (manager->getInterfaceType().compare("Qt") == 0)
		{
			//myInterface = new IOUserInterface_Qt(this);
                        myInterface = DEMUserInterface_Qt::instance(this);
                }
		started = true;
		if (myInterface != NULL)
		{
			myInterface->exec();
                }
	}
	return status;
}

