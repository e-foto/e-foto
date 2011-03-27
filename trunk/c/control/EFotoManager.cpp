/*******************************************************************************
								  EFotoManager.cpp
*******************************************************************************/

#include "EFotoManager.h"
#include "IOManager.h"
#include "SRManager.h"
#include "ProjectManager.h"

#include <QApplication>

// Constructors and destructor
//

/**
 *
 */
EFotoManager::EFotoManager()
{
	xmlData = "";
	savedState = true;
	project = NULL;
	interiorOrientation = NULL;
	spatialRessection = NULL;
	theTerrain = NULL;
	nextModule = 1;
}

/**
 *
 */
EFotoManager::~EFotoManager()
{

}

// Instance objects accessor methods
//

/**
 *
 */
Terrain* EFotoManager::instanceTerrain()
{
		if (theTerrain != NULL)
			return theTerrain;
		EDomElement root(xmlData);
		EDomElement xmlTerrain = root.elementByTagName("terrain");
		if (xmlTerrain.getContent().compare("") == 0)
				return NULL;
		theTerrain = new Terrain();
		theTerrain->xmlSetData(xmlTerrain.getContent());
		return theTerrain;
}

/**
 *
 */
Sensor* EFotoManager::instanceSensor(int id)
{
	for (unsigned int i = 0; i < sensors.size(); i++)
		if (sensors.at(i)->getId() == id)
			return sensors.at(i);
	EDomElement root(xmlData);
	EDomElement xmlSensor = root.elementByTagAtt("sensor", "key", intToString(id));
	if (xmlSensor.getContent().compare("") == 0)
		return NULL;
	if (xmlSensor.elementByTagName("geometry").toString().compare("frame") == 0 &&
		xmlSensor.elementByTagName("calculationMode").toString().compare("With Fiducial Marks") == 0)
	{
		SensorWithFiducialMarks* newSensorWithFiducialMarks = new SensorWithFiducialMarks();
		newSensorWithFiducialMarks->xmlSetData(xmlSensor.getContent());
		sensors.push_back(newSensorWithFiducialMarks);
		return (Sensor*) newSensorWithFiducialMarks;
	}
	if (xmlSensor.elementByTagName("geometry").toString().compare("frame") == 0 &&
		xmlSensor.elementByTagName("calculationMode").toString().compare("With Sensor Dimensions") == 0)
	{
		SensorWithKnowDimensions* newSensorWithKnowDimensions = new SensorWithKnowDimensions();
		newSensorWithKnowDimensions->xmlSetData(xmlSensor.getContent());
		sensors.push_back(newSensorWithKnowDimensions);
		return (Sensor*) newSensorWithKnowDimensions;
	}
	if (xmlSensor.elementByTagName("geometry").toString().compare("frame") == 0 &&
		xmlSensor.elementByTagName("calculationMode").toString().compare("Fixed Parameters") == 0)
	{
		SensorWithKnowParameters* newSensorWithKnowParameters = new SensorWithKnowParameters();
		newSensorWithKnowParameters->xmlSetData(xmlSensor.getContent());
		sensors.push_back(newSensorWithKnowParameters);
		return (Sensor*) newSensorWithKnowParameters;
	}
	return NULL;
}

/**
 *
 */
Flight* EFotoManager::instanceFlight(int id)
{
	for (unsigned int i = 0; i < flights.size(); i++)
		if (flights.at(i)->getId() == id)
			return flights.at(i);
	EDomElement root(xmlData);
	EDomElement xmlFlight = root.elementByTagAtt("flight", "key", intToString(id));
	if (xmlFlight.getContent().compare("") == 0)
		return NULL;
	Flight* newFlight = new Flight();
	newFlight->xmlSetData(xmlFlight.getContent());
	flights.push_back(newFlight);
	return newFlight;
}

/**
 *
 */
Image* EFotoManager::instanceImage(int id)
{
	for (unsigned int i = 0; i < images.size(); i++)
		if (images.at(i)->getId() == id)
			return images.at(i);
	EDomElement root(xmlData);
	EDomElement xmlImage = root.elementByTagAtt("image", "key", intToString(id));
	if (xmlImage.getContent().compare("") == 0)
		return NULL;
	Image* newImage = new Image();
	newImage->xmlSetData(xmlImage.getContent());
	images.push_back(newImage);
	return newImage;
}

/**
 *
 */
