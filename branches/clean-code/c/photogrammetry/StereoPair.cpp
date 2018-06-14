/*******************************************************************************
		 StereoPair.cpp
*******************************************************************************/
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
#include "StereoPair.h"

#include "Image.h"

#include <sstream>

// Constructors and destructors

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

StereoPair::StereoPair()
{
	leftImage = NULL;
	rightImage = NULL;
}

StereoPair::StereoPair(Image* myLeftImage, Image* myRightImage)
{
	leftImage = myLeftImage;
	rightImage = myRightImage;
}

StereoPair::~StereoPair()
{

}

// Aggregation modifiers

void StereoPair::setLeftImage(Image* newImage)
{
	leftImage = newImage;
}

void StereoPair::setRightImage(Image* newImage)
{
	rightImage = newImage;
}

Image* StereoPair::getLeftImage()
{
	return leftImage;
}

Image* StereoPair::getRightImage()
{
	return rightImage;
}

// Consistency testers

bool StereoPair::leftHasIO()
{
	if (leftImage->getIO() == NULL)
		return false;
	else
		return true;
}

bool StereoPair::leftHasEO()
{
	if (leftImage->getEO() == NULL)
		return false;
	else
		return true;
}

bool StereoPair::rightHasIO()
{
	if (rightImage->getIO() == NULL)
		return false;
	else
		return true;
}

bool StereoPair::rightHasEO()
{
	if (rightImage->getEO() == NULL)
		return false;
	else
		return true;
}

bool StereoPair::leftOriented()
{
	if (leftHasIO() && leftHasEO())
		return true;
	else
		return false;
}

bool StereoPair::rightOriented()
{
	if (rightHasIO() && rightHasEO())
		return true;
	else
		return false;
}

bool StereoPair::hasOrientations()
{
	if (leftOriented() && rightOriented())
		return true;
	else
		return false;
}

// XML methods
//
void StereoPair::xmlSetData(std::string xml)
{
    std::string none = xml;
}

std::string StereoPair::xmlGetData()
{
	return "";
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
