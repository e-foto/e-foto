/*******************************************************************************
	  CheckingPoint.h
*******************************************************************************/


#ifndef CHECKINGPOINT_H
#define CHECKINGPOINT_H

#include "Point.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
* class CheckingPoint
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 06/05/2009
* @version 1.1 - Rafael Alves de Aguiar & Irving da Silva Badolato.
*/

class CheckingPoint : public Point
{
	//EOBJECT

public:

	// Constructors
	//
	CheckingPoint();
	CheckingPoint(int myId);

	// EObject methods
	//
	string objectType(void);
	string objectAssociations(void);
	bool is(string s);

	// XML methods
	//
	void xmlSetData(string xml);
	string xmlGetData();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // CHECKINGPOINT_H
