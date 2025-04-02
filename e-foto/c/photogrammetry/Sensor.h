#ifndef SENSOR_H
#define SENSOR_H
/*******************************************************************************
		Sensor.h
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

#include "EObject.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class FrameSensor;
class SensorWithFiducialMarks;
class SensorWithKnowDimensions;
class SensorWithKnowParameters;
class Image;
class Flight;
class DetectorSpaceCoordinate;

// Classes/Structs used in Sensor only.
//

class SpectralRange
{
	int band;
	double inferiorLimit;
	double superiorLimit;

public:

	friend class Sensor;
	friend class FrameSensor;
	friend class SensorWithFiducialMarks;
	friend class SensorWithKnowDimensions;
	friend class SensorWithKnowParameters;
};



// The Sensor class
//

/**
  * class Sensor
  *
  * @author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 07/02/2011
  * @version 1.4 - Rafael Alves de Aguiar & Irving da Silva Badolato.
  *
  */

class Sensor : public EObject
{
	//EOBJECT

protected:

	// Private attributes
	//
	int id;
    std::string sensorId;
	//double focalDistance;
	//double focalDistanceSigma;
	//DetectorSpaceCoordinate principalPointCoordinates; // Isso tem que virar um tipo próprio.
    std::string description;
    std::string geometry;
    std::string detector;
    std::string energySource;
    std::string calculationMode;
    std::string calibrationCertificateNumber;
    std::string calibrationCertificateDispatch;
    std::string calibrationCertificateExpiration;
    //std::string focalDistanceUnit;
    std::string spectralRangesUnit;
    std::deque<SpectralRange> spectralRanges;
	//deque<RadialSymmetricDistortionCoefficient> rsCoefficients;
	//deque<DecenteredDistortionCoefficient> dCoefficients;

	// Associated Objects
	//
    std::deque<Image*> myImages;
    std::deque<Flight*> myFlights;

public:

	// Constructors and destructors
	//
	Sensor();
    //explicit Sensor(const Sensor& sensor);
    explicit Sensor(int myId);
	virtual ~Sensor();

	// Private attributes accessor methods
    //
        void setId(int newId);
        void setSensorId(std::string nickname);
	//void setFocalDistance(double newFocalDistance);
	//void setFocalDistanceSigma(double newFocalDistanceSigma);
	//void setPrincipalPointCoordinates(DetectorSpaceCoordinate newCoordinates);
    void setDescription(std::string newDescription);
    void setCalculationMode(std::string newCalculationMode);
	int getId();
        std::string getSensorId();
	virtual double getFocalDistance() = 0;
	virtual double getFocalDistanceSigma() = 0;
	virtual DetectorSpaceCoordinate getPrincipalPointCoordinates() = 0;
    std::string getDescription();
    std::string getCalculationMode();

	// Associated object accessor methods
	//
	void putImage(Image* newImageAssociation); // Set association sensor. Generally from zero to many images and flights.
	void putFlight(Flight* newFligthAssociation);
        void clearImages() {myImages.clear();} // Remove associations.
        void clearFlights() {myFlights.clear();}
	Image* getImage(int imageId); // Get associated image or flight at sensor by id.
	Flight* getFlight(int flightId);
	int countImages(); // Inform the number of images or flights associated the sensor.
	int countFlights();
	Image* getImageAt(unsigned int index); // Get associated image or flight at sensor by index in deque for iterations.
	Flight* getFlightAt(unsigned int index);

	// EObject methods
	//
    virtual std::string objectType(void);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // SENSOR_H
