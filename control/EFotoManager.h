/*******************************************************************************
                                EFotoManager.h
*******************************************************************************/


#ifndef EFOTOMANAGER_H
#define EFOTOMANAGER_H

#include "EObject.h"
#include "Terrain.h"
#include "ProjectHeader.h"
#include "Aerial.h"
#include "Flight.h"
#include "Image.h"
#include "CheckingPoint.h"
#include "ControlPoint.h"
#include "PhotogrammetricPoint.h"

class ProjectManager;

class EFotoManager
{
    // Execution controller
    //
	int nextModule;

    // Main data element
    //
    string xmlData;
    string interfaceType;
    
    // Associations
    //
    ProjectManager* project;

    // Started objects
    //
	Terrain* theTerrain;
    deque<Sensor*> sensors;
    deque<Flight*> flights;
    deque<Image*> images;
	deque<Point*> points;

public:

    // Constructors and destructor
    //
    EFotoManager();
    ~EFotoManager();

    // Startable objects accessor methods
    //
	Terrain* instanceTerrain();
    Sensor* instanceSensor(int id);
    Flight* instanceFlight(int id);
    Image* instanceImage(int id);
    Point* instancePoint(int id);
    
	void deleteTerrain();
    void deleteSensor(int id);
    void deleteFlight(int id);
    void deleteImage(int id);
    void deletePoint(int id);

	Terrain* terrain();
    Sensor* sensor(int id);
    Flight* flight(int id);
    Image* image(int id);
    Point* point(int id);

    string getXml(string tagname);
    string getXml(string tagname, string att, string value);

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
    void setInterfaceType(string newInterfaceType);
    string getInterfaceType();
	void setNextModule(int newModule);
    bool execProject();
    bool reloadProject();
    bool exec();

};

#endif // PROJECT_H
