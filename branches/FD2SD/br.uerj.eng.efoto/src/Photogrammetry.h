#ifndef BR_UERJ_ENG_EFOTO
#define BR_UERJ_ENG_EFOTO

#include "IPhotogrammetry.h"
#include "EFotoManager.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Q_DECL_EXPORT Photogrammetry: public QObject, public IPhotogrammetry
{
		Q_OBJECT
		Q_INTERFACES(cortex::ISynapse br::uerj::eng::efoto::IPhotogrammetry)

	public:
		Photogrammetry();
		virtual ~Photogrammetry();

	public slots:
		// ISynapse interface:
		virtual bool start();
		virtual bool stop();

	protected:
		virtual cortex::ISynapse* getImplementation();
		EFotoManager manager;

};

}
}
}
} //namespace

#endif	//BR_UERJ_ENG_EFOTO
