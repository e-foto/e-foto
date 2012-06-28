uo/*******************************************************************************
   ImageSpaceCoordinate.cpp
**************************************************************************/

#include "ImageSpaceCoordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

ImageSpaceCoordinate::ImageSpaceCoordinate()
{
	imageId = 0;
	col = 0;
	lin = 0;
	unit = "";
	available = false;
	sigmaAvailable = false;
}

ImageSpaceCoordinate::ImageSpaceCoordinate(int myImageId)
{
	imageId = myImageId;
	col = 0;
	lin = 0;
	unit = "";
	available = false;
	sigmaAvailable = false;
}

ImageSpaceCoordinate::ImageSpaceCoordinate(int myImageId, double myCol, double myLin)
{
	imageId = myImageId;
	col = myCol;
	lin = myLin;
	unit = "px";
	available = true;
	sigmaAvailable = false;
}

ImageSpaceCoordinate::ImageSpaceCoordinate(int myImageId, const PositionMatrix& myPosition)
{
	imageId = myImageId;
	setPosition(myPosition);
	sigmaAvailable = false;
}

ImageSpaceCoordinate::~ImageSpaceCoordinate()
{

}

void ImageSpaceCoordinate::setImageId(int newImageId)
{
	imageId = newImageId;
}

int ImageSpaceCoordinate::getImageId()
{
	return imageId;
}

string ImageSpaceCoordinate::objectType(void)
{
	return "ImageSpaceCoordinate";
}

string ImageSpaceCoordinate::objectAssociations(void)
{
	return "";
}

bool ImageSpaceCoordinate::is(string s)
{
	return (s == "ImageSpaceCoordinate" ? true : false);
}

void ImageSpaceCoordinate::xmlSetData(string xml)
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
		col = gmlPosValues.at(0);
		lin = gmlPosValues.at(1);
		available = true;
	}
	else
	{
		available = false;
	}
}

string ImageSpaceCoordinate::xmlGetData()
{
	stringstream result;
    result << "<imageCoordinates uom=\"#" << unit << "\" image_key=\"" << imageId << "\">";
	if (available)
		result << "<gml:pos>" << Conversion::doubleToString(col) << " " << Conversion::doubleToString(lin) << "</gml:pos>";
	else
		result << "<gml:pos>Not Available</gml:pos>";
	result << "</imageCoordinates>\n";
	return result.str();
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
