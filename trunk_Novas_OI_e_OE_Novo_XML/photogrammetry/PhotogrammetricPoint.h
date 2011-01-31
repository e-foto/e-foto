/*******************************************************************************
                        PhotogrammetricPoint.h
*******************************************************************************/


#ifndef PHOTOGRAMMETRICPOINT_H
#define PHOTOGRAMMETRICPOINT_H

#include "Point.h"

/**
* class PhotogrammetricPoint
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 06/05/2009
* @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato.
*/

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
