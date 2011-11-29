#include "SingleTools.h"
#include "SingleDisplay.h"

#include <QFileDialog>

void SingleTool::paintEvent(const QPaintEvent& event)
{
	QPainter painter(_display);
	painter.setPen(QPen(Qt::yellow));
	if (_autoPan != QPointF(0,0))
	{
		// Draw autoMove feedback
		painter.drawLine(_lastMousePosition, _lastMousePosition - _autoPan*5*_display->getCurrentScene()->getScale());
	}
	else if (_scale > 0)
	{
		// Draw scaleBar feedback
		QPoint endBar(_fixedPoint.x(), _display->getMouseScreenPosition().y());
		painter.drawLine(_fixedPoint, endBar);
		painter.drawText(endBar, QString::number(_display->getCurrentScene()->getScale()*100,'f', 1).append("%"));
	}
	painter.end();
}

void SingleTool::enterEvent(const QHoverEvent& event)
{
}

void SingleTool::leaveEvent(const QHoverEvent& event)
{
}

void SingleTool::moveEvent(const QHoverEvent& event)
{
	// Update mouse position only
	_display->updateMousePosition();
}

void SingleTool::mousePressed(const QMouseEvent &event)
{
	// Prepair move reference
	_lastMousePosition = event.pos();

	// Prepair to zoom default (scaleBar).
	if (event.buttons() & Qt::MidButton)
	{
		_fixedPoint = event.pos();
		_scale = _display->getCurrentScene()->getScale();
	}
}

void SingleTool::mouseReleased(const QMouseEvent &event)
{
	// Stop move default (autoMove).
	_autoPan = QPoint(0,0);

	// Stop zoom default (scaleBar).
	_scale = -1;

	_display->updateAll();
}

void SingleTool::mouseMoved(const QMouseEvent &event)
{
	// Make zoom default (scaleBar).
	if (event.buttons() & Qt::MidButton)
	{
		//Aqui pode entrar um feedback de scale

		int diff = event.pos().y() - _fixedPoint.y();
		double newScale = (_scale*100 - diff)/100;
		_display->getCurrentScene()->scaleTo(newScale, _display->getPosition(_fixedPoint));
		_display->updateAll();
	}

	// Make move default (autoMove).
	if (event.buttons() & Qt::RightButton)
	{
		//Aqui pode entrar um feedback de autoMove

		double scale = _display->getCurrentScene()->getScale();
		QPointF diff = event.posF() - _lastMousePosition;
		_autoPan = -(diff/(5*scale));
	}
}

void SingleTool::mouseDblClicked(const QMouseEvent & event)
{
}

void SingleTool::wheelEvent(const QWheelEvent & event)
{
}

void SingleTool::autoMove()
{
	// Execute move default (autoMove).
	if (_autoPan != QPointF(0,0))
	{
		_display->getCurrentScene()->pan(_autoPan);
		_display->updateAll();
	}
}



ZoomTool::ZoomTool(SingleDisplay* display) :
	SingleTool(display)
{
	_onRubberBand = false;
}

ZoomTool::~ZoomTool()
{
}

void ZoomTool::paintEvent(const QPaintEvent& event)
{
	if (_onRubberBand)
	{
		QRect rubber(_fixedPoint, _display->getMouseScreenPosition().toPoint());
		if (abs(rubber.width()) < 8 && abs(rubber.height()) < 8)
			return;
		QPainter painter(_display);
		painter.setPen(QPen(Qt::yellow));
		painter.drawRect(QRect(_fixedPoint,_display->getMouseScreenPosition().toPoint()));
		painter.end();
	}
	SingleTool::paintEvent(event);
}

//void ZoomTool::enterEvent(const QHoverEvent& event)
//{
//}

//void ZoomTool::leaveEvent(const QHoverEvent& event)
//{
//}

//void ZoomTool::moveEvent(const QHoverEvent& event)
//{
//}

void ZoomTool::mousePressed(const QMouseEvent & event)
{
	// Rubberband zoom
	if (event.buttons() & Qt::LeftButton)
	{
		_fixedPoint = event.pos();
		_onRubberBand = true;
		return;
	}
	SingleTool::mousePressed(event);
}

void ZoomTool::mouseReleased(const QMouseEvent & event)
{
	// Rubberband zoom
	if (_onRubberBand)
	{
		QRect rubber(_fixedPoint, event.pos());
		if (abs(rubber.width()) < 8 && abs(rubber.height()) < 8)
			_display->getCurrentScene()->zoom(0.8);
		else
		{
			double wscale = abs(rubber.width()) == 0 ? 1024 : _display->width()/(double)abs(rubber.width());
			double hscale = abs(rubber.height()) == 0 ? 1024 : _display->height()/(double)abs(rubber.height());
			_display->getCurrentScene()->moveTo(_display->getPosition(rubber.center()));
			_display->getCurrentScene()->zoom(wscale < hscale ? wscale : hscale);
		}
		_onRubberBand = false;
	}
	SingleTool::mouseReleased(event);
}

