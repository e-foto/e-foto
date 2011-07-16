#ifndef PTUSERINTERFACE_H
#define PTUSERINTERFACE_H

#include "PTManager.h"

class PTUserInterface
{
protected:
    PTManager* manager;

public:
    PTManager* getManager();
	virtual bool exec() = 0;
};


#endif // PTUSERINTERFACE_H
