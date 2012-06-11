#ifndef SRUSERINTERFACE_H
#define SRUSERINTERFACE_H

#include "SRManager.h"
#include "PositionMatrix.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class SRUserInterface
{
protected:
    SRManager* manager;
public:
    SRManager* getManager();
    virtual bool exec() = 0;
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // SRUSERINTERFACE_H
