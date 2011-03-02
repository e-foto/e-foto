#ifndef SRUSERINTERFACE_H
#define SRUSERINTERFACE_H

#include "SRManager.h"
#include "PositionMatrix.h"

class SRUserInterface
{
protected:
    SRManager* manager;
public:
    SRManager* getManager();
    virtual bool exec() = 0;
	virtual void stop() = 0;
};

#endif // SRUSERINTERFACE_H
