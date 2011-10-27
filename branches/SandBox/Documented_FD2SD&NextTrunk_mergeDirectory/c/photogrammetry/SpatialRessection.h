/**************************************************************************
						SpatialRessection.h
**************************************************************************/


#ifndef SPATIALRESSECTION_H
#define SPATIALRESSECTION_H

#include "Matrix.h"
#include "ExteriorOrientation.h"
#include "Flight.h"
#include "EOQuality.h"
#include "DigitalImageSpaceCoordinate.h"
#include "AnalogImageSpaceCoordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
  * class SpatialRessection
  *
  * @author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 06/05/2009
  * @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato
  */

class SpatialRessection : public ExteriorOrientation
{
	// Private attributes
	//
	Matrix Xa;
	Matrix La;
	Matrix A;
	Matrix P;
	Matrix X0;
	Matrix L0;
	Matrix Lb;
	Matrix lastL0;

	double X00; double Y00; double Z00; double omega0; double phi0; double kappa0; // Variables just for speeding up calculations, not really needed.
	double r11; double r12; double r13; double r21; double r22; double r23; double r31; double r32; double r33; // To make code reading and maintenance easier.
	deque<int> selectedPoints;
	bool pointForFlightDirectionAvailable;
	DigitalImageSpaceCoordinate pointForFlightDirection;

	int totalIterations;
		bool gnssConverged; bool insConverged;
	bool useDistortions;

	// Composed objects
	//
	EOQuality myQuality;

	// Sets probably accessible only by the Mounter class.
	//
	void setXa(const Matrix& newXa);
	void setLa(const Matrix& newLa);
	void setA(const Matrix& newA);
	void setP(const Matrix& newP);
	void setX0(const Matrix& newX0);
	void setL0(const Matrix& newL0);
	void setLb(const Matrix& newLb);

public:

	// Constructors and destructors
	//
	SpatialRessection();
	SpatialRessection(int myImageId); // Constructor with ids only, needed in project use.
	virtual ~SpatialRessection();

	// Private attribute accessors
	//
	Matrix getXa();
	Matrix getLa();
	Matrix getA();
	Matrix getP();
	Matrix getX0();
	Matrix getL0();
	Matrix getLb();
	Matrix getLastL0();

	deque<int> getSelectedPoints();
	DigitalImageSpaceCoordinate* getPointForFlightDirection();

	int getTotalIterations();
	bool getConverged();
	bool getGnssConverged();
	bool getInsConverged();

	// Composed object accessors
	//
	void setQuality(EOQuality newQuality);
	EOQuality getQuality();

	// Selected points list manipulators
	//
	void selectPoint(int id);
	void unselectPoint(int id);
	void unselectAllPoints();
	int countSelectedPoints();

	//  Selected fiductial mark or point to indicate the direction of flight manipulators
	//
	void setPointForFlightDirection(int col, int row);
	void selectFiductialMarkForFlightDirection(int id);
	void unsetPointForFlightDirection();

	// EObject methods
	//
	string objectType(void);
	string objectAssociations(void);
	bool is(string s);

	// XML methods
	//
	void xmlSetData(string xml);
	string xmlGetData();

	// Other methods
	//
	void generateR();
	void generateInitialA();
	void generateInitialL0();
	void generateInitialP();
	void generateA();
	void generateL0();
	void generateLb();
	void generateP();
	void generateX0();
	void initialize();
	bool calculate(int maxIterations, double gnssPrecision, double insPrecision);

	// Private support methods
	//
	AnalogImageSpaceCoordinate applyDistortions(double xi, double eta);
	AnalogImageSpaceCoordinate applyDistortions(AnalogImageSpaceCoordinate myAnalogCoordinate);
	AnalogImageSpaceCoordinate removeDistortions(double xi, double eta);
	AnalogImageSpaceCoordinate removeDistortions(AnalogImageSpaceCoordinate myAnalogCoordinate);
	AnalogImageSpaceCoordinate getRadialDistortions(double xi, double eta);
	AnalogImageSpaceCoordinate getDecenteredDistortions(double xi, double eta);
	AnalogImageSpaceCoordinate getAtmosphereDistortions(double xi, double eta);
	AnalogImageSpaceCoordinate getCurvatureDistortions(double xi, double eta);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //SPATIALRESSECTION_H
