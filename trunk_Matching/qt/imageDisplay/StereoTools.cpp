#include "StereoTools.h"

#include <QFileDialog>
#include "StereoDisplay.h"

#define NOCURSOR QPixmap::fromImage(SymbolsResource::getBackGround(QColor(0,0,0,0)))

void StereoTool::paintEvent(const QPaintEvent& event)
{
	QPainter painter(_display->getRealDisplay());
	painter.setPen(QPen(Qt::yellow));
	if (_autoPan != QPointF(0,0))
	{
		// Draw autoMove feedback
		painter.drawLine(_lastMousePosition, _lastMousePosition - _autoPan*5*_display->getCurrentScene()->getLeftScene()->getScale());
	}
	else if (_scale > 0)
	{
		// Draw scaleBar feedback
		QPoint endBar(_fixedPoint.x(), _display->getMouseScreenPosition().y());
		painter.drawLine(_fixedPoint, endBar);
		painter.drawText(endBar, QString::number(_display->getCurrentScene()->getLeftScene()->getScale()*100,'f', 1).append("%"));
	}
	painter.end();
}

void StereoTool::enterEvent(const QHoverEvent& event)
{
}

void StereoTool::leaveEvent(const QHoverEvent& event)
{
}

void StereoTool::moveEvent(const QHoverEvent& event)
{
	// Update mouse position only
	_display->updateMousePosition();
	if (!_hasButtomPressed)
		_display->updateAll();
}

void StereoTool::mousePressed(const QMouseEvent &event)
{
	_hasButtomPressed = true;

	// Prepair move reference
	_lastMousePosition = event.pos();

	// Prepair to zoom default (scaleBar).
	if (event.buttons() & Qt::MidButton)
	{
		_fixedPoint = event.pos();
		_scale = _display->getCurrentScene()->getLeftScene()->getScale();
	}
}

void StereoTool::mouseReleased(const QMouseEvent &event)
{
	_hasButtomPressed = false;

	// Stop move default (autoMove).
	_autoPan = QPoint(0,0);

	// Stop zoom default (scaleBar).
	_scale = -1;

	_display->updateAll();
}

void StereoTool::mouseMoved(const QMouseEvent &event)
{
	// Make zoom default (scaleBar).
	if (event.buttons() & Qt::MidButton)
	{
		//Aqui pode entrar um feedback de scale

		int diff = event.pos().y() - _fixedPoint.y();
		double newScale = (_scale*100 - diff)/100;
		_display->getCurrentScene()->getLeftScene()->scaleTo(newScale, _display->getPositionLeft(_fixedPoint));
		_display->getCurrentScene()->getRightScene()->scaleTo(newScale, _display->getPositionRight(_fixedPoint));
		_display->updateAll();
	}

	// Make move default (autoMove).
	if (event.buttons() & Qt::RightButton)
	{
		//Aqui pode entrar um feedback de autoMove

		double scale = _display->getCurrentScene()->getLeftScene()->getScale();
		QPointF diff = event.posF() - _lastMousePosition;
		_autoPan = -(diff/(5*scale));
	}
}

void StereoTool::mouseDblClicked(const QMouseEvent & event)
{
}

void StereoTool::wheelEvent(const QWheelEvent & event)
{
	double numSteps = event.delta() / (15.0 * 8.0);
	if (event.orientation() == Qt::Vertical)
	{
		if (event.modifiers() == Qt::ShiftModifier)
			_display->getCurrentScene()->getRightScene()->pan(QPointF(numSteps, 0));
		else if (event.modifiers() == Qt::ControlModifier)
			_display->getCurrentScene()->getRightScene()->pan(QPointF(0, numSteps));
		else
		{
			_display->setLeftCursorOffset(QPointF(_display->getLeftCursorOffset().x()+numSteps/abs(numSteps),0));
			_display->setRightCursorOffset(QPointF(_display->getRightCursorOffset().x()-numSteps/abs(numSteps),0));
		}
		_display->updateAll();
	}
}

void StereoTool::autoMove()
{
	// Execute move default (autoMove).
	if (_autoPan != QPointF(0,0))
	{
		_display->getCurrentScene()->getLeftScene()->pan(_autoPan);
		_display->getCurrentScene()->getRightScene()->pan(_autoPan);
		_display->updateAll();
	}
}



ZoomStereoTool::ZoomStereoTool(StereoDisplay* display) :
	StereoTool(display)
{
	_onRubberBand = false;
}

ZoomStereoTool::~ZoomStereoTool()
{
}

void ZoomStereoTool::paintEvent(const QPaintEvent &event)
{
	if (_onRubberBand)
	{
		QRect rubber(_fixedPoint, _display->getMouseScreenPosition().toPoint());
		if (abs(rubber.width()) < 8 && abs(rubber.height()) < 8)
			return;
		QPainter painter(_display->getRealDisplay());
		painter.setPen(QPen(Qt::yellow));
		painter.drawRect(QRect(_fixedPoint,_display->getMouseScreenPosition().toPoint()));
		painter.end();
	}
	StereoTool::paintEvent(event);
}

