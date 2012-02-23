/**************************************************************************
           RayTester.cpp
**************************************************************************/

#include "RayTester.h"
#include <string>
#include <exception>

// Constructor

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

RayTester::RayTester(Image *myImage)
{
        this->myImage = myImage;
        this->hasIO = false;
        this->hasEO = false;
        this->hasRotations = false;
}

// Attribute accessors

void RayTester::setIOParameters(Matrix params)
{
        if (params.getRows() == 6)
        {
                a0 = params.get(1,1);
                a1 = params.get(2,1);
                a2 = params.get(3,1);
                b0 = params.get(4,1);
                b1 = params.get(5,1);
                b2 = params.get(6,1);
                hasIO = true;
        }
}

void RayTester::setEOParameters(Matrix params)
{
        if (params.getRows() == 6)
        {
                X0 = params.get(1,1);
                Y0 = params.get(2,1);
                Z0 = params.get(3,1);
                phi = params.get(4,1);
                omega = params.get(5,1);
                kappa = params.get(6,1);
                hasEO = true;
                hasRotations = false;
        }
}

Matrix RayTester::getIOParameters()
{
        Matrix result;
        if (hasIO)
        {
                result.resize(6,1);
                result.set(1,1,a0);
                result.set(2,1,a1);
                result.set(3,1,a2);
                result.set(4,1,b0);
                result.set(5,1,b1);
                result.set(6,1,b2);
        }
        return result;
}

Matrix RayTester::getEOParameters()
{
        Matrix result;
        if (hasEO)
        {
                result.resize(6,1);
                result.set(1,1,X0);
                result.set(2,1,Y0);
                result.set(3,1,Z0);
                result.set(4,1,phi);
                result.set(5,1,omega);
                result.set(6,1,kappa);
        }
        return result;
}

// Other parameters

Matrix RayTester::getRotations()
{
        Matrix result;

        if (!hasRotations)
                calculateRotations();

        if (hasRotations)
        {
                result.resize(3,3);
                result.set(1,1,r11);
                result.set(1,2,r12);
                result.set(1,3,r13);
                result.set(2,1,r21);
                result.set(2,2,r22);
                result.set(2,3,r23);
                result.set(3,1,r31);
                result.set(3,2,r32);
                result.set(3,3,r33);
        }
        return result;
}

double RayTester::getKx(double xi, double eta)
{
        if (myImage != NULL && myImage->getSensor() != NULL && hasEO)
        {
                if (!hasRotations)
                        calculateRotations();

                double xi0 = myImage->getSensor()->getPrincipalPointCoordinates().getXi();
                double eta0 = myImage->getSensor()->getPrincipalPointCoordinates().getEta();
                double c = myImage->getSensor()->getFocalDistance();

                return (r11*(xi-xi0) + r12*(eta-eta0) - r13*c) / (r31*(xi-xi0) + r32*(eta-eta0) - r33*c);
        }
        else
        {
                return 0.0;
        }
}

double RayTester::getKy(double xi, double eta)
{
        if (myImage != NULL && myImage->getSensor() != NULL && hasEO)
        {
                if (!hasRotations)
                        calculateRotations();

                double xi0 = myImage->getSensor()->getPrincipalPointCoordinates().getXi();
                double eta0 = myImage->getSensor()->getPrincipalPointCoordinates().getEta();
                double c = myImage->getSensor()->getFocalDistance();

                return (r21*(xi-xi0) + r22*(eta-eta0) - r23*c) / (r31*(xi-xi0) + r32*(eta-eta0) - r33*c);
        }
        else
        {
                return 0.0;
        }
}

// Internal methods

PositionMatrix RayTester::imageZToDetectorZ(const PositionMatrix& myDigitalPositionsWithZ)
{
        if (myImage != NULL && hasIO)
        {
                PositionMatrix result(myDigitalPositionsWithZ.getRows(), "mm");
                DetectorSpaceCoordinate analogCoordinate;

                for (unsigned int i = 1; i+2 <= myDigitalPositionsWithZ.getRows(); i += 3)
                {
                        analogCoordinate = imageToDetector(myDigitalPositionsWithZ.get(i),myDigitalPositionsWithZ.get(i+1));
                        result.set(i,analogCoordinate.getXi());
                        result.set(i+1,analogCoordinate.getEta());
                        result.set(i+2,myDigitalPositionsWithZ.get(i+2));
                }

                return result;
        }
        else
        {
                return PositionMatrix();
        }
}

