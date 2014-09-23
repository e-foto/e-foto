/*******************************************************************************
		 Image.cpp
*******************************************************************************/
/*Copyright 2002-2014 e-foto team (UERJ)
  This file is part of e-foto.

    e-foto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    e-foto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with e-foto.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Image.h"
#include "Point.h"
#include "SensorWithFiducialMarks.h"
#include "Flight.h"
#include "math.h"

#include <sstream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

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
    flightDirection = 3*M_PI;
    flightDirectionAvailable = false;
    mySensor = NULL;
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
void Image::setResolutionUnit(std::string newResolutionUnit)
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
void Image::setFilename(std::string newFilename)
{
	filename = newFilename;
}

/**
 * Set the value of filepath
 * @param newFilepath the new value of filepath
 */
void Image::setFilepath(std::string newFilepath)
{
	filepath = newFilepath;
}

/**
 * Set the value of flightDirection
 * @param radianAngle the new value of flightDirection
 */
void Image::setFlightDirection(double radianAngle)
{
    flightDirectionAvailable = true;
	flightDirection=radianAngle;
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
std::string Image::getResolutionUnit()
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
std::string Image::getFilename()
{
	return filename;
}

/**
 * Get the value of filepath
 * @return the value of filepath
 */
std::string Image::getFilepath()
{
	return filepath;
}
/**
  * Get the value of flightDirection
  * @return the value of flightDirection
  */
double Image::getFlightDirection()
{
	return flightDirection;
}



/**
 * Get the value of image ID
 * @return the value of image ID
 */
std::string Image::getImageId()
{
	return imageId;
}

bool Image::isGnssAvailable()
{
	return gnssAvailable;
}

bool Image::isInsAvailable()
{
	return insAvailable;
}

double Image::getGnssX0()
{
	return gnssX0;
}

double Image::getGnssY0()
{
	return gnssY0;
}

double Image::getGnssZ0()
{
	return gnssZ0;
}

std::string Image::getGnssType()
{
	return gnssType;
}

double Image::getInsOmega()
{
	return insOmega;
}

double Image::getInsPhi()
{
	return insPhi;
}

double Image::getInsKappa()
{
	return insKappa;
}

std::string Image::getInsType()
{
	return insType;
}


// Composed objects accessor methods
//

/**
 * Set all the values of digFidMarks deque at once
 * @param newDigFidMarks a deque with the new values
 */
void Image::setDigFidMarks(std::deque<ImageFiducialMark> newDigFidMarks)
{
	digFidMarks = newDigFidMarks;
}

/**
 * Get all the values of digFidMarks deque at once
 * @return a deque the values of digFidMarks
 */
std::deque<ImageFiducialMark> Image::getDigFidMarks()
{
	return digFidMarks;
}

/**
 * Add one value to digFidMarks deque
 * @param newDigFidMark the value to be added
 */
void Image::putDigFidMark(ImageFiducialMark newDigFidMark)
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
 * @return the value of the ImageFiducialMark
 */
ImageFiducialMark Image::getDigFidMark(int id)
{
	for (unsigned int i = 0; i < digFidMarks.size(); i++)
		if (digFidMarks.at(i).getId() == id)
			return digFidMarks.at(i);
	return ImageFiducialMark();
}

/**
 * Get one value from digFidMarks deque
 * @param index the position of the value
 * @return the value of the ImageFiducialMark
 */
ImageFiducialMark Image::getDigFidMarkAt(unsigned int index)
{
	if (index < digFidMarks.size())
		return digFidMarks.at(index);
	return ImageFiducialMark();
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

void Image::clearPoints()
{
    myPoints.clear();
}

void Image::removePoint(int id)
{
    for (int i = myPoints.size()-1; i >= 0; i--)
        if (myPoints.at(i)->getId() == id)
            myPoints.erase(myPoints.begin()+i);
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
std::string Image::objectType(void)
{
    std::stringstream result;
	result << "Image " << id;
	return result.str();
}

/**
 *
 */
std::string Image::objectAssociations(void)
{
    std::stringstream result;
	result << mySensor->objectType() << " " << myFlight->objectType();
	return result.str();
}

/**
 *
 */
bool Image::is(std::string s)
{
	return (s == "Image" ? true : false);
}

// XML methods
//

/**
 *
 */
void Image::xmlSetData(std::string xml)
{
	EDomElement root(xml);
	id = Conversion::stringToInt(root.attribute("key"));
	sensorId = Conversion::stringToInt(root.attribute("sensor_key"));
	flightId = Conversion::stringToInt(root.attribute("flight_key"));
	imageId = root.elementByTagName("imageId").toString();
	width = root.elementByTagName("width").toInt();
	height = root.elementByTagName("height").toInt();
	filename = root.elementByTagName("fileName").toString();
	filepath = root.elementByTagName("filePath").toString();
    resolution = root.elementByTagName("resolution").toInt();
    resolutionUnit = root.elementByTagName("resolution").attribute("uom");
    flightDirectionAvailable = root.elementByTagName("flightDirection").getContent() != "";
    if (flightDirectionAvailable)
        flightDirection = root.elementByTagName("flightDirection").toDouble();

	//Isso deve ser corrigido...
	//spatialCoordinates.xmlSetData(root.elementByTagName("spatialCoordinates").getContent());

	EDomElement gnss = root.elementByTagName("GNSS");
    std::deque<double> gnssPos = gnss.elementByTagName("gml:pos").toGmlPos();
	if (gnssPos.size() == 3)
	{
		gnssAvailable = true;
		gnssType = gnss.attribute("type");
		gnssX0 = gnssPos.at(0);
		gnssY0 = gnssPos.at(1);
		gnssZ0 = gnssPos.at(2);
	}
	else
	{
		gnssAvailable = false;
	}
	if (gnss.getContent() == "" || gnss.elementByTagName("sigma").toString() == "Not Available")
	{
		gnssSigmaAvailable = false;
	}
	else
	{
		gnssSigmaAvailable = true;
		gnssSigma.xmlSetData(gnss.elementByTagName("sigma").getContent());
	}

	EDomElement ins = root.elementByTagName("INS");
	if (ins.getContent() == "" || ins.elementByTagName("omega").toString() == "Not Available")
	{
		insAvailable = false;
	}
	else
	{
		insAvailable = true;
		insType = ins.attribute("type");
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
		insSigma.xmlSetData(ins.elementByTagName("sigma").getContent());
	}

	// Fim da gambiarra temporaria.
}

/**
 *
 */
std::string Image::xmlGetData()
{
    std::stringstream result;
	result << "<image key=\"" << Conversion::intToString(id) << "\" sensor_key=\"" << Conversion::intToString(sensorId) << "\" flight_key=\"" << Conversion::intToString(flightId) << "\">\n";
	result << "<imageId>" << imageId << "</imageId>\n";
	result << "<width uom=\"#px\">" << Conversion::intToString(width) << "</width>\n";
	result << "<height uom=\"#px\">" << Conversion::intToString(height) << "</height>\n";
	result << "<fileName>" << filename << "</fileName>\n";
	result << "<filePath>" << filepath << "</filePath>\n";
    result << "<resolution uom=\"" << resolutionUnit << "\">" << Conversion::intToString(resolution) << "</resolution>\n";
    result << "<flightDirection>" << Conversion::doubleToString(flightDirection) << "</flightDirection>\n";

	//Isso deve ser corrigido...
	//result << spatialCoordinates.xmlGetData();


	if (gnssAvailable)
	{
                result << "<GNSS uom=\"#m\" type=\"" << gnssType << "\">\n";
		result << "<gml:pos>" << Conversion::doubleToString(gnssX0) << " " << Conversion::doubleToString(gnssY0) << " " << Conversion::doubleToString(gnssZ0) << "</gml:pos>\n";
	}
	else
	{
		result << "<gml:pos>Not Available</gml:pos>\n";
	}

        if (gnssAvailable && gnssSigmaAvailable)
	{
                result << "<sigma>" << gnssSigma.xmlGetData() << "</sigma>\n";
	}
	else
	{
		result << "<sigma>Not Available</sigma>\n";
	}

        if (gnssAvailable)
            result << "</GNSS>\n";

        if (insAvailable)
	{
                result << "<INS uom=\"#rad\" type=\"" << insType << "\">\n";
                result << "<omega>" << Conversion::doubleToString(insOmega) <<"</sigma>\n";
		result << "<phi>" << Conversion::doubleToString(insPhi) <<"</sigma>\n";
		result << "<kappa>" << Conversion::doubleToString(insKappa) <<"</sigma>\n";
	}
	else
	{
		result << "<omega>Not Available</omega>\n";
		result << "<phi>Not Available</phi>\n";
		result << "<kappa>Not Available</kappa>\n";
	}

        if (insAvailable && insSigmaAvailable)
	{
                result << "<sigma>" << insSigma.xmlGetData() << "</sigma>\n";
	}
	else
	{
		result << "<sigma>Not Available</sigma>\n";
	}

        if (insAvailable)
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

void Image::sortPoints()
{
    std::deque<Point*> listCtrl;

	for(int i=0; i<myPoints.size() ;i++)
	{
		if (myPoints.at(i)->getType() == Point::CONTROL)
			listCtrl.push_front(myPoints.at(i));
		else
			listCtrl.push_back(myPoints.at(i));

		//qDebug("\tponto %d = %s",i, myPoints.at(i)->getPointId().c_str());//pnt->getPointId());
	}
	myPoints=listCtrl;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
