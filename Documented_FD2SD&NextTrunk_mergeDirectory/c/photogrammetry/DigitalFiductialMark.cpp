/*******************************************************************************
						DigitalFiductialMark.cpp
**************************************************************************/

#include "DigitalFiductialMark.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

// Constructors
//

/**
 * DigitalFiductialMark's default empty constructor
 */
DigitalFiductialMark::DigitalFiductialMark()
{
	id = 0;
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
DigitalFiductialMark::DigitalFiductialMark(int myId, int myImageId)
{
	id = myId;
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
DigitalFiductialMark::DigitalFiductialMark(int myId, int myImageId, int myCol, int myLin)
{
	id = myId;
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
DigitalFiductialMark::DigitalFiductialMark(int myId, int myImageId, const PositionMatrix& myPosition)
{
	id = myId;
	imageId = myImageId;
	setPosition(myPosition);
	sigmaAvailable = false;
}

/**
 *
 */
DigitalFiductialMark::~DigitalFiductialMark()
{

}

// Private attribute accessor methods
//

/**
 * Set the value of id
 * @param newId the new value of id
 */
void DigitalFiductialMark::setId(int newId)
{
	id = newId;
}

/**
 * Set the value of imageId
 * @param newImageId the new value of imageId
 */
void DigitalFiductialMark::setImageId(int newImageId)
{
	imageId = newImageId;
}

/**
 * Get the value of id
 * @return the value of id
 */
int DigitalFiductialMark::getId()
{
	return id;
}

/**
 * Get the value of imageId
 * @return the value of imageId
 */
int DigitalFiductialMark::getImageId()
{
	return imageId;
}

// EObject methods
//

/**
 *
 */
string DigitalFiductialMark::objectType(void)
{
	stringstream result;
	result << "DigitalFiductialMark " << id;
	return result.str();
}

/**
 *
 */
string DigitalFiductialMark::objectAssociations(void)
{
	return "";
}

/**
 *
 */
bool DigitalFiductialMark::is(string s)
{
	return (s == "DigitalFiductialMark" ? true : false);
}

// XML methods
//

/**
 *
 */
void DigitalFiductialMark::xmlSetData(string xml)
{
	EDomElement root(xml);
	id = Conversion::stringToInt(root.attribute("key"));
	EDomElement xmlPos = root.elementByTagName("gml:pos");
	if (xmlPos.isAvailable())
	{
		deque<double> gmlPosValues = xmlPos.toGmlPos();
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
string DigitalFiductialMark::xmlGetData()
{
	stringstream result;
	result << "<fiductialMark key=\"" << Conversion::intToString(id) << "\">\n";
	if (available)
		result << "<gml:pos>" << Conversion::intToString(col) << " " << Conversion::intToString(lin) << "</gml:pos>\n";
	else
		result << "<gml:pos>Not Available</gml:pos>\n";
	result << "</fiductialMark>";
	return result.str();
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
