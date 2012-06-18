#include "ViewerService.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {
namespace viewer {


ViewerService::ViewerService()
{
}

ViewerService::~ViewerService()
{
}

bool ViewerService::start()
{
	//TODO Automatically generated implementation
	return true;
}

bool ViewerService::stop()
{
	//TODO Automatically generated implementation
	return true;
}

ISingleViewerPtr ViewerService::instanceSingleViewer()
{
	ISingleViewerPtr sv = ISingleViewerPtr((ISingleViewer*) new SingleViewer());
	single.push_front(sv);
	return sv;
}

IDoubleViewerPtr ViewerService::instanceDoubleViewer()
{
	IDoubleViewerPtr sv = IDoubleViewerPtr((IDoubleViewer*) new SeparatedStereoViewer());
	separated.push_front(sv);
	return sv;
}

IStereoViewerPtr ViewerService::instanceStereoViewer()
{
	IStereoViewerPtr sv = IStereoViewerPtr((IStereoViewer*) new StereoViewer());
	stereo.push_front(sv);
	return sv;
}

// IViewerService interface:

Q_EXPORT_PLUGIN2(br.uerj.eng.efoto.viewer, ViewerService)

}
}
}
}
}	//namespace
