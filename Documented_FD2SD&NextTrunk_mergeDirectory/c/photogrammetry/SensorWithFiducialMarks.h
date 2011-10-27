/**************************************************************************
						SensorWithFiducialMarks.h
**************************************************************************/


#ifndef SENSORWITHFIDUCIALMARKS_H
#define SENSORWITHFIDUCIALMARKS_H
#include "FrameSensor.h"
#include "Matrix.h"
#include "AnalogFiductialMark.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
  * class SensorWithFiducialMarks
  *
  * @author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 04/02/2011
  * @version 1.3 - Rafael Alves de Aguiar & Irving da Silva Badolato
  */

class SensorWithFiducialMarks : public FrameSensor
{

	//EOBJECT

private:

	//Private attributes
	//
	Matrix Lb;
	Matrix SigmaLb;

	//Composed objects
	//
	deque<AnalogFiductialMark> anaFidMarks;

public:

	//Constructors
	//
	SensorWithFiducialMarks();
	SensorWithFiducialMarks(const Sensor& sensor);
	SensorWithFiducialMarks(int myId);
	SensorWithFiducialMarks(int myId, const Matrix& myLb);
	SensorWithFiducialMarks(int myId, const Matrix& myLb, const Matrix& mySigmaLb);

	//Private attribute accessor methods
	//
	void setLb(const Matrix& newLb);
	void setLb(const Matrix& myLb, const Matrix& mySigmaLb);
	Matrix getLb();
	Matrix getSigmaLb();

	//Composed object accessor methods
	//
	void setAnaFidMarks(deque<AnalogFiductialMark> newAnaFidMarks);
	deque<AnalogFiductialMark> getAnaFidMarks();

	void putAnaFidMark(AnalogFiductialMark newAnaFidMark);
	AnalogFiductialMark getAnaFidMark(int id);
	AnalogFiductialMark getAnaFidMarkAt(unsigned int index);
	int countAnaFidMarks();

	void deleteAnaFidMark(int id);
	void deleteAnaFidMarkAt(unsigned int index);
	void clearAnaFidMarks();

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
	void marksToLb();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // SENSORWITHFIDUCIALMARKS_H
