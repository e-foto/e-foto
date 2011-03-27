/*******************************************************************************
                        Coordinate.h
*******************************************************************************/


#ifndef COORDINATE_H
#define COORDINATE_H

#include "EObject.h"
#include "PositionMatrix.h"
#include "Matrix.h"

/******************************* Abstract Class ********************************
Coordinate does not have any pure virtual methods, but its author
  defined it as an abstract class, so you should not use it directly.
  Inherit from it instead and create only objects from the derived classes
*******************************************************************************/

/**
* class Coordinate
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 06/05/2009
* @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato.
*/

class Coordinate : public EObject
{
    //EOBJECT

protected:
    // Protected attributes
    //
    bool available;
    bool sigmaAvailable;

public:

    // Constructors and Destructors
    //
    virtual ~Coordinate();

    // EObject methods
    //
    string objectType(void);

    // Private attribute accessors
    //
    void setAvailable(bool value);
    void setSigmaAvailable(bool value);
    bool isAvailable();
    bool isSigmaAvailable();

    // Other methods
    //
    virtual PositionMatrix getPosition() = 0;
    virtual string getGmlPos();
    virtual void setPosition(const PositionMatrix& newPosition) = 0;

};

#endif // COORDINATE_H
