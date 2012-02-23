/*******************************************************************************
		Sensor.h
*******************************************************************************/

#ifndef SENSOR_H
#define SENSOR_H

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
	string sensorId;
	//double focalDistance;
	//double focalDistanceSigma;
	//DetectorSpaceCoordinate principalPointCoordinates; // Isso tem que virar um tipo próprio.
	string description;
	string geometry;
	string detector;
	string energySource;
	string calculationMode;
	string calibrationCertificateNumber;
	string calibrationCertificateDispatch;
	string calibrationCertificateExpiration;
	//string focalDistanceUnit;
	string spectralRangesUnit;
	deque<SpectralRange> spectralRanges;
	//deque<RadialSymmetricDistortionCoefficient> rsCoefficients;
	//deque<DecenteredDistortionCoefficient> dCoefficients;

	// Associated Objects
	//
	deque<Image*> myImages;
	deque<Flight*> myFlights;

public:

	// Constructors and destructors
	//
	Sensor();
	Sensor(const Sensor& sensor);
	Sensor(int myId);
	virtual ~Sensor();

	// Private attributes accessor methods
	//
	void setId(int newId);
	//void setFocalDistance(double newFocalDistance);
	//void setFocalDistanceSigma(double newFocalDistanceSigma);
	//void setPrincipalPointCoordinates(DetectorSpaceCoordinate newCoordinates);
	void setDescription(string newDescription);
	void setCalculationMode(string newCalculationMode);
	int getId();
	virtual double getFocalDistance() = 0;
	virtual double getFocalDistanceSigma() = 0;
	virtual DetectorSpaceCoordinate getPrincipalPointCoordinates() = 0;
	string getDescription();
	string getCalculationMode();

	// Associated object accessor methods
	//
	void putImage(Image* newImageAssociation); // Set association sensor. Generally from zero to many images and flights.
	void putFlight(Flight* newFligthAssociation);
	Image* getImage(int imageId); // Get associated image or flight at sensor by id.
	Flight* getFlight(int flightId);
	int countImages(); // Inform the number of images or flights associated the sensor.
	int countFlights();
	Image* getImageAt(unsigned int index); // Get associated image or flight at sensor by index in deque for iterations.
	Flight* getFlightAt(unsigned int index);

	// EObject methods
	//
	virtual string objectType(void);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // SENSOR_H
