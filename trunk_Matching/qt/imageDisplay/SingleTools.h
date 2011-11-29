#ifndef SINGLETOOLS_H
#define SINGLETOOLS_H

#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QHoverEvent>
#include <QToolBar>
#include <QAction>
#include <QTimer>
#include <QLabel>
#include <QObject>

#include "GeometryResource.h"

class SingleDisplay;

class SingleTool : public QObject
{
	Q_OBJECT

public:
	SingleTool(SingleDisplay* display) {_display = display; QTimer *timer = new QTimer(this); _scale = -1;
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

protected:
	SingleDisplay * _display;
	QPointF _autoPan;
	QPoint _lastMousePosition;
	QPoint _fixedPoint;
	double _scale;
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
	//void mousePressed(const QMouseEvent & event);
	//void mouseReleased(const QMouseEvent & event);
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
public:
	OverTool(SingleDisplay* display);
	~OverTool();
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
public:
	NearTool(SingleDisplay* display);
	~NearTool();
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
	QAction* openImage;
	QAction* saveImage;

	ZoomTool zoom;
	MoveTool move;
	MarkTool mark;
	InfoTool info;

        void deactivateAllExclusiveTools();
        void setOpenVisible(bool status);
        void setSaveVisible(bool status);
        void setMarkVisible(bool status);
        QLabel* getInfo();

public slots:
	void executeAction(QAction *action);
};

#endif // SINGLETOOLS_H
