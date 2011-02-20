/*******************************************************************************
								FrameSensor.cpp
*******************************************************************************/

#include "FrameSensor.h"

// Constructors and destructors
//

FrameSensor::FrameSensor():Sensor()
{

}

FrameSensor::FrameSensor(const Sensor& sensor):Sensor(sensor)
{
	//*this = sensor;
}

FrameSensor::FrameSensor(const FrameSensor& sensor):Sensor(sensor)
{
	*this = sensor;
}

FrameSensor::FrameSensor(int myId):Sensor(myId)
{

}

FrameSensor::~FrameSensor()
{

}



// Private attributes accessor methods
//

void FrameSensor::setFocalDistance(double newFocalDistance)
{
	focalDistance = newFocalDistance;
}

void FrameSensor::setFocalDistanceSigma(double newFocalDistanceSigma)
{
	focalDistanceSigma = newFocalDistanceSigma;
}

void FrameSensor::setPrincipalPointCoordinates(AnalogImageSpaceCoordinate newCoordinates)
{
	principalPointCoordinates = newCoordinates;
}

double FrameSensor::getFocalDistance()
{
	return focalDistance;
}

double FrameSensor::getFocalDistanceSigma()
{
	return focalDistanceSigma;
}

AnalogImageSpaceCoordinate FrameSensor::getPrincipalPointCoordinates()
{
	return principalPointCoordinates;
}

// Associated object accessor methods
//

// EObject methods
//

string FrameSensor::objectType(void)
{
	return "FrameSensor";
}
