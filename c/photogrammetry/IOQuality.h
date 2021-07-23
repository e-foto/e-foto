#ifndef IOQUALITY_H
#define IOQUALITY_H
/************************************************************************
	  IOQuality.h
**************************************************************************/
/*Copyright 2002-2021 e-foto team (UERJ)
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

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Sensor;
class InteriorOrientation;

/**
  * class IOQuality
  */

class IOQuality : public EObject {
private:
    Matrix V;
    double sigma0Squared;
    Matrix SigmaXa;
    Matrix SigmaLa;

public:

    virtual ~IOQuality();

    Matrix getV() const;
    double getsigma0Squared() const;
    Matrix getSigmaXa() const;
    Matrix getSigmaLa() const;

    std::string objectType(void) override;
    std::string objectAssociations(void) override;
    bool is(std::string s) override;

    void xmlSetData(std::string xml) override;
    std::string xmlGetData() override;

    void calculate(InteriorOrientation* myIO, Sensor* mySensor);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // IOQUALITY_H
