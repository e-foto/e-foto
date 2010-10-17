/*******************************************************************************
                        DigitalFiductialMark.h
*******************************************************************************/


#ifndef DIGITALFIDUCTIALMARK_H
#define DIGITALFIDUCTIALMARK_H

#include "AbstractDigitalCoordinate.h"

class DigitalFiductialMark : public AbstractDigitalCoordinate
{
    //EOBJECT

private:

    // Private attributes
    //
    int id;
    int imageId;

public:

    // Constructors
    //
    DigitalFiductialMark();
    DigitalFiductialMark(int myId, int myImageId);
    DigitalFiductialMark(int myId, int myImageId, int myCol, int myLin);
    DigitalFiductialMark(int myId, int myImageId, const PositionMatrix& myPosition);
    ~DigitalFiductialMark();

    // Private attribute accessor methods
    //
    void setId(int newId);
    void setImageId(int newImageId);
    int getId();
    int getImageId();

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

#endif // DIGITALFIDUCTIALMARK_H
