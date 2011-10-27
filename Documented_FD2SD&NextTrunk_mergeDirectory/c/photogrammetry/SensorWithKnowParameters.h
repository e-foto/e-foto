/**************************************************************************
						SensorWithKnowParameters.h
**************************************************************************/


#ifndef SENSORWITHKNOWPARAMETERS_H
#define SENSORWITHKNOWPARAMETERS_H
#include "FrameSensor.h"
#include "Matrix.h"
#include "AnalogFiductialMark.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
  * class SensorWithKnowParameters
  *
  * @author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 07/02/2011
  * @version 1.4 - Rafael Alves de Aguiar & Irving da Silva Badolato
  */

class SensorWithKnowParameters : public FrameSensor
{

	//EOBJECT

private:

	//Private attributes
	//
	Matrix Xa;
	Matrix SigmaXa;

	//Composed objects
	//

public:

	//Constructors
	//
	SensorWithKnowParameters();
	SensorWithKnowParameters(const Sensor& sensor);
	SensorWithKnowParameters(int myId);
	SensorWithKnowParameters(int myId, const Matrix& myXa);
	SensorWithKnowParameters(int myId, const Matrix& myXa, const Matrix& mySigmaXa);

	//Private attribute accessor methods
	//
	void setXa(const Matrix& newXa);
	void setXa(const Matrix& myXa, const Matrix& mySigmaXa);
	Matrix getXa();
	Matrix getSigmaXa();

	//Composed object accessor methods
	//

	// EObject methods
	//
	string objectType(void);
	string objectAssociations(void);
	bool is(string s);

	// XML methods
	//
	void xmlSetData(string xml);
	string xmlGetData();

	//Other methods
	//

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // SENSORWITHKNOWPARAMETERS_H
