#ifndef RECTSUPPORT_H
#define RECTSUPPORT_H

#include "EObject.h"

/**
* class RectSupport
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 06/05/2009
* @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato.
*/

class RectSupport : public EObject
{
    //EOBJECT

protected:

    unsigned int ncols;
    unsigned int nrows;

public:

    //Constructors and Destructors
    //
    RectSupport(unsigned int parNcols, unsigned int parNrows);
    virtual ~RectSupport();

    // EObject methods
    //
    string objectType(void);

    // Other methods
    //
    unsigned int getCols() const;
    unsigned int getRows() const;

};

#endif //RECTSUPPORT_H
