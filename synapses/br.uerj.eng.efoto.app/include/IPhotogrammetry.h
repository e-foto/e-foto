#ifndef BR_UERJ_ENG_EFOTO_APP_IPHOTOGRAMMETRY_H
#define BR_UERJ_ENG_EFOTO_APP_IPHOTOGRAMMETRY_H

#include "ISynapse.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {
namespace app {

class IPhotogrammetry: public cortex::ISynapse
{
public:
	virtual ~IPhotogrammetry(){}

};

typedef QSharedPointer<IPhotogrammetry> IPhotogrammetryPtr;


}
}
}
}
} //namespace

Q_DECLARE_INTERFACE(br::uerj::eng::efoto::app::IPhotogrammetry, "br.uerj.eng.efoto.app.IPhotogrammetry/1.0")

#endif	//BR_UERJ_ENG_EFOTO_APP_IPHOTOGRAMMETRY_H
