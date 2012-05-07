/************************************************************************
   AbstractImageCoordinate.cpp
**************************************************************************/

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

void AbstractImageCoordinate::setUnit(string newUnit)
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

string AbstractImageCoordinate::getUnit()
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

string AbstractImageCoordinate::objectType(void)
{
	return "AbstractImageCoordinate";
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
