#ifndef STEREOTOOLS_H
#define STEREOTOOLS_H

#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QHoverEvent>
#include <QDockWidget>
#include <QDoubleSpinBox>
#include <QToolBar>
#include <QAction>
#include <QTimer>
#include <QLabel>
#include <QObject>

#include "SingleTools.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class StereoDisplay;

class StereoTool : public QObject {
    Q_OBJECT

  public:
    explicit StereoTool(StereoDisplay* display):
        display_{display} {
        QTimer* timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(autoMove()));
        timer->start(50);
    }
    virtual ~StereoTool() {}

    virtual void paintEvent(const QPaintEvent& event);
    virtual void resizeEvent(const QResizeEvent& event);
    virtual void enterEvent(const QHoverEvent& event);
    virtual void leaveEvent(const QHoverEvent& event);
    virtual void moveEvent(const QHoverEvent& event);
    virtual void mousePressed(const QMouseEvent& event);
    virtual void mouseReleased(const QMouseEvent& event);
    virtual void mouseMoved(const QMouseEvent& event);
    virtual void wheelEvent(const QWheelEvent& event);
    void setPosLabel(QLabel* leftLabel, QLabel* rightLabel, QLabel* stereoLabel) {
        leftPosLabel_ = leftLabel;
        rightPosLabel_ = rightLabel;
        Pos3dLabel_ = stereoLabel;
    }
    void actualizePosLabel();
    void setCursor(QImage cursor, bool stereo);

  public slots:
    void autoMove();

  protected:
    StereoDisplay* display_{};
    QPoint lastMousePosition_{};
    QPoint fixedPoint_{};
    QImage currentCursor_{};
    QImage lastCursor_{};

  private:
    QPointF autoPan_{};
    QPointF fixedPointOnImageLeft_{};
    QPointF fixedPointOnImageRight_{};
    double scale_{-1.0};
    bool hasButtomPressed_{false};
    QLabel* leftPosLabel_{nullptr};
    QLabel* rightPosLabel_{nullptr};
    QLabel* Pos3dLabel_{nullptr};
    bool actualizePosLabel_{true};


  signals:
    void mouseMoved(QPointF, QPointF);
};

class ZoomStereoTool : public StereoTool {
    bool onRubberBand_;
  public:
    explicit ZoomStereoTool(StereoDisplay* display);
    ~ZoomStereoTool();
    void paintEvent(const QPaintEvent& event);
    void mousePressed(const QMouseEvent& event);
    void mouseReleased(const QMouseEvent& event);
    void mouseMoved(const QMouseEvent& event);
};

class MoveStereoTool : public StereoTool {
  public:
    explicit MoveStereoTool(StereoDisplay* display);
    ~MoveStereoTool();
    void mousePressed(const QMouseEvent& event);
    void mouseReleased(const QMouseEvent& event);
    void mouseMoved(const QMouseEvent& event);
};

class MarkStereoTool : public StereoTool {
    Q_OBJECT

    Marker mark_;
    int nextMarkItem_{0};
    bool onlyEmitClickedMode_{false};

  public:
    explicit MarkStereoTool(StereoDisplay* display);
    ~MarkStereoTool();

    void changeMarker(Marker marker);
    Marker* getMarker();

    void addMark(QPointF lPos, QPointF rPos, int key, QString label = "",
                 Marker* marker = nullptr);
    void insertMark(QPointF lPos, QPointF rPos, int key, QString label = "",
                    Marker* marker = nullptr);
    void deleteMark(int key);
    void clear();

    void mousePressed(const QMouseEvent& event);
    void setToOnlyEmitClickedMode();
    void putClickOn(QPointF& lPos, QPointF& rPos);

  signals:
    void clicked(QPointF, QPointF);
};

class NearStereoTool : public StereoTool {
    QDockWidget* nearDock_{nullptr};
    SingleDisplay* leftNear_{nullptr};
    SingleDisplay* rightNear_{nullptr};

  public:
    explicit NearStereoTool(StereoDisplay* display);
    ~NearStereoTool();

    QDockWidget* getNearDock();
    SingleDisplay* getLeftNear();
    SingleDisplay* getRightNear();
    bool nearIsVisible();
    void setNearVisible(bool status);

    void paintEvent(const QPaintEvent& event);
    void resizeEvent(const QResizeEvent& event);
    void enterEvent(const QHoverEvent& event);
    void leaveEvent(const QHoverEvent& event);
    void moveEvent(const QHoverEvent& event);
    void mousePressed(const QMouseEvent& event);
    void mouseReleased(const QMouseEvent& event);
    void mouseMoved(const QMouseEvent& event);
    void wheelEvent(const QWheelEvent& event);
};

class OverStereoTool : public StereoTool {
    QDockWidget* overDock_{nullptr};
    SingleDisplay* leftOver_{nullptr};
    SingleDisplay* rightOver_{nullptr};
    QPointF lastPos_{};
    bool onMove_{false};

  public:
    explicit OverStereoTool(StereoDisplay* display);
    ~OverStereoTool();

    QDockWidget* getOverDock();
    SingleDisplay* getLeftOver();
    SingleDisplay* getRightOver();
    bool overIsVisible();
    void setOverVisible(bool status);

    void paintEvent(const QPaintEvent& event);
    void resizeEvent(const QResizeEvent& event);
    void enterEvent(const QHoverEvent& event);
    void leaveEvent(const QHoverEvent& event);
    void moveEvent(const QHoverEvent& event);
    void mousePressed(const QMouseEvent& event);
    void mouseReleased(const QMouseEvent& event);
    void mouseMoved(const QMouseEvent& event);
    void wheelEvent(const QWheelEvent& event);
};

class StereoToolsBar : public QToolBar {
    Q_OBJECT

  protected:
    StereoDisplay* display_;
    QLabel* leftInfoLabel_;
    QLabel* rightInfoLabel_;
    QLabel* stereoInfoLabel_;
    StereoTool* currentTool_;

  public:
    explicit StereoToolsBar(StereoDisplay* display, QWidget* parent);
    QAction* setZoomTool;
    QAction* setMoveTool;
    QAction* setMarkTool;
    QAction* setFitView;
    QAction* showOverview;
    QAction* showNearview;
    QAction* useAntialias;
    QAction* openLeftImage;
    QAction* saveLeftImage;
    QAction* openRightImage;
    QAction* saveRightImage;

    ZoomStereoTool _zoom;
    MoveStereoTool _move;
    MarkStereoTool _mark;
    NearStereoTool _near;
    OverStereoTool _over;

    SingleDisplay* getLeftNearview();
    SingleDisplay* getRightNearview();
    QDockWidget* getNearviews();
    SingleDisplay* getLeftOverview();
    SingleDisplay* getRightOverview();
    QDockWidget* getOverviews();

    QLabel* getStereoInfo() {
        return stereoInfoLabel_;
    }
    QLabel* getLeftInfo() {
        return leftInfoLabel_;
    }
    QLabel* getRightInfo() {
        return rightInfoLabel_;
    }

    void actualizeStereoInfoLabel(double X, double Y, double Z);

    void setOpenVisible(bool status);
    void setSaveVisible(bool status);
    void setMarkVisible(bool status);

  public slots:
    void executeAction(QAction* action);
    void changeMode(int mode);
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // STEREOTOOLS_H
