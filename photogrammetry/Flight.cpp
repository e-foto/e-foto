/*******************************************************************************
                              Flight.cpp
*******************************************************************************/

#include "Flight.h"
#include "Image.h"
#include "Aerial.h"

// Constructors and destructors
//

/**
 *
 */
Flight::Flight()
{

}

/**
 *
 */
Flight::Flight(int myId, int mySensorId)
{
    id = myId;
    sensorId = mySensorId;
}

/**
 *
 */
Flight::~Flight()
{

}

// Private attribute accessor methods
//

/**
 * Sets the value of id
 * @param newId the new value of id
 */
void Flight::setId(int newId)
{
    id = newId;
}

/**
 * Sets the value of sensorId
 * @param newSensorId the new value of sensorId
 */
void Flight::setSensorId(int newSensorId)
{
    sensorId = newSensorId;
}

/**
 * Sets the value of description
 * @param newDescription the new value of description
 */
void Flight::setDescription(string newDescription)
{
    description = newDescription;
}

/**
 * Sets the value of execution
 * @param newExecution the new value of execution
 */
void Flight::setExecution(string newExecution)
{
    execution = newExecution;
}

/**
 * Sets the value of producerName
 * @param newProducerName the new value of producerName
 */
void Flight::setProducerName(string newProducerName)
{
    producerName = newProducerName;
}

/**
 * Sets the value of scale
 * @param newScale the new value of scale
 */
void Flight::setScale(string newScale)
{
    scale = newScale;
}

/**
 * Sets the value of height
 * @param newHeight the new value of height
 */
void Flight::setHeight(double newHeight)
{
    height = newHeight;
}

/**
 * Sets the value of longitudinalOverlap
 * @param newLongitudinalOverlap the new value of longitudinalOverlap
 */
void Flight::setLongitudinalOverlap(double newLongitudinalOverlap)
{
    longitudinalOverlap = newLongitudinalOverlap;
}

/**
 * Sets the value of transversalOverlap
 * @param newTransversalOverlap the new value of transversalOverlap
 */
void Flight::setTransversalOverlap(double newTransversalOverlap)
{
    transversalOverlap = newTransversalOverlap;
}

/**
 * Gets the value of id
 * @return the value of id
 */
int Flight::getId()
{
    return id;
}

/**
 * Gets the value of sensorId
 * @return the value of sensorId
 */
int Flight::getSensorId()
{
    return sensorId;
}

/**
 * Gets the value of description
 * @return the value of description
 */
string Flight::getDescription()
{
    return description;
}

/**
 * Gets the value of execution
 * @return the value of execution
 */
string Flight::getExecution()
{
    return execution;
}

/**
 * Gets the value of producerName
 * @return the value of producerName
 */
string Flight::getProducerName()
{
    return producerName;
}

/**
 * Gets the value of scale
 * @return the value of scale
 */
string Flight::getScale()
{
    return scale;
}

/**
 * Gets the value of height
 * @return the value of height
 */
double Flight::getHeight()
{
    return height;
}

/**
 * Gets the value of longitudinalOverlap
 * @return the value of longitudinalOverlap
 */
double Flight::getLongitudinalOverlap()
{
    return longitudinalOverlap;
}

/**
 * Gets the value of transversalOverlap
 * @return the value of transversalOverlap
 */
double Flight::getTransversalOverlap()
{
    return transversalOverlap;
}

// Associated objects accessor methods
//

/**
 * Sets the value of myTerrain
 * @param newTerrain the new value of myTerrain
 */
void Flight::setTerrain(Terrain* newTerrain)
{
	myTerrain = newTerrain;
}

/**
 * Gets the value of myTerrain
 * @return the value of myTerrain
 */
Terrain* Flight::getTerrain()
{
	return myTerrain;
}

/**
 * Sets the value of mySensor
 * @param newSensor the new value of mySensor
 */
void Flight::setSensor(Sensor* newSensor)
{
    mySensor = newSensor;
}

/**
 * Gets the value of mySensor
 * @return the value of mySensor
 */
Sensor* Flight::getSensor()
{
    return mySensor;
}

/**
 *
 */
void Flight::putImage(Image* newImageAssociation)
{
    bool insert = true;
    for (unsigned int i = 0; i < myImages.size(); i++)
        if (myImages.at(i) == newImageAssociation)
            insert = false;
    if (insert)
        myImages.push_back(newImageAssociation);
}

/**
 *
 */
Image* Flight::getImage(int imageId)
{
    for (unsigned int i = 0; i < myImages.size(); i++)
        if (myImages.at(i)->getId() == imageId)
            return myImages.at(i);
    return NULL;
}

/**
 *
 */
int Flight::countImages()
{
    return myImages.size();
}

/**
 *
 */
Image* Flight::getImageAt(unsigned int index)
{
    if (index < myImages.size())
        return myImages.at(index);
    return NULL;
}

// EObject methods
//

/**
 *
 */
string Flight::objectType(void)
{
    stringstream result;
    result << "Flight " << id;
    return result.str();
}

/**
 *
 */
string Flight::objectAssociations(void)
{
    return mySensor->objectType();
}

/**
 *
 */
bool Flight::is(string s)
{
    return (s == "Flight" ? true : false);
}

// XML Methods
//

/**
 * 
 */
void Flight::xmlSetData(string xml)
{
    EDomElement root(xml);
    id = stringToInt(root.attribute("key"));
    sensorId = stringToInt(root.attribute("sensor_key"));
    description = root.elementByTagName("description").toString();
    execution = root.elementByTagName("execution").toString();
    producerName = root.elementByTagName("producerName").toString();
    scale = "";
    scale += root.elementsByTagName("mml:mn").at(0).toString();
    scale += ":";
    scale += root.elementsByTagName("mml:mn").at(1).toString();
    height = root.elementByTagName("flightHeight").toDouble();
    longitudinalOverlap = root.elementByTagName("longitudinal").toDouble();
    transversalOverlap = root.elementByTagName("transversal").toDouble();
}

/**
 * 
 */
string Flight::xmlGetData()
{
    stringstream result;
    result << "<flight key=\"" << intToString(id) << "\" sensor_key=\"" << intToString(sensorId) << "\">\n";
    result << "<description>" << description << "</description>\n";
    result << "<execution>" << execution << "</execution>\n";
    result << "<producerName>" << producerName << "</producerName>\n";
    result << "<nominalScale>\n";
    result << "<mml:mfrac>\n";
    result << "<mml:mn>" << scale.substr(0, scale.find(":")) << "</mml:mn>\n";
    result << "<mml:mn>" << scale.substr(scale.find(":") + 1) << "</mml:mn>\n";
    result << "</mml:mfrac>\n";
    result << "</nominalScale>\n";
    result << "<flightHeight uom=\"#m\">" << doubleToString(height) << "</flightHeight>\n";
    result << "<overlap>\n";
    result << "<longitudinal uom=\"#%\">" << doubleToString(longitudinalOverlap) << "</longitudinal>\n";
    result << "<transversal uom=\"#%\">" << doubleToString(transversalOverlap) << "</transversal>\n";
    result << "</overlap>\n";
    result << "</flight>\n";
    return result.str();
}
