#ifndef BR_UERJ_ENG_EFOTO_VIEWER
#define BR_UERJ_ENG_EFOTO_VIEWER

#include "IViewerService.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {
namespace viewer {

class Q_DECL_EXPORT ViewerService: public QObject, public IViewerService
{
		Q_OBJECT
		Q_INTERFACES(cortex::ISynapse br::uerj::eng::efoto::viewer::IViewerService)

	public:
		Q_INVOKABLE ViewerService();
		virtual ~ViewerService();

		// ISynapse interface:
		virtual bool start();
		virtual bool stop();

};

}
}
}
}
} //namespace

#endif	//BR_UERJ_ENG_EFOTO_VIEWER
