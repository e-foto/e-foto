/************************************************************************
   AbstractDetectorCoordinate.cpp
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
#include "AbstractDetectorCoordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

AbstractDetectorCoordinate::~AbstractDetectorCoordinate()
{

}

void AbstractDetectorCoordinate::setXi(double newXi)
{
	xi = newXi;
}

void AbstractDetectorCoordinate::setEta(double newEta)
{
	eta = newEta;
}

void AbstractDetectorCoordinate::setSigmaXi(double newSigmaXi)
{
	sigmaXi = newSigmaXi;
}

void AbstractDetectorCoordinate::setSigmaEta(double newSigmaEta)
{
	sigmaEta = newSigmaEta;
}

void AbstractDetectorCoordinate::setSigmaXiEta(double newSigmaXiEta)
{
	sigmaXi = newSigmaXiEta;
}

void AbstractDetectorCoordinate::setUnit(std::string newUnit)
{
	unit = newUnit;
}

double AbstractDetectorCoordinate::getXi()
{
	return xi;
}

double AbstractDetectorCoordinate::getEta()
{
	return eta;
}

double AbstractDetectorCoordinate::getSigmaXi()
{
	return sigmaXi;
}

double AbstractDetectorCoordinate::getSigmaEta()
{
	return sigmaEta;
}

double AbstractDetectorCoordinate::getSigmaXiEta()
{
	return sigmaXiEta;
}

std::string AbstractDetectorCoordinate::getUnit()
{
	return unit;
}

void AbstractDetectorCoordinate::setPosition(const PositionMatrix& newPosition)
{
	if (newPosition.getRows() == 2 && newPosition.getCols() == 1)
	{
		xi = newPosition.get(1);
		eta = newPosition.get(2);
		unit = newPosition.getUnit();
	}
	available = true;
}

void AbstractDetectorCoordinate::setPositionSigmas(const Matrix& newPositionSigmas)
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

PositionMatrix AbstractDetectorCoordinate::getPosition()
{
	PositionMatrix position(2, unit);
	position.set(1,xi);
	position.set(2,eta);
	return position;
}

Matrix AbstractDetectorCoordinate::getPositionSigmas()
{
	Matrix positionSigmas(2,2);
	positionSigmas.set(1,1,sigmaXi);
	positionSigmas.set(2,2,sigmaEta);
	positionSigmas.set(1,2,sigmaXiEta);
	positionSigmas.set(2,1,sigmaXiEta);
	return positionSigmas;
}

std::string AbstractDetectorCoordinate::objectType(void)
{
	return "AbstractDetectorCoordinate";
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
