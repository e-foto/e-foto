/************************************************************************
	  IOQuality.h
**************************************************************************/

#ifndef IOQUALITY_H
#define IOQUALITY_H

#include "EObject.h"
#include "Matrix.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Sensor;
class InteriorOrientation;

/**
  * class IOQuality
  *
  * @Author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 06/05/2009
  * @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato
  *
  */

class IOQuality : public EObject
{
private:

	//EOBJECT

	// Private attributes
	//
	Matrix V;
	double sigma0Squared;
	Matrix SigmaXa;
	Matrix SigmaLa;

public:

	// Constructors and Destructor
	//
	virtual ~IOQuality();

	// Private attribute accessors
	//
	Matrix getV();
	double getsigma0Squared();
	Matrix getSigmaXa();
	Matrix getSigmaLa();

	// EObject methods
	//
	string objectType(void);
	string objectAssociations(void);
	bool is(string s);

	// XML methods
	//
	void xmlSetData(string xml);
	string xmlGetData();

	// Other method
	//
	void calculate(InteriorOrientation* myIO, Sensor* mySensor);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // IOQUALITY_H
