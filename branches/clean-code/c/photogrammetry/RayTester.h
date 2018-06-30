#ifndef RAYTESTER_H
#define RAYTESTER_H
/*******************************************************************************
                  RayTester.h
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

#include "ImageSpaceCoordinate.h"
#include "DetectorSpaceCoordinate.h"
#include "ObjectSpaceCoordinate.h"

/**
* class RayTester
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 06/21/2011
* @version 1.0 - Rafael Alves de Aguiar
*/

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Image;

class RayTester
{

protected:

        Image* myImage_;
        double a0_{0.0}, a1_{0.0}, a2_{0.0}, b0_{0.0}, b1_{0.0}, b2_{0.0};
        double X0_{0.0}, Y0_{0.0}, Z0_{0.0}, omega_{0.0}, phi_{0.0}, kappa_{0.0};
        double r11_{0.0}, r12_{0.0}, r13_{0.0}, r21_{0.0}, r22_{0.0}, r23_{0.0}, r31_{0.0}, r32_{0.0}, r33_{0.0};
        bool hasIO_{false}, hasEO_{false}, hasRotations_{false};

public:
        explicit RayTester(Image* myImage_);

        // Attribute accessors
        void setIOParameters(Matrix params);
        void setEOParameters(Matrix params);

        // Other parameters
        double getKx(double xi, double eta);
        double getKy(double xi, double eta);

protected:

        PositionMatrix imageZToDetectorZ(const PositionMatrix& myDigitalPositionsWithZ);
        PositionMatrix removeDistortionsZ(const PositionMatrix& myAnalogPositionsWithZ);
        void calculateRotations();

public:

        // Accessors
        //
        void setImage(Image* newImage);
        Image* getImage();

        // Other methods
        //
        DetectorSpaceCoordinate imageToDetector(double col, double lin);
        DetectorSpaceCoordinate imageToDetector(ImageSpaceCoordinate myDigitalCoordinate);
        PositionMatrix imageToDetector(const PositionMatrix& myDigitalPositions);
        ImageSpaceCoordinate detectorToImage(double xi, double eta) const;
        ImageSpaceCoordinate detectorToImage(DetectorSpaceCoordinate myAnalogCoordinate) const;
        PositionMatrix detectorToImage(const PositionMatrix& myAnalogPositions) const;

        ObjectSpaceCoordinate detectorToObject(double xi, double eta, double Z);
        ObjectSpaceCoordinate detectorToObject(DetectorSpaceCoordinate myAnalogCoordinate, double Z);
        PositionMatrix detectorToObject(const PositionMatrix& myAnalogPositions, double Z);
        PositionMatrix detectorToObject(const PositionMatrix& myAnalogPositionsWithZ);
        DetectorSpaceCoordinate objectToDetector(double X, double Y, double Z);
        DetectorSpaceCoordinate objectToDetector(ObjectSpaceCoordinate myObjectCoordinate);
        PositionMatrix objectToDetector(const PositionMatrix& myObjectPositions);

        DetectorSpaceCoordinate applyDistortions(double xi, double eta);
        DetectorSpaceCoordinate applyDistortions(DetectorSpaceCoordinate myAnalogCoordinate);
        PositionMatrix applyDistortions(const PositionMatrix& myAnalogPositions);
        DetectorSpaceCoordinate removeDistortions(double xi, double eta);
        DetectorSpaceCoordinate removeDistortions(DetectorSpaceCoordinate myAnalogCoordinate);
        PositionMatrix removeDistortions(const PositionMatrix& myAnalogPositions);

        ObjectSpaceCoordinate imageToObject(double col, double lin, double Z, bool useDistortions = true);
        ObjectSpaceCoordinate imageToObject(ImageSpaceCoordinate myDigitalCoordinate, double Z, bool useDistortions = true);
        PositionMatrix imageToObject(const PositionMatrix& myDigitalPositions, double Z, bool useDistortions = true);
        PositionMatrix imageToObject(const PositionMatrix& myDigitalPositionsWithZ, bool useDistortions = true);
        ImageSpaceCoordinate objectToImage(double X, double Y, double Z, bool useDistortions = true);
        ImageSpaceCoordinate objectToImage(ObjectSpaceCoordinate myObjectCoordinate, bool useDistortions = true);
        PositionMatrix objectToImage(const PositionMatrix& myObjectPositions, bool useDistortions = true);

private:

        DetectorSpaceCoordinate getRadialDistortions(double xi, double eta);
        DetectorSpaceCoordinate getDecenteredDistortions(double xi, double eta);
        DetectorSpaceCoordinate getAtmosphereDistortions(double xi, double eta);
        DetectorSpaceCoordinate getCurvatureDistortions(double xi, double eta);
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // PROJECTIVERAY_H
