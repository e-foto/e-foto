#ifndef BR_UERJ_ENG_EFOTO_IPHOTOGRAMMETRY_H
#define BR_UERJ_ENG_EFOTO_IPHOTOGRAMMETRY_H

#include "ISynapse.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class IPhotogrammetry: public cortex::ISynapse
{
public:
	virtual ~IPhotogrammetry(){}

};

typedef QSharedPointer<IPhotogrammetry> IPhotogrammetryPtr;


}
}
}
} //namespace

Q_DECLARE_INTERFACE(br::uerj::eng::efoto::IPhotogrammetry, "br.uerj.eng.efoto.IPhotogrammetry/1.0.0")

#endif	//BR_UERJ_ENG_EFOTO_IPHOTOGRAMMETRY_H
