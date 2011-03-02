/**************************************************************************
							 IOUserInterface.h
**************************************************************************/


#ifndef IOUSERINTERFACE_H
#define IOUSERINTERFACE_H

#include "IOManager.h"
#include "PositionMatrix.h"

//#include "EObject.h"

/**
  * class IOUserInterface
  *
  * @author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 08/05/2009
  * @version 1.0 - Rafael Alves de Aguiar & Irving da Silva Badolato
  *
  */

class IOUserInterface //: public EObject
{

protected:

	// Associated Objects
	//
	IOManager* manager;

public:

	// Association Methods
	//
	IOManager* getManager();

	// Other Methods
	//
	virtual bool measureMark(int id, int col, int lin) = 0;
	virtual bool calculateIO() = 0;
	virtual bool exec() = 0;
	virtual void stop() = 0;

};

#endif //IOUSERINTERFACE_H
