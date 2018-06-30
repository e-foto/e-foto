#ifndef STEREODISPLAY_H
#define STEREODISPLAY_H

#include <QGLWidget>
#include <QGridLayout>

#include "SingleDisplay.h"
#include "StereoScene.h"
#include "StereoTools.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class StereoDisplay;

class GLDisplay : public QGLWidget {
  public:
    explicit GLDisplay(StereoDisplay* parent);
    ~GLDisplay();

    bool _GLDisplayUpdate{false};

    void updateMousePosition();
    QPointF getMouseScreenPosition();
    void setGLCursor(QImage cursor);
    QImage getGLCursor();
    void setReverseLensGlasses(bool opt);
    void setColorMaskLeft(bool r, bool g, bool b);
    void setColorMaskRight(bool r, bool g, bool b);
    int setStereoMode(bool mode);

    bool painting();

    void setActivatedTool(StereoTool* tool, bool active = true);

  private:
    StereoDisplay* stereoDisplay_{nullptr};
    unsigned int ltexture_{0};
    unsigned int rtexture_{0};
    unsigned int ctexture_{0};
    unsigned int btexture_{0};
    QImage ltext_{};
    QImage rtext_{};
    QList< StereoTool* > tool_{};

    QPointF mouseScreenPos_{};
    QImage cursor_{};
    bool onPainting_{false};
    bool stereo_mode{false};

    // Default Anagliph filter
    bool L_Red_{true}, L_Green_{false}, L_Blue_{false};
    bool R_Red_{false}, R_Green_{true}, R_Blue_{true};
    bool reverseLensGlasses{false};

    void paintEvent(QPaintEvent* e);
    void resizeEvent(QResizeEvent*);
    bool eventFilter(QObject* o, QEvent* e);
    void enterEvent(QHoverEvent* e);
    void leaveEvent(QHoverEvent* e);
    void moveEvent(QHoverEvent* e);
    void mousePressEvent(QMouseEvent* e);
    void wheelEvent(QWheelEvent* e);
};

class StereoDisplay : public QWidget {
    Q_OBJECT

  public:
    explicit StereoDisplay(QWidget* parent, StereoScene* currentScene = NULL);
    ~StereoDisplay();

    StereoScene* getCurrentScene();
    SingleDisplay* getLeftDisplay();
    SingleDisplay* getRightDisplay();
    SingleDisplay* getLeftOverDisplay();
    SingleDisplay* getRightOverDisplay();
    SingleDisplay* getLeftNearDisplay();
    SingleDisplay* getRightNearDisplay();
    GLDisplay* getRealDisplay();

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
    bool isStereoCursor() {
        return stereoCursor_;
    }

    void updateMousePosition();
    QPointF screenPosition(QPointF position, bool leftChannel = true);
    QPointF getMouseScreenPosition();
    QPointF getPositionLeft(QPoint screenPosition);
    QPointF getPositionRight(QPoint screenPosition);

    void adjustFit(QPointF leftCenter, QPointF rightCenter, double scale) {
        centerOnLeft_ = leftCenter;
        centerOnRight_ = rightCenter;
        fitScale_ = scale;
        currentScene_->getLeftScene()->setLimitScale(scale,
                currentScene_->getLeftScene()->getMaxScale());
        currentScene_->getRightScene()->setLimitScale(scale,
                currentScene_->getRightScene()->getMaxScale());
    }

    virtual void fitView();
    virtual void zoom(double zoomFactor);

    void updateAll();
    void updateAll(QPointF* left, QPointF* right, bool emitClicked = false);
    void updateDetail(QPointF* left, QPointF* right, bool emitClicked = false);

    void setActivatedTool(StereoTool* tool, bool active = true);

    bool painting();
    void resizeEvent(QResizeEvent*);

    void setReverseLensGlasses(bool opt);
    void setColorMaskLeft(bool r, bool g, bool b);
    void setColorMaskRight(bool r, bool g, bool b);


  private:
    StereoScene* currentScene_{nullptr};
    SingleDisplay* leftDisplay_{nullptr};
    SingleDisplay* rightDisplay_{nullptr};
    GLDisplay* glDisplay_{nullptr};
    QPointF leftCursorOffset_{};
    QPointF rightCursorOffset_{};
    double currentZ_{0.0};
    bool stereoCursor_{false};

    QPointF centerOnLeft_{};
    QPointF centerOnRight_{};
    double fitScale_{0.0};

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
