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

double Terrain::getMeanAltitude() const
{
	return meanAltitude;
}

double Terrain::getMinAltitude() const
{
	return minAltitude;
}

double Terrain::getMaxAltitude() const
{
	return maxAltitude;
}

std::string Terrain::getGRS() const
{
	return GRS;
}

std::string Terrain::getCPS() const
{
	return CPS;
}

std::string Terrain::getCentralCoordLat() const
{
	return workAreaCentralCoordinateLat;
}

std::string Terrain::getCentralCoordLong() const
{
	return workAreaCentralCoordinateLong;
}

int Terrain::getUtmFuse() const
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
