/************************************************************************
                        Coordinate.cpp
**************************************************************************/

#include "Coordinate.h"

// Destructor
//

/**
 *
 */
Coordinate::~Coordinate()
{

}

// EObject methods
//

/**
 *
 */
string Coordinate::objectType(void)
{
    return "Coordinate";
}


// Other methods
//

/**
 *
 */
void Coordinate::setAvailable(bool value)
{
    available = value;
}

/**
 *
 */
void Coordinate::setSigmaAvailable(bool value)
{
    sigmaAvailable = value;
}

/**
 *
 */
bool Coordinate::isAvailable()
{
    return available;
}

/**
 *
 */
bool Coordinate::isSigmaAvailable()
{
    return sigmaAvailable;
}

/**
 * @return string
 */
string Coordinate::getGmlPos()
{
    return getPosition().toGmlPosFormat();
}
