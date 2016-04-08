#ifndef SPATIALINTERSECTION_H
#define SPATIALINTERSECTION_H
/*******************************************************************************
		 SpatialIntersection.h
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
#include "DetectorSpaceCoordinate.h"
#include "ImageSpaceCoordinate.h"

/**
* class SpatialIntersection
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 07/06/2011
* @version 1.0 - Rafael Alves de Aguiar
*/

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class ProjectiveRay;
class StereoPair;

class SpatialIntersection : public EObject
{
	//EOBJECT

	// Aggregated objects
	//
	StereoPair* stereoPair;
	ProjectiveRay* leftRay;
	ProjectiveRay* rightRay;

public:

	// Constructors and destructors
	//
	SpatialIntersection();
	SpatialIntersection(StereoPair* myStereoPair);
	~SpatialIntersection();

	// Aggregation modifiers
	//
	void setStereoPair(StereoPair* newPair);
	StereoPair* getStereoPair();

	// Other methods
	//
	ObjectSpaceCoordinate calculateIntersection(double xiLeft, double etaLeft, double xiRight, double etaRight);
	ObjectSpaceCoordinate calculateIntersection(DetectorSpaceCoordinate leftCoord, DetectorSpaceCoordinate rightCoord);
	ObjectSpaceCoordinate calculateIntersection(int colLeft, int rowLeft, int colRight, int rowRight);
	ObjectSpaceCoordinate calculateIntersectionSubPixel(double colLeft, double rowLeft, double colRight, double rowRight);
	ObjectSpaceCoordinate calculateIntersection(ImageSpaceCoordinate leftCoord, ImageSpaceCoordinate rightCoord);
	PositionMatrix calculateIntersection(PositionMatrix leftPos, PositionMatrix rightPos);

	// EObject methods
	//
    std::string objectType(void);
    std::string objectAssociations(void);
    bool is(std::string s);

	// XML methods
	//
    void xmlSetData(std::string xml);
    std::string xmlGetData();

protected:

	// Support method
	//
	double calculateZ(double xiLeft, double etaLeft, double xiRight, double etaRight);
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // STEREOPAIR_H
