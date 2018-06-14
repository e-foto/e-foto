/*******************************************************************************
   Terrain.cpp
*******************************************************************************/

#include "Terrain.h"

#include <sstream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

Terrain::~Terrain()
{
}

void Terrain::setMeanAltitude(double newMeanAltitude)
{
	meanAltitude = newMeanAltitude;
}

void Terrain::setMeanAltitudeUnit(std::string newMeanAltitudeUnit)
{
	meanAltitudeUnit = newMeanAltitudeUnit;
}

void Terrain::setMinAltitude(double newMinAltitude)
{
	minAltitude = newMinAltitude;
}

void Terrain::setMinAltitudeUnit(std::string newMinAltitudeUnit)
{
	minAltitudeUnit = newMinAltitudeUnit;
}

void Terrain::setMaxAltitude(double newMaxAltitude)
{
	maxAltitude = newMaxAltitude;
}

void Terrain::setMaxAltitudeUnit(std::string newMaxAltitudeUnit)
{
	maxAltitudeUnit = newMaxAltitudeUnit;
}

void Terrain::setGRS(std::string newGRS)
{
	GRS = newGRS;
}

void Terrain::setCPS(std::string newCPS)
{
	CPS = newCPS;
}

void Terrain::setCentralCoordLat(std::string newCentralCoordLat)
{
	workAreaCentralCoordinateLat = newCentralCoordLat;
}

void Terrain::setCentralCoordLong(std::string newCentralCoordLong)
{
	workAreaCentralCoordinateLong = newCentralCoordLong;
}

void Terrain::setUtmFuse(int newUtmFuse)
{
	utmFuse = newUtmFuse;
}

double Terrain::getMeanAltitude()
{
	return meanAltitude;
}

std::string Terrain::getMeanAltitudeUnit()
{
	return meanAltitudeUnit;
}

double Terrain::getMinAltitude()
{
	return minAltitude;
}

std::string Terrain::getMinAltitudeUnit()
{
	return minAltitudeUnit;
}

double Terrain::getMaxAltitude()
{
	return maxAltitude;
}

std::string Terrain::getMaxAltitudeUnit()
{
	return maxAltitudeUnit;
}

std::string Terrain::getGRS()
{
	return GRS;
}

std::string Terrain::getCPS()
{
	return CPS;
}

std::string Terrain::getCentralCoordLat()
{
	return workAreaCentralCoordinateLat;
}

std::string Terrain::getCentralCoordLong()
{
	return workAreaCentralCoordinateLong;
}

int Terrain::getUtmFuse()
{
	return utmFuse;
}

void Terrain::xmlSetData(std::string xml)
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

std::string Terrain::xmlGetData()
{
    std::stringstream result;
	result << "<terrain>\n";
    result << "<meanAltitude uom=\"" << meanAltitudeUnit << "\">" << Conversion::doubleToString(meanAltitude) << "</meanAltitude>\n";
    result << "<minAltitude uom=\"" << minAltitudeUnit << "\">" << Conversion::doubleToString(minAltitude) << "</minAltitude>\n";
    result << "<maxAltitude uom=\"" << maxAltitudeUnit << "\">" << Conversion::doubleToString(maxAltitude) << "</maxAltitude>\n";
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

    result << "<utmFuse>" << Conversion::intToString(utmFuse) << "</utmFuse>\n";
	result << "</workAreaCenterCoordinates>\n";
	result << "</terrain>\n";
	return result.str();
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
