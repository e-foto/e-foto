/************************************************************************
   AbstractDigitalCoordinate.cpp
**************************************************************************/

#include "AbstractDigitalCoordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

AbstractDigitalCoordinate::~AbstractDigitalCoordinate()
{

}

void AbstractDigitalCoordinate::setCol(int newCol)
{
	col = newCol;
}

void AbstractDigitalCoordinate::setLin(int newLin)
{
	lin = newLin;
}

void AbstractDigitalCoordinate::setUnit(string newUnit)
{
	unit = newUnit;
}

int AbstractDigitalCoordinate::getCol()
{
	return col;
}

int AbstractDigitalCoordinate::getLin()
{
	return lin;
}

string AbstractDigitalCoordinate::getUnit()
{
	return unit;
}

void AbstractDigitalCoordinate::setPosition(const PositionMatrix& newPosition)
{
	if (newPosition.getRows() == 2 && newPosition.getCols() == 1)
	{
		col = newPosition.getInt(1);
		lin = newPosition.getInt(2);
		unit = "px";
	}
	available = true;
}

PositionMatrix AbstractDigitalCoordinate::getPosition()
{
	PositionMatrix position(2, "px");
	position.setInt(1,col);
	position.setInt(2,lin);
	return position;
}

string AbstractDigitalCoordinate::objectType(void)
{
	return "AbstractDigitalCoordinate";
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
