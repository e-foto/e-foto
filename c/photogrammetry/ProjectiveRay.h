#ifndef PROJECTIVERAY_H
#define PROJECTIVERAY_H
/*******************************************************************************
                  ProjectiveRay.h
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
* class ProjectiveRay
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 12/03/2011
* @version 1.0 - Rafael Alves de Aguiar
*/

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Image;

class ProjectiveRay
{

protected:

        Image* myImage;
        double r11, r12, r13, r21, r22, r23, r31, r32, r33;
        bool hasRotations;

protected:

        PositionMatrix imageZToDetectorZ(const PositionMatrix& myDigitalPositionsWithZ);
        PositionMatrix detectorZToImageZ(const PositionMatrix& myAnalogPositionsWithZ);
        PositionMatrix applyDistortionsZ(const PositionMatrix& myAnalogPositionsWithZ);
        void calculateRotations();

public:

        // Constructors
        //
        ProjectiveRay(Image* myImage);

        // Other methods
        //
        double getKx(double xi, double eta); // to be friended by SpatialIntersection!
        double getKy(double xi, double eta); // to be friended by SpatialIntersection!

        DetectorSpaceCoordinate imageToDetector(double col, double lin);
        DetectorSpaceCoordinate imageToDetector(ImageSpaceCoordinate myDigitalCoordinate);
        PositionMatrix imageToDetector(const PositionMatrix& myDigitalPositions);
        ImageSpaceCoordinate detectorToImage(double xi, double eta);
        ImageSpaceCoordinate detectorToImage(DetectorSpaceCoordinate myAnalogCoordinate);
        PositionMatrix detectorToImage(const PositionMatrix& myAnalogPositions);
        void detectorToImage(double xi, double eta, double &x, double &y);

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
