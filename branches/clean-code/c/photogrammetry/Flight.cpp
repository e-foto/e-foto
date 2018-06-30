/*******************************************************************************
         Flight.cpp
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

#include "Flight.h"
#include "Image.h"
#include "SensorWithFiducialMarks.h"

#include <sstream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

// Constructors and destructors
//

/**
 *
 */
Flight::Flight():Flight(0,0)
{

}

/**
 *
 */
Flight::Flight(int myId, int mySensorId):
    id_{myId},
    sensorId_{mySensorId}
{
}

/**
 *
 */
Flight::~Flight()
{

}


/**
 * Gets the value of id
 * @return the value of id
 */
int Flight::getId() const
{
    return id_;
}

/**
 * Gets the value of flightId
 * @return the value of flightId
 */
std::string Flight::getFlightId() const {
    return flightId;
}

/**
 * Gets the value of sensorId
 * @return the value of sensorId
 */
int Flight::getSensorId() const
{
    return sensorId_;
}

/**
 * Gets the value of execution
 * @return the value of execution
 */
std::string Flight::getExecution() const
{
    return execution;
}

/**
 * Gets the value of scale
 * @return the value of scale
 */
std::string Flight::getScale() const
{
    return scale;
}

double Flight::getScaleDen() const
{
    return scaleDen;
}

/**
 * Gets the value of height
 * @return the value of height
 */
double Flight::getHeight() const
{
    return height;
}

/**
 * Gets the value of longitudinalOverlap
 * @return the value of longitudinalOverlap
 */
double Flight::getLongitudinalOverlap() const
{
    return longitudinalOverlap;
}

/**
 * Gets the value of transversalOverlap
 * @return the value of transversalOverlap
 */
double Flight::getTransversalOverlap() const
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

void Flight::xmlSetData(std::string xml)
{
    EDomElement root(xml);
    id_ = Conversion::stringToInt(root.attribute("key"));
    sensorId_ = Conversion::stringToInt(root.attribute("sensor_key"));
    flightId = root.elementByTagName("flightId").toString();
    description = root.elementByTagName("description").toString();
    execution = root.elementByTagName("execution").toString();
    producerName = root.elementByTagName("producerName").toString();
    scale = "";
    scale += root.elementsByTagName("mml:mn").at(0).toString();
    scale += ":";
    scale += root.elementsByTagName("mml:mn").at(1).toString();
    scaleDen = root.elementsByTagName("mml:mn").at(1).toDouble();
    height = root.elementByTagName("flightHeight").toDouble();
    longitudinalOverlap = root.elementByTagName("longitudinal").toDouble();
    transversalOverlap = root.elementByTagName("transversal").toDouble();
}

std::string Flight::xmlGetData()
{
    std::stringstream result;
    result << "<flight key=\"" << Conversion::intToString(id_) << "\" sensor_key=\"" << Conversion::intToString(sensorId_) << "\">\n";
    result << "<flightId>" << flightId << "</flightId>\n";
    result << "<description>" << description << "</description>\n";
    result << "<execution>" << execution << "</execution>\n";
    result << "<producerName>" << producerName << "</producerName>\n";
    result << "<nominalScale>\n";
    result << "<mml:mfrac>\n";
    result << "<mml:mn>" << scale.substr(0, scale.find(":")) << "</mml:mn>\n";
    result << "<mml:mn>" << scale.substr(scale.find(":") + 1) << "</mml:mn>\n";
    result << "</mml:mfrac>\n";
    result << "</nominalScale>\n";
    result << "<flightHeight uom=\"#m\">" << Conversion::doubleToString(height) << "</flightHeight>\n";
    result << "<overlap>\n";
    result << "<longitudinal uom=\"#%\">" << Conversion::doubleToString(longitudinalOverlap) << "</longitudinal>\n";
    result << "<transversal uom=\"#%\">" << Conversion::doubleToString(transversalOverlap) << "</transversal>\n";
    result << "</overlap>\n";
    result << "</flight>\n";
    return result.str();
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
