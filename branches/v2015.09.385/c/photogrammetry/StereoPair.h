#ifndef STEREOPAIR_H
#define STEREOPAIR_H
/*******************************************************************************
		  StereoPair.h
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

#include "EObject.h"

#include <string>

/**
* class StereoPair
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 07/04/2011
* @version 1.0 - Rafael Alves de Aguiar
*/

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Image;

class StereoPair : public EObject
{
	//EOBJECT

	// Aggregated objects
	//
	Image* leftImage;
	Image* rightImage;

public:

	// Constructors and destructors
	//
	StereoPair();
	StereoPair(Image* myLeftImage, Image* myRightImage);
	~StereoPair();

	// Aggregation modifiers
	//
	void setLeftImage(Image* newImage);
	void setRightImage(Image* newImage);
	Image* getLeftImage();
	Image* getRightImage();

	// Consistency testers
	//
	bool leftHasIO();
	bool leftHasEO();
	bool rightHasIO();
	bool rightHasEO();
	bool leftOriented();
	bool rightOriented();
	bool hasOrientations();

	// EObject methods
	//
    std::string objectType(void);
    std::string objectAssociations(void);
    bool is(std::string s);

	// XML methods
	//
    void xmlSetData(std::string xml);
    std::string xmlGetData();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // STEREOPAIR_H
