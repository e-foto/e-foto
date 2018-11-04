#ifndef SINGLETOOLS_H
#define SINGLETOOLS_H

#include "CommonDef.h"

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

class SingleTool : public QObject
{
    Q_OBJECT

public:
    explicit SingleTool(SingleDisplay* display) {_display = display; QTimer *timer = new QTimer(this); _scale = -1; _scaleSpin = NULL; _posLabel = NULL; _propagateMoveTo = _propagateScaleTo = NULL;
                                        connect(timer,SIGNAL(timeout()),this,SLOT(autoMove())); timer->start(50); setImageMode(); _actualizePosLabel = true; }
    virtual ~SingleTool() {}

    virtual void paintEvent(const QPaintEvent& event);
    virtual void resizeEvent(const QResizeEvent& event);
    virtual void enterEvent(const QHoverEvent& event);
    virtual void leaveEvent(const QHoverEvent& event);
    virtual void moveEvent(const QHoverEvent& event);
    virtual void mousePressed(const QMouseEvent& event);
    virtual void mouseReleased(const QMouseEvent& event);
    virtual void mouseMoved(const QMouseEvent& event);
    virtual void mouseDblClicked(const QMouseEvent& event);
    virtual void wheelEvent(const QWheelEvent& event);

    void setImageMode();
    void setOrtoImageMode(double xi, double dx, double yi, double dy);
    void setElevationImageMode(double xi, double dx, double yi, double dy, double zi, double dz);

    void setScaleSpin(QDoubleSpinBox* scaleSpin) {_scaleSpin = scaleSpin;}
    void setPosLabel(QLabel* label) {_posLabel = label;}
    void actualizeScaleSpin(double scale);
    void actualizePosLabel(SingleDisplay* display, bool force = false);

    void propagateMove(QPointF desloc);
    void propagateScale(double scale, QPoint at, int movementMode = 0);

public slots:
    void autoMove();
    void propagateMoveTo(SingleTool* tool = NULL) {_propagateMoveTo = tool;}
    void propagateScaleTo(SingleTool* tool = NULL) {_propagateScaleTo = tool;}

protected:
    SingleDisplay* _display;
    SingleTool* _propagateMoveTo;
    SingleTool* _propagateScaleTo;

    QPointF _autoPan;
    QPoint _lastMousePosition;
    QPointF _fixedPointOnImage;
    QPoint _fixedPoint;
    double _scale;

    QCursor _currentCursor;
    QDoubleSpinBox* _scaleSpin;
    QLabel* _posLabel;
    bool _actualizePosLabel;

    bool _printZ;
    bool _invertY;
    double _xi;
    double _yi;
    double _zi;
    double _dx;
    double _dy;
    double _dz;
};

class ZoomTool : public SingleTool
{
    bool _onRubberBand;
public:
    explicit ZoomTool(SingleDisplay* display);
    ~ZoomTool();
    void paintEvent(const QPaintEvent& event);
    //void resizeEvent(const QResizeEvent &event);
    //void enterEvent(const QHoverEvent& event);
    //void leaveEvent(const QHoverEvent& event);
    //void moveEvent(const QHoverEvent& event);
    void mousePressed(const QMouseEvent & event);
    void mouseReleased(const QMouseEvent & event);
    void mouseMoved(const QMouseEvent & event);
    void mouseDblClicked(const QMouseEvent & event);
    //void wheelEvent(const QWheelEvent& event);
};

class MoveTool : public SingleTool
{
public:
    explicit MoveTool(SingleDisplay* display);
    ~MoveTool();
    //void paintEvent(const QPaintEvent& event);
    //void resizeEvent(const QResizeEvent &event);
    //void enterEvent(const QHoverEvent& event);
    //void leaveEvent(const QHoverEvent& event);
    //void moveEvent(const QHoverEvent& event);
    void mousePressed(const QMouseEvent & event);
    void mouseReleased(const QMouseEvent & event);
    void mouseMoved(const QMouseEvent & event);
    //void mouseDblClicked(const QMouseEvent & event);
    //void wheelEvent(const QWheelEvent& event);
};

class MarkTool : public SingleTool
{
    Q_OBJECT

    Marker mark;
    int nextMarkItem;
    bool onlyEmitClickedMode;

public:
    explicit MarkTool(SingleDisplay* display);
    ~MarkTool();

    void changeMarker(Marker marker);
    Marker* getMarker();

    void addMark(QPointF location, int key, QString label = "", Marker* marker = NULL);
    void insertMark(QPointF location, int key, QString label = "", Marker* marker = NULL);

