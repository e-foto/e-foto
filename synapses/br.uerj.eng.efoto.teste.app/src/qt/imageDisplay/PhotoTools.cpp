#include "PhotoTools.h"

#include "StereoDisplay.h"

#include <QFileDialog>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

void PhotoTool::paintEvent(const QPaintEvent& event)
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

void PhotoTool::enterEvent(const QHoverEvent& event)
{
}

void PhotoTool::leaveEvent(const QHoverEvent& event)
{
}

void PhotoTool::moveEvent(const QHoverEvent& event)
{
	// Update mouse position only
	_display->updateMousePosition();
	double leftCol = _display->getPositionLeft((_display->getMouseScreenPosition()+_display->getLeftCursorOffset()).toPoint()).x();
	double leftLin = _display->getPositionLeft((_display->getMouseScreenPosition()+_display->getLeftCursorOffset()).toPoint()).y();
	double z = _display->getCurrentZ();
	double rightCol = _display->getPositionRight((_display->getMouseScreenPosition()+_display->getRightCursorOffset()).toPoint()).x();
	double rightLin = _display->getPositionRight((_display->getMouseScreenPosition()+_display->getRightCursorOffset()).toPoint()).y();
	double scale = _display->getCurrentScene()->getLeftScene()->getScale();

	//qDebug("%fx%f %fx%f %f", leftCol, leftLin, rightCol, rightLin, z);
	//QPointF diff = QPointF(_engine->moveAtZ(leftCol, leftLin, z).at(0), _engine->moveAtZ(leftCol, leftLin, z).at(1));
	//qDebug("%fx%f %fx%f %fx%f", diff.x(), diff.y(), diff.x()-rightCol, diff.y()-rightLin, _display->getRightCursorOffset().x() + diff.x()-rightCol, _display->getRightCursorOffset().y() + diff.y()-rightLin);
	//qDebug("%f", scale);


	_display->setRightCursorOffset(_display->getRightCursorOffset() + QPointF(_engine->moveAtZ(leftCol, leftLin, z).at(0) -rightCol, _engine->moveAtZ(leftCol, leftLin, z).at(1) -rightLin)*scale);


	/*if (abs(_display->getLeftCursorOffset().y() - _display->getRightCursorOffset().y()) > 4)
	{
		if (_display->getLeftCursorOffset().y() < _display->getRightCursorOffset().y())
		{
			double leftPan = _display->getPositionLeft(_display->getMouseScreenPosition().toPoint()).y() - _display->getPositionLeft((_display->getMouseScreenPosition()+_display->getLeftCursorOffset()).toPoint()).y();
			double rightPan = _display->getPositionRight(_display->getMouseScreenPosition().toPoint()).y() - _display->getPositionRight((_display->getMouseScreenPosition()+_display->getRightCursorOffset()).toPoint()).y();
			_display->getCurrentScene()->getLeftScene()->pan(QPointF(0,-leftPan));
			_display->getCurrentScene()->getRightScene()->pan(QPointF(0,-rightPan));
			_display->setLeftCursorOffset(QPointF(_display->getLeftCursorOffset().x(),0));
			_display->setRightCursorOffset(QPointF(_display->getRightCursorOffset().x(),0));
		}
		else
		{
			double leftPan = _display->getPositionLeft(_display->getMouseScreenPosition().toPoint()).y() - _display->getPositionLeft((_display->getMouseScreenPosition()+_display->getLeftCursorOffset()).toPoint()).y();
			double rightPan = _display->getPositionRight(_display->getMouseScreenPosition().toPoint()).y() - _display->getPositionRight((_display->getMouseScreenPosition()+_display->getRightCursorOffset()).toPoint()).y();
			_display->getCurrentScene()->getLeftScene()->pan(QPointF(0,-leftPan));
			_display->getCurrentScene()->getRightScene()->pan(QPointF(0,-rightPan));
			_display->setLeftCursorOffset(QPointF(_display->getLeftCursorOffset().x(),0));
			_display->setRightCursorOffset(QPointF(_display->getRightCursorOffset().x(),0));
		}
	}*/

	_display->updateAll();
}