void ZoomTool::mouseMoved(const QMouseEvent & event)
{
	// Rubberband zoom.
	if (event.buttons() & Qt::LeftButton)
	{
		_display->updateAll();
		return;
	}
	SingleTool::mouseMoved(event);
}

void ZoomTool::mouseDblClicked(const QMouseEvent & event)
{
	_display->fitView();
}

//void ZoomTool::wheelEvent(const QWheelEvent & event)
//{
	/* // Old style zoom
	int numDegrees = event.delta() / 8.0;
	int numSteps = numDegrees / 15.0;
	if (event.orientation() == Qt::Vertical)
	{
		double zoomStep;
		if (numSteps>0)
			zoomStep = 1.044273782; // 1*2^(1÷(2^4))
		else if (numSteps<0)
			zoomStep = 0.957603281; // 1/2^(1÷(2^4))
		for (int i = 0; i<abs(numSteps);i++)
			_display->getCurrentScene()->zoom(zoomStep, _display->getPosition(event.pos()));
		_display->updateAll();
	}
	*/
//}



MoveTool::MoveTool(SingleDisplay* display) :
	SingleTool(display)
{
}

MoveTool::~MoveTool()
{
}

//void MoveTool::enterEvent(const QHoverEvent& event)
//{
//}

//void MoveTool::leaveEvent(const QHoverEvent& event)
//{
//}

//void MoveTool::moveEvent(const QHoverEvent& event)
//{
//}

//void MoveTool::mousePressed(const QMouseEvent & event)
//{
//}

//void MoveTool::mouseReleased(const QMouseEvent & event)
//{
//}

void MoveTool::mouseMoved(const QMouseEvent & event)
{
	// Move
	if (event.buttons() & Qt::LeftButton)
	{
		QPointF diff = event.posF() - _lastMousePosition;
		_lastMousePosition = event.pos();
		double scale;
		scale = _display->getCurrentScene()->getScale();
		_display->getCurrentScene()->pan(-(diff/scale));
		_display->updateAll();
		return;
	}
	SingleTool::mouseMoved(event);
}

//void MoveTool::mouseDblClicked(const QMouseEvent & event)
//{
//}

//void MoveTool::wheelEvent(const QWheelEvent & event)
//{
//}



MarkTool::MarkTool(SingleDisplay* display) :
	SingleTool(display),
	mark(GreenMark)
{
}

MarkTool::~MarkTool()
{
}

//void MarkTool::enterEvent(const QHoverEvent& event)
//{
//}

//void MarkTool::leaveEvent(const QHoverEvent& event)
//{
//}

//void MarkTool::moveEvent(const QHoverEvent& event)
//{
//}

void MarkTool::mousePressed(const QMouseEvent & event)
{
	// Add mark
	if (event.buttons() & Qt::LeftButton)
	{
		QPointF local = _display->getPosition(event.pos());
		if (local.x() >= 0 && local.y() >= 0 && local.x() <= _display->getCurrentScene()->getWidth() && local.y() <= _display->getCurrentScene()->getHeight())
		{
			int geometriesCount;
			_display->getCurrentScene()->geometries(geometriesCount)->addPoint(local, "PQP", &mark);
			_display->getCurrentScene()->geometries(geometriesCount)->setLinkPointsMode(0);
			_display->update();
		}
	}
	SingleTool::mousePressed(event);
}

//void MarkTool::mouseReleased(const QMouseEvent & event)
//{
//}

//void MarkTool::mouseMoved(const QMouseEvent & event)
//{
//}

//void MarkTool::mouseDblClicked(const QMouseEvent & event)
//{
//}

//void MarkTool::wheelEvent(const QWheelEvent & event)
//{
//}



InfoTool::InfoTool(SingleDisplay* display) :
	SingleTool(display)
{
}

InfoTool::~InfoTool()
{
}

void InfoTool::paintEvent(const QPaintEvent& event)
{
}

void InfoTool::enterEvent(const QHoverEvent& event)
{
}

void InfoTool::leaveEvent(const QHoverEvent& event)
{
}

void InfoTool::moveEvent(const QHoverEvent& event)
{
	SingleTool::moveEvent(event);

	// Inform mouse updated position
	QPointF p = _display->getLastMousePosition();
        QString info = QString::number(p.x()) + QString(" x ") + QString::number(p.y());
	_infoLabel.setText(info);
}

void InfoTool::mousePressed(const QMouseEvent & event)
{
}

void InfoTool::mouseReleased(const QMouseEvent & event)
{
}

void InfoTool::mouseMoved(const QMouseEvent & event)
{
}

void InfoTool::mouseDblClicked(const QMouseEvent & event)
{
}