//void ZoomStereoTool::enterEvent(const QHoverEvent& event)
//{
//}

//void ZoomStereoTool::leaveEvent(const QHoverEvent& event)
//{
//}

//void ZoomStereoTool::moveEvent(const QHoverEvent& event)
//{
//}

void ZoomStereoTool::mousePressed(const QMouseEvent & event)
{
	// Rubberband zoom
	if (event.buttons() & Qt::LeftButton)
	{
		_fixedPoint = event.pos();
		_onRubberBand = true;
		return;
	}
	StereoTool::mousePressed(event);
}

void ZoomStereoTool::mouseReleased(const QMouseEvent & event)
{
	// Rubberband zoom
	if (_onRubberBand)
	{
		QRect rubber(_fixedPoint, event.pos());
		if (abs(rubber.width()) < 8 && abs(rubber.height()) < 8)
		{
			_display->getCurrentScene()->getLeftScene()->zoom(0.8);
			_display->getCurrentScene()->getRightScene()->zoom(0.8);
		}
		else
		{
			double wscale = abs(rubber.width()) == 0 ? 1024 : _display->width()/(double)abs(rubber.width());
			double hscale = abs(rubber.height()) == 0 ? 1024 : _display->height()/(double)abs(rubber.height());
			_display->getCurrentScene()->getLeftScene()->moveTo(_display->getPositionLeft(rubber.center()));
			_display->getCurrentScene()->getLeftScene()->zoom(wscale < hscale ? wscale : hscale);
			_display->getCurrentScene()->getRightScene()->moveTo(_display->getPositionRight(rubber.center()));
			_display->getCurrentScene()->getRightScene()->zoom(wscale < hscale ? wscale : hscale);
		}
		_onRubberBand = false;
	}
	StereoTool::mouseReleased(event);
}

void ZoomStereoTool::mouseMoved(const QMouseEvent & event)
{
	// Rubberband zoom.
	if (event.buttons() & Qt::LeftButton)
	{
		_display->updateAll();
		return;
	}
	StereoTool::mouseMoved(event);
}

void ZoomStereoTool::mouseDblClicked(const QMouseEvent & event)
{
	_display->fitView();
}

/*
void ZoomStereoTool::wheelEvent(const QWheelEvent & event)
{
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
		{
			_display->getCurrentScene()->getLeftScene()->zoom(zoomStep, _display->getPositionLeft(event.pos()));
			_display->getCurrentScene()->getRightScene()->zoom(zoomStep, _display->getPositionRight(event.pos()));
		}
		_display->updateAll();
	}
}
*/


MoveStereoTool::MoveStereoTool(StereoDisplay* display) :
	StereoTool(display)
{
}

MoveStereoTool::~MoveStereoTool()
{
}

//void MoveStereoTool::enterEvent(const QHoverEvent& event)
//{
//}

//void MoveStereoTool::leaveEvent(const QHoverEvent& event)
//{
//}

//void MoveStereoTool::moveEvent(const QHoverEvent& event)
//{
//	_display->updateMousePosition();
//}

//void MoveStereoTool::mousePressed(const QMouseEvent & event)
//{
//}

//void MoveStereoTool::mouseReleased(const QMouseEvent & event)
//{
//}

void MoveStereoTool::mouseMoved(const QMouseEvent & event)
{
	// Move
	if (event.buttons() & Qt::LeftButton)
	{
		QPointF diff = event.posF() - _lastMousePosition;
		_lastMousePosition = event.pos();
		double scale;
		scale = _display->getCurrentScene()->getLeftScene()->getScale();
		if (event.modifiers() != Qt::ShiftModifier)
			_display->getCurrentScene()->getLeftScene()->pan(-(diff/scale));
		if (event.modifiers() != Qt::ControlModifier)
			_display->getCurrentScene()->getRightScene()->pan(-(diff/scale));
		_display->updateAll();
		return;
	}
	StereoTool::mouseMoved(event);
}

//void MoveStereoTool::mouseDblClicked(const QMouseEvent & event)
//{
//}

//void MoveStereoTool::wheelEvent(const QWheelEvent & event)
//{
//}



MarkStereoTool::MarkStereoTool(StereoDisplay* display) :
	StereoTool(display),
	mark(WhiteMark)
{
}

MarkStereoTool::~MarkStereoTool()
{
}

//void MarkStereoTool::enterEvent(const QHoverEvent& event)
//{
//}

//void MarkStereoTool::leaveEvent(const QHoverEvent& event)
//{
//}