Point* EFotoManager::instancePoint(int id)
{
	for (unsigned int i = 0; i < points.size(); i++)
		if (points.at(i)->getId() == id)
			return points.at(i);
	EDomElement root(xmlData);
	EDomElement xmlPoint = root.elementByTagAtt("point", "key", intToString(id));
	if (xmlPoint.getContent().compare("") == 0)
		return NULL;
	if (xmlPoint.attribute("type").compare("control") == 0)
	{
		ControlPoint* newPoint = new ControlPoint();
		newPoint->xmlSetData(xmlPoint.getContent());
		points.push_back(newPoint);
		return (Point*) newPoint;
	}
	if (xmlPoint.attribute("type").compare("checking") == 0)
	{
		CheckingPoint* newPoint = new CheckingPoint();
		newPoint->xmlSetData(xmlPoint.getContent());
		points.push_back(newPoint);
		return (Point*) newPoint;
	}
	if (xmlPoint.attribute("type").compare("photogrammetric") == 0)
	{
		PhotogrammetricPoint* newPoint = new PhotogrammetricPoint();
		newPoint->xmlSetData(xmlPoint.getContent());
		points.push_back(newPoint);
		return (Point*) newPoint;
	}
	return NULL;
}

/**
 *
 */
InteriorOrientation* EFotoManager::instanceIO(int imageId)
{
	for (unsigned int i = 0; i < IOs.size(); i++)
		if (IOs.at(i)->getImageId() == imageId)
			return IOs.at(i);
	EDomElement root(xmlData);
	EDomElement xmlIO = root.elementByTagAtt("imageIO", "image_key", intToString(imageId));
	if (xmlIO.getContent().compare("") == 0)
		return NULL;
	InteriorOrientation* newIO = new InteriorOrientation();
	newIO->setImage(image(imageId));
	newIO->xmlSetData(xmlIO.getContent());
	newIO->setImage(NULL);
	IOs.push_back(newIO);
	return newIO;
}

/**
 *
 */
ExteriorOrientation* EFotoManager::instanceEO(int imageId)
{
	for (unsigned int i = 0; i < EOs.size(); i++)
		if (EOs.at(i)->getImageId() == imageId)
			return EOs.at(i);
	EDomElement root(xmlData);
	EDomElement xmlEO = root.elementByTagAtt("imageEO", "image_key", intToString(imageId));
	if (xmlEO.getContent().compare("") == 0)
		return NULL;
	if (xmlEO.attribute("type").compare("spatialRessection") == 0)
	{
		SpatialRessection* newEO = new SpatialRessection();
		newEO->xmlSetData(xmlEO.getContent());
		EOs.push_back(newEO);
		return (ExteriorOrientation*) newEO;
	}
	return NULL;
}

/**
 *
 */
void EFotoManager::deleteTerrain()
{
	if (theTerrain != NULL)
	{
		delete(theTerrain);
		theTerrain = NULL;
	}
}

/**
 *
 */
void EFotoManager::deleteSensor(int id)
{
	unsigned int i;
	Sensor* mySensor = NULL;
	for (i = 0; i < sensors.size(); i++)
		if (sensors.at(i)->getId() == id)
		{
		mySensor = sensors.at(i);
		break;
	}
	if (mySensor != NULL)
	{
		EDomElement xmlSensor(mySensor->xmlGetData());
		if (xmlSensor.elementByTagName("geometry").toString().compare("frame") == 0 &&
			xmlSensor.elementByTagName("platform").toString().compare("aerial") == 0 &&
			xmlSensor.elementByTagName("detector").toString().compare("film") == 0)
		{
			SensorWithFiducialMarks* mySensorWithFiducialMarks = (SensorWithFiducialMarks*) mySensor;
			delete(mySensorWithFiducialMarks);
		}
		sensors.erase(sensors.begin() + i);
	}
}

/**
 *
 */
void EFotoManager::deleteFlight(int id)
{
		unsigned int i;
	Flight* myFlight = NULL;
		for (i = 0; i < flights.size(); i++)
				if (flights.at(i)->getId() == id)
				{
						myFlight = flights.at(i);
						break;
				}
	if (myFlight != NULL)
	{
		delete(myFlight);
				flights.erase(flights.begin() + i);
		}
}

/**
 *
 */
void EFotoManager::deleteImage(int id)
{
	unsigned int i;
	Image* myImage = NULL;
	for (i = 0; i < images.size(); i++)
		if (images.at(i)->getId() == id)
		{
		myImage = images.at(i);
		break;
	}
	if (myImage != NULL)
	{
		delete(myImage);
		images.erase(images.begin() + i);
	}
}

