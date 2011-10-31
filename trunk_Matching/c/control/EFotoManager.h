/*******************************************************************************
								EFotoManager.h
*******************************************************************************/


#ifndef EFOTOMANAGER_H
#define EFOTOMANAGER_H

#include "EObject.h"
#include "Terrain.h"
#include "ProjectHeader.h"
#include "SensorWithFiducialMarks.h"
#include "SensorWithKnowDimensions.h"
#include "SensorWithKnowParameters.h"
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
class DEMManager;

/**
  * class EFotoManager
  *
  * This class manages the data flow within the application.
  * @author The e-foto team
  * @date 14/10/2010
  * @version 1.0.21
  *
  */

class EFotoManager
{

	int nextModule;
	int nextImage;

	bool savedState;
	string xmlData;
	string interfaceType;


	ProjectManager* project;
	IOManager* interiorOrientation;
	SRManager* spatialRessection;
        DEMManager* dem;

	Terrain* theTerrain;
	deque<Sensor*> sensors;
	deque<Flight*> flights;
	deque<Image*> images;
	deque<Point*> points;
	deque<InteriorOrientation*> IOs;
	deque<ExteriorOrientation*> EOs;

public:


	/**
	 * Default constructor.
	 */
	EFotoManager();

	/**
	 * Default destructor.
	 */
	~EFotoManager();

	/**
	 * This method creates an instance of the @e Terrain class.
	 */
	Terrain* instanceTerrain();

	/**
	 * This method creates an instance of the @e Sensor class.
	 * @param id The number of the @e Sensor identifier.
	 */
	Sensor* instanceSensor(int id);

	/**
	 * This method creates an instance of the @e Flight class.
	 * @param id The number of the @e Flight identifier.
	 */
	Flight* instanceFlight(int id);

	/**
	 * This method creates an instance of the @e Image class.
	 * @param id The number of the @e Image identifier.
	 */
	Image* instanceImage(int id);

        /**
         * This method creates an instance of the @e Point class.
         * @param id The number of the @e Point identifier.
         */
        void instanceAllPoints();
        void instanceAllIOs();
        void instanceAllEOs();
        void instanceAllImages();

	/**
	 * This method creates an instance of the @e Point class.
	 * @param id The number of the @e Point identifier.
	 */
	Point* instancePoint(int id);

	/**
	 * This method creates an instance of the @e InteriorOrientation class.
	 * @param imageId The number of the image identifier.
	 */
	InteriorOrientation* instanceIO(int imageId);

	/**
	 * This method creates an instance of the @e ExteriorOrientation class.
	 * @param imageId The number of the image identifier.
	 */
	ExteriorOrientation* instanceEO(int imageId);

	/**
	 * This method deletes an instance of the @e Terrain class.
	 */
	void deleteTerrain();

	/**
	 * This method deletes an instance of the @e Sensor class.
	 * @param id Sensor identifier.
	 */
	void deleteSensor(int id);

	/**
	 * This method deletes an instance of the @e Flight class.
	 * @param id Flight identifier.
	 */
	void deleteFlight(int id);

	/**
	 * This method deletes an instance of the @e Image class.
	 * @param id Image identifier.
	 */
	void deleteImage(int id);

	/**
	 * This method deletes an instance of the @e Point class.
	 * @param id Point identifier.
	 */
	void deletePoint(int id);

	/**
	 * This method deletes an instance of the @e InteriorOrientation class.
	 * @param id InteriorOrientation identifier.
	 */
	void deleteIO(int id);

	/**
	 * This method deletes an instance of the @e ExteriorOrientation class.
	 * @param id ExteriorOrientation identifier.
	 */
	void deleteEO(int id);

	/**
	 * This method returns the pointer of an existing @e Terrain object.
	 */
	Terrain* terrain();

	/**
	 * This method returns the pointer of an existing @e Sensor object.
	 * @param id The number of the sensor identifier.
	 */
	Sensor* sensor(int id);

	/**
	 * This method returns the pointer of an existing @e Flight object.
	 * @param id The number of the flight identifier.
	 */
	Flight* flight(int id);

	/**
	 * This method returns the pointer of an existing @e Image object.
	 * @param id The number of the image identifier.
	 */
	Image* image(int id);

	/**
	 * This method returns the pointer of an existing @e Point object.
	 * @param id The number of the point identifier.
	 */
	Point* point(int id);

	/**
	 * This method returns the pointer of an existing @e InteriorOrientation object.
	 * @param id The number of an identifier.
	 */
	InteriorOrientation* IO(int id);

	/**
	 * This method returns the pointer of an existing @e ExteriorOrientation object.
	 * @param id The number of an identifier.
	 */
	ExteriorOrientation* EO(int id);

	/**
	 * This method returns data from a specific node in a XML string.
	 * @param tagname The name of the intended XML tag.
	 */
	string getXml(string tagname);

	/**
	 * This method returns data from a specific node in a XML string.
	 * @param tagname The name of the intended XML tag.
	 * @param att The value of the attribute of the intended XML tag.
	 * @param value The value of the node of the intended XML tag.
	 */
	string getXml(string tagname, string att, string value);

	/**
	 * This method return a string containing the object type.
	 */
	string objectType(void);

	/**
	 * This method return a string containing all the object associations.
	 */
	string objectAssociations(void);

	/**
	 * This method checks if the current object @e is of a specific class.
	 * @param s Identifier of the intended instance.
	 */
	bool is(string s);

	/**
	 * This method reads data from a XML string and sets the internal variables with them.
	 * @param xml A string containing the XML data.
	 */
	void xmlSetData(string xml);

	/**
	 * This method returns the current data from the program to a XML string.
	 */
	string xmlGetData();

	void setSavedState(bool state);
	bool getSavedState();

	/**
	 * This sets the type of user interface which the program will run under.
	 * @param newInterfaceType String from Interface Type.
	 */
	void setInterfaceType(string newInterfaceType);

	/**
	 * This method gets the current interface type.
	 */
	string getInterfaceType();

	/**
	 * This method sets the next module to be executed.
	 * @param newModule New Module identification.
	 */
	void setNextModule(int newModule);

	/**
	 * This method sets the next image to be processed.
	 * @param newImage New Image identification.
	 */
	void setNextImage(int newImage);

	/**
	 * This method starts the Project Manager module.
	 */
	bool execProject(string filename = "");

	/**
	 * This method reloads the Project Manager module.
	 */
	bool reloadProject();

	/**
	 * This method starts the IO module.
	 * @param id An image identifier.
	 */
	bool execIO(int id);
	void stopIO(int id);

        /**
         * This method starts the SR module.
         * @param id An image identifier.
         */
        bool execSR(int id);
        void stopSR(int id);

        /**
         * This method starts the SR module.
         * @param id An image identifier.
         */
        bool execDEM();
        void stopDEM();

	/**
	 * This method starts the main execution loop.
	 */
	bool exec(string filename= "");

};

#endif // PROJECT_H
