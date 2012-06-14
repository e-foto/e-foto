#ifndef BR_UERJ_ENG_EFOTO_ENGINE
#define BR_UERJ_ENG_EFOTO_ENGINE

#include "IPhotogrammetricEngine.h"
#include <deque>
using namespace std;

namespace br {
namespace uerj {
namespace eng {
namespace efoto {
namespace engine {

class Q_DECL_EXPORT PhotogrammetricEngine: public QObject, public IPhotogrammetricEngine
{
		Q_OBJECT
		Q_INTERFACES(cortex::ISynapse br::uerj::eng::efoto::engine::IPhotogrammetricEngine)

        deque<IProjectPtr> project;

	public:
		Q_INVOKABLE PhotogrammetricEngine();
		virtual ~PhotogrammetricEngine();

		// ISynapse interface:
		virtual bool start();
		virtual bool stop();

        IProjectPtr getProject();
};

}
}
}
}
} //namespace

#endif	//BR_UERJ_ENG_EFOTO_ENGINE
