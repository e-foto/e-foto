/************************************************************************
                        ObjectSpaceCoordinate.h
**************************************************************************/


#ifndef OBJECTSPACECOORDINATE_H
#define OBJECTSPACECOORDINATE_H

#include "Coordinate.h"

/**
* class ObjectSpaceCoordinate
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 06/05/2009
* @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato.
*/

class ObjectSpaceCoordinate : public Coordinate
{
    //EOBJECT

private:

    // Private attributes
    //
    double x;
    double y;
    double z;
    double sigmaX;
    double sigmaY;
    double sigmaZ;
    double sigmaXY;
    double sigmaXZ;
    double sigmaYZ;
    string unit;

public:

    // Constructors
    //
    ObjectSpaceCoordinate();
    ObjectSpaceCoordinate(string myUnit, double myX, double myY, double myZ, double mySigmaX = 1, double mySigmaY = 1, double mySigmaZ = 1, double mySigmaXY = 0, double mySigmaXZ = 0, double mySigmaYZ = 0);
    ObjectSpaceCoordinate(const PositionMatrix& myPosition);
    ObjectSpaceCoordinate(const PositionMatrix& myPosition, const Matrix& myPositionSigmas);

    // Private attribute accessor methods
    //
    void setX(double newX);
    void setY(double newY);
    void setZ(double newZ);
    void setSigmaX(double newSigmaX);
    void setSigmaY(double newSigmaY);
    void setSigmaZ(double newSigmaZ);
    void setSigmaXY(double newSigmaXY);
    void setSigmaXZ(double newSigmaXZ);
    void setSigmaYZ(double newSigmaYZ);
    void setUnit(string newUnit);
    double getX();
    double getY();
    double getZ();
    double getSigmaX();
    double getSigmaY();
    double getSigmaZ();
    double getSigmaXY();
    double getSigmaXZ();
    double getSigmaYZ();
    string getUnit();

    //Matrix use methods
    //
    void setPosition(const PositionMatrix& newPosition);
    void setPositionSigmas(const Matrix& newPosition);
    PositionMatrix getPosition();
    Matrix getPositionSigmas();

    // EObject methods
    //
    string objectType(void);
    string objectAssociations(void);
    bool is(string s);

    // XML methods
    //
    void xmlSetData(string xml);
    string xmlGetData();

};

#endif // OBJECTSPACECOORDINATE_H
