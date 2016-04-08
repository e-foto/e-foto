/**************************************************************************
	  ExteriorOrientation.cpp
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
#include "ExteriorOrientation.h"
#include "Image.h"

#include <sstream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

// Associated object accessor methods
//
ExteriorOrientation::ExteriorOrientation():imageId(-1), myImage(NULL)
{

}

void ExteriorOrientation::setImageId(int newImageId)
{
	imageId = newImageId;
}

/**
 * Sets the value of myImage
 * @param newImage the new value of myImage
 */
void ExteriorOrientation::setImage(Image* newImage)
{
	myImage = newImage;
}


int ExteriorOrientation::getImageId() const
{
	return imageId;
}

/**
 * Gets the value of myImage
 * @return the value of myImage
 */
Image* ExteriorOrientation::getImage() const
{
	return myImage;
}

// EObject methods
//

/**
 *
 */
std::string ExteriorOrientation::objectType(void)
{
	std::stringstream result;
	result << "ExteriorOrientation " << imageId;
	return result.str();
}

/**
 *
 */
std::string ExteriorOrientation::objectAssociations(void)
{
	return myImage->objectType();
}

/**
 *
 */
bool ExteriorOrientation::is(std::string s)
{
	return (s == "ExteriorOrientation" ? true : false);
}

std::string ExteriorOrientation::xmlGetData()
{
	// extrair codigo da SR para este ponto
    return std::string();
}

void ExteriorOrientation::xmlSetData(std::string xml)
{
	// extrair codigo da SR para este ponto
}

// Composed object accessors
//
void ExteriorOrientation::setQuality(EOQuality newQuality)
{
	myQuality = newQuality;
}

EOQuality ExteriorOrientation::getQuality()
{
	return myQuality;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
