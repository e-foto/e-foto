/*******************************************************************************
                                  EFotoManager.cpp
*******************************************************************************/

#include "EFotoManager.h"
#include "ProjectManager.h"

// Constructors and destructor
//

/**
 *
 */
EFotoManager::EFotoManager()
{
    xmlData = "";
    project = NULL;
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
        xmlSensor.elementByTagName("platform").toString().compare("aerial") == 0 &&
        xmlSensor.elementByTagName("detector").toString().compare("film") == 0)
    {
        Aerial* newAerial = new Aerial();
        newAerial->xmlSetData(xmlSensor.getContent());
        sensors.push_back(newAerial);
        return (Sensor*) newAerial;
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
            Aerial* myAerial = (Aerial*) mySensor;
            delete(myAerial);
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
bool EFotoManager::execProject()
{
    bool result;
    nextModule = 0;
    if (project == NULL)
    {
        project = new ProjectManager(this);
    }
    result = project->exec();
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
bool EFotoManager::exec()
{
    while (nextModule != 0)
    {
        switch (nextModule)
        {
        case 1:
            execProject();
            break;
        case 2:
			reloadProject();
            break;
        default:
            nextModule = 0;
        }
    }
    return true;
}