    void deleteMark(int key);
    void clear();

    void mousePressed(const QMouseEvent & event);

    void setToOnlyEmitClickedMode();
    void setToAutoCreateMarkFrom(unsigned int start);
    void putClickOn(QPointF& pos);


    /* Methods into disuse:
     *
    void editMark(int key, QPointF location, Marker* marker = NULL);
    void editMark(int key, QPointF location, QString label, Marker* marker = NULL);

    //void mouseReleased(const QMouseEvent & event);
    //void mouseMoved(const QMouseEvent & event);
    //void mouseDblClicked(const QMouseEvent & event);
    //void wheelEvent(const QWheelEvent& event);
    //void paintEvent(const QPaintEvent& event);
    //void resizeEvent(const QResizeEvent &event);
    //void enterEvent(const QHoverEvent& event);
    //void leaveEvent(const QHoverEvent& event);
    //void moveEvent(const QHoverEvent& event);
    */

signals:
    void clicked(QPointF);
};

/*
class InfoTool : public SingleTool
{
public:
 InfoTool(SingleDisplay* display);
 ~InfoTool();

 void paintEvent(const QPaintEvent& event);
 void resizeEvent(const QResizeEvent &event);
 void enterEvent(const QHoverEvent& event);
 void leaveEvent(const QHoverEvent& event);
 void moveEvent(const QHoverEvent& event);
 void mousePressed(const QMouseEvent & event);
 void mouseReleased(const QMouseEvent & event);
 void mouseMoved(const QMouseEvent & event);
 void mouseDblClicked(const QMouseEvent & event);
 void wheelEvent(const QWheelEvent& event);
};
*/

class OverTool : public SingleTool
{
    QDockWidget* _overDock;
    SingleDisplay* _over;
    QPointF _lastPos;
    bool _onMove;
public:
    explicit OverTool(SingleDisplay* display);
    ~OverTool();

    QDockWidget* getOverDock();
    bool overIsVisible();
    void setOverVisible(bool status);

    void paintEvent(const QPaintEvent& event);
    void resizeEvent(const QResizeEvent &event);
    void enterEvent(const QHoverEvent& event);
    void leaveEvent(const QHoverEvent& event);
    void moveEvent(const QHoverEvent& event);
    void mousePressed(const QMouseEvent & event);
    void mouseReleased(const QMouseEvent & event);
    void mouseMoved(const QMouseEvent & event);
    void mouseDblClicked(const QMouseEvent & event);
    void wheelEvent(const QWheelEvent& event);
};

class NearTool : public SingleTool
{
    QDockWidget* _nearDock;
    SingleDisplay* _near;
    //QPointF _lastPos;
    MarkTool* _marker;
    bool _cursorIsVisible;
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
    void resizeEvent(const QResizeEvent &event);
    void enterEvent(const QHoverEvent& event);
    void leaveEvent(const QHoverEvent& event);
    void moveEvent(const QHoverEvent& event);
    void mousePressed(const QMouseEvent & event);
    void mouseReleased(const QMouseEvent & event);
    void mouseMoved(const QMouseEvent & event);
    void mouseDblClicked(const QMouseEvent & event);
    void wheelEvent(const QWheelEvent& event);
};

class SingleToolsBar : public QToolBar
{
    Q_OBJECT

protected:
    SingleDisplay* _display;
    QLabel* _infoLabel;
    SingleTool* currentTool;

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
    //InfoTool info;

    void setOpenVisible(bool status);
    void setSaveVisible(bool status);
    void setMarkVisible(bool status);
    QLabel* getInfo();
    QDockWidget* getOverview();
    QDockWidget* getNearview();

public slots:
    void executeAction(QAction *action);

    void rescaleDisplay();
    void changeDetailZoom(int nz);

    void setImageMode();
    void setOrtoImageMode(double xi, double dx, double yi, double dy);
    void setElevationImageMode(double xi, double dx, double yi, double dy, double zi, double dz);

};

class SeparatedStereoToolsBar : public QToolBar
{
    Q_OBJECT

protected:
    SingleDisplay* _leftDisplay;
    SingleDisplay* _rightDisplay;
    QLabel* _leftInfoLabel;
    QLabel* _rightInfoLabel;
    SingleTool* currentLeftTool;
    SingleTool* currentRightTool;

public:
    explicit SeparatedStereoToolsBar(SingleDisplay* leftDisplay, SingleDisplay* rightDisplay, QWidget* parent);

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
    void executeAction(QAction *action);
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
