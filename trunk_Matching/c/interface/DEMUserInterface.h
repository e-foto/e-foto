/**************************************************************************
                                                         DEMUserInterface.h
**************************************************************************/


#ifndef DEMUSERINTERFACE_H
#define DEMUSERINTERFACE_H

#include "DEMManager.h"
#include "PositionMatrix.h"

class DEMUserInterface //: public EObject
{

protected:

	// Associated Objects
	//
        DEMManager* manager;

public:

	// Association Methods
	//
        DEMManager* getManager();

	// Other Methods
        //
	virtual bool exec() = 0;

};

#endif //DEMUSERINTERFACE_H
