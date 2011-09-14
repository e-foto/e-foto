#include "StereoService.h"

StereoService::StereoService()
{
}
StereoService::~StereoService()
{
}
MonoDisplay* StereoService::getMonoDisplay(QWidget *parent, MonoView *currentView)
{
	return new MonoDisplay(parent,currentView);
}
DetailDisplay* StereoService::getDetailDisplay(MonoDisplay *parent)
{
	return new DetailDisplay(parent);
}
OverviewDisplay* StereoService::getOverviewDisplay(MonoDisplay *parent)
{
	return new OverviewDisplay(parent);
}
StereoDisplay* StereoService::getStereoDisplay(QWidget *parent, StereoView *currentView)
{
	return new StereoDisplay(parent,currentView);
}
MonoView* StereoService::getMonoView(QObject* parent, QString filepath)
{
	return new MonoView(parent,filepath);
}
StereoView* StereoService::getStereoView(QObject* parent, QString leftImageFilepath, QString rightImageFilepath)
{
	return new StereoView(parent, leftImageFilepath, rightImageFilepath);
}
