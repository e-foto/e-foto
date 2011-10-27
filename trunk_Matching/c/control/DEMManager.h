/**************************************************************************
                                                           DEMManager.h
**************************************************************************/


#ifndef DEMMANAGER_H
#define DEMMANAGER_H

#include "EObject.h"

class DEMUserInterface;
class EFotoManager;

class DEMManager
{
	// Private Attributes
	//
	bool started;
	bool status;
        DEMUserInterface* myInterface;
        EFotoManager* manager;

public:

	// Constructors and Destructors
	//
        DEMManager();
        DEMManager(EFotoManager* manager);
        ~DEMManager();

	// Association Methods
	//
        void setInterface(DEMUserInterface* newInterface);
        DEMUserInterface* getInterface();

	// Other Methods
	//
        bool exec();

};

#endif //DEMMANAGER_H
