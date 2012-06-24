#ifndef BR_UERJ_ENG_EFOTO_VIEWER
#define BR_UERJ_ENG_EFOTO_VIEWER

#include "IViewerService.h"
#include "SingleViewer.h"
#include "DoubleViewer.h"
#include "StereoViewer.h"
#include <deque>
using namespace std;

namespace br {
namespace uerj {
namespace eng {
namespace efoto {
namespace viewer {

class Q_DECL_EXPORT ViewerService: public QObject, public IViewerService
{
	Q_OBJECT
#ifdef EFOTO_VIEWER
    Q_INTERFACES(cortex::ISynapse br::uerj::eng::efoto::viewer::IViewerService)
#endif //EFOTO_VIEWER

	deque<ISingleViewerPtr> single;
    deque<IDoubleViewerPtr> separated;
    deque<IStereoViewerPtr> stereo;

public:
	Q_INVOKABLE ViewerService();
	virtual ~ViewerService();

	ISingleViewerPtr instanceSingleViewer();
    IDoubleViewerPtr instanceDoubleViewer();
    IStereoViewerPtr instanceStereoViewer();

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
