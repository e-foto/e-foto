/**************************************************************************
														 DEMUserInterface.h
**************************************************************************/


#ifndef DEMUSERINTERFACE_H
#define DEMUSERINTERFACE_H

#include "DEMManager.h"
#include "PositionMatrix.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

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

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //DEMUSERINTERFACE_H
