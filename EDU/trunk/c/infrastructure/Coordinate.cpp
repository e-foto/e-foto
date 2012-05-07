/************************************************************************
   Coordinate.cpp
**************************************************************************/

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

bool Coordinate::isAvailable()
{
	return available;
}

bool Coordinate::isSigmaAvailable()
{
	return sigmaAvailable;
}

string Coordinate::getGmlPos()
{
	return getPosition().toGmlPosFormat();
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
