/************************************************************************
   AbstractAnalogCoordinate.cpp
**************************************************************************/

#include "AbstractAnalogCoordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

AbstractAnalogCoordinate::~AbstractAnalogCoordinate()
{

}

void AbstractAnalogCoordinate::setXi(double newXi)
{
	xi = newXi;
}

void AbstractAnalogCoordinate::setEta(double newEta)
{
	eta = newEta;
}

void AbstractAnalogCoordinate::setSigmaXi(double newSigmaXi)
{
	sigmaXi = newSigmaXi;
}

void AbstractAnalogCoordinate::setSigmaEta(double newSigmaEta)
{
	sigmaEta = newSigmaEta;
}

void AbstractAnalogCoordinate::setSigmaXiEta(double newSigmaXiEta)
{
	sigmaXi = newSigmaXiEta;
}

void AbstractAnalogCoordinate::setUnit(string newUnit)
{
	unit = newUnit;
}

double AbstractAnalogCoordinate::getXi()
{
	return xi;
}

double AbstractAnalogCoordinate::getEta()
{
	return eta;
}

double AbstractAnalogCoordinate::getSigmaXi()
{
	return sigmaXi;
}

double AbstractAnalogCoordinate::getSigmaEta()
{
	return sigmaEta;
}

double AbstractAnalogCoordinate::getSigmaXiEta()
{
	return sigmaXiEta;
}

string AbstractAnalogCoordinate::getUnit()
{
	return unit;
}

void AbstractAnalogCoordinate::setPosition(const PositionMatrix& newPosition)
{
	if (newPosition.getRows() == 2 && newPosition.getCols() == 1)
	{
		xi = newPosition.get(1);
		eta = newPosition.get(2);
		unit = newPosition.getUnit();
	}
	available = true;
}

void AbstractAnalogCoordinate::setPositionSigmas(const Matrix& newPositionSigmas)
{
	if (newPositionSigmas.getRows() == 2 && newPositionSigmas.getCols() == 1)
	{
		sigmaXi = newPositionSigmas.get(1,1);
		sigmaEta = newPositionSigmas.get(2,1);
		sigmaXiEta = 0;
	}
	else if (newPositionSigmas.getRows() == 2 && newPositionSigmas.getCols() == 2)
	{
		sigmaXi = newPositionSigmas.get(1,1);
		sigmaEta = newPositionSigmas.get(2,2);
		sigmaXiEta = newPositionSigmas.get(2,1);
	}
	sigmaAvailable = true;
}

PositionMatrix AbstractAnalogCoordinate::getPosition()
{
	PositionMatrix position(2, unit);
	position.set(1,xi);
	position.set(2,eta);
	return position;
}

Matrix AbstractAnalogCoordinate::getPositionSigmas()
{
	Matrix positionSigmas(2,2);
	positionSigmas.set(1,1,sigmaXi);
	positionSigmas.set(2,2,sigmaEta);
	positionSigmas.set(1,2,sigmaXiEta);
	positionSigmas.set(2,1,sigmaXiEta);
	return positionSigmas;
}

string AbstractAnalogCoordinate::objectType(void)
{
	return "AbstractAnalogCoordinate";
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
