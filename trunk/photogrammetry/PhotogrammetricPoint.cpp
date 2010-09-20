/*******************************************************************************
                        PhotogrammetricPoint.cpp
*******************************************************************************/

#include "PhotogrammetricPoint.h"

/**
 *
 */
PhotogrammetricPoint::PhotogrammetricPoint():Point()
{

}

/**
 *
 */
PhotogrammetricPoint::PhotogrammetricPoint(int myId):Point(myId)
{

}

// EObject methods
//

/**
 *
 */
string PhotogrammetricPoint::objectType(void)
{
    stringstream result;
    result << "PhotogrammetricPoint " << id;
    return result.str();
}

/**
 *
 */
string PhotogrammetricPoint::objectAssociations(void)
{
    return "";
}

/**
 *
 */
bool PhotogrammetricPoint::is(string s)
{
    return (s == "PhotogrammetricPoint" ? true : false);
}

// XML methods
//

/**
 *
 */
void PhotogrammetricPoint::xmlSetData(string xml)
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
string PhotogrammetricPoint::xmlGetData()
{
    stringstream result;
    result << "<point key=\"" << intToString(id) << "\" type=\"photogrammetric\">\n";
    result << "<gcp_id>" << longToString(gcpId) << "</gcp_id>\n";
    result << "<description>" << description << "</description>\n";
    result << objectCoordinate.xmlGetData();
    result << "<imagesMeasurements>\n";
    for (unsigned int i = 0; i < digitalCoordinates.size(); i++)
    {
        result << digitalCoordinates.at(i).xmlGetData();
    }
    result << "</imagesMeasurements>\n";
    result << "</point>\n";
    return result.str();
}
