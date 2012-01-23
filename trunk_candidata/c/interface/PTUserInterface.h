#ifndef PTUSERINTERFACE_H
#define PTUSERINTERFACE_H

#include "PTManager.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class PTUserInterface
{
protected:
	PTManager* manager;

public:
	PTManager* getManager();
	virtual bool exec() = 0;
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // PTUSERINTERFACE_H
