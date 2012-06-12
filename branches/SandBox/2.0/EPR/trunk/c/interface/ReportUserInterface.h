/**************************************************************************
		ReportUserInterface.h
**************************************************************************/


#ifndef REPORTUSERINTERFACE_H
#define REPORTUSERINTERFACE_H

#include "ReportManager.h"
#include "PositionMatrix.h"

//#include "EObject.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {


class ReportUserInterface //: public EObject
{

protected:

	// Associated Objects
	//
	ReportManager* manager;

public:

	// AssociatReportn Methods
	//
	ReportManager* getManager();

	// Other Methods
	//
	virtual bool exec() = 0;

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //REPORTUSERINTERFACE_H
