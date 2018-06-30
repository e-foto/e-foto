#ifndef EOQUALITY_H
#define EOQUALITY_H
/************************************************************************
	  EOQuality.h
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

#include "EDom.h"
#include "Matrix.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class ExteriorOrientation;
class SpatialRessection;

/**
  * class EOQuality
  *
  * @Author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 06/05/2009
  * @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato - Initial version.
  *
  */

class EOQuality : public EDom
{
	//EOBJECT

private:


	// Private attributes
	//
    Matrix V{};
    double sigma0Squared{0.0};
    Matrix SigmaXa{};
    Matrix SigmaLa{};

public:
    EOQuality(){};
	virtual ~EOQuality();

	// Private attribute accessors
	//
    Matrix getV() const;
    double getsigma0Squared() const;
    Matrix getSigmaXa() const;
    Matrix getSigmaLa() const;

	// XML methods
	//
    void xmlSetData(std::string xml);
    std::string xmlGetData();

	// Other methods
	//
	void calculate(SpatialRessection* myEO);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // EOQUALITY_H