PositionMatrix RayTester::detectorZToImageZ(const PositionMatrix& myAnalogPositionsWithZ)
{
        if (myImage != NULL && hasIO)
        {
                PositionMatrix result(myAnalogPositionsWithZ.getRows(), "px");
                ImageSpaceCoordinate digitalCoordinate;

                for (unsigned int i = 1; i+2 <= myAnalogPositionsWithZ.getRows(); i += 3)
                {
                        digitalCoordinate = detectorToImage(myAnalogPositionsWithZ.get(i),myAnalogPositionsWithZ.get(i+1));
                        result.set(i,digitalCoordinate.getCol());
                        result.set(i+1,digitalCoordinate.getLin());
                        result.set(i+2,myAnalogPositionsWithZ.get(i+2));
                }

                return result;
        }
        else
        {
                return PositionMatrix();
        }
}

PositionMatrix RayTester::removeDistortionsZ(const PositionMatrix& myAnalogPositionsWithZ)
{
        if (myImage != NULL && myImage->getSensor() != NULL && hasEO)
        {
                PositionMatrix result(myAnalogPositionsWithZ.getRows(), "mm");
                DetectorSpaceCoordinate analogCoordinate;

                for (unsigned int i = 1; i+2 <= myAnalogPositionsWithZ.getRows(); i += 3)
                {
                        analogCoordinate = removeDistortions(myAnalogPositionsWithZ.get(i),myAnalogPositionsWithZ.get(i+1));
                        result.set(i,analogCoordinate.getXi());
                        result.set(i+1,analogCoordinate.getEta());
                        result.set(i+1,myAnalogPositionsWithZ.get(i+2));
                }

                return result;
        }
        else
        {
                return PositionMatrix();
        }
}

void RayTester::calculateRotations()
{
        if (myImage != NULL && hasEO)
        {
                r11 = cos(phi)*cos(kappa);
                r12 = -cos(phi)*sin(kappa);
                r13 = sin(phi);
                r21 = cos(omega)*sin(kappa)+sin(omega)*sin(phi)*cos(kappa);
                r22 = cos(omega)*cos(kappa)-sin(omega)*sin(phi)*sin(kappa);
                r23 = -sin(omega)*cos(phi);
                r31 = sin(omega)*sin(kappa)-cos(omega)*sin(phi)*cos(kappa);
                r32 = sin(omega)*cos(kappa)+cos(omega)*sin(phi)*sin(kappa);
                r33 = cos(omega)*cos(phi);

                hasRotations = true;
        }
}

// Accessors

void RayTester::setImage(Image* newImage)
{
        myImage = newImage;
}

Image* RayTester::getImage()
{
        return myImage;
}

// Other methods

DetectorSpaceCoordinate RayTester::imageToDetector(double col, double lin)
{
        if (myImage != NULL && hasIO)
        {
                DetectorSpaceCoordinate result;

                result.setXi(a0 + a1*col + a2*lin);
                result.setEta(b0 + b1*col + b2*lin);

                result.setAvailable(true);
                result.setSigmaAvailable(false);

                return result;
        }
        else
        {
                return DetectorSpaceCoordinate();
        }
}

DetectorSpaceCoordinate RayTester::imageToDetector(ImageSpaceCoordinate myDigitalCoordinate)
{
        return imageToDetector(myDigitalCoordinate.getCol(), myDigitalCoordinate.getLin());
}

PositionMatrix RayTester::imageToDetector(const PositionMatrix& myDigitalPositions)
{
        if (myImage != NULL && hasIO)
        {
                PositionMatrix result(myDigitalPositions.getRows(), "mm");
                DetectorSpaceCoordinate analogCoordinate;

                for (unsigned int i = 1; i+1 <= myDigitalPositions.getRows(); i += 2)
                {
                        analogCoordinate = imageToDetector(myDigitalPositions.get(i),myDigitalPositions.get(i+1));
                        result.set(i,analogCoordinate.getXi());
                        result.set(i+1,analogCoordinate.getEta());
                }

                return result;
        }
        else
        {
                return PositionMatrix();
        }
}

ImageSpaceCoordinate RayTester::detectorToImage(double xi, double eta)
{
        if (myImage != NULL && hasIO)
        {
                ImageSpaceCoordinate result;

                // From Silveira, M.T. - Master Thesis - UERJ, Geomatica 2005
                result.setCol(((b2*xi - b2*a0 - a2*eta + b0*a2) / (a1*b2 - b1*a2)));
                result.setLin(((a1*eta - a1*b0 - b1*xi + b1*a0) / (a1*b2 - b1*a2)));

                result.setAvailable(true);

                return result;
        }
        else
        {
                return ImageSpaceCoordinate();
        }
}

