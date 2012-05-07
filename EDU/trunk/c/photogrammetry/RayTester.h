/*******************************************************************************
                  RayTester.h
*******************************************************************************/


#ifndef RAYTESTER_H
#define RAYTESTER_H

#include "Terrain.h"
#include "Flight.h"
#include "FrameSensor.h"
#include "Image.h"
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

class RayTester
{

protected:

        Image* myImage;
        double a0, a1, a2, b0, b1, b2;
        double X0, Y0, Z0, omega, phi, kappa;
        double r11, r12, r13, r21, r22, r23, r31, r32, r33;
        bool hasIO, hasEO, hasRotations;

public:

        // Constructors
        //
        RayTester(Image* myImage);

        // Attribute accessors
        //
        void setIOParameters(Matrix params);
        void setEOParameters(Matrix params);
        Matrix getIOParameters();
        Matrix getEOParameters();

        // Other parameters
        //
        Matrix getRotations();
        double getKx(double xi, double eta);
        double getKy(double xi, double eta);

protected:

        PositionMatrix imageZToDetectorZ(const PositionMatrix& myDigitalPositionsWithZ);
        PositionMatrix detectorZToImageZ(const PositionMatrix& myAnalogPositionsWithZ);
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
        ImageSpaceCoordinate detectorToImage(double xi, double eta);
        ImageSpaceCoordinate detectorToImage(DetectorSpaceCoordinate myAnalogCoordinate);
        PositionMatrix detectorToImage(const PositionMatrix& myAnalogPositions);

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
