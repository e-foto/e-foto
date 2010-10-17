/**************************************************************************
                             ProjectUserInterface.h
**************************************************************************/


#ifndef PROJECTUSERINTERFACE_H
#define PROJECTUSERINTERFACE_H

#include "ProjectManager.h"

class ProjectManager;

class ProjectUserInterface //: public EObject
{

protected:

    // Associated Objects
    //
    ProjectManager* manager;

public:

    // Association Methods
    //
    ProjectManager* getManager();

    // Other Methods
    //
    virtual bool exec() = 0;
};

#endif //PROJECTUSERINTERFACE_H