/**
 *
 */
void EFotoManager::deletePoint(int id)
{
	unsigned int i;
	Point* myPoint = NULL;
	for (i = 0; i < points.size(); i++)
		if (points.at(i)->getId() == id)
		{
		myPoint = points.at(i);
		break;
	}
	if (myPoint != NULL)
	{
		EDomElement xmlPoint(myPoint->xmlGetData());
		if (xmlPoint.attribute("type").compare("control") == 0)
		{
			ControlPoint* myControl = (ControlPoint*) myPoint;
			delete(myControl);
		}
		else if (xmlPoint.attribute("type").compare("checking") == 0)
		{
			CheckingPoint* myChecking = (CheckingPoint*) myPoint;
			delete(myChecking);
		}
		else if (xmlPoint.attribute("type").compare("control") == 0)
		{
			PhotogrammetricPoint* myPhotogrammetric = (PhotogrammetricPoint*) myPoint;
			delete(myPhotogrammetric);
		}
		points.erase(points.begin() + i);
	}
}

/**
 *
 */
void EFotoManager::deleteIO(int id)
{
	unsigned int i;
	InteriorOrientation* myIO = NULL;
	for (i = 0; i < IOs.size(); i++)
		if (IOs.at(i)->getImageId() == id)
		{
		myIO = IOs.at(i);
		break;
	}
	if (myIO != NULL)
	{
		delete(myIO);
		IOs.erase(IOs.begin() + i);
	}
}

/**
 *
 */
void EFotoManager::deleteEO(int id)
{
	unsigned int i;
	ExteriorOrientation* myEO = NULL;
	for (i = 0; i < EOs.size(); i++)
		if (EOs.at(i)->getImageId() == id)
		{
		myEO = EOs.at(i);
		break;
	}
	if (myEO != NULL)
	{
		//EDomElement xmlEO(myEO->xmlGetData());
		//if (xmlEO.attribute("type").compare("spatialRessection") == 0)
		if (myEO->is("SpatialRessection"))
		{
			SpatialRessection* mySR = (SpatialRessection*) myEO;
			delete(mySR);
		}
		EOs.erase(EOs.begin() + i);
	}
}

/**
 *
 */
Terrain* EFotoManager::terrain()
{
	if (theTerrain != NULL)
		return theTerrain;
	return NULL;
}

/**
 *
 */
Sensor* EFotoManager::sensor(int id)
{
		for (unsigned int i = 0; i < sensors.size(); i++)
				if (sensors.at(i)->getId() == id)
						return sensors.at(i);
				return NULL;
}

/**
 *
 */
Flight* EFotoManager::flight(int id)
{
	for (unsigned int i = 0; i < flights.size(); i++)
		if (flights.at(i)->getId() == id)
			return flights.at(i);
	return NULL;
}

/**
 *
 */
Image* EFotoManager::image(int id)
{
	for (unsigned int i = 0; i < images.size(); i++)
		if (images.at(i)->getId() == id)
			return images.at(i);
	return NULL;
}

/**
 *
 */
Point* EFotoManager::point(int id)
{
	for (unsigned int i = 0; i < points.size(); i++)
		if (points.at(i)->getId() == id)
			return points.at(i);
	return NULL;
}

/**
 *
 */
InteriorOrientation* EFotoManager::IO(int id)
{
	for (unsigned int i = 0; i < IOs.size(); i++)
		if (IOs.at(i)->getImageId() == id)
			return IOs.at(i);
	return NULL;
}

/**
 *
 */
ExteriorOrientation* EFotoManager::EO(int id)
{
	for (unsigned int i = 0; i < EOs.size(); i++)
		if (EOs.at(i)->getImageId() == id)
			return EOs.at(i);
	return NULL;
}

/**
 *
 */
string EFotoManager::getXml(string tagname)
{
	EDomElement root(xmlData);
	return root.elementByTagName(tagname).getContent();
}

/**
 *
 */
string EFotoManager::getXml(string tagname, string att, string value)
{
	EDomElement root(xmlData);
	return root.elementByTagAtt(tagname, att, value).getContent();
}


// EObject methods
//

/**
 *
 */
string EFotoManager::objectType(void)
{
	return "EFotoManager";
}

/**
 *
 */
string EFotoManager::objectAssociations(void)
{
	return "";
}

/**
 *
 */
bool EFotoManager::is(string s)
{
	return (s == "EFotoManager" ? true : false);
}


