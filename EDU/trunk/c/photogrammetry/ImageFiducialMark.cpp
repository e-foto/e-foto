/*******************************************************************************
   ImageFiducialMark.cpp
**************************************************************************/

#include "ImageFiducialMark.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

ImageFiducialMark::ImageFiducialMark()
{
	id = 0;
	imageId = 0;
	col = 0;
	lin = 0;
	unit = "";
	available = false;
	sigmaAvailable = false;
}

ImageFiducialMark::ImageFiducialMark(int myId, int myImageId)
{
	id = myId;
	imageId = myImageId;
	col = 0;
	lin = 0;
	unit = "";
	available = false;
	sigmaAvailable = false;
}

ImageFiducialMark::ImageFiducialMark(int myId, int myImageId, double myCol, double myLin)
{
	id = myId;
	imageId = myImageId;
	col = myCol;
	lin = myLin;
	unit = "px";
	available = true;
	sigmaAvailable = false;
}

ImageFiducialMark::ImageFiducialMark(int myId, int myImageId, const PositionMatrix& myPosition)
{
	id = myId;
	imageId = myImageId;
	setPosition(myPosition);
	sigmaAvailable = false;
}

ImageFiducialMark::~ImageFiducialMark()
{

}

void ImageFiducialMark::setId(int newId)
{
	id = newId;
}

void ImageFiducialMark::setImageId(int newImageId)
{
	imageId = newImageId;
}

int ImageFiducialMark::getId()
{
	return id;
}

int ImageFiducialMark::getImageId()
{
	return imageId;
}

string ImageFiducialMark::objectType(void)
{
	stringstream result;
	result << "ImageFiducialMark " << id;
	return result.str();
}

string ImageFiducialMark::objectAssociations(void)
{
	return "";
}

bool ImageFiducialMark::is(string s)
{
	return (s == "ImageFiducialMark" ? true : false);
}

void ImageFiducialMark::xmlSetData(string xml)
{
	EDomElement root(xml);
	id = Conversion::stringToInt(root.attribute("key"));
	EDomElement xmlPos = root.elementByTagName("gml:pos");
	if (xmlPos.isAvailable())
	{
		deque<double> gmlPosValues = xmlPos.toGmlPos();
		col = gmlPosValues.at(0);
		lin = gmlPosValues.at(1);
		available = true;
	}
	else
	{
		available = false;
	}
}

string ImageFiducialMark::xmlGetData()
{
	stringstream result;
	result << "<fiducialMark key=\"" << Conversion::intToString(id) << "\">\n";
	if (available)
		result << "<gml:pos>" << Conversion::doubleToString(col) << " " << Conversion::doubleToString(lin) << "</gml:pos>\n";
	else
		result << "<gml:pos>Not Available</gml:pos>\n";
	result << "</fiducialMark>";
	return result.str();
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
