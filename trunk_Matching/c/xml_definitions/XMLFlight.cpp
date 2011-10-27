#include "XMLFlight.h"

XMLFlight::XMLFlight()
{
}

void XMLFlight::xmlSetData(string xml)
{
    EDomElement root(xml);

    setId(Conversion::stringToInt(root.attribute("key")));
    setSensorId(Conversion::stringToInt(root.attribute("sensor_key")));

    //questionar sobre esse metodo
    //flightId = root.elementByTagName("flightId").toString();

    setDescription(root.elementByTagName("description").toString());
    setExecution(root.elementByTagName("execution").toString());
    setProducerName(root.elementByTagName("producerName").toString());
    string scale;
    scale = "";
    scale += root.elementsByTagName("mml:mn").at(0).toString();
    scale += ":";
    scale += root.elementsByTagName("mml:mn").at(1).toString();
    setScale(scale);

    setHeight(root.elementByTagName("flightHeight").toDouble());
    setLongitudinalOverlap(root.elementByTagName("longitudinal").toDouble());
    setTransversalOverlap(root.elementByTagName("transversal").toDouble());
}

string XMLFlight::xmlGetData()
{
    stringstream result;
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
