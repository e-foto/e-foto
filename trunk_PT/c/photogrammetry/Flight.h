/*******************************************************************************
                               Flight.h
*******************************************************************************/

#ifndef FLIGHT_H
#define FLIGHT_H

#include "EObject.h"

class Terrain;
class Sensor;
class Image;

/**
* class Flight
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 06/05/2009
* @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato.
*/

class Flight : public EObject
{
    //EOBJECT

private:

    // Private attributes
    //
    int id;
    int sensorId;
	string flightId;
    string description;
    string execution;
    string producerName;
    string scale;
	double scaleDen;
    double height;
    double longitudinalOverlap;
    double transversalOverlap;


    // Associated objects
    //
	Terrain* myTerrain;
    Sensor* mySensor;
    deque<Image*> myImages;


public:

    // Constructors and destructors
    //
    Flight();
    Flight(int myId, int mySensorId);
    virtual ~Flight();

    // Private attribute accessor methods
    //
    void setId(int newId);
    void setSensorId(int newSensorId);
    void setDescription(string newDescription);
    void setExecution(string newExecution);
    void setProducerName(string newProducerName);
    void setScale(string newScale);
    void setHeight(double newHeight);
    void setLongitudinalOverlap(double newLongitudinalOverlap);
    void setTransversalOverlap(double newTransversalOverlap);
    int getId();
    int getSensorId();
    string getDescription();
    string getExecution();
    string getProducerName();
    string getScale();
	double getScaleDen();
    double getHeight();
    double getLongitudinalOverlap();
    double getTransversalOverlap();

    // Associated objects accessor methods
    //
	void setTerrain(Terrain* newTerrain); // associate one terrain only.
	Terrain* getTerrain();
    void setSensor(Sensor* newSensor); // associate one sensor only.
    Sensor* getSensor();

    void putImage(Image* newImageAssociation); // Set flight associations. Generally from zero to many images.
    Image* getImage(int imageId); // Get associated flight at image by id.
    int countImages(); // Inform the number of images associated the flight.
    Image* getImageAt(unsigned int index); // Get associated flight at image by index in deque for iterations.

    // EObject methods
    //
    string objectType(void);
    string objectAssociations(void);
    bool is(string s);

    // XML methods
    void xmlSetData(string xml);
    string xmlGetData();

};

#endif //FLIGHT_H
