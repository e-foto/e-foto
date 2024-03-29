/*******************************************************************************
		FrameSensor.cpp
*******************************************************************************/
/*Copyright 2002-2014 e-foto team (UERJ)
  This file is part of e-foto.

    e-foto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    e-foto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with e-foto.  If not, see <http://www.gnu.org/licenses/>.
*/
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

//FrameSensor::FrameSensor(const FrameSensor& sensor):Sensor(sensor)
//{
//	*this = sensor;
//}

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

void FrameSensor::setPrincipalPointCoordinates(DetectorSpaceCoordinate newCoordinates)
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

DetectorSpaceCoordinate FrameSensor::getPrincipalPointCoordinates()
{
	return principalPointCoordinates;
}

std::deque<RadialSymmetricDistortionCoefficient> FrameSensor::getRadialSymmetricCoefficients()
{
	return rsCoefficients;
}

std::deque<DecenteredDistortionCoefficient> FrameSensor::getDecenteredCoefficients()
{
	return dCoefficients;
}

std::string FrameSensor::objectType(void)
{
	return "FrameSensor";
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