//void MarkStereoTool::moveEvent(const QHoverEvent& event)
//{
//}

void MarkStereoTool::mousePressed(const QMouseEvent & event)
{
	// Add mark
	if (event.buttons() & Qt::LeftButton)
	{
		QPointF lLocal = _display->getPositionLeft(event.pos()+_display->getLeftCursorOffset().toPoint());
		QPointF rLocal = _display->getPositionRight(event.pos()+_display->getRightCursorOffset().toPoint());
		if ((lLocal.x() >= 0 && lLocal.y() >= 0 && lLocal.x() <= _display->getCurrentScene()->getLeftScene()->getWidth() && lLocal.y() <= _display->getCurrentScene()->getLeftScene()->getHeight())
			&& (rLocal.x() >= 0 && rLocal.y() >= 0 && rLocal.x() <= _display->getCurrentScene()->getRightScene()->getWidth() && rLocal.y() <= _display->getCurrentScene()->getRightScene()->getHeight()))
		{
			int geometriesCount;
			_display->getCurrentScene()->getLeftScene()->geometries(geometriesCount)->addPoint(lLocal, "PQP", &mark);
			_display->getCurrentScene()->getRightScene()->geometries(geometriesCount)->addPoint(rLocal, "PQP", &mark);
			_display->getCurrentScene()->getLeftScene()->geometries(geometriesCount)->setLinkPointsMode(0);
			_display->getCurrentScene()->getRightScene()->geometries(geometriesCount)->setLinkPointsMode(0);
			_display->updateAll();
		}
	}
	StereoTool::mousePressed(event);
}

//void MarkStereoTool::mouseReleased(const QMouseEvent & event)
//{
//}

//void MarkStereoTool::mouseMoved(const QMouseEvent & event)
//{
//}

//void MarkStereoTool::mouseDblClicked(const QMouseEvent & event)
//{
//}

//void MarkStereoTool::wheelEvent(const QWheelEvent & event)
//{
//}



InfoStereoTool::InfoStereoTool(StereoDisplay* display) :
	StereoTool(display)
{
}

InfoStereoTool::~InfoStereoTool()
{
}

void InfoStereoTool::paintEvent(const QPaintEvent &event)
{
}

void InfoStereoTool::enterEvent(const QHoverEvent& event)
{
	_display->setCursor(currentCursor);
}

void InfoStereoTool::leaveEvent(const QHoverEvent& event)
{
	currentCursor = _display->getCursor();
	_display->setCursor(NOCURSOR);
}

void InfoStereoTool::moveEvent(const QHoverEvent& event)
{
	StereoTool::moveEvent(event);

	// Inform mouse updated position
	QPointF pl = _display->getPositionLeft(event.pos() + _display->getLeftCursorOffset().toPoint());
	QPointF pr = _display->getPositionRight(event.pos() + _display->getRightCursorOffset().toPoint());
	QString info = QString::number(pl.x()) + QString("x") + QString::number(pl.y()) + QString(" ") + QString::number(pr.x()) + QString("x") + QString::number(pr.y());
	this->setText(info);
}

void InfoStereoTool::mousePressed(const QMouseEvent & event)
{
}

void InfoStereoTool::mouseReleased(const QMouseEvent & event)
{
}

void InfoStereoTool::mouseMoved(const QMouseEvent & event)
{
}

void InfoStereoTool::mouseDblClicked(const QMouseEvent & event)
{
}

void InfoStereoTool::wheelEvent(const QWheelEvent & event)
{
}



StereoToolsBar::StereoToolsBar(StereoDisplay *display, QWidget *parent) :
	QToolBar(parent),
	zoom(display),
	move(display),
	mark(display),
	info(display)
{
	_display = display;
	_display->setActivatedTool(&info);
	resize(680, 28);

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
	addSeparator();
	addWidget(&info);

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
	setMarkTool->trigger();
}

void StereoToolsBar::deactivateAllExclusiveTools()
{
	_display->setActivatedTool(&move, false);
	_display->setActivatedTool(&mark, false);
	_display->setActivatedTool(&zoom, false);
}

void StereoToolsBar::executeAction(QAction *action)
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
			SingleScene* scene = (SingleScene*)_display->getCurrentScene()->getLeftScene();
			scene->loadImage(filename);
			_display->updateAll();
		}
	}
	if (action ==  saveImage )
	{
		QString filename = QFileDialog::getOpenFileName(0, "Open Image", ".", "*.bmp");
		if (!filename.isEmpty())
		{
			SingleScene* scene = (SingleScene*)_display->getCurrentScene()->getRightScene();
			scene->loadImage(filename);
			_display->updateAll();
		}
	}
}

