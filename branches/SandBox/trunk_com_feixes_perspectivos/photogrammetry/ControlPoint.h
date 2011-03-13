/*******************************************************************************
                        ControlPoint.h
*******************************************************************************/


#ifndef CONTROLPOINT_H
#define CONTROLPOINT_H

#include "Point.h"

/**
* class ControlPoint
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 06/05/2009
* @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato.
*/

class ControlPoint : public Point
{
    //EOBJECT

public:

    // Constructors
    //
    ControlPoint();
    ControlPoint(int myId);
    
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

#endif // CONTROLPOINT_H
