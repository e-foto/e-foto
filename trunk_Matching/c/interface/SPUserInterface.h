/**************************************************************************
                                                         SPUserInterface.h
**************************************************************************/


#ifndef SPUSERINTERFACE_H
#define SPUSERINTERFACE_H

#include "SPManager.h"
#include "PositionMatrix.h"

class SPUserInterface //: public EObject
{

protected:

	// Associated Objects
	//
        SPManager* manager;

public:

	// Association Methods
	//
        SPManager* getManager();

	// Other Methods
        //
	virtual bool exec() = 0;

};

#endif //SPUSERINTERFACE_H
