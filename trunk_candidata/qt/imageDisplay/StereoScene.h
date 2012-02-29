#ifndef STEREOSCENE_H
#define STEREOSCENE_H

#include "SingleScene.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class StereoScene: public QObject
{
	Q_OBJECT

protected:
	SingleScene* leftScene_;
	SingleScene* rightScene_;

public:
	StereoScene(QObject* parent, QString leftImageFilepath, QString rightImageFilepath);
	~StereoScene();

	bool isValid();
	SingleScene* getLeftScene() const;
	SingleScene* getRightScene() const;

	double getScale() {return leftScene_->getScale();}

	void setLeftScene(SingleScene* leftScene);
	void setRightScene(SingleScene* rightScene);

	void setViewport(QSize viewportSize);
	QPointF getChannelsOffset();
	unsigned int getWidth();
	unsigned int getHeight();
};

inline bool StereoScene::isValid()
{
	return leftScene_->isValid() && rightScene_->isValid();
}
inline SingleScene* StereoScene::getLeftScene() const
{
	return leftScene_;
}
inline SingleScene* StereoScene::getRightScene() const
{
	return rightScene_;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // STEREOSCENE_H
