/*******************************************************************************
                                 Terrain.cpp
*******************************************************************************/

#include "Terrain.h"


// Constructors and Destructor
//

/**
 *
 */
Terrain::~Terrain()
{
}

// Private attribute accessor methods
//

/**
 * 
 */
void Terrain::setMeanAltitude(double newMeanAltitude)
{
    meanAltitude = newMeanAltitude;
}

/**
 * 
 */
void Terrain::setMeanAltitudeUnit(string newMeanAltitudeUnit)
{
    meanAltitudeUnit = newMeanAltitudeUnit;
}

/**
 * 
 */
void Terrain::setMinAltitude(double newMinAltitude)
{
    minAltitude = newMinAltitude;
}

/**
 * 
 */
void Terrain::setMinAltitudeUnit(string newMinAltitudeUnit)
{
    minAltitudeUnit = newMinAltitudeUnit;
}

/**
 * 
 */
void Terrain::setMaxAltitude(double newMaxAltitude)
{
    maxAltitude = newMaxAltitude;
}

/**
 * 
 */
void Terrain::setMaxAltitudeUnit(string newMaxAltitudeUnit)
{
    maxAltitudeUnit = newMaxAltitudeUnit;
}

/**
 * 
 */
void Terrain::setGRS(string newGRS)
{
    GRS = newGRS;
}

/**
 * 
 */
void Terrain::setCPS(string newCPS)
{
    CPS = newCPS;
}

/**
 * 
 */
void Terrain::setCentralCoordLat(string newCentralCoordLat)
{
    workAreaCentralCoordinateLat = newCentralCoordLat;
}

/**
 * 
 */
void Terrain::setCentralCoordLong(string newCentralCoordLong)
{
    workAreaCentralCoordinateLong = newCentralCoordLong;
}

/**
 * 
 */
void Terrain::setUtmFuse(int newUtmFuse)
{
    utmFuse = newUtmFuse;
}

/**
 * 
 */
double Terrain::getMeanAltitude()
{
    return meanAltitude;
}

/**
 * 
 */
string Terrain::getMeanAltitudeUnit()
{
    return meanAltitudeUnit;
}

/**
 * 
 */
double Terrain::getMinAltitude()
{
    return minAltitude;
}

/**
 * 
 */
string Terrain::getMinAltitudeUnit()
{
    return minAltitudeUnit;
}

/**
 * 
 */
double Terrain::getMaxAltitude()
{
    return maxAltitude;
}

/**
 * 
 */
string Terrain::getMaxAltitudeUnit()
{
    return maxAltitudeUnit;
}

/**
 * 
 */
string Terrain::getGRS()
{
    return GRS;
}

/**
 * 
 */
string Terrain::getCPS()
{
    return CPS;
}

/**
 * 
 */
string Terrain::getCentralCoordLat()
{
    return workAreaCentralCoordinateLat;
}

/**
 * 
 */
string Terrain::getCentralCoordLong()
{
    return workAreaCentralCoordinateLong;
}

/**
 * 
 */
int Terrain::getUtmFuse()
{
    return utmFuse;
}



// EObject methods
//

/**
 *
 */
string Terrain::objectType(void)
{
    return "Terrain";
}

/**
 *
 */
string Terrain::objectAssociations(void)
{
    return "";
}

/**
 *
 */
bool Terrain::is(string s)
{
    return (s == "Terrain" ? true : false);
}

// XML Methods
//

/**
 * 
 */
