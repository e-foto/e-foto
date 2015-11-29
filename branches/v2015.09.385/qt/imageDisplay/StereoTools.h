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

class StereoTool : public QObject
{
    Q_OBJECT

public:
    StereoTool(StereoDisplay* display) {_display = display; QTimer *timer = new QTimer(this); _scale = -1; _scaleSpin = NULL; _leftPosLabel = NULL; _rightPosLabel = NULL; _3dPosLabel = NULL;
                                        connect(timer,SIGNAL(timeout()),this,SLOT(autoMove())); timer->start(50); _hasButtomPressed = false; _actualizePosLabel = true;}
    virtual ~StereoTool() {}

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

    //void setScaleSpin(QDoubleSpinBox* scaleSpin) {_scaleSpin = scaleSpin;}
    void setPosLabel(QLabel* leftLabel, QLabel* rightLabel, QLabel* stereoLabel) {_leftPosLabel = leftLabel; _rightPosLabel = rightLabel; _3dPosLabel = stereoLabel;}
    //void actualizeScaleSpin(double scale);
    void actualizePosLabel();
    //void actualizePosLabel(SingleDisplay* display, bool force = false);

    void setCursor(QImage cursor, bool stereo);

public slots:
    void autoMove();

protected:
    StereoDisplay * _display;
    QPointF _autoPan;
    QPoint _lastMousePosition;
    QPointF _fixedPointOnImageLeft;
    QPointF _fixedPointOnImageRight;
    QPoint _fixedPoint;
    double _scale;
    bool _hasButtomPressed;

    QImage _currentCursor;
    QImage _lastCursor;
    QDoubleSpinBox* _scaleSpin;
    QLabel* _leftPosLabel;
    QLabel* _rightPosLabel;
    QLabel* _3dPosLabel;
    bool _actualizePosLabel;

signals:
    void mouseMoved(QPointF, QPointF);
};

class ZoomStereoTool : public StereoTool
{
    bool _onRubberBand;
public:
    ZoomStereoTool(StereoDisplay* display);
    ~ZoomStereoTool();
    void paintEvent(const QPaintEvent& event);
    //void enterEvent(const QHoverEvent& event);
    //void leaveEvent(const QHoverEvent& event);
    //void moveEvent(const QHoverEvent& event);
    void mousePressed(const QMouseEvent & event);
    void mouseReleased(const QMouseEvent & event);
    void mouseMoved(const QMouseEvent & event);
    void mouseDblClicked(const QMouseEvent & event);
    //void wheelEvent(const QWheelEvent& event);
};

class MoveStereoTool : public StereoTool
{
public:
    MoveStereoTool(StereoDisplay* display);
    ~MoveStereoTool();
    //void paintEvent(const QPaintEvent& event);
    //void enterEvent(const QHoverEvent& event);
    //void leaveEvent(const QHoverEvent& event);
    //void moveEvent(const QHoverEvent& event);
    void mousePressed(const QMouseEvent & event);
    void mouseReleased(const QMouseEvent & event);
    void mouseMoved(const QMouseEvent & event);
    //void mouseDblClicked(const QMouseEvent & event);
    //void wheelEvent(const QWheelEvent& event);
};

class MarkStereoTool : public StereoTool
{
    Q_OBJECT

    Marker mark;
    int nextMarkItem;
    bool onlyEmitClickedMode;

public:
    MarkStereoTool(StereoDisplay* display);
    ~MarkStereoTool();

    void changeMarker(Marker marker);
    Marker* getMarker();

    void addMark(QPointF lPos, QPointF rPos, int key, QString label = "", Marker* marker = NULL);
    void insertMark(QPointF lPos, QPointF rPos, int key, QString label = "", Marker* marker = NULL);
    //void editMark(int key, QPointF lPos, QPointF rPos, Marker* marker = NULL);
    //void editMark(int key, QPointF lPos, QPointF rPos, QString label, Marker* marker = NULL);
    void deleteMark(int key);
    void clear();

    //void paintEvent(const QPaintEvent& event);
    //void enterEvent(const QHoverEvent& event);
    //void leaveEvent(const QHoverEvent& event);
    //void moveEvent(const QHoverEvent& event);
    void mousePressed(const QMouseEvent & event);
    //void mouseReleased(const QMouseEvent & event);
    //void mouseMoved(const QMouseEvent & event);
    //void mouseDblClicked(const QMouseEvent & event);
    //void wheelEvent(const QWheelEvent& event);

    void setToOnlyEmitClickedMode();
    void setToAutoCreateMarkFrom(unsigned int start);
    void putClickOn(QPointF& lPos, QPointF& rPos);

signals:
    void clicked(QPointF, QPointF);
};

class NearStereoTool : public StereoTool
{
    QDockWidget* _nearDock;
    SingleDisplay* _leftNear;
    SingleDisplay* _rightNear;
    //MarkTool* _marker;
    bool _cursorIsVisible;

public:
    NearStereoTool(StereoDisplay* display);
    ~NearStereoTool();

    QDockWidget* getNearDock();
    SingleDisplay* getLeftNear();
    SingleDisplay* getRightNear();
    bool nearIsVisible();
    void setNearVisible(bool status);

    // Methods into disuse:
    //void setMarker(MarkTool* marker);
    //void setNearCursor(QCursor cursor);

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

class OverStereoTool : public StereoTool
{
    QDockWidget* _overDock;
    SingleDisplay* _leftOver;
    SingleDisplay* _rightOver;
    QPointF _lastPos;
    bool _onMove;

public:
    OverStereoTool(StereoDisplay* display);
    ~OverStereoTool();

    QDockWidget* getOverDock();
    SingleDisplay* getLeftOver();
    SingleDisplay* getRightOver();
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

class StereoToolsBar : public QToolBar
{
    Q_OBJECT

protected:
    StereoDisplay* _display;
    QLabel* _leftInfoLabel;
    QLabel* _rightInfoLabel;
    QLabel* _stereoInfoLabel;
    StereoTool* currentTool;

public:
    StereoToolsBar(StereoDisplay* display, QWidget* parent);
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

    QLabel* getStereoInfo() {return _stereoInfoLabel;}
    QLabel* getLeftInfo() {return _leftInfoLabel;}
    QLabel* getRightInfo() {return _rightInfoLabel;}

    void clearStereoInfoLabel();
    void actualizeStereoInfoLabel(double X, double Y, double Z);

    void setOpenVisible(bool status);
    void setSaveVisible(bool status);
    void setMarkVisible(bool status);

public slots:
    void executeAction(QAction *action);
    void changeMode(int mode);
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // STEREOTOOLS_H
