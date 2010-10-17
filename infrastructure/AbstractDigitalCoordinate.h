/*******************************************************************************
                        AbstractDigitalCoordinate.h
*******************************************************************************/


#ifndef ABSTRACTDIGITALCOORDINATE_H
#define ABSTRACTDIGITALCOORDINATE_H

#include "Coordinate.h"

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
