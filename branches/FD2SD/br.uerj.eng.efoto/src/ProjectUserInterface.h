/**************************************************************************
							 ProjectUserInterface.h
**************************************************************************/


#ifndef PROJECTUSERINTERFACE_H
#define PROJECTUSERINTERFACE_H

#include "ProjectManager.h"

class ProjectManager;

/**
  * class ProjectUserInterface
  *
  * @author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 30/06/2009
  * @version 1.0 - Rafael Alves de Aguiar & Irving da Silva Badolato
  *
  */

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
	virtual void stop() = 0;
};

#endif //PROJECTUSERINTERFACE_H
