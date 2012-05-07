/**************************************************************************
									 OrthoUserInterface.h
**************************************************************************/


#ifndef ORTHOUSERINTERFACE_H
#define ORTHOUSERINTERFACE_H

#include "OrthoManager.h"
#include "PositionMatrix.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

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

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //ORTHOUSERINTERFACE_H
