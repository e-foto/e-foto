/*******************************************************************************
								FrameSensor.cpp
*******************************************************************************/

#include "FrameSensor.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

// Subclass methods
//

double RadialSymmetricDistortionCoefficient::getValue()
{
	return value;
}

double RadialSymmetricDistortionCoefficient::getSigma()
{
	return sigma;
}

double DecenteredDistortionCoefficient::getValue()
{
	return value;
}

double DecenteredDistortionCoefficient::getSigma()
{
	return sigma;
}

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

deque<RadialSymmetricDistortionCoefficient> FrameSensor::getRadialSymmetricCoefficients()
{
	return rsCoefficients;
}

deque<DecenteredDistortionCoefficient> FrameSensor::getDecenteredCoefficients()
{
	return dCoefficients;
}

// EObject methods
//

string FrameSensor::objectType(void)
{
	return "FrameSensor";
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
