/*******************************************************************************
   ImageFiducialMark.cpp
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
#include "ImageFiducialMark.h"

#include <sstream>

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

int ImageFiducialMark::getId() const
{
	return id;
}

int ImageFiducialMark::getImageId() const
{
	return imageId;
}

void ImageFiducialMark::xmlSetData(std::string xml)
{
	EDomElement root(xml);
	id = Conversion::stringToInt(root.attribute("key"));
	EDomElement xmlPos = root.elementByTagName("gml:pos");
	if (xmlPos.isAvailable())
	{
        std::deque<double> gmlPosValues = xmlPos.toGmlPos();
		col = gmlPosValues.at(0);
		lin = gmlPosValues.at(1);
		available = true;
	}
	else
	{
		available = false;
	}
}

std::string ImageFiducialMark::xmlGetData()
{
    std::stringstream result;
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
