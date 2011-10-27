/*******************************************************************************
                        DigitalImageSpaceCoordinate.cpp
**************************************************************************/

#include "DigitalImageSpaceCoordinate.h"

// Constructors
//

/**
 * DigitalImageSpaceCoordinate's default empty constructor
 */
DigitalImageSpaceCoordinate::DigitalImageSpaceCoordinate()
{
    imageId = 0;
    col = 0;
    lin = 0;
    unit = "";
    available = false;
    sigmaAvailable = false;
}

/**
 *
 */
DigitalImageSpaceCoordinate::DigitalImageSpaceCoordinate(int myImageId)
{
    imageId = myImageId;
    col = 0;
    lin = 0;
    unit = "";
    available = false;
    sigmaAvailable = false;
}

/**
 *
 */
DigitalImageSpaceCoordinate::DigitalImageSpaceCoordinate(int myImageId, int myCol, int myLin)
{
    imageId = myImageId;
    col = myCol;
    lin = myLin;
    unit = "px";
    available = true;
    sigmaAvailable = false;
}

/**
 *
 */
DigitalImageSpaceCoordinate::DigitalImageSpaceCoordinate(int myImageId, const PositionMatrix& myPosition)
{
    imageId = myImageId;
    setPosition(myPosition);
    sigmaAvailable = false;
}

/**
 *
 */
DigitalImageSpaceCoordinate::~DigitalImageSpaceCoordinate()
{

}

// Private attribute accessor methods
//

/**
 * Set the value of imageId
 * @param newImageId the new value of imageId
 */
void DigitalImageSpaceCoordinate::setImageId(int newImageId)
{
    imageId = newImageId;
}

/**
 * Get the value of imageId
 * @return the value of imageId
 */
int DigitalImageSpaceCoordinate::getImageId()
{
    return imageId;
}

// EObject methods
//

/**
 *
 */
string DigitalImageSpaceCoordinate::objectType(void)
{
    return "DigitalImageSpaceCoordinate";
}

/**
 *
 */
string DigitalImageSpaceCoordinate::objectAssociations(void)
{
    return "";
}

/**
 *
 */
bool DigitalImageSpaceCoordinate::is(string s)
{
    return (s == "DigitalImageSpaceCoordinate" ? true : false);
}

// XML methods
//

/**
 * 
 */
void DigitalImageSpaceCoordinate::xmlSetData(string xml)
{
    EDomElement root(xml);
    imageId = Conversion::stringToInt(root.attribute("image_key"));
    unit = root.attribute("uom");
    EDomElement xmlPos = root.elementByTagName("gml:pos");
    if (xmlPos.isAvailable())
    {
        deque<double> gmlPosValues = xmlPos.toGmlPos();
        if (gmlPosValues.size() != 2)
            return;
        col = (int) gmlPosValues.at(0);
        lin = (int) gmlPosValues.at(1);
        available = true;
    }
    else
    {
        available = false;
    }
}

/**
 * 
 */
string DigitalImageSpaceCoordinate::xmlGetData()
{
    stringstream result;
    result << "<imageCoordinates uom=\"" << unit << "\" image_key=\"" << imageId << "\">";
    if (available)
        result << "<gml:pos>" << Conversion::intToString(col) << " " << Conversion::intToString(lin) << "</gml:pos>";
    else
        result << "<gml:pos>Not Available</gml:pos>";
    result << "</imageCoordinates>\n";
    return result.str();
}