void InfoTool::wheelEvent(const QWheelEvent & event)
{
}

QLabel* InfoTool::getInfoLabel()
{
	return &_infoLabel;
}



OverTool::OverTool(SingleDisplay* display) :
	SingleTool(display)
{
}

OverTool::~OverTool()
{
}

void OverTool::paintEvent(const QPaintEvent& event)
{
}

void OverTool::enterEvent(const QHoverEvent& event)
{
}

void OverTool::leaveEvent(const QHoverEvent& event)
{
}

void OverTool::moveEvent(const QHoverEvent& event)
{
}

void OverTool::mousePressed(const QMouseEvent & event)
{
}

void OverTool::mouseReleased(const QMouseEvent & event)
{
}

void OverTool::mouseMoved(const QMouseEvent & event)
{
}

void OverTool::mouseDblClicked(const QMouseEvent & event)
{
}

void OverTool::wheelEvent(const QWheelEvent & event)
{
}



NearTool::NearTool(SingleDisplay* display) :
	SingleTool(display)
{
}

NearTool::~NearTool()
{
}

void NearTool::paintEvent(const QPaintEvent& event)
{
}

void NearTool::enterEvent(const QHoverEvent& event)
{
}

void NearTool::leaveEvent(const QHoverEvent& event)
{
}

void NearTool::moveEvent(const QHoverEvent& event)
{
}

void NearTool::mousePressed(const QMouseEvent & event)
{
}

void NearTool::mouseReleased(const QMouseEvent & event)
{
}

void NearTool::mouseMoved(const QMouseEvent & event)
{
}

void NearTool::mouseDblClicked(const QMouseEvent & event)
{
}

void NearTool::wheelEvent(const QWheelEvent & event)
{
}



SingleToolsBar::SingleToolsBar(SingleDisplay *display, QWidget *parent) :
	QToolBar(parent),
	zoom(display),
	move(display),
	mark(display),
	info(display)
{
	_display = display;
	_display->setActivatedTool(&info);
	resize(380, 28);

	setZoomTool = new QAction("Zoom", this);
	setMoveTool = new QAction("Move", this);
	setMarkTool = new QAction("Mark", this);
	setFitView = new QAction("FitView", this);
	openImage = new QAction("Open", this);
	saveImage = new QAction("Save", this);

	addAction(openImage);
	addAction(saveImage);
	addSeparator();
	addAction(setMarkTool);
	addAction(setMoveTool);
	addAction(setZoomTool);
	addSeparator();
	addAction(setFitView);
        //addSeparator();
        //addWidget(info.getInfoLabel());

	setZoomTool->setCheckable(true);
	setMoveTool->setCheckable(true);
	setMarkTool->setCheckable(true);

	QActionGroup* navegation = new QActionGroup(this);
	navegation->addAction(setZoomTool);
	navegation->addAction(setMoveTool);
	navegation->addAction(setMarkTool);
	navegation->setExclusive(true);

	QActionGroup* loadAndSave = new QActionGroup(this);
	loadAndSave->addAction(openImage);
	loadAndSave->addAction(saveImage);
	//loadAndSave->setVisible(false);

	connect(this, SIGNAL(actionTriggered(QAction*)), this, SLOT(executeAction(QAction*)));
        setMoveTool->trigger();
}

void SingleToolsBar::deactivateAllExclusiveTools()
{
	_display->setActivatedTool(&move, false);
	_display->setActivatedTool(&mark, false);
	_display->setActivatedTool(&zoom, false);
}

void  SingleToolsBar::setOpenVisible(bool status)
{
    openImage->setVisible(status);
}

void  SingleToolsBar::setSaveVisible(bool status)
{
    saveImage->setVisible(status);
}

void  SingleToolsBar::setMarkVisible(bool status)
{
    setMarkTool->setVisible(status);
}

QLabel* SingleToolsBar::getInfo()
{
    return info.getInfoLabel();
}

void SingleToolsBar::executeAction(QAction *action)
{
	if (action ==  setZoomTool )
	{
		deactivateAllExclusiveTools();
		_display->setActivatedTool(&zoom);
	}
	if (action ==  setMoveTool )
	{
		deactivateAllExclusiveTools();
		_display->setActivatedTool(&move);
	}
	if (action ==  setMarkTool )
	{
		deactivateAllExclusiveTools();
		_display->setActivatedTool(&mark);
	}
	if (action ==  setFitView )
	{
		_display->fitView();
	}
	if (action ==  openImage )
	{
		QString filename = QFileDialog::getOpenFileName(0, "Open Image", ".", "*.bmp");
		if (!filename.isEmpty())
		{
			SingleScene* scene = (SingleScene*)_display->getCurrentScene();
			scene->loadImage(filename);
			_display->updateAll();
		}
	}
	if (action ==  saveImage )
	{
	}
}
