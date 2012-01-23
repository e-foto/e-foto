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
namespace br {
namespace uerj {
namespace eng {
namespace efoto {


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

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //IOUSERINTERFACE_H