/* mousePressed
if (e->button() == Qt::RightButton)//:MidButton)
{
	moveLastPos_ = e->posF();
	onMove_ = true;
}
else if (e->button() == Qt::LeftButton)
{
	moveLastPos_ = e->posF();
	if (stereoDisplay_->getCurrentScene())
	{
		QPointF diffTocenter(moveLastPos_.x() - size().width() / 2, moveLastPos_.y() - size().height() / 2);
		QPointF* leftDetail = NULL;
		QPointF* rightDetail = NULL;
		if (stereoDisplay_->getCurrentScene()->getLeftScene() && stereoDisplay_->getCurrentScene()->getLeftScene()->imageLoaded())
			leftDetail = new QPointF(stereoDisplay_->getCurrentScene()->getLeftScene()->getViewpoint() + (diffTocenter+stereoDisplay_->getLeftCursorOffset()) / stereoDisplay_->getCurrentScene()->getLeftScene()->getScale());
		if (stereoDisplay_->getCurrentScene()->getRightScene() && stereoDisplay_->getCurrentScene()->getRightScene()->imageLoaded())
			rightDetail = new QPointF(stereoDisplay_->getCurrentScene()->getRightScene()->getViewpoint() + (diffTocenter+stereoDisplay_->getRightCursorOffset()) / stereoDisplay_->getCurrentScene()->getRightScene()->getScale());
		stereoDisplay_->updateDetail(leftDetail, rightDetail, true);
		if (leftDetail)
			delete(leftDetail);
		if (rightDetail)
			delete(rightDetail);
	}
}
QWidget::mousePressEvent(e);
*/

/* mouseReleased
onMove_ = false;
QWidget::mouseReleaseEvent(e);
*/

/* mouseMoved
QPointF diff = e->posF() - moveLastPos_;
moveLastPos_ = e->posF();
if (stereoDisplay_->getCurrentScene())
{
	QPointF diffTocenter(moveLastPos_.x() -diff.x() - size().width() / 2, moveLastPos_.y() -diff.y() - size().height() / 2);
	QPointF* leftDetail = NULL;
	QPointF* rightDetail = NULL;*/
	/*
	if (stereoDisplay_->getCurrentScene()->getLeftScene() && stereoDisplay_->getCurrentScene()->getLeftScene()->imageLoaded())
		leftDetail = new QPointF(stereoDisplay_->getCurrentScene()->getLeftScene()->getViewpoint() + (diffTocenter+stereoDisplay_->getLeftCursorOffset()) / stereoDisplay_->getCurrentScene()->getLeftScene()->getScale());
	if (stereoDisplay_->getCurrentScene()->getRightScene() && stereoDisplay_->getCurrentScene()->getRightScene()->imageLoaded())
		rightDetail = new QPointF(stereoDisplay_->getCurrentScene()->getRightScene()->getViewpoint() + (diffTocenter+stereoDisplay_->getRightCursorOffset()) / stereoDisplay_->getCurrentScene()->getRightScene()->getScale());

	if (onMove_ && (e->buttons() & Qt::RightButton))//Qt::MidButton))
	{
		if (stereoDisplay_->getCurrentScene()->getLeftScene())
		{
			double lscale = stereoDisplay_->getCurrentScene()->getLeftScene()->getScale();
			stereoDisplay_->getCurrentScene()->getLeftScene()->pan(-(diff/lscale));
		}
		if (stereoDisplay_->getCurrentScene()->getRightScene())
		{
			double rscale = stereoDisplay_->getCurrentScene()->getRightScene()->getScale();
			stereoDisplay_->getCurrentScene()->getRightScene()->pan(-(diff/rscale));
		}
		stereoDisplay_->updateAll(leftDetail, rightDetail);
	}
	else */ /* if (!onMove_)
	{
		update();
		stereoDisplay_->updateDetail(leftDetail, rightDetail);
	}

	if (leftDetail)
		delete(leftDetail);
	if (rightDetail)
		delete(rightDetail);
}
QWidget::mouseMoveEvent(e); */

/* wheelEvent
int numDegrees = e->delta() / 8.0;
int numSteps = numDegrees / 15.0;
if (stereoDisplay_->getCurrentScene() && stereoDisplay_->getCurrentScene()->getLeftScene() && stereoDisplay_->getCurrentScene()->getRightScene())
{
	if (e->orientation() == Qt::Vertical)
	{
		double zoomStep;
		if (numSteps>0)
			zoomStep = 1.044273782; // 1*2^(1÷(2^4))
		else if (numSteps<0)
			zoomStep = 0.957603281; // 1/2^(1÷(2^4))
		for (int i = 0; i<abs(numSteps);i++)
		{
			stereoDisplay_->getCurrentScene()->getLeftScene()->zoom(zoomStep);
			stereoDisplay_->getCurrentScene()->getRightScene()->zoom(zoomStep);
		}
		stereoDisplay_->updateAll();
	}
}
QWidget::wheelEvent(e);
*/
