/*******************************************************************************
                        CheckingPoint.cpp
*******************************************************************************/

#include "CheckingPoint.h"



/**
 *
 */
CheckingPoint::CheckingPoint():Point()
{

}

/**
 *
 */
CheckingPoint::CheckingPoint(int myId):Point(myId)
{

}

// EObject methods
//

/**
 *
 */
string CheckingPoint::objectType(void)
{
    stringstream result;
    result << "CheckingPoint " << id;
    return result.str();
}

/**
 *
 */
string CheckingPoint::objectAssociations(void)
{
    return "";
}

/**
 *
 */
bool CheckingPoint::is(string s)
{
    return (s == "CheckingPoint" ? true : false);
}

// XML methods
//

/**
 *
 */
void CheckingPoint::xmlSetData(string xml)
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
string CheckingPoint::xmlGetData()
{
    stringstream result;
    result << "<point key=\"" << intToString(id) << "\" type=\"checking\">\n";
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
