/**************************************************************************
	  ExteriorOrientation.h
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

#ifndef EXTERIORORIENTATION_H
#define EXTERIORORIENTATION_H

#include "EObject.h"
#include "Matrix.h"
#include "EOQuality.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Image;

/**
  * class ExteriorOrientation
  *
  * @author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 06/05/2009
  * @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato
  */

class ExteriorOrientation : public EObject
{
	//EOBJECT

protected:

	// Protected attributes
	//
	Matrix Xa;
	std::string type;
	int totalIterations;

	// Composed objects
	//
	EOQuality myQuality;

	// Associated objects
	//
	int imageId;
	Image* myImage;

public:

	// Associated objects accessor methods
	//
	ExteriorOrientation();
	void setImageId(int newImageId);
	void setImage(Image* newImage);
	int getImageId()const;
	Image* getImage() const;
	Matrix getXa() {return Xa;}
	void setXa(const Matrix& newXa) {Xa = newXa;}
	std::string getType() {return type;}
	void setType(std::string newType) {type = newType;}
	int getTotalIterations() {return totalIterations;}

	// Composed object accessors
	//
	void setQuality(EOQuality newQuality);
	EOQuality getQuality();

	// EObject methods
	//
	std::string objectType(void);
	std::string objectAssociations(void);
	bool is(std::string s);

	std::string xmlGetData();
	void xmlSetData(std::string xml);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //EXTERIORORIENTATION_H
