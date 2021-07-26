#ifndef STEREODISPLAY_H
#define STEREODISPLAY_H

#include <utility>
#include <QGLWidget>
#include <QGridLayout>

#include "SingleDisplay.h"
#include "StereoScene.h"
#include "StereoTools.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class GLDisplay;

class StereoDisplay : public QWidget
{
    Q_OBJECT
protected:
    StereoScene* currentScene_;
    SingleDisplay* leftDisplay_;
    SingleDisplay* rightDisplay_;
    GLDisplay* glDisplay_;
    QPointF leftCursorOffset_;
    QPointF rightCursorOffset_;
    double _currentZ;
    bool _stereoCursor;

    QPointF _centerOnLeft;
    QPointF _centerOnRight;
    double _fitScale;
    //void focusInEvent(QFocusEvent *event) override;
    //void focusOutEvent(QFocusEvent *event) override;

public:
    explicit StereoDisplay(QWidget * parent, StereoScene* currentScene = NULL);
    ~StereoDisplay();

    StereoScene* getCurrentScene();
    SingleDisplay* getLeftDisplay();
    SingleDisplay* getRightDisplay();
    SingleDisplay* getLeftOverDisplay();
    SingleDisplay* getRightOverDisplay();
    SingleDisplay* getLeftNearDisplay();
    SingleDisplay* getRightNearDisplay();
    GLDisplay* getRealDisplay();

    void loadBackground(QImage image);
    void loadLeftImage(QString filename);
    void loadRightImage(QString filename);
    void loadLeftImage(QImage* image);
    void loadRightImage(QImage* image);
    void loadLeftImage(Matrix* image, bool isGrayscale = true);
    void loadRightImage(Matrix* image, bool isGrayscale = true);
    int setStereoMode(bool mode);

    QPointF getLeftCursorOffset();
    QPointF getRightCursorOffset();
    void setLeftCursorOffset(QPointF offset);
    void setRightCursorOffset(QPointF offset);
    void setCursor(QImage newCursor, bool stereo = true);
    QImage getCursor();
    bool isStereoCursor() {return _stereoCursor;}

    double getCurrentZ();
    void setCurrentZ(double z);

    void updateMousePosition();
    QPointF screenPosition(QPointF position, bool leftChannel = true);
    QPointF getMouseScreenPosition();
    QPointF getPositionLeft(QPoint screenPosition);
    QPointF getPositionRight(QPoint screenPosition);

    void adjustFit(QPointF leftCenter, QPointF rightCenter, double scale) {_centerOnLeft = leftCenter; _centerOnRight = rightCenter; _fitScale = scale; currentScene_->getLeftScene()->setLimitScale(scale, currentScene_->getLeftScene()->getMaxScale()); currentScene_->getRightScene()->setLimitScale(scale, currentScene_->getRightScene()->getMaxScale());}

    virtual void fitView();
    /* Method into disuse:
    virtual void pan(int dx, int dy);
    */
    virtual void zoom(double zoomFactor/*, QPoint* atPoint = NULL*/);

    void updateAll(std::pair<bool, bool> scene = std::pair<bool, bool>(true,true));
    void updateAll(QPointF* left, QPointF* right, bool emitClicked = false);
    void updateDetail(QPointF* left, QPointF* right, bool emitClicked = false);

    void setActivatedTool(StereoTool* tool, bool active = true);

    bool painting();
    void resizeEvent(QResizeEvent *);

    void setReverseLensGlasses(bool opt);
    void setColorMaskLeft(bool r, bool g, bool b);
    void setColorMaskRight(bool r, bool g, bool b);

signals:
    void mousePositionsChanged(QPointF*, QPointF*);
    void mouseClicked(QPointF*, QPointF*);
    void resized(int, int);
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // STEREODISPLAY_H
