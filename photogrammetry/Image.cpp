/*******************************************************************************
                              Image.cpp
*******************************************************************************/

#include "Image.h"
#include "Point.h"
#include "Aerial.h"
#include "Flight.h"

// Constructors and destructors

/**
 *
 */
Image::Image()
{

}

/**
 *
 */
Image::Image(int myId, int mySensorId)
{
    id = myId;
    sensorId = mySensorId;
}

/**
 *
 */
Image::~Image()
{

}

// Private attribute accessor methods
//

/**
 * Set the value of id
 * @param newId the new value of id
 */
void Image::setId(int newId)
{
    id = newId;
}

/**
 * Set the value of sensorId
 * @param newSensorId the new value of sensorId
 */
void Image::setSensorId(int newSensorId)
{
    sensorId = newSensorId;
}

/**
 * Set the value of flightId
 * @param newFlightId the new value of flightId
 */
void Image::setFlightId(int newFlightId)
{
    flightId = newFlightId;
}

/**
 * Set the value of resolution
 * @param newResolution the new value of resolution
 */
void Image::setResolution(unsigned int newResolution)
{
    resolution = newResolution;
}

/**
 * Set the value of resolutionUnit
 * @param newResolutionUnit the new value of resolutionUnit
 */
void Image::setResolutionUnit(string newResolutionUnit)
{
    resolutionUnit = newResolutionUnit;
}

/**
 * Set the value of width
 * @param newWidth the new value of width
 */
void Image::setWidth(unsigned int newWidth)
{
    width = newWidth;
}

/**
 * Set the value of height
 * @param newHeight the new value of height
 */
void Image::setHeight(unsigned int newHeight)
{
    height = newHeight;
}

/**
 * Set the value of filename
 * @param newFilename the new value of filename
 */
void Image::setFilename(string newFilename)
{
    filename = newFilename;
}

/**
 * Set the value of filepath
 * @param newFilepath the new value of filepath
 */
void Image::setFilepath(string newFilepath)
{
    filepath = newFilepath;
}

/**
 * Get the value of id
 * @return the value of id
 */
int Image::getId()
{
    return id;
}

/**
 * Get the value of sensorId
 * @return the value of sensorId
 */
int Image::getSensorId()
{
    return sensorId;
}

/**
 * Get the value of flightId
 * @return the value of flightId
 */
int Image::getFlightId()
{
    return flightId;
}

/**
 * Get the value of resolution
 * @return the value of resolution
 */
unsigned int Image::getResolution()
{
    return resolution;
}

/**
 * Get the value of resolutionUnit
 * @return the value of resolutionUnit
 */
string Image::getResolutionUnit()
{
    return resolutionUnit;
}

/**
 * Get the value of width
 * @return the value of width
 */
unsigned int Image::getWidth()
{
    return width;
}

/**
 * Get the value of height
 * @return the value of height
 */
unsigned int Image::getHeight()
{
    return height;
}

/**
 * Get the value of filename
 * @return the value of filename
 */
string Image::getFilename()
{
    return filename;
}

/**
 * Get the value of filepath
 * @return the value of filepath
 */
string Image::getFilepath()
{
    return filepath;
}



// Composed objects accessor methods
//

/**
 * Set all the values of digFidMarks deque at once
 * @param newDigFidMarks a deque with the new values
 */
void Image::setDigFidMarks(deque<DigitalFiductialMark> newDigFidMarks)
{
    digFidMarks = newDigFidMarks;
}

/**
 * Get all the values of digFidMarks deque at once
 * @return a deque the values of digFidMarks
 */
deque<DigitalFiductialMark> Image::getDigFidMarks()
{
    return digFidMarks;
}

/**
 * Add one value to digFidMarks deque
 * @param newDigFidMark the value to be added
 */
void Image::putDigFidMark(DigitalFiductialMark newDigFidMark)
{
    if (digFidMarks.empty())
    {
        digFidMarks.push_back(newDigFidMark);
    }
    else
    {
        if (digFidMarks.back().getId() < newDigFidMark.getId())
        {
            digFidMarks.push_back(newDigFidMark);
        }
        else
        {
            for (unsigned int i = 0; i < digFidMarks.size(); i++)
            {
                if (digFidMarks.at(i).getId() > newDigFidMark.getId())
                {
                    digFidMarks.insert(digFidMarks.begin()+i,newDigFidMark);
                    break;
                }
            }
        }
    }
}

