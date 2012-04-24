/**************************************************************************
														 SPUserInterface.h
**************************************************************************/


#ifndef SPUSERINTERFACE_H
#define SPUSERINTERFACE_H

#include "SPManager.h"
#include "PositionMatrix.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

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
    virtual void centerImages(ObjectSpaceCoordinate coord, double zoom) = 0;

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //SPUSERINTERFACE_H
