/*******************************************************************************
								FrameSensor.h
*******************************************************************************/

#ifndef FRAMESENSOR_H
#define FRAMESENSOR_H

#include "Sensor.h"
#include "AnalogImageSpaceCoordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class SensorWithFiducialMarks;
class SensorWithKnowDimensions;
class SensorWithKnowParameters;
//class AnalogImageSpaceCoordinate;

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
	//string sensorId;
	double focalDistance;
	double focalDistanceSigma;
	AnalogImageSpaceCoordinate principalPointCoordinates; // Isso tem que virar um tipo próprio.
	//string description;
	//string geometry;
	//string detector;
	//string energySource;
	//string calibrationCertificateNumber;
	//string calibrationCertificateDispatch;
	//string calibrationCertificateExpiration;
	string focalDistanceUnit;
	//string spectralRangesUnit;
	//deque<SpectralRange> spectralRanges;
	deque<RadialSymmetricDistortionCoefficient> rsCoefficients;
	deque<DecenteredDistortionCoefficient> dCoefficients;

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
	void setPrincipalPointCoordinates(AnalogImageSpaceCoordinate newCoordinates);
	//void setDescription(string newDescription);
	//int getId();
	double getFocalDistance();
	double getFocalDistanceSigma();
	AnalogImageSpaceCoordinate getPrincipalPointCoordinates();
	//string getDescription();
	deque<RadialSymmetricDistortionCoefficient> getRadialSymmetricCoefficients();
	deque<DecenteredDistortionCoefficient> getDecenteredCoefficients();

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
	virtual string objectType(void);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // FRAMESENSOR_H
