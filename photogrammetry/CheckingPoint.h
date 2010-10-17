/*******************************************************************************
                        CheckingPoint.h
*******************************************************************************/


#ifndef CHECKINGPOINT_H
#define CHECKINGPOINT_H

#include "Point.h"

class CheckingPoint : public Point
{
    //EOBJECT

public:

    // Constructors
    //
    CheckingPoint();
    CheckingPoint(int myId);
    
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

#endif // CHECKINGPOINT_H
