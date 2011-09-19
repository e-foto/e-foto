/*******************************************************************************
                               Image.h
*******************************************************************************/

#ifndef IMAGE_H
#define IMAGE_H

#include "EObject.h"
#include "DigitalFiductialMark.h"
#include "ObjectSpaceCoordinate.h"

class Sensor;
class Flight;
class Point;
class InteriorOrientation;
class SpatialRessection;

/**
* class Image
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 06/05/2009
* @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato.
*/

class Image : public EObject
{
    //EOBJECT

    // Private attributes
    //
    int id;
    int sensorId;
    int flightId;
    unsigned int resolution;
    string resolutionUnit;
    unsigned int width;
    unsigned int height;
    string filename;
    string filepath;

	string imageId;
    ObjectSpaceCoordinate spatialCoordinates;

    // IMPORTANTE: Gambiarra temporaria...
	bool gnssAvailable, gnssSigmaAvailable, insAvailable, insSigmaAvailable;
	string gnssType, insType;
    double gnssX0, gnssY0, gnssZ0;
    Matrix gnssSigma;
    double insOmega, insPhi, insKappa;
    Matrix insSigma;

    // Composed objects
    //
    deque<DigitalFiductialMark> digFidMarks;

    // Associated objects
    //
    Sensor* mySensor;
    Flight* myFlight;
    InteriorOrientation* myIO;
    SpatialRessection* myEO; // Muito em breve (quando definirmos a fototri) a classe usada aqui será uma ExteriorOrientation.
    deque<Point*> myPoints;

public:

    // Constructors and destructors
    //
    Image();
    Image(int myId, int mySensorId); // Constructor with ids only, needed in project use.
    virtual ~Image();

    // Private attribute accessor methods
    //
    void setId(int newId);
    void setSensorId(int newSensorId);
    void setFlightId(int newFlightId);
    void setResolution(unsigned int newResolution);
    void setResolutionUnit(string newResolutionUnit);
    void setWidth(unsigned int newWidth);
    void setHeight(unsigned int newHeight);
    void setFilename(string newFilename);
    void setFilepath(string newFilepath);
    int getId();
    int getSensorId();
    int getFlightId();
    unsigned int getResolution();
    string getResolutionUnit();
    unsigned int getWidth();
    unsigned int getHeight();
    string getFilename();
    string getFilepath();
	bool isGnssAvailable();
	bool isInsAvailable();
	double getGnssX0();
	double getGnssY0();
	double getGnssZ0();
	string getGnssType();
	double getInsOmega();
	double getInsPhi();
	double getInsKappa();
	string getInsType();

    // Composed objects accessor methods
    //
    void setDigFidMarks(deque<DigitalFiductialMark> newDigFidMarks); // Set for many DigFidMarks using deque.
    deque<DigitalFiductialMark> getDigFidMarks(); // Get for all DigFidMarks using deque.

    void putDigFidMark(DigitalFiductialMark newDigFidMark); // Put one DigFidMark between the components of the image.
    DigitalFiductialMark getDigFidMark(int id);
    DigitalFiductialMark getDigFidMarkAt(unsigned int index); // Get one DigFidMark by index for iterations.
    int countDigFidMarks();  // Inform the number of DigFidMarks composed in the image.

    void deleteDigFidMark(int id); // Delete one DigFidMark by id in the image.
    void deleteDigFidMarkAt(unsigned int index); // Delete one DigFidMark by index in the image.
    void clearDigFidMarks(); // Delete all DigFidMarks composed in the image.

    // Associated object accessor methods
    //
    void setSensor(Sensor* newSensor); // Set image association. Generally one sensor only.
    Sensor* getSensor();  // Get associated sensor at image.
    void setFlight(Flight* newFlight); // Set image association. Generally one sensor only.
    Flight* getFlight();  // Get associated sensor at image.

    void setIO(InteriorOrientation* newIO); // Set image association. Generally zero or one IO.
    InteriorOrientation* getIO(); // Get associated IO at image if there.
    void setEO(SpatialRessection* newEO); // Set image association. Generally zero or one EO.
    SpatialRessection* getEO(); // Get associated EO at image if there.

    void putPoint(Point* newPointAssociation); // Set image association. Generally from zero to many points.
    Point* getPoint(int pointId); // Get associated point at image by id.
    int countPoints(); // Inform the number of points associated the image.
    Point* getPointAt(unsigned int index); // Get associated point at image by index in deque for iterations.
	void sortPoints(); // sort points with ControlPoints before PhotogrammetricPoints
	void clearPoints(); // Paulo Andre 19/09/2011
	// EObject methods
    //
    string objectType(void);
    string objectAssociations(void);
    bool is(string s);

    // XML methods
    //
    void xmlSetData(string xml);
    string xmlGetData();

    //Other methods
    //
    int load(); // Try load the image using filePath and fileName sets in the image.

};

#endif // IMAGE_H
