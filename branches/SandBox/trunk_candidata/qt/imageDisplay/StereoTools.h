#ifndef STEREOTOOLS_H
#define STEREOTOOLS_H

#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QHoverEvent>
#include <QToolBar>
#include <QAction>
#include <QTimer>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QObject>

#include "GeometryResource.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class StereoDisplay;

class StereoTool : public QObject
{
	Q_OBJECT

public:
	StereoTool(StereoDisplay* display) {_display = display; QTimer *timer = new QTimer(this); _scale = -1;
										connect(timer,SIGNAL(timeout()),this,SLOT(autoMove())); timer->start(50); _hasButtomPressed = false;}
	virtual ~StereoTool() {}

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

protected:
	StereoDisplay * _display;
	QPointF _autoPan;
	QPoint _lastMousePosition;
	QPoint _fixedPoint;
	double _scale;
	bool _hasButtomPressed;
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
	//void mousePressed(const QMouseEvent & event);
	//void mouseReleased(const QMouseEvent & event);
	void mouseMoved(const QMouseEvent & event);
	//void mouseDblClicked(const QMouseEvent & event);
	//void wheelEvent(const QWheelEvent& event);
};

class MarkStereoTool : public StereoTool
{
	Marker mark;
public:
	MarkStereoTool(StereoDisplay* display);
	~MarkStereoTool();
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

class InfoStereoTool : public StereoTool, public QLabel
{
	QCursor currentCursor;
public:
	InfoStereoTool(StereoDisplay* display);
	~InfoStereoTool();
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


class StereoToolsBar : public QToolBar
{
	Q_OBJECT

protected:
	StereoDisplay* _display;

public:
	StereoToolsBar(StereoDisplay* display, QWidget* parent);
	QAction* setZoomTool;
	QAction* setMoveTool;
	QAction* setMarkTool;
	QAction* setFitView;
	QAction* openImage;
	QAction* saveImage;

	ZoomStereoTool zoom;
	MoveStereoTool move;
	MarkStereoTool mark;
	InfoStereoTool info;

	void deactivateAllExclusiveTools();

public slots:
	void executeAction(QAction *action);
	/*
	Q_OBJECT

protected:
	StereoDisplay* _display;
	QLabel* _leftInfoLabel;
	QLabel* _rightInfoLabel;
	QLabel* _objectInfoLabel;
	StereoTool* currentLeftTool;
	StereoTool* currentRightTool;

public:
	StereoToolsBar(StereoDisplay* display, QWidget* parent);

	QAction* setZoomTool;
	QAction* setMoveTool;
	QAction* setMarkTool;

	QAction* setLeftEffect;
	QAction* setRightEffect;
	QAction* setBothEffect;

	QAction* setFitView;
	QAction* showOverview;
	QAction* showNearview;
	QAction* useAntialias;
	QAction* openLeftImage;
	QAction* openRightImage;
	QAction* saveLeftImage;
	QAction* saveRightImage;
	QDoubleSpinBox* scaleSpinBox;
	QComboBox* detailComboBox;

	ZoomStereoTool zoom;
	MoveStereoTool move;
	MarkStereoTool mark;
	NearTool leftNear;
	OverTool leftOver;
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
	void rescaleLeftDisplay();
	void rescaleRightDisplay();
	void changeDetailZoom(int nz);
	*/
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // STEREOTOOLS_H
