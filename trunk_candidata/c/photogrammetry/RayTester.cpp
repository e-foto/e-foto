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

PositionMatrix RayTester::digitalZToAnalogZ(const PositionMatrix& myDigitalPositionsWithZ)
{
	if (myImage != NULL && hasIO)
	{
		PositionMatrix result(myDigitalPositionsWithZ.getRows(), "mm");
		AnalogImageSpaceCoordinate analogCoordinate;

		for (unsigned int i = 1; i+2 <= myDigitalPositionsWithZ.getRows(); i += 3)
		{
			analogCoordinate = digitalToAnalog(myDigitalPositionsWithZ.getInt(i),myDigitalPositionsWithZ.getInt(i+1));
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

PositionMatrix RayTester::analogZToDigitalZ(const PositionMatrix& myAnalogPositionsWithZ)
{
	if (myImage != NULL && hasIO)
	{
		PositionMatrix result(myAnalogPositionsWithZ.getRows(), "px");
		DigitalImageSpaceCoordinate digitalCoordinate;

		for (unsigned int i = 1; i+2 <= myAnalogPositionsWithZ.getRows(); i += 3)
		{
			digitalCoordinate = analogToDigital(myAnalogPositionsWithZ.get(i),myAnalogPositionsWithZ.get(i+1));
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
		AnalogImageSpaceCoordinate analogCoordinate;

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

AnalogImageSpaceCoordinate RayTester::digitalToAnalog(unsigned int col, unsigned int lin)
{
	if (myImage != NULL && hasIO)
	{
		AnalogImageSpaceCoordinate result;

		result.setXi(a0 + a1*col + a2*lin);
		result.setEta(b0 + b1*col + b2*lin);

		result.setAvailable(true);
		result.setSigmaAvailable(false);

		return result;
	}
	else
	{
		return AnalogImageSpaceCoordinate();
	}
}

AnalogImageSpaceCoordinate RayTester::digitalToAnalog(DigitalImageSpaceCoordinate myDigitalCoordinate)
{
	return digitalToAnalog(myDigitalCoordinate.getCol(), myDigitalCoordinate.getLin());
}

PositionMatrix RayTester::digitalToAnalog(const PositionMatrix& myDigitalPositions)
{
	if (myImage != NULL && hasIO)
	{
		PositionMatrix result(myDigitalPositions.getRows(), "mm");
		AnalogImageSpaceCoordinate analogCoordinate;

		for (unsigned int i = 1; i+1 <= myDigitalPositions.getRows(); i += 2)
		{
			analogCoordinate = digitalToAnalog(myDigitalPositions.getInt(i),myDigitalPositions.getInt(i+1));
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

DigitalImageSpaceCoordinate RayTester::analogToDigital(double xi, double eta)
{
	if (myImage != NULL && hasIO)
	{
		DigitalImageSpaceCoordinate result;

		// From Silveira, M.T. - Master Thesis - UERJ, Geomatica 2005
		result.setCol((int)((b2*xi - b2*a0 - a2*eta + b0*a2) / (a1*b2 - b1*a2)));
		result.setLin((int)((a1*eta - a1*b0 - b1*xi + b1*a0) / (a1*b2 - b1*a2)));

		result.setAvailable(true);

		return result;
	}
	else
	{
		return DigitalImageSpaceCoordinate();
	}
}

DigitalImageSpaceCoordinate RayTester::analogToDigital(AnalogImageSpaceCoordinate myAnalogCoordinate)
{
	return analogToDigital(myAnalogCoordinate.getXi(), myAnalogCoordinate.getEta());
}

PositionMatrix RayTester::analogToDigital(const PositionMatrix& myAnalogPositions)
{
	if (myImage != NULL && hasIO)
	{
		PositionMatrix result(myAnalogPositions.getRows(), "px");
		DigitalImageSpaceCoordinate digitalCoordinate;

		for (unsigned int i = 1; i+1 <= myAnalogPositions.getRows(); i += 2)
		{
			digitalCoordinate = analogToDigital(myAnalogPositions.get(i),myAnalogPositions.get(i+1));
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

ObjectSpaceCoordinate RayTester::analogToObject(double xi, double eta, double Z)
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

ObjectSpaceCoordinate RayTester::analogToObject(AnalogImageSpaceCoordinate myAnalogCoordinate, double Z)
{
	return analogToObject(myAnalogCoordinate.getXi(), myAnalogCoordinate.getEta(), Z);
}

// Note: this only works with fixed Z value!
PositionMatrix RayTester::analogToObject(const PositionMatrix& myAnalogPositions, double Z)
{
	if (myImage != NULL && myImage->getSensor() != NULL && hasEO && myAnalogPositions.getRows()%2 == 0)
	{
		PositionMatrix result(myAnalogPositions.getRows() * 3 / 2, "m");
		ObjectSpaceCoordinate objectCoordinate;

		unsigned int i, j;
		for (i = 1, j = 1; i+1 <= myAnalogPositions.getRows(); i += 2, j += 3)
		{
			objectCoordinate = analogToObject(myAnalogPositions.get(i),myAnalogPositions.get(i+1),Z);
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
PositionMatrix RayTester::analogToObject(const PositionMatrix& myAnalogPositionsWithZ)
{
	if (myImage != NULL && myImage->getSensor() != NULL && hasEO)
	{
		PositionMatrix result(myAnalogPositionsWithZ.getRows(), "m");
		ObjectSpaceCoordinate objectCoordinate;

		for (unsigned int i = 1; i+2 <= myAnalogPositionsWithZ.getRows(); i += 3)
		{
			objectCoordinate = analogToObject(myAnalogPositionsWithZ.get(i),myAnalogPositionsWithZ.get(i+1),myAnalogPositionsWithZ.get(i+2));
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

AnalogImageSpaceCoordinate RayTester::objectToAnalog(double X, double Y, double Z)
{
	if (myImage != NULL && myImage->getSensor() != NULL && hasEO)
	{
		AnalogImageSpaceCoordinate result;

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
		return AnalogImageSpaceCoordinate();
	}
}

AnalogImageSpaceCoordinate RayTester::objectToAnalog(ObjectSpaceCoordinate myObjectCoordinate)
{
	return objectToAnalog(myObjectCoordinate.getX(), myObjectCoordinate.getY(), myObjectCoordinate.getZ());
}

PositionMatrix RayTester::objectToAnalog(const PositionMatrix& myObjectPositions)
{
	if (myImage != NULL && myImage->getSensor() != NULL && hasEO && myObjectPositions.getRows()%3 == 0)
	{
		PositionMatrix result(myObjectPositions.getRows() * 2 / 3, "mm");
		AnalogImageSpaceCoordinate analogCoordinate;

		unsigned int i, j;
		for (i = 1, j = 1; i+2 <= myObjectPositions.getRows(); i += 3, j += 2)
		{
			analogCoordinate = objectToAnalog(myObjectPositions.get(i),myObjectPositions.get(i+1),myObjectPositions.get(i+2));
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

AnalogImageSpaceCoordinate RayTester::applyDistortions(double xi, double eta)
{
	// Por enquanto estou aplicando todas. Crio a flag depois.

	AnalogImageSpaceCoordinate radial = getRadialDistortions(xi, eta);
	AnalogImageSpaceCoordinate decentered = getDecenteredDistortions(xi, eta);
	AnalogImageSpaceCoordinate atmosphere = getAtmosphereDistortions(xi, eta);
	AnalogImageSpaceCoordinate curvature = getCurvatureDistortions(xi, eta);

	double xiFinal = xi + (radial.getXi() + decentered.getXi() + atmosphere.getXi() + curvature.getXi());
	double etaFinal = eta + (radial.getEta() + decentered.getEta() + atmosphere.getEta() + curvature.getEta());

	AnalogImageSpaceCoordinate result;
	result.setXi(xiFinal);
	result.setEta(etaFinal);
	result.setAvailable(true);

	return result;
}

AnalogImageSpaceCoordinate RayTester::applyDistortions(AnalogImageSpaceCoordinate myAnalogCoordinate)
{
	return applyDistortions(myAnalogCoordinate.getXi(), myAnalogCoordinate.getEta());
}

PositionMatrix RayTester::applyDistortions(const PositionMatrix& myAnalogPositions)
{
	if (myImage != NULL && myImage->getSensor() != NULL && hasEO)
	{
		PositionMatrix result(myAnalogPositions.getRows(), "mm");
		AnalogImageSpaceCoordinate analogCoordinate;

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

AnalogImageSpaceCoordinate RayTester::removeDistortions(double xi, double eta)
{
	// Por enquanto estou aplicando todas. Crio a flag depois.

	AnalogImageSpaceCoordinate radial = getRadialDistortions(xi, eta);
	AnalogImageSpaceCoordinate decentered = getDecenteredDistortions(xi, eta);
	AnalogImageSpaceCoordinate atmosphere = getAtmosphereDistortions(xi, eta);
	AnalogImageSpaceCoordinate curvature = getCurvatureDistortions(xi, eta);

	double xiFinal = xi - (radial.getXi() + decentered.getXi() + atmosphere.getXi() + curvature.getXi());
	double etaFinal = eta - (radial.getEta() + decentered.getEta() + atmosphere.getEta() + curvature.getEta());

	AnalogImageSpaceCoordinate result;
	result.setXi(xiFinal);
	result.setEta(etaFinal);
	result.setAvailable(true);

	return result;
}

AnalogImageSpaceCoordinate RayTester::removeDistortions(AnalogImageSpaceCoordinate myAnalogCoordinate)
{
	return removeDistortions(myAnalogCoordinate.getXi(), myAnalogCoordinate.getEta());
}

PositionMatrix RayTester::removeDistortions(const PositionMatrix& myAnalogPositions)
{
	if (myImage != NULL && myImage->getSensor() != NULL && hasEO)
	{
		PositionMatrix result(myAnalogPositions.getRows(), "mm");
		AnalogImageSpaceCoordinate analogCoordinate;

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

ObjectSpaceCoordinate RayTester::digitalToObject(unsigned int col, unsigned int lin, double Z, bool useDistortions)
{
	if (myImage != NULL && myImage->getSensor() != NULL && hasIO && hasEO)
	{
		AnalogImageSpaceCoordinate analog = digitalToAnalog(col, lin);
		if (useDistortions)
			analog = removeDistortions(analog);
		return analogToObject(analog, Z);
	}
	else
	{
		return ObjectSpaceCoordinate();
	}
}

ObjectSpaceCoordinate RayTester::digitalToObject(DigitalImageSpaceCoordinate myDigitalCoordinate, double Z, bool useDistortions)
{
	return digitalToObject(myDigitalCoordinate.getCol(), myDigitalCoordinate.getLin(), Z, useDistortions);
}

// Note: this only works with fixed Z value!
PositionMatrix RayTester::digitalToObject(const PositionMatrix& myDigitalPositions, double Z, bool useDistortions)
{
	if (myImage != NULL && myImage->getSensor() != NULL && hasIO && hasEO)
	{
		PositionMatrix myAnalogPositions = digitalToAnalog(myDigitalPositions);
		if (useDistortions)
			myAnalogPositions = removeDistortions(myAnalogPositions);
		return analogToObject(myAnalogPositions, Z);
	}
	else
	{
		return PositionMatrix();
	}
}

// Note: for this one, the digital positions matrix must have the Z value for each coordinate!
PositionMatrix RayTester::digitalToObject(const PositionMatrix& myDigitalPositionsWithZ, bool useDistortions)
{
	if (myImage != NULL && myImage->getSensor() != NULL && hasIO && hasEO)
	{
		PositionMatrix myAnalogPositions = digitalZToAnalogZ(myDigitalPositionsWithZ);
		if (useDistortions)
			myAnalogPositions = removeDistortionsZ(myAnalogPositions);
		return analogToObject(myAnalogPositions);
	}
	else
	{
		return PositionMatrix();
	}
}

DigitalImageSpaceCoordinate RayTester::objectToDigital(double X, double Y, double Z, bool useDistortions)
{
	if (myImage != NULL && myImage->getSensor() != NULL && hasIO && hasEO)
	{
		AnalogImageSpaceCoordinate analog = objectToAnalog(X, Y, Z);
		if (useDistortions)
			analog = applyDistortions(analog);
		return analogToDigital(analog);
	}
	else
	{
		return DigitalImageSpaceCoordinate();
	}
}

DigitalImageSpaceCoordinate RayTester::objectToDigital(ObjectSpaceCoordinate myObjectCoordinate, bool useDistortions)
{
	return objectToDigital(myObjectCoordinate.getX(), myObjectCoordinate.getY(), myObjectCoordinate.getZ(), useDistortions);
}

PositionMatrix RayTester::objectToDigital(const PositionMatrix& myObjectPositions, bool useDistortions)
{
	if (myImage != NULL && myImage->getSensor() != NULL && hasIO && hasEO)
	{
		PositionMatrix myAnalogPositions = objectToAnalog(myObjectPositions);
		if (useDistortions)
			myAnalogPositions = applyDistortions(myAnalogPositions);
		return analogToDigital(myAnalogPositions);
	}
	else
	{
		return PositionMatrix();
	}
}

// Private support methods

AnalogImageSpaceCoordinate RayTester::getRadialDistortions(double xi, double eta)
{
	AnalogImageSpaceCoordinate result;
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

AnalogImageSpaceCoordinate RayTester::getDecenteredDistortions(double xi, double eta)
{
	AnalogImageSpaceCoordinate result;
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

AnalogImageSpaceCoordinate RayTester::getAtmosphereDistortions(double xi, double eta)
{
	AnalogImageSpaceCoordinate result;
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

AnalogImageSpaceCoordinate RayTester::getCurvatureDistortions(double xi, double eta)
{
	AnalogImageSpaceCoordinate result;
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
