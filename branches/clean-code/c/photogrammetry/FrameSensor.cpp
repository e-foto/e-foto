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

double RadialSymmetricDistortionCoefficient::getValue() const
{
	return value;
}

double DecenteredDistortionCoefficient::getValue() const
{
    return value;
}

// Constructors and destructors
FrameSensor::FrameSensor():Sensor()
{

}

FrameSensor::FrameSensor(const Sensor& sensor):Sensor(sensor)
{
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

// Associated object accessor methods
//

std::deque<RadialSymmetricDistortionCoefficient> FrameSensor::getRadialSymmetricCoefficients() const
{
	return rsCoefficients;
}

std::deque<DecenteredDistortionCoefficient> FrameSensor::getDecenteredCoefficients() const
{
	return dCoefficients;
}


} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
