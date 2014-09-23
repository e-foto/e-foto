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
#include "XMLFlight.h"

#include "EDom.h"

#include <sstream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

XMLFlight::XMLFlight()
{
}

void XMLFlight::xmlSetData(std::string xml)
{
	EDomElement root(xml);

	setId(Conversion::stringToInt(root.attribute("key")));
	setSensorId(Conversion::stringToInt(root.attribute("sensor_key")));

	//questionar sobre esse metodo
	//flightId = root.elementByTagName("flightId").toString();

	setDescription(root.elementByTagName("description").toString());
	setExecution(root.elementByTagName("execution").toString());
	setProducerName(root.elementByTagName("producerName").toString());
    std::string scale;
	scale = "";
	scale += root.elementsByTagName("mml:mn").at(0).toString();
	scale += ":";
	scale += root.elementsByTagName("mml:mn").at(1).toString();
	setScale(scale);

	setHeight(root.elementByTagName("flightHeight").toDouble());
	setLongitudinalOverlap(root.elementByTagName("longitudinal").toDouble());
	setTransversalOverlap(root.elementByTagName("transversal").toDouble());
}

std::string XMLFlight::xmlGetData()
{
    std::stringstream result;
	result << "<flight key=\"" << getId() << "\" sensor_key=\"" << getSensorId() << "\">\n";

	//questionar sobre essa linha
	//result << "<flightId>" << flightId << "</flightId>\n";

	result << "<description>" << getDescription() << "</description>\n";
	result << "<execution>" << getExecution() << "</execution>\n";
	result << "<producerName>" << getProducerName() << "</producerName>\n";
	result << "<nominalScale>\n";
	result << "<mml:mfrac>\n";
	result << "<mml:mn>" << getScale().substr(0, getScale().find(":")) << "</mml:mn>\n";
	result << "<mml:mn>" << getScale().substr(getScale().find(":") + 1) << "</mml:mn>\n";
	result << "</mml:mfrac>\n";
	result << "</nominalScale>\n";
	result << "<flightHeight uom=\"#m\">" << getHeight() << "</flightHeight>\n";
	result << "<overlap>\n";
	result << "<longitudinal uom=\"#%\">" << getLongitudinalOverlap() << "</longitudinal>\n";
	result << "<transversal uom=\"#%\">" << getTransversalOverlap() << "</transversal>\n";
	result << "</overlap>\n";
	result << "</flight>\n";
	return result.str();
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
