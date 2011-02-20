/**************************************************************************
						SensorWithKnowDimensions.h
**************************************************************************/


#ifndef SENSORWITHKNOWDIMENSIONS_H
#define SENSORWITHKNOWDIMENSIONS_H
#include "FrameSensor.h"
#include "Matrix.h"
#include "AnalogFiductialMark.h"

/**
  * class SensorWithKnowDimensions
  *
  * @author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 07/02/2011
  * @version 1.4 - Rafael Alves de Aguiar & Irving da Silva Badolato
  */

class SensorWithKnowDimensions : public FrameSensor
{

	//EOBJECT

private:

	//Private attributes
	//
	double pixelSize;
	int frameColumns, frameRows;

	//Composed objects
	//

public:

	//Constructors
	//
	SensorWithKnowDimensions();
	SensorWithKnowDimensions(const Sensor& sensor);
	SensorWithKnowDimensions(int myId);
	SensorWithKnowDimensions(int myId, double myPixelSize, int myFrameRows, int myFrameColumns);

	//Private attribute accessor methods
	//
	void setPixelSize(double newPixelSize);
	void setFrameDimensions(int newFrameRows, int newFrameColumns);
	double getPixelSize();
	int getFrameRows();
	int getFrameColumns();
	Matrix forgeLb();

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

#endif // SENSORWITHKNOWDIMENSIONS_H
