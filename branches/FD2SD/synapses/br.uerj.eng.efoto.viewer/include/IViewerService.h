#ifndef BR_UERJ_ENG_EFOTO_VIEWER_IVIEWERSERVICE_H
#define BR_UERJ_ENG_EFOTO_VIEWER_IVIEWERSERVICE_H

#include "ISynapse.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {
namespace viewer {

class IViewerService: public cortex::ISynapse
{
public:
	virtual ~IViewerService(){}

};

typedef QSharedPointer<IViewerService> IViewerServicePtr;


}
}
}
}
} //namespace

Q_DECLARE_INTERFACE(br::uerj::eng::efoto::viewer::IViewerService, "br.uerj.eng.efoto.viewer.IViewerService/1.0")

#endif	//BR_UERJ_ENG_EFOTO_VIEWER_IVIEWERSERVICE_H
