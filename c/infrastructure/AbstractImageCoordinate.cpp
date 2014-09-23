/************************************************************************
   AbstractImageCoordinate.cpp
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

#include "AbstractImageCoordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

AbstractImageCoordinate::~AbstractImageCoordinate()
{

}

void AbstractImageCoordinate::setCol(double newCol)
{
	col = newCol;
}

void AbstractImageCoordinate::setLin(double newLin)
{
	lin = newLin;
}

void AbstractImageCoordinate::setUnit(std::string newUnit)
{
	unit = newUnit;
}

double AbstractImageCoordinate::getCol()
{
	return col;
}

double AbstractImageCoordinate::getLin()
{
	return lin;
}

std::string AbstractImageCoordinate::getUnit()
{
	return unit;
}

void AbstractImageCoordinate::setPosition(const PositionMatrix& newPosition)
{
	if (newPosition.getRows() == 2 && newPosition.getCols() == 1)
	{
		col = newPosition.get(1);
		lin = newPosition.get(2);
		unit = "px";
	}
	available = true;
}

PositionMatrix AbstractImageCoordinate::getPosition()
{
	PositionMatrix position(2, "px");
	position.set(1, col);
	position.set(2, lin);
	return position;
}

std::string AbstractImageCoordinate::objectType(void)
{
	return "AbstractImageCoordinate";
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