void PhotoTool::mousePressed(const QMouseEvent &event)
{
	// Prepair move reference
	_lastMousePosition = event.pos();

	// Prepair to zoom default (scaleBar).
	if (event.buttons() & Qt::MidButton)
	{
		_fixedPoint = event.pos();
		_scale = _display->getCurrentScene()->getLeftScene()->getScale();
	}
}

void PhotoTool::mouseReleased(const QMouseEvent &event)
{
	// Stop move default (autoMove).
	_autoPan = QPoint(0,0);

	// Stop zoom default (scaleBar).
	_scale = -1;

	_display->updateAll();
}

void PhotoTool::mouseMoved(const QMouseEvent &event)
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

void PhotoTool::mouseDblClicked(const QMouseEvent & event)
{
}

void PhotoTool::wheelEvent(const QWheelEvent & event)
{
	double numSteps = event.delta() / (15.0 * 8.0);
	if (event.orientation() == Qt::Vertical)
	{
		//_display->setLeftCursorOffset(QPointF(_display->getLeftCursorOffset().x()+numSteps/abs(numSteps),0));

		//double leftCol = _display->getPositionLeft((_display->getMouseScreenPosition()+_display->getLeftCursorOffset()).toPoint()).x();
		//double leftLin = _display->getPositionLeft((_display->getMouseScreenPosition()+_display->getLeftCursorOffset()).toPoint()).y();
		//double rightCol = _display->getPositionRight((_display->getMouseScreenPosition()+_display->getRightCursorOffset()).toPoint()).x();
		//double rightLin = _display->getPositionRight((_display->getMouseScreenPosition()+_display->getRightCursorOffset()).toPoint()).y();
		//QList<double> enh = _engine->getSpatialIntersection(leftCol, leftLin, rightCol, rightLin);

		//_display->setCurrentZ(enh.at(2));

		_display->setCurrentZ(_display->getCurrentZ()+numSteps);
		double leftCol = _display->getPositionLeft((_display->getMouseScreenPosition()+_display->getLeftCursorOffset()).toPoint()).x();
		double leftLin = _display->getPositionLeft((_display->getMouseScreenPosition()+_display->getLeftCursorOffset()).toPoint()).y();
		double z = _display->getCurrentZ();
		double rightCol = _display->getPositionRight((_display->getMouseScreenPosition()+_display->getRightCursorOffset()).toPoint()).x();
		double rightLin = _display->getPositionRight((_display->getMouseScreenPosition()+_display->getRightCursorOffset()).toPoint()).y();
		_display->setRightCursorOffset(_display->getRightCursorOffset() + QPointF(_engine->moveAtZ(leftCol, leftLin, z).at(0) -rightCol, _engine->moveAtZ(leftCol, leftLin, z).at(1) -rightLin));

		_display->updateAll();
	}
}

void PhotoTool::setEngine(PhotogrammetryEngineInterface *engine)
{
	_engine = engine;
}

void PhotoTool::autoMove()
{
	// Execute move default (autoMove).
	if (_autoPan != QPointF(0,0))
	{
		_display->getCurrentScene()->getLeftScene()->pan(_autoPan);
		_display->getCurrentScene()->getRightScene()->pan(_autoPan);
		_display->updateAll();
	}
}



ZoomPhotoTool::ZoomPhotoTool(StereoDisplay* display) :
	PhotoTool(display)
{
	_onRubberBand = false;
}

ZoomPhotoTool::~ZoomPhotoTool()
{
}

void ZoomPhotoTool::paintEvent(const QPaintEvent &event)
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
	PhotoTool::paintEvent(event);
}

//void ZoomPhotoTool::enterEvent(const QHoverEvent& event)
//{
//}

//void ZoomPhotoTool::leaveEvent(const QHoverEvent& event)
//{
//}

//void ZoomPhotoTool::moveEvent(const QHoverEvent& event)
//{
//}

void ZoomPhotoTool::mousePressed(const QMouseEvent & event)
{
	// Rubberband zoom
	if (event.buttons() & Qt::LeftButton)
	{
		_fixedPoint = event.pos();
		_onRubberBand = true;
		return;
	}
	PhotoTool::mousePressed(event);
}

void ZoomPhotoTool::mouseReleased(const QMouseEvent & event)
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
	PhotoTool::mouseReleased(event);
}

