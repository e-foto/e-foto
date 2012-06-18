#ifndef BR_UERJ_ENG_EFOTO_VIEWER_IVIEWERSERVICE_H
#define BR_UERJ_ENG_EFOTO_VIEWER_IVIEWERSERVICE_H

#include "ISynapse.h"
#include "ISingleViewer.h"
#include "IDoubleViewer.h"
#include "IStereoViewer.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {
namespace viewer {

class IViewerService: public cortex::ISynapse
{
public:
    virtual ~IViewerService() {}
	virtual ISingleViewerPtr instanceSingleViewer() = 0;
    virtual IDoubleViewerPtr instanceDoubleViewer() = 0;
    virtual IStereoViewerPtr instanceStereoViewer() = 0;

};

typedef QSharedPointer<IViewerService> IViewerServicePtr;


}
}
}
}
} //namespace

Q_DECLARE_INTERFACE(br::uerj::eng::efoto::viewer::IViewerService, "br.uerj.eng.efoto.viewer.IViewerService/1.0")

#endif	//BR_UERJ_ENG_EFOTO_VIEWER_IVIEWERSERVICE_H
