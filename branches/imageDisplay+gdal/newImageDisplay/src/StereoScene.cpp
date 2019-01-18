#include "StereoScene.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

// StereoScene class
StereoScene::StereoScene(QObject* parent, QString leftImageFilepath, QString rightImageFilepath):
	QObject(parent)
{
	leftScene_ = new SingleScene(parent, leftImageFilepath);
	rightScene_ = new SingleScene(parent, rightImageFilepath);
}

StereoScene::~StereoScene()
{
    leftScene_ = rightScene_ = nullptr;
}

void StereoScene::setLeftScene(SingleScene* leftScene)
{
	leftScene_ = leftScene;
}

void StereoScene::setRightScene(SingleScene* rightScene)
{
	rightScene_ = rightScene;
}

void StereoScene::setViewport(QSize viewportSize)
{
	leftScene_->setViewport(viewportSize);
	rightScene_->setViewport(viewportSize);
}

QPointF StereoScene::getChannelsOffset()
{
	return rightScene_->getViewpoint() - leftScene_->getViewpoint();
}

unsigned int StereoScene::getWidth()
{
	return leftScene_->getWidth()/2.0 + getChannelsOffset().x() + rightScene_->getWidth()/2.0;
}

unsigned int StereoScene::getHeight()
{
	return leftScene_->getHeight()/2.0 + getChannelsOffset().y() + rightScene_->getHeight()/2.0;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
