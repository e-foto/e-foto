/*******************************************************************************
                        PhotogrammetricPoint.h
*******************************************************************************/


#ifndef PHOTOGRAMMETRICPOINT_H
#define PHOTOGRAMMETRICPOINT_H

#include "Point.h"

class PhotogrammetricPoint : public Point
{
    //EOBJECT

public:

    // Constructors
    //
    PhotogrammetricPoint();
    PhotogrammetricPoint(int myId);
    
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

#endif // PHOTOGRAMMETRICPOINT_H