ImageSpaceCoordinate RayTester::detectorToImage(DetectorSpaceCoordinate myAnalogCoordinate)
{
        return detectorToImage(myAnalogCoordinate.getXi(), myAnalogCoordinate.getEta());
}

PositionMatrix RayTester::detectorToImage(const PositionMatrix& myAnalogPositions)
{
        if (myImage != NULL && hasIO)
        {
                PositionMatrix result(myAnalogPositions.getRows(), "px");
                ImageSpaceCoordinate digitalCoordinate;

                for (unsigned int i = 1; i+1 <= myAnalogPositions.getRows(); i += 2)
                {
                        digitalCoordinate = detectorToImage(myAnalogPositions.get(i),myAnalogPositions.get(i+1));
                        result.set(i,digitalCoordinate.getCol());
                        result.set(i+1,digitalCoordinate.getLin());
                }

                return result;
        }
        else
        {
                return PositionMatrix();
        }
}

ObjectSpaceCoordinate RayTester::detectorToObject(double xi, double eta, double Z)
{
        if (myImage != NULL && myImage->getSensor() != NULL && hasEO)
        {
                ObjectSpaceCoordinate result;

                if (!hasRotations)
                        calculateRotations();

                double xi0 = myImage->getSensor()->getPrincipalPointCoordinates().getXi();
                double eta0 = myImage->getSensor()->getPrincipalPointCoordinates().getEta();
                double c = myImage->getSensor()->getFocalDistance();

                double kx = (r11*(xi-xi0) + r12*(eta-eta0) - r13*c) / (r31*(xi-xi0) + r32*(eta-eta0) - r33*c);
                double ky = (r21*(xi-xi0) + r22*(eta-eta0) - r23*c) / (r31*(xi-xi0) + r32*(eta-eta0) - r33*c);

                double X = X0 + (Z - Z0) * kx;
                double Y = Y0 + (Z - Z0) * ky;

                result.setX(X);
                result.setY(Y);
                result.setZ(Z);
                result.setAvailable(true);

                return result;
        }
        else
        {
                return ObjectSpaceCoordinate();
        }
}

ObjectSpaceCoordinate RayTester::detectorToObject(DetectorSpaceCoordinate myAnalogCoordinate, double Z)
{
        return detectorToObject(myAnalogCoordinate.getXi(), myAnalogCoordinate.getEta(), Z);
}

// Note: this only works with fixed Z value!
PositionMatrix RayTester::detectorToObject(const PositionMatrix& myAnalogPositions, double Z)
{
        if (myImage != NULL && myImage->getSensor() != NULL && hasEO && myAnalogPositions.getRows()%2 == 0)
        {
                PositionMatrix result(myAnalogPositions.getRows() * 3 / 2, "m");
                ObjectSpaceCoordinate objectCoordinate;

                unsigned int i, j;
                for (i = 1, j = 1; i+1 <= myAnalogPositions.getRows(); i += 2, j += 3)
                {
                        objectCoordinate = detectorToObject(myAnalogPositions.get(i),myAnalogPositions.get(i+1),Z);
                        result.set(j,objectCoordinate.getX());
                        result.set(j+1,objectCoordinate.getY());
                        result.set(j+2,objectCoordinate.getZ());
                }

                return result;
        }
        else
        {
                return PositionMatrix();
        }
}

// Note: for this one, the analogic positions matrix must have the Z value for each coordinate!
PositionMatrix RayTester::detectorToObject(const PositionMatrix& myAnalogPositionsWithZ)
{
        if (myImage != NULL && myImage->getSensor() != NULL && hasEO)
        {
                PositionMatrix result(myAnalogPositionsWithZ.getRows(), "m");
                ObjectSpaceCoordinate objectCoordinate;

                for (unsigned int i = 1; i+2 <= myAnalogPositionsWithZ.getRows(); i += 3)
                {
                        objectCoordinate = detectorToObject(myAnalogPositionsWithZ.get(i),myAnalogPositionsWithZ.get(i+1),myAnalogPositionsWithZ.get(i+2));
                        result.set(i,objectCoordinate.getX());
                        result.set(i+1,objectCoordinate.getY());
                        result.set(i+2,objectCoordinate.getZ());
                }

                return result;
        }
        else
        {
                return PositionMatrix();
        }
}

