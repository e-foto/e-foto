/*******************************************************************************
                        AbstractAnalogCoordinate.h
*******************************************************************************/


#ifndef ABSTRACTANALOGCOORDINATE_H
#define ABSTRACTANALOGCOORDINATE_H

#include "Coordinate.h"

/**
* class AbstractAnalogCoordinate
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 06/05/2009
* @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato.
*/

class AbstractAnalogCoordinate : public Coordinate
{
    //EOBJECT

protected:

    // Private attributes
    //
    double xi;
    double eta;
    double sigmaXi;
    double sigmaEta;
    double sigmaXiEta;
    string unit;

public:

    //Constructors and destructors methods
    //
    virtual ~AbstractAnalogCoordinate();

    // Private attribute accessor methods
    //

    void setXi(double newXi);
    void setEta(double newEta);
    void setSigmaXi(double newSigmaXi);
    void setSigmaEta(double newSigmaEta);
    void setSigmaXiEta(double newSigmaXiEta);
    void setUnit(string newUnit);
    double getXi();
    double getEta();
    double getSigmaXi();
    double getSigmaEta();
    double getSigmaXiEta();
    string getUnit();

    //Matrix use methods
    //
    void setPosition(const PositionMatrix& newPosition);
    void setPositionSigmas(const Matrix& newPositionSigmas);
    PositionMatrix getPosition();
    Matrix getPositionSigmas();

    // EObject methods
    //
    string objectType(void);

};

#endif // ANALOGIMAGESPACE_H
