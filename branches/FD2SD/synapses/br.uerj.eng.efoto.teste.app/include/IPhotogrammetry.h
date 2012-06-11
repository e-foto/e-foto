#ifndef BR_UERJ_ENG_EFOTO_APPV1_IPHOTOGRAMMETRY_H
#define BR_UERJ_ENG_EFOTO_APPV1_IPHOTOGRAMMETRY_H

#include "ISynapse.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {
namespace appV1 {

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

Q_DECLARE_INTERFACE(br::uerj::eng::efoto::appV1::IPhotogrammetry, "br.uerj.eng.efoto.appV1.IPhotogrammetry/1.0")

#endif	//BR_UERJ_ENG_EFOTO_APPV1_IPHOTOGRAMMETRY_H
