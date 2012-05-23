#include "XMLTerrain.h"


namespace br {
namespace uerj {
namespace eng {
namespace efoto {

XMLTerrain::XMLTerrain()
{
}

void XMLTerrain :: xmlSetData(string xml)
{
	EDomElement root(xml);

	setMeanAltitudeUnit(root.elementByTagName("meanAltitude").attribute("uom"));
	setMeanAltitude(root.elementByTagName("meanAltitude").toDouble());
	setMinAltitudeUnit(root.elementByTagName("minAltitude").attribute("uom"));
	setMinAltitude(root.elementByTagName("minAltitude").toDouble());
	setMaxAltitudeUnit(root.elementByTagName("maxAltitude").attribute("uom"));
	setMaxAltitude(root.elementByTagName("maxAltitude").toDouble());
	setGRS(root.elementByTagName("GRS").toString());
	setCPS(root.elementByTagName("CPS").toString());

	EDomElement xmlLat = root.elementByTagName("Lat");
	string workAreaCentralCoordinateLat;
	workAreaCentralCoordinateLat = "";
	workAreaCentralCoordinateLat += xmlLat.elementByTagName("degrees").toString();
	workAreaCentralCoordinateLat += " ";
	workAreaCentralCoordinateLat += xmlLat.elementByTagName("minutes").toString();
	workAreaCentralCoordinateLat += " ";
	workAreaCentralCoordinateLat += xmlLat.elementByTagName("seconds").toString();
	workAreaCentralCoordinateLat += " ";
	workAreaCentralCoordinateLat += xmlLat.attribute("direction");

	setCentralCoordLat(workAreaCentralCoordinateLat);

	EDomElement xmlLong = root.elementByTagName("Long");
	string workAreaCentralCoordinateLong;

	workAreaCentralCoordinateLong = "";
	workAreaCentralCoordinateLong += xmlLong.elementByTagName("degrees").toString();
	workAreaCentralCoordinateLong += " ";
	workAreaCentralCoordinateLong += xmlLong.elementByTagName("minutes").toString();
	workAreaCentralCoordinateLong += " ";
	workAreaCentralCoordinateLong += xmlLong.elementByTagName("seconds").toString();
	workAreaCentralCoordinateLong += " ";
	workAreaCentralCoordinateLong += xmlLong.attribute("direction");

	setCentralCoordLong(workAreaCentralCoordinateLong);
	setUtmFuse(root.elementByTagName("utmFuse").toInt());
}

string XMLTerrain ::xmlGetData()
{
	stringstream result;

	result << "<terrain>\n";
	result << "<meanAltitude uom=\"" << getMeanAltitudeUnit() << "\">" << getMeanAltitude() << "</meanAltitude>\n";
	result << "<minAltitude uom=\"" << getMinAltitudeUnit() << "\">" << getMinAltitude() << "</minAltitude>\n";
	result << "<maxAltitude uom=\"" << getMaxAltitudeUnit() << "\">" << getMaxAltitude() << "</maxAltitude>\n";
	result << "<GRS>" << getGRS() << "</GRS>\n";
	result << "<CPS>" << getCPS() << "</CPS>\n";
	result << "<workAreaCenterCoordinates>\n";

	int firstDiv, secDiv, thirdDiv;
	firstDiv = getCentralCoordLat().find(" ");
	secDiv = getCentralCoordLat().find(" ", firstDiv + 1);
	thirdDiv = getCentralCoordLat().rfind(" ");
	result << "<Lat direction=\"" << getCentralCoordLat().substr(thirdDiv + 1) << "\">\n";
	result << "<degrees>" << getCentralCoordLat().substr(0, firstDiv)  << "</degrees>\n";
	result << "<minutes>" << getCentralCoordLat().substr(firstDiv + 1, secDiv - firstDiv) << "</minutes>\n";
	result << "<seconds>" << getCentralCoordLat().substr(secDiv + 1, thirdDiv - secDiv) << "</seconds>\n";
	result << "</Lat>\n";

	firstDiv = getCentralCoordLong().find(" ");
	secDiv = getCentralCoordLong().find(" ", firstDiv + 1);
	thirdDiv = getCentralCoordLong().rfind(" ");
	result << "<Long direction=\"" << getCentralCoordLong().substr(thirdDiv + 1) << "\">\n";
	result << "<degrees>" << getCentralCoordLong().substr(0, firstDiv)  << "</degrees>\n";
	result << "<minutes>" << getCentralCoordLong().substr(firstDiv + 1, secDiv - firstDiv) << "</minutes>\n";
	result << "<seconds>" << getCentralCoordLong().substr(secDiv + 1, thirdDiv - secDiv) << "</seconds>\n";
	result << "</Long>\n";

	result << "<utmFuse>" << getUtmFuse() << "</utmFuse>\n";
	result << "</workAreaCenterCoordinates>\n";
	result << "</terrain>\n";
	return result.str();

}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