void Terrain::xmlSetData(string xml)
{
    EDomElement root(xml);
    meanAltitudeUnit = root.elementByTagName("meanAltitude").attribute("uom");
    meanAltitude = root.elementByTagName("meanAltitude").toDouble();
    minAltitudeUnit = root.elementByTagName("minAltitude").attribute("uom");
    minAltitude = root.elementByTagName("minAltitude").toDouble();
    maxAltitudeUnit = root.elementByTagName("maxAltitude").attribute("uom");
    maxAltitude = root.elementByTagName("maxAltitude").toDouble();
    GRS = root.elementByTagName("GRS").toString();
    CPS = root.elementByTagName("CPS").toString();

    EDomElement xmlLat = root.elementByTagName("Lat");
    workAreaCentralCoordinateLat = "";
    workAreaCentralCoordinateLat += xmlLat.elementByTagName("degrees").toString();
    workAreaCentralCoordinateLat += " ";
    workAreaCentralCoordinateLat += xmlLat.elementByTagName("minutes").toString();
    workAreaCentralCoordinateLat += " ";
    workAreaCentralCoordinateLat += xmlLat.elementByTagName("seconds").toString();
    workAreaCentralCoordinateLat += " ";
    workAreaCentralCoordinateLat += xmlLat.attribute("direction");

    EDomElement xmlLong = root.elementByTagName("Long");
    workAreaCentralCoordinateLong = "";
    workAreaCentralCoordinateLong += xmlLong.elementByTagName("degrees").toString();
    workAreaCentralCoordinateLong += " ";
    workAreaCentralCoordinateLong += xmlLong.elementByTagName("minutes").toString();
    workAreaCentralCoordinateLong += " ";
    workAreaCentralCoordinateLong += xmlLong.elementByTagName("seconds").toString();
    workAreaCentralCoordinateLong += " ";
    workAreaCentralCoordinateLong += xmlLong.attribute("direction");

    utmFuse = root.elementByTagName("utmFuse").toInt();
}

/**
 * 
 */
string Terrain::xmlGetData()
{
    stringstream result;
    result << "<terrain>\n";
    result << "<meanAltitude uom=\"" << meanAltitudeUnit << "\">" << doubleToString(meanAltitude) << "</meanAltitude>\n";
    result << "<minAltitude uom=\"" << minAltitudeUnit << "\">" << doubleToString(minAltitude) << "</minAltitude>\n";
    result << "<maxAltitude uom=\"" << maxAltitudeUnit << "\">" << doubleToString(maxAltitude) << "</maxAltitude>\n";
    result << "<GRS>" << GRS << "</GRS>\n";
    result << "<CPS>" << CPS << "</CPS>\n";
    result << "<workAreaCenterCoordinates>\n";

    int firstDiv, secDiv, thirdDiv;
    firstDiv = workAreaCentralCoordinateLat.find(" ");
    secDiv = workAreaCentralCoordinateLat.find(" ", firstDiv + 1);
    thirdDiv = workAreaCentralCoordinateLat.rfind(" ");
    result << "<Lat direction=\"" << workAreaCentralCoordinateLat.substr(thirdDiv + 1) << "\">\n";
    result << "<degrees>" << workAreaCentralCoordinateLat.substr(0, firstDiv)  << "</degrees>\n";
    result << "<minutes>" << workAreaCentralCoordinateLat.substr(firstDiv + 1, secDiv - firstDiv) << "</minutes>\n";
    result << "<seconds>" << workAreaCentralCoordinateLat.substr(secDiv + 1, thirdDiv - secDiv) << "</seconds>\n";
    result << "</Lat>\n";
    firstDiv = workAreaCentralCoordinateLong.find(" ");
    secDiv = workAreaCentralCoordinateLong.find(" ", firstDiv + 1);
    thirdDiv = workAreaCentralCoordinateLong.rfind(" ");
    result << "<Long direction=\"" << workAreaCentralCoordinateLong.substr(thirdDiv + 1) << "\">\n";
    result << "<degrees>" << workAreaCentralCoordinateLong.substr(0, firstDiv)  << "</degrees>\n";
    result << "<minutes>" << workAreaCentralCoordinateLong.substr(firstDiv + 1, secDiv - firstDiv) << "</minutes>\n";
    result << "<seconds>" << workAreaCentralCoordinateLong.substr(secDiv + 1, thirdDiv - secDiv) << "</seconds>\n";
    result << "</Long>\n";

    result << "<utmFuse>" << intToString(utmFuse) << "</utmFuse>\n";
    result << "</workAreaCenterCoordinates>\n";
    result << "</terrain>\n";
    return result.str();
}
