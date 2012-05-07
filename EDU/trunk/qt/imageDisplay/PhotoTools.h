#ifndef PHOTOTOOLS_H
#define PHOTOTOOLS_H

#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QHoverEvent>
#include <QToolBar>
#include <QAction>
#include <QTimer>
#include <QLabel>
#include <QObject>

#include "StereoTools.h"
#include "PhotogrammetryEngine_Qt.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class StereoDisplay;

class PhotoTool : public StereoTool
{
	Q_OBJECT

public:
	PhotoTool(StereoDisplay* display) : StereoTool(display) {_engine = NULL;}
	virtual ~PhotoTool() {}

	virtual void paintEvent(const QPaintEvent& event);
	virtual void enterEvent(const QHoverEvent& event);
	virtual void leaveEvent(const QHoverEvent& event);
	virtual void moveEvent(const QHoverEvent& event);
	virtual void mousePressed(const QMouseEvent& event);
	virtual void mouseReleased(const QMouseEvent& event);
	virtual void mouseMoved(const QMouseEvent& event);
	virtual void mouseDblClicked(const QMouseEvent& event);
	virtual void wheelEvent(const QWheelEvent& event);
	void setEngine(PhotogrammetryEngineInterface* engine);

public slots:
	void autoMove();

protected:
	PhotogrammetryEngineInterface* _engine;
};

class ZoomPhotoTool : public PhotoTool
{
	bool _onRubberBand;
public:
	ZoomPhotoTool(StereoDisplay* display);
	~ZoomPhotoTool();
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

class MovePhotoTool : public PhotoTool
{
public:
	MovePhotoTool(StereoDisplay* display);
	~MovePhotoTool();
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

class MarkPhotoTool : public PhotoTool
{
	Marker mark;
public:
	MarkPhotoTool(StereoDisplay* display);
	~MarkPhotoTool();
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

class InfoPhotoTool : public PhotoTool, public QLabel
{
public:
	InfoPhotoTool(StereoDisplay* display);
	~InfoPhotoTool();
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


class PhotoToolsBar : public QToolBar
{
	Q_OBJECT

protected:
	StereoDisplay* _display;
	ZoomPhotoTool zoom;
	MovePhotoTool move;
	MarkPhotoTool mark;
	InfoPhotoTool info;
	PhotogrammetryEngineInterface* _engine;

	void deactivateAllExclusiveTools();

public:
	PhotoToolsBar(StereoDisplay* display, QWidget* parent);
	QAction* setZoomTool;
	QAction* setMoveTool;
	QAction* setMarkTool;
	QAction* setFitView;
	QAction* openImage;
	QAction* saveImage;

public slots:
	void executeAction(QAction *action);
};


} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // PHOTOTOOLS_H
