/*******************************************************************************
		FrameSensor.h
*******************************************************************************/

#ifndef FRAMESENSOR_H
#define FRAMESENSOR_H
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

#include "Sensor.h"
#include "DetectorSpaceCoordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class SensorWithFiducialMarks;
class SensorWithKnowDimensions;
class SensorWithKnowParameters;

// Classes/Structs used in FrameSensor only.
//

class RadialSymmetricDistortionCoefficient
{
	double value;
	double sigma;

public:

	double getValue();
	double getSigma();

	friend class FrameSensor;
	friend class SensorWithFiducialMarks;
	friend class SensorWithKnowDimensions;
	friend class SensorWithKnowParameters;
};

class DecenteredDistortionCoefficient
{
	double value;
	double sigma;

public:

	double getValue();
	double getSigma();

	friend class FrameSensor;
	friend class SensorWithFiducialMarks;
	friend class SensorWithKnowDimensions;
	friend class SensorWithKnowParameters;
};

// The Sensor class
//

/**
  * class FrameSensor
  *
  * @author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 07/02/2011
  * @version 1.4 - Rafael Alves de Aguiar & Irving da Silva Badolato.
  *
  */

class FrameSensor : public Sensor
{

protected:

	double focalDistance;
	double focalDistanceSigma;
	DetectorSpaceCoordinate principalPointCoordinates; // Isso tem que virar um tipo próprio.

    std::string focalDistanceUnit;

    std::deque<RadialSymmetricDistortionCoefficient> rsCoefficients;
    std::deque<DecenteredDistortionCoefficient> dCoefficients;


public:

	FrameSensor();
    explicit FrameSensor(const Sensor& sensor);
    explicit FrameSensor(const FrameSensor& sensor);
    explicit FrameSensor(int myId);
	virtual ~FrameSensor();

	void setFocalDistance(double newFocalDistance);
	void setFocalDistanceSigma(double newFocalDistanceSigma);
	void setPrincipalPointCoordinates(DetectorSpaceCoordinate newCoordinates);

	double getFocalDistance();
	double getFocalDistanceSigma();
	DetectorSpaceCoordinate getPrincipalPointCoordinates();
    //std::string getDescription();
    std::deque<RadialSymmetricDistortionCoefficient> getRadialSymmetricCoefficients();
    std::deque<DecenteredDistortionCoefficient> getDecenteredCoefficients();
    virtual std::string objectType(void);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // FRAMESENSOR_H
