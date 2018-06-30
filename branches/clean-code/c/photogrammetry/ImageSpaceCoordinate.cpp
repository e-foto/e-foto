/*******************************************************************************
   ImageSpaceCoordinate.cpp
**************************************************************************/
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
#include "ImageSpaceCoordinate.h"

#include <sstream>

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

int ImageSpaceCoordinate::getImageId() const
{
	return imageId;
}

void ImageSpaceCoordinate::xmlSetData(std::string xml)
{
	EDomElement root(xml);
	imageId = Conversion::stringToInt(root.attribute("image_key"));
	unit = root.attribute("uom");
	EDomElement xmlPos = root.elementByTagName("gml:pos");
	if (xmlPos.isAvailable())
	{
		std::deque<double> gmlPosValues = xmlPos.toGmlPos();
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

std::string ImageSpaceCoordinate::xmlGetData()
{
	std::stringstream result;
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
