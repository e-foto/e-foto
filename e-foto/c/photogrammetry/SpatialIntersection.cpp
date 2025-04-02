/*******************************************************************************
        SpatialIntersection.cpp
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

#include "SpatialIntersection.h"

#include "SpatialRessection.h"
#include "ProjectiveRay.h"
#include "PositionMatrix.h"
#include "StereoPair.h"
#include "Image.h"

#include <sstream>

// Constructors and destructors

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

SpatialIntersection::SpatialIntersection()
{
    stereoPair = NULL;
    leftRay = NULL;
    rightRay = NULL;
}

SpatialIntersection::SpatialIntersection(StereoPair* myStereoPair)
{
    stereoPair = myStereoPair;
    leftRay = new ProjectiveRay(stereoPair->getLeftImage());
    rightRay = new ProjectiveRay(stereoPair->getRightImage());
}

SpatialIntersection::~SpatialIntersection()
{
    if (leftRay != NULL)
    {
        delete leftRay;
        leftRay = NULL;
    }
    if (rightRay != NULL)
    {
        delete rightRay;
        rightRay = NULL;
    }
}

// Aggregation modifiers

void SpatialIntersection::setStereoPair(StereoPair* newPair)
{
    stereoPair = newPair;
    if (leftRay != NULL)
    {
        delete leftRay;
    }
    leftRay = new ProjectiveRay(stereoPair->getLeftImage());
    if (rightRay != NULL)
    {
        delete rightRay;
    }
    rightRay = new ProjectiveRay(stereoPair->getRightImage());
}

StereoPair* SpatialIntersection::getStereoPair()
{
    return stereoPair;
}

// Other methods

ObjectSpaceCoordinate SpatialIntersection::calculateIntersection(double xiLeft, double etaLeft, double xiRight, double etaRight)
{
    if (stereoPair->hasOrientations())
    {
        double Z = calculateZ(xiLeft, etaLeft, xiRight, etaRight);

        ObjectSpaceCoordinate leftObject = leftRay->detectorToObject(xiLeft, etaLeft, Z);
        ObjectSpaceCoordinate rightObject = rightRay->detectorToObject(xiRight, etaRight, Z);

        double X = (leftObject.getX() + rightObject.getX()) / 2;
        double Y = (leftObject.getY() + rightObject.getY()) / 2;

        ObjectSpaceCoordinate result("m", X, Y, Z);
        return result;
    }
    else
    {
        return ObjectSpaceCoordinate();
    }
}

ObjectSpaceCoordinate SpatialIntersection::calculateIntersection(DetectorSpaceCoordinate leftCoord, DetectorSpaceCoordinate rightCoord)
{
    return calculateIntersection(leftCoord.getXi(), leftCoord.getEta(), rightCoord.getXi(), rightCoord.getEta());
}

ObjectSpaceCoordinate SpatialIntersection::calculateIntersection(int colLeft, int rowLeft, int colRight, int rowRight)
{
    if (stereoPair->hasOrientations())
    {
        DetectorSpaceCoordinate leftAnalog = leftRay->imageToDetector(colLeft, rowLeft);
        DetectorSpaceCoordinate rightAnalog = rightRay->imageToDetector(colRight, rowRight);
        return calculateIntersection(leftAnalog.getXi(), leftAnalog.getEta(), rightAnalog.getXi(), rightAnalog.getEta());
    }
    else
    {
        return ObjectSpaceCoordinate();
    }
}

ObjectSpaceCoordinate SpatialIntersection::calculateIntersectionSubPixel(double colLeft, double rowLeft, double colRight, double rowRight)
{
    if (stereoPair->hasOrientations())
    {
        DetectorSpaceCoordinate leftAnalog = leftRay->imageToDetector(colLeft, rowLeft);
        DetectorSpaceCoordinate rightAnalog = rightRay->imageToDetector(colRight, rowRight);
        return calculateIntersection(leftAnalog.getXi(), leftAnalog.getEta(), rightAnalog.getXi(), rightAnalog.getEta());
    }
    else
    {
        return ObjectSpaceCoordinate();
    }
}

ObjectSpaceCoordinate SpatialIntersection::calculateIntersection(ImageSpaceCoordinate leftCoord, ImageSpaceCoordinate rightCoord)
{
    return calculateIntersection(leftCoord.getCol(), leftCoord.getLin(), rightCoord.getCol(), rightCoord.getLin());
}

PositionMatrix SpatialIntersection::calculateIntersection(PositionMatrix leftPos, PositionMatrix rightPos)
{
    if (stereoPair->hasOrientations() && leftPos.getRows() == rightPos.getRows() && leftPos.getRows()%2 == 0)
    {
        PositionMatrix result(leftPos.getRows() * 3 / 2, "m");
        ObjectSpaceCoordinate objectCoordinate;

        unsigned int i, j;
        for (i = 1, j = 1; i+1 <= leftPos.getRows(); i += 2, j += 3)
        {
            objectCoordinate = calculateIntersection(leftPos.get(i),leftPos.get(i+1),rightPos.get(i),rightPos.get(i+1));
            result.set(j,objectCoordinate.getX());
            result.set(j+1,objectCoordinate.getY());
            result.set(j+2,objectCoordinate.getZ());
        }

        return result;
    }
    else
    {
        return PositionMatrix();
    }
}

// EObject methods
//
std::string SpatialIntersection::objectType(void)
{
    std::stringstream result;
    result << "SpatialIntersection " << stereoPair->getLeftImage()->getId() << " " << stereoPair->getRightImage()->getId();
    return result.str();
}

std::string SpatialIntersection::objectAssociations(void)
{
    return "";
}

bool SpatialIntersection::is(std::string s)
{
    return (s == "SpatialIntersection" ? true : false);
}

// XML methods
//
void SpatialIntersection::xmlSetData(std::string xml)
{
    std::string none = xml;
}

std::string SpatialIntersection::xmlGetData()
{
    return "";
}

// Support method

double SpatialIntersection::calculateZ(double xiLeft, double etaLeft, double xiRight, double etaRight)
{
    double result = 0.0;

    if (stereoPair->hasOrientations())
    {
        double kxLeft = leftRay->getKx(xiLeft, etaLeft);
        double kyLeft = leftRay->getKy(xiLeft, etaLeft);
        double kxRight = rightRay->getKx(xiRight, etaRight);
        double kyRight = rightRay->getKy(xiRight, etaRight);

        Matrix XaLeft = stereoPair->getLeftImage()->getEO()->getXa();
        Matrix XaRight = stereoPair->getRightImage()->getEO()->getXa();

        double X0left = XaLeft.get(1,1);
        double Y0left = XaLeft.get(2,1);
        double Z0left = XaLeft.get(3,1);
        double X0right = XaRight.get(1,1);
        double Y0right = XaRight.get(2,1);
        double Z0right = XaRight.get(3,1);

        double kx = kxLeft - kxRight;
        double ky = kyLeft - kyRight;

        if (fabs(kx) >= fabs(ky))
        {
            result = (X0right - Z0right * kxRight + Z0left * kxLeft - X0left) / (kxLeft - kxRight); // Zx
        }
        else
        {
            result = (Y0right - Z0right * kyRight + Z0left * kyLeft - Y0left) / (kyLeft - kyRight); // Zy
        }
    }

    return result;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
