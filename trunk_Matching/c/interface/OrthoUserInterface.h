/**************************************************************************
                                     OrthoUserInterface.h
**************************************************************************/


#ifndef ORTHOUSERINTERFACE_H
#define ORTHOUSERINTERFACE_H

#include "OrthoManager.h"
#include "PositionMatrix.h"

class OrthoUserInterface //: public EObject
{

protected:

	// Associated Objects
	//
        OrthoManager* manager;

public:

	// Association Methods
	//
        OrthoManager* getManager();

	// Other Methods
        //
	virtual bool exec() = 0;

};

#endif //ORTHOUSERINTERFACE_H
