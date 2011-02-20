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

    double X00, Y00, Z00, omega0, phi0, kappa0; // Variables just for speeding up calculations, not really needed.
    double r11, r12, r13, r21, r22, r23, r31, r32, r33; // To make code reading and maintenance easier.
    deque<int> selectedPoints;
    bool pointForFlightDirectionAvailable;
    DigitalImageSpaceCoordinate pointForFlightDirection;

    int totalIterations;
	bool gnssConverged, insConverged;

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

};

#endif //SPATIALRESSECTION_H