DetectorSpaceCoordinate RayTester::objectToDetector(double X, double Y, double Z)
{
        if (myImage != NULL && myImage->getSensor() != NULL && hasEO)
        {
                DetectorSpaceCoordinate result;

                if (!hasRotations)
                        calculateRotations();

                double xi0 = myImage->getSensor()->getPrincipalPointCoordinates().getXi();
                double eta0 = myImage->getSensor()->getPrincipalPointCoordinates().getEta();
                double c = myImage->getSensor()->getFocalDistance();

                double xi = xi0 - c*(r11*(X-X0)+r21*(Y-Y0)+r31*(Z-Z0)) / (r13*(X-X0)+r23*(Y-Y0)+r33*(Z-Z0));
                double eta = eta0 - c*(r12*(X-X0)+r22*(Y-Y0)+r32*(Z-Z0)) / (r13*(X-X0)+r23*(Y-Y0)+r33*(Z-Z0));

                result.setXi(xi);
                result.setEta(eta);
                result.setAvailable(true);

                return result;
        }
        else
        {
                return DetectorSpaceCoordinate();
        }
}

DetectorSpaceCoordinate RayTester::objectToDetector(ObjectSpaceCoordinate myObjectCoordinate)
{
        return objectToDetector(myObjectCoordinate.getX(), myObjectCoordinate.getY(), myObjectCoordinate.getZ());
}

PositionMatrix RayTester::objectToDetector(const PositionMatrix& myObjectPositions)
{
        if (myImage != NULL && myImage->getSensor() != NULL && hasEO && myObjectPositions.getRows()%3 == 0)
        {
                PositionMatrix result(myObjectPositions.getRows() * 2 / 3, "mm");
                DetectorSpaceCoordinate analogCoordinate;

                unsigned int i, j;
                for (i = 1, j = 1; i+2 <= myObjectPositions.getRows(); i += 3, j += 2)
                {
                        analogCoordinate = objectToDetector(myObjectPositions.get(i),myObjectPositions.get(i+1),myObjectPositions.get(i+2));
                        result.set(j,analogCoordinate.getXi());
                        result.set(j+1,analogCoordinate.getEta());
                }

                return result;
        }
        else
        {
                return PositionMatrix();
        }
}

DetectorSpaceCoordinate RayTester::applyDistortions(double xi, double eta)
{
        // Por enquanto estou aplicando todas. Crio a flag depois.

        DetectorSpaceCoordinate radial = getRadialDistortions(xi, eta);
        DetectorSpaceCoordinate decentered = getDecenteredDistortions(xi, eta);
        DetectorSpaceCoordinate atmosphere = getAtmosphereDistortions(xi, eta);
        DetectorSpaceCoordinate curvature = getCurvatureDistortions(xi, eta);

        double xiFinal = xi + (radial.getXi() + decentered.getXi() + atmosphere.getXi() + curvature.getXi());
        double etaFinal = eta + (radial.getEta() + decentered.getEta() + atmosphere.getEta() + curvature.getEta());

        DetectorSpaceCoordinate result;
        result.setXi(xiFinal);
        result.setEta(etaFinal);
        result.setAvailable(true);

        return result;
}

DetectorSpaceCoordinate RayTester::applyDistortions(DetectorSpaceCoordinate myAnalogCoordinate)
{
        return applyDistortions(myAnalogCoordinate.getXi(), myAnalogCoordinate.getEta());
}

PositionMatrix RayTester::applyDistortions(const PositionMatrix& myAnalogPositions)
{
        if (myImage != NULL && myImage->getSensor() != NULL && hasEO)
        {
                PositionMatrix result(myAnalogPositions.getRows(), "mm");
                DetectorSpaceCoordinate analogCoordinate;

                for (unsigned int i = 1; i+1 <= myAnalogPositions.getRows(); i += 2)
                {
                        analogCoordinate = applyDistortions(myAnalogPositions.get(i),myAnalogPositions.get(i+1));
                        result.set(i,analogCoordinate.getXi());
                        result.set(i+1,analogCoordinate.getEta());
                }

                return result;
        }
        else
        {
                return PositionMatrix();
        }
}

