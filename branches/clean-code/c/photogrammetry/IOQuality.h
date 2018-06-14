#ifndef IOQUALITY_H
#define IOQUALITY_H
/************************************************************************
	  IOQuality.h
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

#include "Matrix.h"
#include "EDom.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Sensor;
class InteriorOrientation;

/**
  * class IOQuality
  *
  * @Author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 06/05/2009
  * @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato
  *
  */

class IOQuality : public EDom
{
private:
	Matrix V;
	double sigma0Squared;
	Matrix SigmaXa;
	Matrix SigmaLa;

public:

	// Constructors and Destructor
	//
	virtual ~IOQuality();

	// Private attribute accessors
	//
	Matrix getV();
	double getsigma0Squared();
	Matrix getSigmaXa();
	Matrix getSigmaLa();

    // XML methods
	//
    void xmlSetData(std::string xml);
    std::string xmlGetData();

	// Other method
	//
	void calculate(InteriorOrientation* myIO, Sensor* mySensor);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // IOQUALITY_H
