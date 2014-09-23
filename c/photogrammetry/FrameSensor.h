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
	//EOBJECT

protected:

	// Private attributes
	//
	//int id;
    //std::string sensorId;
	double focalDistance;
	double focalDistanceSigma;
	DetectorSpaceCoordinate principalPointCoordinates; // Isso tem que virar um tipo próprio.
    //std::string description;
    //std::string geometry;
    //std::string detector;
    //std::string energySource;
    //std::string calibrationCertificateNumber;
    //std::string calibrationCertificateDispatch;
    //std::string calibrationCertificateExpiration;
    std::string focalDistanceUnit;
    //std::string spectralRangesUnit;
	//deque<SpectralRange> spectralRanges;
    std::deque<RadialSymmetricDistortionCoefficient> rsCoefficients;
    std::deque<DecenteredDistortionCoefficient> dCoefficients;

	// Associated Objects
	//
	//deque<Image*> myImages;
	//deque<Flight*> myFlights;

public:

	// Constructors and destructors
	//
	FrameSensor();
	FrameSensor(const Sensor& sensor);
	FrameSensor(const FrameSensor& sensor);
	FrameSensor(int myId);
	virtual ~FrameSensor();

	// Private attributes accessor methods
	//
	//void setId(int newId);
	void setFocalDistance(double newFocalDistance);
	void setFocalDistanceSigma(double newFocalDistanceSigma);
	void setPrincipalPointCoordinates(DetectorSpaceCoordinate newCoordinates);
    //void setDescription(std::string newDescription);
	//int getId();
	double getFocalDistance();
	double getFocalDistanceSigma();
	DetectorSpaceCoordinate getPrincipalPointCoordinates();
    //std::string getDescription();
    std::deque<RadialSymmetricDistortionCoefficient> getRadialSymmetricCoefficients();
    std::deque<DecenteredDistortionCoefficient> getDecenteredCoefficients();

	// Associated object accessor methods
	//
	//void putImage(Image* newImageAssociation); // Set association sensor. Generally from zero to many images and flights.
	//void putFlight(Flight* newFligthAssociation);
	//Image* getImage(int imageId); // Get associated image or flight at sensor by id.
	//Flight* getFlight(int flightId);
	//int countImages(); // Inform the number of images or flights associated the sensor.
	//int countFlights();
	//Image* getImageAt(unsigned int index); // Get associated image or flight at sensor by index in deque for iterations.
	//Flight* getFlightAt(unsigned int index);

	// EObject methods
	//
    virtual std::string objectType(void);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // FRAMESENSOR_H
