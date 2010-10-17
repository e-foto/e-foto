/*******************************************************************************
                        DigitalImageSpaceCoordinate.h
*******************************************************************************/


#ifndef DIGITALIMAGESPACECOORDINATE_H
#define DIGITALIMAGESPACECOORDINATE_H

#include "AbstractDigitalCoordinate.h"

class DigitalImageSpaceCoordinate : public AbstractDigitalCoordinate
{
    //EOBJECT

private:

    // Private attributes
    //
    int imageId;

public:

    // Constructors
    //
    DigitalImageSpaceCoordinate();
    DigitalImageSpaceCoordinate(int myImageId);
    DigitalImageSpaceCoordinate(int myImageId, int myCol, int myLin);
    DigitalImageSpaceCoordinate(int myImageId, const PositionMatrix& myPosition);
    ~DigitalImageSpaceCoordinate();

    // Private attribute accessor methods
    //
    void setImageId(int newImageId);
    int getImageId();

    // EObject methods
    //
    string objectType(void);
    string objectAssociations(void);
    bool is(string s);

    // Xml methods
    //
    string xmlGetData();
    void xmlSetData(string xml);

};

#endif // DIGITALIMAGESPACECOORDINATE_H
