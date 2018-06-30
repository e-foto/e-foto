#ifndef INTERIORORIENTATION_H
#define INTERIORORIENTATION_H
/**************************************************************************
          InteriorOrientation.h
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

#include "IOQuality.h"
#include "ImageSpaceCoordinate.h"
#include "DetectorSpaceCoordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class SensorWithFiducialMarks;
class Image;

/**
  * class InteriorOrientation
  *
  * @author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 06/05/2009
  * @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato
  *
  */

class InteriorOrientation : public EDom
{

public:

        InteriorOrientation();
        explicit InteriorOrientation(int myImageId); // Constructor with ids only, needed in project use.
        virtual ~InteriorOrientation();

        int getImageId() const;
        Matrix getXa() const;
        Matrix getLa() const;
        Matrix getA() const;
        Matrix getP() const;

        IOQuality getQuality() const;

        void setImage(Image *myNewImage);
        Image* getImage();

        // XML methods
        void xmlSetData(std::string xml);
        std::string xmlGetData();

        void calculate();
        DetectorSpaceCoordinate imageToDetector(double col, double lin);
        DetectorSpaceCoordinate imageToDetector(ImageSpaceCoordinate myDigitalCoordinate);
        PositionMatrix imageToDetector(const PositionMatrix& myDigitalPositions);
        ImageSpaceCoordinate detectorToImage(double x, double y) const;
        ImageSpaceCoordinate detectorToImage(DetectorSpaceCoordinate myAnalogCoordinate) const;
        PositionMatrix detectorToImage(const PositionMatrix& myAnalogPositions) const;

private:
        int imageId{-1};
        Matrix Xa{};
        Matrix La{};
        Matrix A{};
        Matrix P{};

        IOQuality myQuality{};

        Image* myImage{nullptr};

        void generateA();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // INTERIORORIENTATION_H
