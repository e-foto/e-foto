/************************************************************************
   Coordinate.cpp
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

#include "Coordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

Coordinate::~Coordinate()
{

}

void Coordinate::setAvailable(bool value)
{
	available = value;
}

void Coordinate::setSigmaAvailable(bool value)
{
	sigmaAvailable = value;
}

bool Coordinate::isAvailable() const
{
	return available;
}

std::string Coordinate::getGmlPos()
{
	return getPosition().toGmlPosFormat();
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
