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

class SingleDisplay;
class SingleToolsBar;

class SingleTool : public QObject
{
	Q_OBJECT

public:
	SingleTool(SingleDisplay* display) {_display = display; QTimer *timer = new QTimer(this); _scale = -1; _scaleSpin = NULL;
										connect(timer,SIGNAL(timeout()),this,SLOT(autoMove())); timer->start(50); }
	virtual ~SingleTool() {}

	virtual void paintEvent(const QPaintEvent& event);
	virtual void enterEvent(const QHoverEvent& event);
	virtual void leaveEvent(const QHoverEvent& event);
	virtual void moveEvent(const QHoverEvent& event);
	virtual void mousePressed(const QMouseEvent& event);
	virtual void mouseReleased(const QMouseEvent& event);
	virtual void mouseMoved(const QMouseEvent& event);
	virtual void mouseDblClicked(const QMouseEvent& event);
	virtual void wheelEvent(const QWheelEvent& event);

public slots:
	void autoMove();
	void setScaleSpin(QDoubleSpinBox* scaleSpin) {_scaleSpin = scaleSpin;}
	void actualizeScaleSpin(double scale);

protected:
	SingleDisplay* _display;
	QPointF _autoPan;
	QPoint _lastMousePosition;
	QPoint _fixedPoint;
	double _scale;
	QCursor _currentCursor;
	QDoubleSpinBox* _scaleSpin;
};

class ZoomTool : public SingleTool
{
	bool _onRubberBand;
public:
	ZoomTool(SingleDisplay* display);
	~ZoomTool();
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

class MoveTool : public SingleTool
{
public:
	MoveTool(SingleDisplay* display);
	~MoveTool();
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

class MarkTool : public SingleTool
{
	Marker mark;
public:
	MarkTool(SingleDisplay* display);
	~MarkTool();
	//void paintEvent(const QPaintEvent& event);
	//void enterEvent(const QHoverEvent& event);
	//void leaveEvent(const QHoverEvent& event);
	//void moveEvent(const QHoverEvent& event);
	void mousePressed(const QMouseEvent & event);
	//void mouseReleased(const QMouseEvent & event);
	//void mouseMoved(const QMouseEvent & event);
	//void mouseDblClicked(const QMouseEvent & event);
	//void wheelEvent(const QWheelEvent& event);
};

class InfoTool : public SingleTool
{
	QLabel _infoLabel;
public:
	InfoTool(SingleDisplay* display);
	~InfoTool();
	void paintEvent(const QPaintEvent& event);
	void enterEvent(const QHoverEvent& event);
	void leaveEvent(const QHoverEvent& event);
	void moveEvent(const QHoverEvent& event);
	void mousePressed(const QMouseEvent & event);
	void mouseReleased(const QMouseEvent & event);
	void mouseMoved(const QMouseEvent & event);
	void mouseDblClicked(const QMouseEvent & event);
	void wheelEvent(const QWheelEvent& event);
	QLabel* getInfoLabel();
};

class OverTool : public SingleTool
{
	QDockWidget* _overDock;
	SingleDisplay* _over;
	QPointF _lastPos;
	bool _onMove;
public:
	OverTool(SingleDisplay* display);
	~OverTool();

	QDockWidget* getOverDock();
	bool overIsVisible();
	void setOverVisible(bool status);

	void paintEvent(const QPaintEvent& event);
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
public:
	NearTool(SingleDisplay* display);
	~NearTool();

	QDockWidget* getNearDock();
	bool nearIsVisible();
	void setNearVisible(bool status);

	void paintEvent(const QPaintEvent& event);
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

public:
	SingleToolsBar(SingleDisplay* display, QWidget* parent);

	QAction* setZoomTool;
	QAction* setMoveTool;
	QAction* setMarkTool;
	QAction* setFitView;
	QAction* showOverview;
	QAction* showNearview;
	QAction* useAntialias;
	QAction* openImage;
	QAction* saveImage;
	QDoubleSpinBox* scaleSpinBox;
	QComboBox* detailComboBox;

	ZoomTool zoom;
	MoveTool move;
	MarkTool mark;
	NearTool near;
	OverTool over;
	InfoTool info;

	void deactivateAllExclusiveTools();
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
};

class SeparatedStereoToolsBar : public QToolBar
{
	Q_OBJECT

protected:
	SingleDisplay* _leftDisplay;
	SingleDisplay* _rightDisplay;

public:
	SeparatedStereoToolsBar(SingleDisplay* leftDisplay, SingleDisplay* rightDisplay, QWidget* parent);

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
	InfoTool leftInfo;
	ZoomTool rightZoom;
	MoveTool rightMove;
	MarkTool rightMark;
	NearTool rightNear;
	OverTool rightOver;
	InfoTool rightInfo;

	void deactivateAllExclusiveTools();
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
	void rescaleLeftDisplay();
	void rescaleRightDisplay();
	void changeDetailZoom(int nz);
};

#endif // SINGLETOOLS_H
