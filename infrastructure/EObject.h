/*******************************************************************************
                               EObject.h
*******************************************************************************/

#ifndef EOBJECT_H
#define EOBJECT_H

#include "EDom.h"

#include <string>
#include <sstream>
#include <fstream>
#include <deque>

using namespace std;

//class Mounter;

class EObject : public EDom
{
    //EOBJECT

public:

    virtual string objectType(void) = 0;
    virtual string objectAssociations(void) = 0;
    virtual bool is(string s) = 0;

};

#endif //EOBJECT_H
