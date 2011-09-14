/************************************************************************
                        AbstractDigitalCoordinate.cpp
**************************************************************************/

#include "AbstractDigitalCoordinate.h"

//Constructors and destructors methods
//

AbstractDigitalCoordinate::~AbstractDigitalCoordinate()
{

}

// Private attribute accessor methods
//

/**
 * Set the value of col
 * @param newCol the new value of col
 */
void AbstractDigitalCoordinate::setCol(int newCol)
{
	col = newCol;
}

/**
 * Set the value of lin
 * @param newLin the new value of lin
 */
void AbstractDigitalCoordinate::setLin(int newLin)
{
    lin = newLin;
}

/**
 * Set the value of unit
 * @param newUnit the new value of unit
 */
void AbstractDigitalCoordinate::setUnit(string newUnit)
{
    unit = newUnit;
}

/**
 * Get the value of col
 * @return the value of col
 */
int AbstractDigitalCoordinate::getCol()
{
    return col;
}

/**
 * Get the value of lin
 * @return the value of lin
 */
int AbstractDigitalCoordinate::getLin()
{
    return lin;
}

/**
 * Get the value of unit
 * @return the value of unit
 */
string AbstractDigitalCoordinate::getUnit()
{
    return unit;
}

// Matrix use methods
//

/**
 *
 * @param newPosition
 */
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

/**
 *
 * @return
 */
PositionMatrix AbstractDigitalCoordinate::getPosition()
{
    PositionMatrix position(2, "px");
    position.setInt(1,col);
    position.setInt(2,lin);
    return position;
}

// EObject methods
//

/**
 *
 */
string AbstractDigitalCoordinate::objectType(void)
{
    return "AbstractDigitalCoordinate";
}
