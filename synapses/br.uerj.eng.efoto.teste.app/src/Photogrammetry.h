#ifndef BR_UERJ_ENG_EFOTO_APPV1
#define BR_UERJ_ENG_EFOTO_APPV1

#include "IPhotogrammetry.h"
#include "EFotoManager.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {
namespace appV1 {

class Q_DECL_EXPORT Photogrammetry: public QObject, public IPhotogrammetry
{
		Q_OBJECT
		Q_INTERFACES(cortex::ISynapse br::uerj::eng::efoto::appV1::IPhotogrammetry)

        EFotoManager manager;

	public:
		Q_INVOKABLE Photogrammetry();
		virtual ~Photogrammetry();

		// ISynapse interface:
		virtual bool start();
		virtual bool stop();

};

}
}
}
}
} //namespace

#endif	//BR_UERJ_ENG_EFOTO_APPV1