/**
 * Get one value from digFidMarks deque
 * @param index the position of the value
 * @return the value of the DigitalFiductialMark
 */
DigitalFiductialMark Image::getDigFidMark(int id)
{
    for (unsigned int i = 0; i < digFidMarks.size(); i++)
	if (digFidMarks.at(i).getId() == id)
            return digFidMarks.at(i);
    return DigitalFiductialMark();
}

/**
 * Get one value from digFidMarks deque
 * @param index the position of the value
 * @return the value of the DigitalFiductialMark
 */
DigitalFiductialMark Image::getDigFidMarkAt(unsigned int index)
{
    if (index < digFidMarks.size())
        return digFidMarks.at(index);
    return DigitalFiductialMark();
}

/**
 * Count the number of value in digFidMarks deque
 * @return the number of values
 */
int Image::countDigFidMarks()
{
    return digFidMarks.size();
}

/**
 * Remove one value from digFidMarks deque
 * @param id the id of the value
 */
void Image::deleteDigFidMark(int id)
{
    for (unsigned int i = 0; i < digFidMarks.size(); i++)
        if (digFidMarks.at(i).getId() == id)
            digFidMarks.erase(digFidMarks.begin()+i);
}

/**
 * Remove one value from digFidMarks deque
 * @param index the position of the value
 */
void Image::deleteDigFidMarkAt(unsigned int index)
{
    if (index < digFidMarks.size())
        digFidMarks.erase(digFidMarks.begin()+index);
}

/**
 * Clear all the values of the digFidMarks deque
 */
void Image::clearDigFidMarks()
{
    digFidMarks.clear();
}


// Associated object accessor methods
//
void Image::setSensor(Sensor* newSensor)
{
    mySensor = newSensor;
}

Sensor* Image::getSensor()
{
    return mySensor;
}

void Image::setFlight(Flight* newFlight)
{
    myFlight = newFlight;
}

Flight* Image::getFlight()
{
    return myFlight;
}

void Image::setIO(InteriorOrientation* newIO)
{
    myIO = newIO;
}

InteriorOrientation* Image::getIO()
{
    return myIO;
}

void Image::setEO(SpatialRessection* newEO)
{
    myEO = newEO;
}

SpatialRessection* Image::getEO()
{
    return myEO;
}

void Image::putPoint(Point* newPointAssociation)
{
    bool insert = true;
    for (unsigned int i = 0; i < myPoints.size(); i++)
        if (myPoints.at(i) == newPointAssociation)
            insert = false;
    if (insert)
        myPoints.push_back(newPointAssociation);
}

Point* Image::getPoint(int pointId)
{
    for (unsigned int i = 0; i < myPoints.size(); i++)
	if (myPoints.at(i)->getId() == pointId)
            return myPoints.at(i);
    return NULL;
}

int Image::countPoints()
{
    return myPoints.size();
}

Point* Image::getPointAt(unsigned int index)
{
    if (index < myPoints.size())
        return myPoints.at(index);
    return NULL;
}

// EObject methods
//

/**
 *
 */
string Image::objectType(void)
{
    stringstream result;
    result << "Image " << id;
    return result.str();
}

/**
 *
 */
string Image::objectAssociations(void)
{
    stringstream result;
    result << mySensor->objectType() << " " << myFlight->objectType();
    return result.str();
}

/**
 *
 */
bool Image::is(string s)
{
    return (s == "Image" ? true : false);
}

// XML methods
//

/**
 * 
 */