void ZoomPhotoTool::mouseMoved(const QMouseEvent & event)
{
	// Rubberband zoom.
	if (event.buttons() & Qt::LeftButton)
	{
		_display->updateAll();
		return;
	}
	PhotoTool::mouseMoved(event);
}

void ZoomPhotoTool::mouseDblClicked(const QMouseEvent & event)
{
	_display->fitView();
}

/*
void ZoomPhotoTool::wheelEvent(const QWheelEvent & event)
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


MovePhotoTool::MovePhotoTool(StereoDisplay* display) :
	PhotoTool(display)
{
}

MovePhotoTool::~MovePhotoTool()
{
}

//void MovePhotoTool::enterEvent(const QHoverEvent& event)
//{
//}

//void MovePhotoTool::leaveEvent(const QHoverEvent& event)
//{
//}

//void MovePhotoTool::moveEvent(const QHoverEvent& event)
//{
//	_display->updateMousePosition();
//}

//void MovePhotoTool::mousePressed(const QMouseEvent & event)
//{
//}

//void MovePhotoTool::mouseReleased(const QMouseEvent & event)
//{
//}

void MovePhotoTool::mouseMoved(const QMouseEvent & event)
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
	PhotoTool::mouseMoved(event);
}

//void MovePhotoTool::mouseDblClicked(const QMouseEvent & event)
//{
//}

//void MovePhotoTool::wheelEvent(const QWheelEvent & event)
//{
//}



MarkPhotoTool::MarkPhotoTool(StereoDisplay* display) :
	PhotoTool(display),
	mark(WhiteMark)
{
}

MarkPhotoTool::~MarkPhotoTool()
{
}

//void MarkPhotoTool::enterEvent(const QHoverEvent& event)
//{
//}

//void MarkPhotoTool::leaveEvent(const QHoverEvent& event)
//{
//}

//void MarkPhotoTool::moveEvent(const QHoverEvent& event)
//{
//}

void MarkPhotoTool::mousePressed(const QMouseEvent & event)
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
	PhotoTool::mousePressed(event);
}

//void MarkPhotoTool::mouseReleased(const QMouseEvent & event)
//{
//}

//void MarkPhotoTool::mouseMoved(const QMouseEvent & event)
//{
//}

//void MarkPhotoTool::mouseDblClicked(const QMouseEvent & event)
//{
//}

//void MarkPhotoTool::wheelEvent(const QWheelEvent & event)
//{
//}



InfoPhotoTool::InfoPhotoTool(StereoDisplay* display) :
	PhotoTool(display)
{
}

InfoPhotoTool::~InfoPhotoTool()
{
}

void InfoPhotoTool::paintEvent(const QPaintEvent &event)
{
}

void InfoPhotoTool::enterEvent(const QHoverEvent& event)
{
	_display->setCursor(WhiteCursor);
}

void InfoPhotoTool::leaveEvent(const QHoverEvent& event)
{
	_display->setCursor(NoCursor);
}

void InfoPhotoTool::moveEvent(const QHoverEvent& event)
{
	PhotoTool::moveEvent(event);

	// Inform mouse updated position
	QPointF pl = _display->getPositionLeft(event.pos() + _display->getLeftCursorOffset().toPoint());
	QPointF pr = _display->getPositionRight(event.pos() + _display->getRightCursorOffset().toPoint());

	double leftCol = _display->getPositionLeft((_display->getMouseScreenPosition()+_display->getLeftCursorOffset()).toPoint()).x();
	double leftLin = _display->getPositionLeft((_display->getMouseScreenPosition()+_display->getLeftCursorOffset()).toPoint()).y();
	double rightCol = _display->getPositionRight((_display->getMouseScreenPosition()+_display->getRightCursorOffset()).toPoint()).x();
	double rightLin = _display->getPositionRight((_display->getMouseScreenPosition()+_display->getRightCursorOffset()).toPoint()).y();
	QList<double> enh = _engine->getSpatialIntersection(leftCol, leftLin, rightCol, rightLin);

	QString info = QString::number(pl.x(),'f',0) + QString("x") + QString::number(pl.y(),'f',0) + QString(" ") + QString::number(pr.x(),'f',0) + QString("x") + QString::number(pr.y(),'f',0) + QString(" ") + QString::number(enh.at(0)) + QString("x") + QString::number(enh.at(1),'f',1) + QString("x") + QString::number(enh.at(2),'f',1);
	this->setText(info);
}

void InfoPhotoTool::mousePressed(const QMouseEvent & event)
{
}

void InfoPhotoTool::mouseReleased(const QMouseEvent & event)
{
}

void InfoPhotoTool::mouseMoved(const QMouseEvent & event)
{
}

void InfoPhotoTool::mouseDblClicked(const QMouseEvent & event)
{
}

void InfoPhotoTool::wheelEvent(const QWheelEvent & event)
{
	// Inform mouse updated position
	QPointF pl = _display->getPositionLeft(event.pos() + _display->getLeftCursorOffset().toPoint());
	QPointF pr = _display->getPositionRight(event.pos() + _display->getRightCursorOffset().toPoint());

	double leftCol = _display->getPositionLeft((_display->getMouseScreenPosition()+_display->getLeftCursorOffset()).toPoint()).x();
	double leftLin = _display->getPositionLeft((_display->getMouseScreenPosition()+_display->getLeftCursorOffset()).toPoint()).y();
	double rightCol = _display->getPositionRight((_display->getMouseScreenPosition()+_display->getRightCursorOffset()).toPoint()).x();
	double rightLin = _display->getPositionRight((_display->getMouseScreenPosition()+_display->getRightCursorOffset()).toPoint()).y();
	QList<double> enh = _engine->getSpatialIntersection(leftCol, leftLin, rightCol, rightLin);

	QString info = QString::number(pl.x()) + QString("x") + QString::number(pl.y()) + QString(" ") + QString::number(pr.x()) + QString("x") + QString::number(pr.y()) + QString(" ") + QString::number(enh.at(0),'f',1) + QString("x") + QString::number(enh.at(1),'f',1) + QString("x") + QString::number(enh.at(2),'f',1);
	this->setText(info);
}



PhotoToolsBar::PhotoToolsBar(StereoDisplay *display, QWidget *parent) :
	QToolBar(parent),
	zoom(display),
	move(display),
	mark(display),
	info(display)
{
	_display = display;
	_display->setActivatedTool(&info);
	resize(1000, 28);

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
	loadAndSave->setVisible(true);

	connect(this, SIGNAL(actionTriggered(QAction*)), this, SLOT(executeAction(QAction*)));
	setMarkTool->trigger();

	_engine = (PhotogrammetryEngineInterface*) new PhotogrammetryEngine_Qt;
	zoom.setEngine(_engine);
	mark.setEngine(_engine);
	move.setEngine(_engine);
	info.setEngine(_engine);
}

void PhotoToolsBar::deactivateAllExclusiveTools()
{
	_display->setActivatedTool(&move, false);
	_display->setActivatedTool(&mark, false);
	_display->setActivatedTool(&zoom, false);
}

void PhotoToolsBar::executeAction(QAction *action)
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
		QString filename = QFileDialog::getOpenFileName(0, "Open File", ".", "*.epp");
		if (!filename.isEmpty())
		{
			bool ok;
			_engine->openEpp(filename.toStdString());
			_engine->setStereoPair(1,2);
			_display->getCurrentScene()->getLeftScene()->loadImage(QString(_engine->getAbsoluteImageNamePath(1, &ok).c_str()));
			_display->getCurrentScene()->getRightScene()->loadImage(QString(_engine->getAbsoluteImageNamePath(2, &ok).c_str()));
			_display->getCurrentScene()->getLeftScene()->moveTo(QPointF(_engine->getLeftSpatialResection(681361.203, 7465299.251, 7.796).at(0),_engine->getLeftSpatialResection(681361.203, 7465299.251, 7.796).at(1)));
			_display->getCurrentScene()->getRightScene()->moveTo(QPointF(_engine->getRightSpatialResection(681361.203, 7465299.251, 7.796).at(0),_engine->getRightSpatialResection(681361.203, 7465299.251, 7.796).at(1)));
			_display->getCurrentScene()->getLeftScene()->scaleTo(4);
			_display->getCurrentScene()->getRightScene()->scaleTo(4);
			_display->updateAll();
		}
	}
	if (action ==  saveImage )
	{
	}
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