DetectorSpaceCoordinate RayTester::removeDistortions(double xi, double eta)
{
        // Por enquanto estou aplicando todas. Crio a flag depois.

        DetectorSpaceCoordinate radial = getRadialDistortions(xi, eta);
        DetectorSpaceCoordinate decentered = getDecenteredDistortions(xi, eta);
        DetectorSpaceCoordinate atmosphere = getAtmosphereDistortions(xi, eta);
        DetectorSpaceCoordinate curvature = getCurvatureDistortions(xi, eta);

        double xiFinal = xi - (radial.getXi() + decentered.getXi() + atmosphere.getXi() + curvature.getXi());
        double etaFinal = eta - (radial.getEta() + decentered.getEta() + atmosphere.getEta() + curvature.getEta());

        DetectorSpaceCoordinate result;
        result.setXi(xiFinal);
        result.setEta(etaFinal);
        result.setAvailable(true);

        return result;
}

DetectorSpaceCoordinate RayTester::removeDistortions(DetectorSpaceCoordinate myAnalogCoordinate)
{
        return removeDistortions(myAnalogCoordinate.getXi(), myAnalogCoordinate.getEta());
}

PositionMatrix RayTester::removeDistortions(const PositionMatrix& myAnalogPositions)
{
        if (myImage != NULL && myImage->getSensor() != NULL && hasEO)
        {
                PositionMatrix result(myAnalogPositions.getRows(), "mm");
                DetectorSpaceCoordinate analogCoordinate;

                for (unsigned int i = 1; i+1 <= myAnalogPositions.getRows(); i += 2)
                {
                        analogCoordinate = removeDistortions(myAnalogPositions.get(i),myAnalogPositions.get(i+1));
                        result.set(i,analogCoordinate.getXi());
                        result.set(i+1,analogCoordinate.getEta());
                }

                return result;
        }
        else
        {
                return PositionMatrix();
        }
}

ObjectSpaceCoordinate RayTester::imageToObject(double col, double lin, double Z, bool useDistortions)
{
        if (myImage != NULL && myImage->getSensor() != NULL && hasIO && hasEO)
        {
                DetectorSpaceCoordinate analog = imageToDetector(col, lin);
                if (useDistortions)
                        analog = removeDistortions(analog);
                return detectorToObject(analog, Z);
        }
        else
        {
                return ObjectSpaceCoordinate();
        }
}

ObjectSpaceCoordinate RayTester::imageToObject(ImageSpaceCoordinate myDigitalCoordinate, double Z, bool useDistortions)
{
        return imageToObject(myDigitalCoordinate.getCol(), myDigitalCoordinate.getLin(), Z, useDistortions);
}

// Note: this only works with fixed Z value!
PositionMatrix RayTester::imageToObject(const PositionMatrix& myDigitalPositions, double Z, bool useDistortions)
{
        if (myImage != NULL && myImage->getSensor() != NULL && hasIO && hasEO)
        {
                PositionMatrix myAnalogPositions = imageToDetector(myDigitalPositions);
                if (useDistortions)
                        myAnalogPositions = removeDistortions(myAnalogPositions);
                return detectorToObject(myAnalogPositions, Z);
        }
        else
        {
                return PositionMatrix();
        }
}

// Note: for this one, the digital positions matrix must have the Z value for each coordinate!
PositionMatrix RayTester::imageToObject(const PositionMatrix& myDigitalPositionsWithZ, bool useDistortions)
{
        if (myImage != NULL && myImage->getSensor() != NULL && hasIO && hasEO)
        {
                PositionMatrix myAnalogPositions = imageZToDetectorZ(myDigitalPositionsWithZ);
                if (useDistortions)
                        myAnalogPositions = removeDistortionsZ(myAnalogPositions);
                return detectorToObject(myAnalogPositions);
        }
        else
        {
                return PositionMatrix();
        }
}

ImageSpaceCoordinate RayTester::objectToImage(double X, double Y, double Z, bool useDistortions)
{
        if (myImage != NULL && myImage->getSensor() != NULL && hasIO && hasEO)
        {
                DetectorSpaceCoordinate analog = objectToDetector(X, Y, Z);
                if (useDistortions)
                        analog = applyDistortions(analog);
                return detectorToImage(analog);
        }
        else
        {
                return ImageSpaceCoordinate();
        }
}

ImageSpaceCoordinate RayTester::objectToImage(ObjectSpaceCoordinate myObjectCoordinate, bool useDistortions)
{
        return objectToImage(myObjectCoordinate.getX(), myObjectCoordinate.getY(), myObjectCoordinate.getZ(), useDistortions);
}

