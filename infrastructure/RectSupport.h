#ifndef RECTSUPPORT_H
#define RECTSUPPORT_H

#include "EObject.h"

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
