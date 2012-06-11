#ifndef BR_UERJ_ENG_EFOTO_ENGINE_IPHOTOGRAMMETRICENGINE_H
#define BR_UERJ_ENG_EFOTO_ENGINE_IPHOTOGRAMMETRICENGINE_H

#include "ISynapse.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {
namespace engine {

class IPhotogrammetricEngine: public cortex::ISynapse
{
public:
	virtual ~IPhotogrammetricEngine(){}

};

typedef QSharedPointer<IPhotogrammetricEngine> IPhotogrammetricEnginePtr;


}
}
}
}
} //namespace

Q_DECLARE_INTERFACE(br::uerj::eng::efoto::engine::IPhotogrammetricEngine, "br.uerj.eng.efoto.engine.IPhotogrammetricEngine/1.0")

#endif	//BR_UERJ_ENG_EFOTO_ENGINE_IPHOTOGRAMMETRICENGINE_H
