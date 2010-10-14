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
#include "InteriorOrientation.h"
#include "SpatialRessection.h"

class IOManager;
class SRManager;
class ProjectManager;

/**
  * class EFotoManager
  *
  * @author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 09/06/2009
  * @version 1.0 - Rafael Alves de Aguiar & Irving da Silva Badolato.
  *
  */

class EFotoManager
{
    // Execution controller
    //
    int nextModule;
    int nextImage;
    /* These objects must be changed into something more reliable in the future.
	 * The current way they are treated won't suport phototriangulation. */

    // Main data element
    //
    string xmlData;
    string interfaceType;
    
    // Associations
    //
    ProjectManager* project;
    IOManager* interiorOrientation;
    SRManager* spatialRessection;

    // Started objects
    //
	Terrain* theTerrain;
    deque<Sensor*> sensors;
    deque<Flight*> flights;
    deque<Image*> images;
    deque<Point*> points;
    deque<InteriorOrientation*> IOs;
    deque<ExteriorOrientation*> EOs;

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
    InteriorOrientation* instanceIO(int imageId);
    ExteriorOrientation* instanceEO(int imageId);
    
	void deleteTerrain();
    void deleteSensor(int id);
    void deleteFlight(int id);
    void deleteImage(int id);
    void deletePoint(int id);
    void deleteIO(int id);
    void deleteEO(int id);

	Terrain* terrain();
    Sensor* sensor(int id);
    Flight* flight(int id);
    Image* image(int id);
    Point* point(int id);
    InteriorOrientation* IO(int id);
    ExteriorOrientation* EO(int id);

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
    void setNextImage(int newImage);
    bool execProject();
    bool reloadProject();
    bool execIO(int id);
    bool execSR(int id);
    bool exec();

};

#endif // PROJECT_H
