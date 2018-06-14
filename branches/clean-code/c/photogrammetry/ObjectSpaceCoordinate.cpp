/*******************************************************************************
   ObjectSpaceCoordinate.cpp
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
#include "ObjectSpaceCoordinate.h"
#include "PositionMatrix.h"

#include <sstream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

ObjectSpaceCoordinate::ObjectSpaceCoordinate()
{
	available = false;
	sigmaAvailable = false;
}

ObjectSpaceCoordinate::ObjectSpaceCoordinate(std::string myUnit, double myX, double myY, double myZ, double mySigmaX, double mySigmaY, double mySigmaZ, double mySigmaXY, double mySigmaXZ, double mySigmaYZ)
{
	setUnit(myUnit);
	setX(myX);
	setY(myY);
	setZ(myZ);
	setSigmaX(mySigmaX);
	setSigmaY(mySigmaY);
	setSigmaZ(mySigmaZ);
	setSigmaXY(mySigmaXY);
	setSigmaXZ(mySigmaXZ);
	setSigmaYZ(mySigmaYZ);
	available = true;
	sigmaAvailable = true;
}

ObjectSpaceCoordinate::ObjectSpaceCoordinate(const PositionMatrix& myPosition)
{
	setPosition(myPosition);
	sigmaAvailable = false;
}

ObjectSpaceCoordinate::ObjectSpaceCoordinate(const PositionMatrix& myPosition, const Matrix& myPositionSigmas)
{
	setPosition(myPosition);
	setPositionSigmas(myPositionSigmas);
}

void ObjectSpaceCoordinate::setX(double newX)
{
	x = newX;
}

void ObjectSpaceCoordinate::setY(double newY)
{
	y = newY;
}

void ObjectSpaceCoordinate::setZ(double newZ)
{
	z = newZ;
}

void ObjectSpaceCoordinate::setSigmaX(double newSigmaX)
{
	sigmaX = newSigmaX;
}

void ObjectSpaceCoordinate::setSigmaY(double newSigmaY)
{
	sigmaY = newSigmaY;
}

void ObjectSpaceCoordinate::setSigmaZ(double newSigmaZ)
{
	sigmaZ = newSigmaZ;
}

void ObjectSpaceCoordinate::setSigmaXY(double newSigmaXY)
{
	sigmaXY = newSigmaXY;
}

void ObjectSpaceCoordinate::setSigmaXZ(double newSigmaXZ)
{
	sigmaXZ = newSigmaXZ;
}

void ObjectSpaceCoordinate::setSigmaYZ(double newSigmaYZ)
{
	sigmaYZ = newSigmaYZ;
}

void ObjectSpaceCoordinate::setUnit(std::string newUnit)
{
	unit = newUnit;
}

double ObjectSpaceCoordinate::getX()
{
	return x;
}

double ObjectSpaceCoordinate::getY()
{
	return y;
}

double ObjectSpaceCoordinate::getZ()
{
	return z;
}

double ObjectSpaceCoordinate::getSigmaX()
{
	return sigmaX;
}

double ObjectSpaceCoordinate::getSigmaY()
{
	return sigmaY;
}

double ObjectSpaceCoordinate::getSigmaZ()
{
	return sigmaZ;
}

double ObjectSpaceCoordinate::getSigmaXY()
{
	return sigmaXY;
}

double ObjectSpaceCoordinate::getSigmaXZ()
{
	return sigmaXZ;
}

double ObjectSpaceCoordinate::getSigmaYZ()
{
	return sigmaYZ;
}

std::string ObjectSpaceCoordinate::getUnit()
{
	return unit;
}

void ObjectSpaceCoordinate::setPosition(const PositionMatrix& newPosition)
{
	if (newPosition.getRows() == 3 && newPosition.getCols() == 1)
	{
		x = newPosition.get(1);
		y = newPosition.get(2);
		z = newPosition.get(3);
		unit = newPosition.getUnit();
	}
	available = true;
}

void ObjectSpaceCoordinate::setPositionSigmas(const Matrix& newPositionSigmas)
{
	if (newPositionSigmas.getRows() == 3 && newPositionSigmas.getCols() == 1)
	{
		sigmaX = newPositionSigmas.get(1,1);
		sigmaY = newPositionSigmas.get(2,1);
		sigmaZ = newPositionSigmas.get(3,1);
		sigmaXY = 0;
		sigmaXZ = 0;
		sigmaYZ = 0;
	}
	else if (newPositionSigmas.getRows() == 3 && newPositionSigmas.getCols() == 3)
	{
		sigmaX = newPositionSigmas.get(1,1);
		sigmaY = newPositionSigmas.get(2,2);
		sigmaZ = newPositionSigmas.get(3,3);
		sigmaXY = newPositionSigmas.get(1,2);
		sigmaXZ = newPositionSigmas.get(1,3);
		sigmaYZ = newPositionSigmas.get(2,3);
	}
	sigmaAvailable = true;
}

PositionMatrix ObjectSpaceCoordinate::getPosition()
{
	PositionMatrix position(3,unit);
	position.set(1,x);
	position.set(2,y);
	position.set(3,z);
	return position;
}

Matrix ObjectSpaceCoordinate::getPositionSigmas()
{
	Matrix positionSigmas(3,3);
	positionSigmas.set(1,1,sigmaX);
	positionSigmas.set(2,2,sigmaY);
	positionSigmas.set(3,3,sigmaZ);
	positionSigmas.set(1,2,sigmaXY);
	positionSigmas.set(2,1,sigmaXY);
	positionSigmas.set(1,3,sigmaXZ);
	positionSigmas.set(3,1,sigmaXZ);
	positionSigmas.set(2,3,sigmaYZ);
	positionSigmas.set(3,2,sigmaYZ);
	return positionSigmas;
}

void ObjectSpaceCoordinate::xmlSetData(std::string xml)
{
	EDomElement root(xml);
	unit = root.attribute("uom");
	EDomElement xmlPos = root.elementByTagName("gml:pos");
	if (xmlPos.isAvailable())
	{
        std::deque<double> gmlPosValues = root.elementByTagName("gml:pos").toGmlPos();
		x = gmlPosValues.at(0);
		y = gmlPosValues.at(1);
		z = gmlPosValues.at(2);
		available = true;
	}
	else
	{
		available = false;
	}
	Matrix sigmaMatrix;
	EDomElement xmlSigma = root.elementByTagName("sigma");
	if (xmlSigma.isAvailable())
	{
		sigmaMatrix.xmlSetData(xmlSigma.elementByTagName("mml:matrix").getContent());
		sigmaAvailable = true;
	}
	else
	{
		sigmaMatrix.identity(3);
		sigmaAvailable = false;
	}
	setPositionSigmas(sigmaMatrix);
}

std::string ObjectSpaceCoordinate::xmlGetData()
{
    std::stringstream result;
	result << "<spatialCoordinates uom=\"" << unit << "\">\n";
	if (available)
		result << "<gml:pos>" << Conversion::doubleToString(x) << " " << Conversion::doubleToString(y) << " " << Conversion::doubleToString(z) << "</gml:pos>\n";
	else
		result << "<gml:pos>Not Available</gml:pos>\n";
	if (sigmaAvailable)
		result << "<sigma>\n" << getPositionSigmas().xmlGetData() << "</sigma>\n";
	else
		result << "<sigma>Not Available</sigma>\n";
	result << "</spatialCoordinates>";
	return result.str();
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
