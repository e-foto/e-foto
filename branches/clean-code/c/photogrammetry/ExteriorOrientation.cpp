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
ExteriorOrientation::ExteriorOrientation():
    imageId_(-1),
    myImage_(NULL)
{
}

void ExteriorOrientation::setImageId(int newImageId)
{
    imageId_ = newImageId;
}

/**
 * Sets the value of myImage
 * @param newImage the new value of myImage
 */
void ExteriorOrientation::setImage(Image* newImage)
{
    myImage_ = newImage;
}


int ExteriorOrientation::getImageId() const
{
    return imageId_;
}

/**
 * Gets the value of myImage
 * @return the value of myImage
 */
Image* ExteriorOrientation::getImage() const
{
    return myImage_;
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
	EDomElement root(xml);
	// extrair codigo da SR para este ponto
}

// Composed object accessors
EOQuality ExteriorOrientation::getQuality() const
{
    return myQuality_;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
