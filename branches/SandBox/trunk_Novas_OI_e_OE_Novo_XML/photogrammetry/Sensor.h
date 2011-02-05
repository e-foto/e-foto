/*******************************************************************************
                        		Sensor.h
*******************************************************************************/

#ifndef SENSOR_H
#define SENSOR_H

#include "EObject.h"
#include "AnalogImageSpaceCoordinate.h"

class SensorWithFiducialMarks;
class Image;
class Flight;
class AnalogImageSpaceCoordinate;

// Classes/Structs used in sensor only.
//

class SpectralRange
{
    int band;
    double inferiorLimit;
    double superiorLimit;

public:

    friend class Sensor;
	friend class SensorWithFiducialMarks;
};

class RadialSymmetricDistortionCoefficient
{
    double value;
    double sigma;

public:

    friend class Sensor;
	friend class SensorWithFiducialMarks;
};

class DecenteredDistortionCoefficient
{
    double value;
    double sigma;

public:

    friend class Sensor;
	friend class SensorWithFiducialMarks;
};



// The Sensor class
//

/**
  * class Sensor
  *
  * @author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 06/05/2009
  * @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato.
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
    double focalDistance;
    double focalDistanceSigma;
    AnalogImageSpaceCoordinate principalPointCoordinates; // Isso tem que virar um tipo próprio.
    string description;
    string geometry;
    string detector;
    string energySource;
    string calibrationCertificateNumber;
    string calibrationCertificateDispatch;
    string calibrationCertificateExpiration;
    string focalDistanceUnit;
    string spectralRangesUnit;
    deque<SpectralRange> spectralRanges;
    deque<RadialSymmetricDistortionCoefficient> rsCoefficients;
    deque<DecenteredDistortionCoefficient> dCoefficients;

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
    void setFocalDistance(double newFocalDistance);
    void setFocalDistanceSigma(double newFocalDistanceSigma);
    void setPrincipalPointCoordinates(AnalogImageSpaceCoordinate newCoordinates);
    void setDescription(string newDescription);
    int getId();
    double getFocalDistance();
    double getFocalDistanceSigma();
    AnalogImageSpaceCoordinate getPrincipalPointCoordinates();
    string getDescription();

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
    string objectType(void);

};

#endif // SENSOR_H
