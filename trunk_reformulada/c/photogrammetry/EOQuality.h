/************************************************************************
                        EOQuality.h
**************************************************************************/

#ifndef EOQUALITY_H
#define EOQUALITY_H

#include "EObject.h"
#include "Matrix.h"

class ExteriorOrientation;
class SpatialRessection;

/**
  * class EOQuality
  *
  * @Author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 06/05/2009
  * @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato - Initial version.
  *
  */

class EOQuality : public EObject
{
    //EOBJECT

private:


    // Private attributes
    //
    Matrix V;
    double sigma0Squared;
    Matrix SigmaXa;
    Matrix SigmaLa;

public:

    // Constructors and Destructor
    //
    virtual ~EOQuality();

    // Private attribute accessors
    //
    Matrix getV();
    double getsigma0Squared();
    Matrix getSigmaXa();
    Matrix getSigmaLa();

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
    void calculate(SpatialRessection* myEO);

};

#endif // EOQUALITY_H