PositionMatrix RayTester::objectToImage(const PositionMatrix& myObjectPositions, bool useDistortions)
{
        if (myImage != NULL && myImage->getSensor() != NULL && hasIO && hasEO)
        {
                PositionMatrix myAnalogPositions = objectToDetector(myObjectPositions);
                if (useDistortions)
                        myAnalogPositions = applyDistortions(myAnalogPositions);
                return detectorToImage(myAnalogPositions);
        }
        else
        {
                return PositionMatrix();
        }
}

// Private support methods

DetectorSpaceCoordinate RayTester::getRadialDistortions(double xi, double eta)
{
        DetectorSpaceCoordinate result;
        result.setXi(0.0);
        result.setEta(0.0);
        result.setAvailable(true);

        Sensor* sensor = myImage->getSensor();
        FrameSensor* frame = dynamic_cast<FrameSensor*>(sensor);
        if (frame != NULL)
        {
                double r = sqrt(xi*xi+eta*eta);
                deque<RadialSymmetricDistortionCoefficient> rs = frame->getRadialSymmetricCoefficients();
                double dr = 0;

                for (unsigned int i = 0; i < rs.size(); i++)
                {
                        dr += rs.at(i).getValue() * pow(r,2*i+1);
                }

                double dx = dr * xi / r;
                double dy = dr * eta / r;

                result.setXi(dx);
                result.setEta(dy);
        }

        return result;
}

DetectorSpaceCoordinate RayTester::getDecenteredDistortions(double xi, double eta)
{
        DetectorSpaceCoordinate result;
        result.setXi(0.0);
        result.setEta(0.0);
        result.setAvailable(true);

        Sensor* sensor = myImage->getSensor();
        FrameSensor* frame = dynamic_cast<FrameSensor*>(sensor);
        if (frame != NULL)
        {
                double r = sqrt(xi*xi+eta*eta);
                deque<DecenteredDistortionCoefficient> dec = frame->getDecenteredCoefficients();
                double P1 = dec.at(0).getValue();
                double P2 = dec.at(1).getValue();

                double dx = P1 * (r*r + 2*xi*xi) + 2*P2*xi*eta;
                double dy = 2*P1*xi*eta + P2 * (r*r + 2*eta*eta);

                result.setXi(dx);
                result.setEta(dy);
        }

        return result;
}

DetectorSpaceCoordinate RayTester::getAtmosphereDistortions(double xi, double eta)
{
        DetectorSpaceCoordinate result;
        result.setXi(0.0);
        result.setEta(0.0);
        result.setAvailable(true);

        Flight* flight = myImage->getFlight();
        Sensor* sensor = myImage->getSensor();
        if (flight != NULL && sensor != NULL)
        {
                Terrain* terrain = flight->getTerrain();
                if (terrain != NULL)
                {
                        if (!hasEO)
                        {
                                Z0 = flight->getHeight()+terrain->getMeanAltitude();
                        }
                        double Zp = terrain->getMeanAltitude();

                        Z0 /= 1000;
                        Zp /= 1000;

                        double aux1 = Z0 / ((Z0*Z0) - 6*Z0 + 250);
                        double aux2 = Zp / Z0*((Zp*Zp) - 6*Zp + 250);

                        double K = 2410 * 0.000001 * (aux1 - aux2);

                        double r = sqrt(xi*xi+eta*eta);
                        double f = sensor->getFocalDistance();

                        double dr = r * (1 + (r*r/f*f)) * K;

                        double dx = dr * xi / r;
                        double dy = dr * eta / r;

                        result.setXi(dx);
                        result.setEta(dy);
                }
        }

        return result;
}

DetectorSpaceCoordinate RayTester::getCurvatureDistortions(double xi, double eta)
{
        DetectorSpaceCoordinate result;
        result.setXi(0.0);
        result.setEta(0.0);
        result.setAvailable(true);

        Flight* flight = myImage->getFlight();
        Sensor* sensor = myImage->getSensor();
        if (flight != NULL && sensor != NULL)
        {
                Terrain* terrain = flight->getTerrain();
                if (terrain != NULL)
                {
                        if (!hasEO)
                        {
                                Z0 = flight->getHeight()+terrain->getMeanAltitude();
                        }

                        Z0 /= 1000;

                        double R = 6371004.0; // De onde eu tiro isso? Deixo essa constante?
                        double r = sqrt(xi*xi+eta*eta);
                        double f = sensor->getFocalDistance();

                        double aux = (r*r*Z0)/(2*R*f*f);

                        double dx = xi*aux;
                        double dy = eta*aux;

                        result.setXi(dx);
                        result.setEta(dy);
                }
        }

        return result;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
