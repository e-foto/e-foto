#ifndef SINGLETOOLS_H
#define SINGLETOOLS_H

#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QHoverEvent>
#include <QDockWidget>
#include <QToolBar>
#include <QAction>
#include <QTimer>
#include <QRect>
#include <QPixmap>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QObject>

#include "GeometryResource.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class SingleDisplay;
class SingleToolsBar;

class SingleTool : public QObject {
    Q_OBJECT

  public:
    explicit SingleTool(SingleDisplay* display):
        display_{display}
    {
        QTimer* timer = new QTimer(this);           
        connect(timer, SIGNAL(timeout()), this, SLOT(autoMove()));
        timer->start(50);
        setImageMode();
    }
    virtual ~SingleTool() {}

    virtual void paintEvent(const QPaintEvent& event);
    virtual void resizeEvent(const QResizeEvent& event);
    virtual void enterEvent(const QHoverEvent& event);
    virtual void leaveEvent(const QHoverEvent& event);
    virtual void moveEvent(const QHoverEvent& event);
    virtual void mousePressed(const QMouseEvent& event);
    virtual void mouseReleased(const QMouseEvent& event);
    virtual void mouseMoved(const QMouseEvent& event);
    virtual void wheelEvent(const QWheelEvent& event);

    void setImageMode();
    void setOrtoImageMode(double xi, double dx, double yi, double dy);
    void setElevationImageMode(double xi, double dx, double yi, double dy,
                               double zi, double dz);

    void setScaleSpin(QDoubleSpinBox* scaleSpin) {
        scaleSpin_ = scaleSpin;
    }
    void setPosLabel(QLabel* label) {
        posLabel_ = label;
    }
    void actualizeScaleSpin(double scale);
    void actualizePosLabel(SingleDisplay* display, bool force = false);

    void propagateMove(QPointF desloc);
    void propagateScale(double scale, QPoint at, int movementMode = 0);

  public slots:
    void autoMove();

    void propagateMoveTo(SingleTool* tool = nullptr)
    {
        propagateMoveTo_ = tool;
    }

    void propagateScaleTo(SingleTool* tool = nullptr)
    {
        propagateScaleTo_ = tool;
    }

  protected:
    SingleDisplay* display_{};
    SingleTool* propagateMoveTo_{nullptr};
    SingleTool* propagateScaleTo_{nullptr};

    QPointF autoPan_{};
    QPoint lastMousePosition_{};
    QPointF fixedPointOnImage_{};
    QPoint fixedPoint_{};
    double scale_{-1.0};

    QCursor currentCursor_{};
    QDoubleSpinBox* scaleSpin_{};
    QLabel* posLabel_{nullptr};
    bool actualizePosLabel_{true};

    bool printZ_{false};
    bool invertY_{false};
    double xi_{0.0};
    double yi_{0.0};
    double zi_{0.0};
    double dx_{0.0};
    double dy_{0.0};
    double dz_{0.0};
};

class ZoomTool : public SingleTool {
    bool onRubberBand_;
  public:
    explicit ZoomTool(SingleDisplay* display);
    ~ZoomTool();
    void paintEvent(const QPaintEvent& event);
    void mousePressed(const QMouseEvent& event);
    void mouseReleased(const QMouseEvent& event);
    void mouseMoved(const QMouseEvent& event);
};

class MoveTool : public SingleTool {
  public:
    explicit MoveTool(SingleDisplay* display);
    ~MoveTool();
    void mousePressed(const QMouseEvent& event);
    void mouseReleased(const QMouseEvent& event);
    void mouseMoved(const QMouseEvent& event);
};

class MarkTool : public SingleTool {
    Q_OBJECT

    Marker mark_;
    int nextMarkItem_;
    bool onlyEmitClickedMode_;

  public:
    explicit MarkTool(SingleDisplay* display);
    ~MarkTool();

    void changeMarker(Marker marker);
    Marker* getMarker();

    void addMark(QPointF location, int key, QString label = "", Marker* marker = nullptr);
    void insertMark(QPointF location, int key, QString label = "", Marker* marker = nullptr);

    void deleteMark(int key);
    void clear();

    void mousePressed(const QMouseEvent& event);

    void setToOnlyEmitClickedMode();
    void putClickOn(QPointF& pos);

