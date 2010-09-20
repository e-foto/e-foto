/*******************************************************************************
                        ControlPoint.cpp
*******************************************************************************/

#include "ControlPoint.h"



/**
 *
 */
ControlPoint::ControlPoint():Point()
{

}

/**
 *
 */
ControlPoint::ControlPoint(int myId):Point(myId)
{

}

// EObject methods
//

/**
 *
 */
string ControlPoint::objectType(void)
{
    stringstream result;
    result << "ControlPoint " << id;
    return result.str();
}

/**
 *
 */
string ControlPoint::objectAssociations(void)
{
    return "";
}

/**
 *
 */
bool ControlPoint::is(string s)
{
    return (s == "ControlPoint" ? true : false);
}

// XML methods
//

/**
 *
 */
void ControlPoint::xmlSetData(string xml)
{
    EDomElement root(xml);
    id = stringToInt(root.attribute("key"));
    gcpId = root.elementByTagName("gcp_id").toLong();
    description = root.elementByTagName("description").toString();
    objectCoordinate.xmlSetData(root.elementByTagName("spatialCoordinates").getContent());
    deque<EDomElement> xmlDigitalCoordinates = root.elementsByTagName("imageCoordinates");
    digitalCoordinates.clear();
    for (unsigned int i = 0; i < xmlDigitalCoordinates.size(); i++)
    {
        DigitalImageSpaceCoordinate* disc = new DigitalImageSpaceCoordinate;
        disc->xmlSetData(xmlDigitalCoordinates.at(i).getContent());
        digitalCoordinates.push_back(*disc);
    }
}

/**
 *
 */
string ControlPoint::xmlGetData()
{
    stringstream result;
    result << "<point key=\"" << intToString(id) << "\" type=\"control\">\n";
    result << "<gcp_id>" << longToString(gcpId) << "</gcp_id>\n";
    result << "<description>" << description << "</description>\n";
    result << objectCoordinate.xmlGetData() << "\n";
    result << "<imagesMeasurements>\n";
    for (unsigned int i = 0; i < digitalCoordinates.size(); i++)
    {
        result << digitalCoordinates.at(i).xmlGetData();
    }
    result << "</imagesMeasurements>\n";
    result << "</point>\n";
    return result.str();
}