void Image::xmlSetData(string xml)
{
    EDomElement root(xml);
    id = stringToInt(root.attribute("key"));
    sensorId = stringToInt(root.attribute("sensor_key"));
    flightId = stringToInt(root.attribute("flight_key"));
	imageId = root.elementByTagName("imageId").toString();
    width = root.elementByTagName("width").toInt();
    height = root.elementByTagName("height").toInt();
    filename = root.elementByTagName("fileName").toString();
    filepath = root.elementByTagName("filePath").toString();
    resolution = root.elementByTagName("resolution").toInt();
    resolutionUnit = root.elementByTagName("resolution").attribute("uom");

    //Isso deve ser corrigido...
    //spatialCoordinates.xmlSetData(root.elementByTagName("spatialCoordinates").getContent());

    EDomElement gnss = root.elementByTagName("GNSS");
    deque<double> gnssPos = gnss.elementByTagName("gml:pos").toGmlPos();
    if (gnssPos.size() == 3)
    {
	gnssAvailable = true;
	gnssX0 = gnssPos.at(0);
	gnssY0 = gnssPos.at(1);
	gnssZ0 = gnssPos.at(2);
    }
    else
    {
	gnssAvailable = false;
    }
    if (gnss.elementByTagName("sigma").toString() == "Not Available")
    {
	gnssSigmaAvailable = false;
    }
    else
    {
	gnssSigmaAvailable = true;
	gnssSigma.xmlSetData(gnss.elementByTagName("sigma").elementByTagName("mml:matrix").getContent());
    }

    EDomElement ins = root.elementByTagName("INS");
    if (gnss.elementByTagName("omega").toString() == "Not Available")
    {
	insAvailable = false;
    }
    else
    {
	insAvailable = true;
	insOmega = ins.elementByTagName("omega").toDouble();
	insPhi = ins.elementByTagName("phi").toDouble();
	insKappa = ins.elementByTagName("kappa").toDouble();
    }
    if (ins.elementByTagName("sigma").toString() == "Not Available")
    {
	insSigmaAvailable = false;
    }
    else
    {
	insSigmaAvailable = true;
	insSigma.xmlSetData(ins.elementByTagName("sigma").elementByTagName("mml:matrix").getContent());
    }

    // Fim da gambiarra temporaria.
}

/**
 * 
 */
string Image::xmlGetData()
{
    stringstream result;
    result << "<image key=\"" << intToString(id) << "\" sensor_key=\"" << intToString(sensorId) << "\" flight_key=\"" << intToString(flightId) << "\">\n";
	result << "<imageId>" << imageId << "</imageId>\n";
    result << "<width uom=\"#px\">" << intToString(width) << "</width>\n";
    result << "<height uom=\"#px\">" << intToString(height) << "</height>\n";
    result << "<fileName>" << filename << "</fileName>\n";
    result << "<filePath>" << filepath << "</filePath>\n";
    result << "<resolution uom=\"" << resolutionUnit << "\">" << intToString(resolution) << "</resolution>\n";

    //Isso deve ser corrigido...
    //result << spatialCoordinates.xmlGetData();

    result << "<GNSS uom=\"#m\">\n";
    if (gnssAvailable)
    {
	result << "<gml:pos>" << doubleToString(gnssX0) << " " << doubleToString(gnssY0) << " " << doubleToString(gnssZ0) << "</gml:pos>\n";
    }
    else
    {
	result << "<gml:pos>Not Available</gml:pos>\n";
    }
    if (gnssSigmaAvailable)
    {
	result << "<sigma>" << gnssSigma.xmlGetData() << "</gml:sigma>\n";
    }
    else
    {
	result << "<sigma>Not Available</sigma>\n";
    }
    result << "</GNSS>\n";

    result << "<INS uom=\"#rad\">\n";
    if (insAvailable)
    {
	result << "<omega>" << doubleToString(insOmega) <<"</sigma>\n";
	result << "<phi>" << doubleToString(insPhi) <<"</sigma>\n";
	result << "<kappa>" << doubleToString(insKappa) <<"</sigma>\n";
    }
    else
    {
	result << "<omega>Not Available</omega>\n";
	result << "<phi>Not Available</phi>\n";
	result << "<kappa>Not Available</kappa>\n";
    }
    if (insSigmaAvailable)
    {
	result << "<sigma>" << insSigma.xmlGetData() << "</gml:sigma>\n";
    }
    else
    {
	result << "<sigma>Not Available</sigma>\n";
    }
    result << "</INS>\n";

    //Fim da gambiarra temporaria.

    result << "</image>\n";
    return result.str();
}

// Other methods
//

/**
 *
 */
int Image::load()
{
    return 0;
}