// XML methods
//
void EFotoManager::xmlSetData(string xml)
{
	xmlData = xml;
}

string EFotoManager::xmlGetData()
{
	return xmlData;
}


//Other methods
//

void EFotoManager::setSavedState(bool state)
{
	savedState = state;
}

bool EFotoManager::getSavedState()
{
	return savedState; // O uso desta variável precisar ser revisto!
}

/**
 *
 */
void EFotoManager::setInterfaceType(string newInterfaceType)
{
	interfaceType = newInterfaceType;
}

/**
 *
 */
string EFotoManager::getInterfaceType()
{
	return interfaceType;
}

/**
 *
 */
void EFotoManager::setNextModule(int newModule)
{
	nextModule = newModule;
}

/**
 *
 */
void EFotoManager::setNextImage(int newImage)
{
	nextImage = newImage;
}

/**
 *
 */
bool EFotoManager::execProject(string filename)
{
	bool result;
	nextModule = 0;
	if (project == NULL)
	{
		project = new ProjectManager(this);
	}
	result = project->exec(filename);
	return result;
}

/**
 *
 */
bool EFotoManager::reloadProject()
{
	nextModule = 0;
	if (project != NULL)
	{
		if (interiorOrientation != NULL)
		{
			stopIO(interiorOrientation->getId());
		}
		if (spatialRessection != NULL)
		{
			stopSR(spatialRessection->getId());
		}
		return project->reload();
	}
	else
	{
		return false;
	}
}

/**
 *
 */
bool EFotoManager::execIO(int id)
{
	bool result;
	nextModule = 2;
	Image* ioImage = instanceImage(id);
	if (ioImage == NULL)
	{
		return false;
	}
	Sensor* ioSensor = instanceSensor(ioImage->getSensorId());
	InteriorOrientation* io = instanceIO(id);
	if (io == NULL)
	{
		io = new InteriorOrientation(id);
		IOs.push_back(io);
	}
	else
	{

	}
	interiorOrientation = new IOManager(this, ioSensor, ioImage, io);
	result = interiorOrientation->exec();
	/* precisa de um stop
	delete interiorOrientation;
	deleteIO(id);
	deleteSensor(ioImage->getSensorId());
	deleteImage(id);
	*/
	return result;
}

void EFotoManager::stopIO(int id)
{
	Image* ioImage = instanceImage(id);
	delete interiorOrientation;
	interiorOrientation = NULL;
	deleteIO(id);
	deleteSensor(ioImage->getSensorId());
	deleteImage(id);
}

/**
 *
 */
bool EFotoManager::execSR(int id)
{
	bool result;
	nextModule = 2;
	Image* srImage = instanceImage(id);
	if (srImage == NULL)
	{
		return false;
	}
	Sensor* srSensor = instanceSensor(srImage->getSensorId());
	Flight* srFlight = instanceFlight(srImage->getFlightId());
	InteriorOrientation* srIO = instanceIO(id);
	SpatialRessection* sr = (SpatialRessection*) instanceEO(id);
	Terrain* srTerrain = instanceTerrain();
	srFlight->setTerrain(srTerrain);
	if (sr == NULL)
	{
		sr = new SpatialRessection(id);
		EOs.push_back(sr);
	}
	else
	{

	}
	spatialRessection = new SRManager(this, srTerrain, srSensor, srFlight, srImage, srIO, sr);

	result = spatialRessection->exec();
/* precisa de um stop
	delete spatialRessection;
	deleteEO(id);
	deleteIO(id);
	deleteFlight(srImage->getFlightId());
	deleteSensor(srImage->getSensorId());
	deleteTerrain();
	deleteImage(id);
	*/
	return result;
}

void EFotoManager::stopSR(int id)
{
	Image* srImage = instanceImage(id);
	delete spatialRessection;
	spatialRessection = NULL;
	deleteEO(id);
	deleteIO(id);
	deleteFlight(srImage->getFlightId());
	deleteSensor(srImage->getSensorId());
	deleteTerrain();
	deleteImage(id);
}

/**
 *
 */
bool EFotoManager::exec(string filename)
{
	if (filename != "")
	{
		execProject(filename);
	}
	//while (nextModule != 0)
	//{
		switch (nextModule)
		{
		case 1:
			execProject();
			break;
		case 2:
			reloadProject();
			break;
		case 3:
			savedState = true;
			execIO(nextImage);
			break;
		case 4:
			savedState = true;
			execSR(nextImage);
			break;
		default:
			nextModule = 0;
		}
	//}

	return true;
}