  signals:
    void clicked(QPointF);
};

class OverTool : public SingleTool {
    QDockWidget* overDock_;
    SingleDisplay* over_;
    QPointF lastPos_;
    bool onMove_;
  public:
    explicit OverTool(SingleDisplay* display);
    ~OverTool();

    QDockWidget* getOverDock();
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

class NearTool : public SingleTool {
    QDockWidget* nearDock_;
    SingleDisplay* near_;
    MarkTool* marker_;
    bool cursorIsVisible_;
  public:
    explicit NearTool(SingleDisplay* display);
    ~NearTool();

    QDockWidget* getNearDock();
    bool nearIsVisible();
    void setNearVisible(bool status);

    void setMarker(MarkTool* marker);
    void setNearCursor(QCursor cursor);
    //void setActivatedToolOnNear(SingleTool* tool, bool activate = true);

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

class SingleToolsBar : public QToolBar {
    Q_OBJECT

  protected:
    SingleDisplay* display_;
    QLabel* infoLabel_;
    SingleTool* currentTool_;

  public:
    explicit SingleToolsBar(SingleDisplay* display, QWidget* parent);

    QAction* setZoomTool;
    QAction* setMoveTool;
    QAction* setMarkTool;
    QAction* setFitView;
    QAction* showOverview;
    QAction* showNearview;
    QAction* useAntialias;
    QAction* useFixedNearview;
    QAction* openImage;
    QAction* saveImage;
    QDoubleSpinBox* scaleSpinBox;
    QComboBox* detailComboBox;

    ZoomTool zoom;
    MoveTool move;
    MarkTool mark;
    NearTool near_;
    OverTool over;

    void setOpenVisible(bool status);
    void setSaveVisible(bool status);
    void setMarkVisible(bool status);
    QLabel* getInfo();
    QDockWidget* getOverview();
    QDockWidget* getNearview();

  public slots:
    void executeAction(QAction* action);

    void rescaleDisplay();
    void changeDetailZoom(int nz);

    void setImageMode();
    void setOrtoImageMode(double xi, double dx, double yi, double dy);
    void setElevationImageMode(double xi, double dx, double yi, double dy,
                               double zi, double dz);

};

class SeparatedStereoToolsBar : public QToolBar {
    Q_OBJECT

  private:
    SingleDisplay* _leftDisplay;
    SingleDisplay* _rightDisplay;
    QLabel* _leftInfoLabel;
    QLabel* _rightInfoLabel;
    SingleTool* currentLeftTool;
    SingleTool* currentRightTool;

  public:
    explicit SeparatedStereoToolsBar(SingleDisplay* leftDisplay,
                                     SingleDisplay* rightDisplay, QWidget* parent);

    QAction* setZoomTool;
    QAction* setMoveTool;
    QAction* setMarkTool;
    QAction* setFitLeftView;
    QAction* setFitRightView;
    QAction* setFitBothView;
    QAction* showOverview;
    QAction* showNearview;
    QAction* useAntialias;
    QAction* openLeftImage;
    QAction* openRightImage;
    QAction* saveLeftImage;
    QAction* saveRightImage;
    QAction* setEqualMovements;
    QAction* setEqualScales;
    QDoubleSpinBox* scaleLeftSpinBox;
    QDoubleSpinBox* scaleRightSpinBox;
    QComboBox* detailComboBox;

    ZoomTool leftZoom;
    MoveTool leftMove;
    MarkTool leftMark;
    NearTool leftNear;
    OverTool leftOver;
    ZoomTool rightZoom;
    MoveTool rightMove;
    MarkTool rightMark;
    NearTool rightNear;
    OverTool rightOver;

    void setOpenVisible(bool status);
    void setSaveVisible(bool status);
    void setMarkVisible(bool status);
    QLabel* getLeftInfo();
    QLabel* getRightInfo();
    QDockWidget* getLeftOverview();
    QDockWidget* getRightOverview();
    QDockWidget* getLeftNearview();
    QDockWidget* getRightNearview();

  public slots:
    void executeAction(QAction* action);
    void changeMode(int action);
    void rescaleLeftDisplay();
    void rescaleRightDisplay();
    void changeDetailZoom(int nz);
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // SINGLETOOLS_H
