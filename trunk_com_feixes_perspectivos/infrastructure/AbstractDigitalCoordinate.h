/*******************************************************************************
                        AbstractDigitalCoordinate.h
*******************************************************************************/


#ifndef ABSTRACTDIGITALCOORDINATE_H
#define ABSTRACTDIGITALCOORDINATE_H

#include "Coordinate.h"

/**
* class AbstractDigitalCoordinate
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 06/05/2009
* @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato.
*/

class AbstractDigitalCoordinate : public Coordinate
{
    //EOBJECT

protected:

    // Private attributes
    //
    int lin;
    int col;
    string unit;

public:

    //Constructors and destructors methods
    //
    virtual ~AbstractDigitalCoordinate();

    // Private attribute accessor methods
    //
    void setLin(int newLin);
    void setCol(int newCol);
    void setUnit(string newUnit);
    int getLin();
    int getCol();
    string getUnit();

    //Matrix use methods
    //
    void setPosition(const PositionMatrix& newPosition);
    PositionMatrix getPosition();

    // EObject methods
    //
    string objectType(void);

};

#endif //ABSTRACTDIGITALCOORDINATE_H
