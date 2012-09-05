#include "SingleDisplay.h"

#include "StereoTools.h"

#include <QCursor>
#include <QPainter>
#include <QColor>
#include <QTransform>
#include <QResizeEvent>

#include <math.h>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

SingleDisplay::SingleDisplay(QWidget *parent, AbstractScene *currentScene):
	QWidget(parent)
{
	if (currentScene)
		_currentScene = currentScene;
	else
		_currentScene = new SingleScene(this, "");
	_displayMode = IntermediatedScreen;
	//_onMove = false;
	_cloneScale = true;
	_showDetailArea = false;
	_blockShowDetailArea = false;
	_onPainting = false;
	_over = NULL;
	_detail = NULL;
	setMinimumSize(150,150);

	setAttribute(Qt::WA_Hover, true);
	installEventFilter(this);

	//Cursor cursor(BlackAndWhiteCursor);
	//setCursor(cursor.toQCursor());
}

SingleDisplay::~SingleDisplay()
{
}
void SingleDisplay::updateMousePosition()
{
	_mouseLastPos = mapFromGlobal(QCursor::pos());
}

bool SingleDisplay::positionIsVisible(QPointF pos)
{
	/*
 QPointF screenPos;
 SingleScene* currentScene = (SingleScene*)_currentScene;
 if (_displayMode == TopViewScreen)
  screenPos = (pos - QPointF(currentScene->getWidth()/2.0,currentScene->getHeight()/2.0))*currentScene->getThumbScale() + QPointF(width()/2, height()/2);
 else
  screenPos = (pos - _currentScene->getViewpoint())*_currentScene->getScale() + QPointF(width()/2, height()/2);
  */

	QPointF screenPos = screenPosition(pos);
	if (screenPos.x() >= 0 && screenPos.y() >= 0 && screenPos.x() <= width() && screenPos.y() <= height())
		return true;
	else
		return false;
}

QPointF SingleDisplay::screenPosition(QPointF pos)
{
	QPointF screenPos;
	SingleScene* currentScene = (SingleScene*)_currentScene;
	if (_displayMode == TopViewScreen)
		screenPos = (pos - QPointF(currentScene->getWidth()/2.0,currentScene->getHeight()/2.0))*currentScene->getThumbScale() + QPointF(width()/2, height()/2);
	else if (_displayMode == MostDetailedScreen)
		screenPos = (pos - currentScene->getDetailedPoint())*currentScene->getScale()*currentScene->getDetailZoom() + QPointF(width()/2, height()/2);
	else
		screenPos = (pos - _currentScene->getViewpoint())*_currentScene->getScale() + QPointF(width()/2, height()/2);

	return screenPos;
}

QPointF SingleDisplay::getLastMousePosition()
{
	QPointF diffTocenter(_mouseLastPos.x() - size().width() / 2, _mouseLastPos.y() - size().height() / 2);
	double scale;
	SingleScene* currentScene = (SingleScene*)_currentScene;
	QPointF mousePos;
	if (_displayMode == TopViewScreen)
	{
		scale = currentScene->getThumbScale();
		mousePos = QPointF(currentScene->getWidth()/2.0,currentScene->getHeight()/2.0) + diffTocenter / scale;
	}
	else if (_displayMode == MostDetailedScreen)
	{
		scale = currentScene->getScale()*currentScene->getDetailZoom();
		mousePos = currentScene->getDetailedPoint() + diffTocenter / scale;
	}
	else
	{
		scale = _currentScene->getScale();
		mousePos = _currentScene->getViewpoint() + diffTocenter / scale;
	}
	return mousePos;
}

QPointF SingleDisplay::getPosition(QPoint screenPosition)
{
	QPointF diffTocenter(screenPosition.x() - size().width() / 2, screenPosition.y() - size().height() / 2);
	double scale;
	scale = _currentScene->getScale();
	return _currentScene->getViewpoint() + diffTocenter / scale;
}

QPointF SingleDisplay::getMouseScreenPosition()
{
	return _mouseLastPos;
}

bool SingleDisplay::painting()
{
	return _onPainting;
}

bool SingleDisplay::showDetailedArea()
{
	return _showDetailArea;
}

void SingleDisplay::setShowDetailedArea(bool status)
{
	_showDetailArea = status;
}

void SingleDisplay::blockShowDetailedArea(bool status)
{
	_blockShowDetailArea = status;
}

void SingleDisplay::setCurrentScene(AbstractScene *newScene)
{
	_currentScene = newScene;
	update();
}

AbstractScene* SingleDisplay::getCurrentScene()
{
	return _currentScene;
}

SingleDisplay* SingleDisplay::getOverDisplay()
{
	if (_over == NULL)
	{
		SingleDisplay* result = new SingleDisplay(0);
		result->setOverviewMode(this);
		_over = result;
	}
	return _over;
}

SingleDisplay* SingleDisplay::getDetailDisplay()
{
	if (_detail == NULL)
	{
		SingleDisplay* result = new SingleDisplay(0);
		result->setDetailMode(this);
		_detail = result;
	}
	return _detail;
}

void SingleDisplay::fitView()
{
	_currentScene->setViewport(size());
	double wscale = width() / (double)_currentScene->getWidth();
	double hscale = height() / (double)_currentScene->getHeight();
	_currentScene->scaleTo(wscale < hscale ? wscale : hscale);
	_currentScene->centerContent();
	update();
}

void SingleDisplay::pan(int dx, int dy)
{
}

void SingleDisplay::zoom(double zoomFactor, QPoint* atPoint)
{
}

//void SingleDisplay::setDetailZoom(double zoomFactor)
//{
//if (_detail)
//_detail->zoom(zoomFactor);
//updateDetail();
//}

void SingleDisplay::setCloneScale(bool status)
{
	_cloneScale = status;
	update();
}

void SingleDisplay::updateAll()
{
	QWidget::update();
	if(_displayMode == IntermediatedScreen)
	{
		if (_detail)
			_detail->update();
		if (_over)
			_over->update();
	}
}

void SingleDisplay::updateDetail()
{
	if (_detail)
		_detail->update();
}

void SingleDisplay::updateDetail(QPointF* mousePosition, bool emitClicked)
{
	if (!_currentScene || !mousePosition)
		return;
	double scale;
	scale = _currentScene->getScale();
	_mouseLastPos.setX(((*mousePosition - _currentScene->getViewpoint()) * scale).x() + size().width() / 2);
	_mouseLastPos.setY(((*mousePosition - _currentScene->getViewpoint()) * scale).y() + size().height() / 2);
	if (_detail)
		_detail->update();
	if (emitClicked)
	{
		QPointF lastPos = getLastMousePosition();
		emit mouseClicked(&lastPos);
	}
}

void SingleDisplay::setOverviewMode(SingleDisplay *display)
{
	_displayMode = TopViewScreen;
	_detail = display;
	_currentScene = _detail->getCurrentScene();
}

void SingleDisplay::setDetailMode(SingleDisplay *display)
{
	_displayMode = MostDetailedScreen;
	_over = display;
	_currentScene = _over->getCurrentScene();
}

DisplayMode SingleDisplay::getDisplayMode()
{
	return _displayMode;
}

void SingleDisplay::setActivatedTool(SingleTool *tool, bool active)
{
	for (int i = _tool.size() - 1; i >=  0; i--)
	{
		if (tool == _tool.at(i))
		{
			_tool.removeAt(i);
		}
	}
	if (active)
		_tool.prepend(tool);
}

void SingleDisplay::setActivatedTool(StereoTool *tool, bool active)
{
	for (int i = _stool.size() - 1; i >=  0; i--)
	{
		if (tool == _stool.at(i))
		{
			_stool.removeAt(i);
		}
	}
	if (active)
		_stool.prepend(tool);
}

void SingleDisplay::paintEvent(QPaintEvent *e)
{
	_onPainting = true;
	if (_displayMode == IntermediatedScreen)
	{
		QPainter painter(this);
		painter.fillRect(rect(),QBrush(SymbolsResource::getBackGround(Qt::darkGray)));
		if (_currentScene->isValid())
		{
			QRect target = rect();

			if (_showDetailArea && !_blockShowDetailArea && _detail && _detail->isVisible())
				painter.drawImage(0, 0,_currentScene->getFrame(target.size(), _detail->size()));
			else
				painter.drawImage(0, 0,_currentScene->getFrame(target.size()));
			if (_detail)
				_detail->update();
			if (_over)
				_over->update();

			/*
   if (_detail && _detail->isVisible())// && _showDetailArea)
   {
	SingleScene* currentScene = (SingleScene*)_currentScene;
	//QRect viewedRect(((viewpoint_-QPointF(rect->width()/(scale_*2.0), rect->height()/(scale_*2.0)))*scale).toPoint(), ((scale*QRectF(*rect).size())/scale_).toSize());
	QRect detailedRect(((currentScene->getDetailedPoint() -QPointF(_detail->width()/(currentScene->getDetailZoom()*currentScene->getScale()*2.0), _detail->height()/(currentScene->getDetailZoom()*currentScene->getScale()*2.0)))*currentScene->getScale()).toPoint(), ((currentScene->getScale()*QSizeF(_detail->size()))/(currentScene->getScale()*currentScene->getDetailZoom())).toSize());
	painter.setPen(QPen(QColor(Qt::yellow)));
	painter.drawRect(detailedRect);
   }
   */
		}
		//painter.end();
	}
	else if (_displayMode == TopViewScreen)
	{
		QPainter painter(this);
		painter.fillRect(rect(),QBrush(SymbolsResource::getBackGround(Qt::darkGray)));
		AbstractScene* currentScene;
		if (_detail && (currentScene = _detail->getCurrentScene()) && currentScene->isValid())
		{
			QRect target = rect();
			QSize targetSize = target.size();

                        //QImage thumb = currentScene->getThumb(targetSize,&(_detail->rect()));
                        QRect detailRect = _detail->rect();
                        QImage thumb = currentScene->getThumb(targetSize,&detailRect);
			painter.drawImage((targetSize.width()-thumb.width())/2, (targetSize.height()-thumb.height())/2, thumb);
			//painter.drawImage(0, 0, thumb);
		}
		painter.end();
	}
	else if (_displayMode == MostDetailedScreen)
	{
		QPainter painter(this);
		painter.fillRect(rect(),QBrush(SymbolsResource::getBackGround(Qt::darkGray)));
		SingleScene* currentScene;
		if (_over && (currentScene = (SingleScene*)_over->getCurrentScene()) && currentScene->isValid())
		{
			QRect target = rect();
			QSize targetSize = target.size();
			//double zoom = ceil(currentScene->getScale()*currentScene->getDetailZoom());
			double zoom = currentScene->getScale()*currentScene->getDetailZoom();

			//QImage detail = currentScene->getDetail(targetSize, _over->getLastMousePosition(), zoom);
			QImage detail = currentScene->getDetail(targetSize, currentScene->getDetailedPoint(), zoom);
			painter.drawImage(0, 0, detail);

			/*
   if (!_over->showDetailedArea())
   {
	QPixmap ico(_over->cursor().pixmap());
	QRect reg(QPoint(),ico.size());
	reg.moveCenter(QPoint(width()/2, height()/2));
	painter.drawPixmap(reg,ico);
   }
   */
		}
		painter.end();
	}
	if (!_currentScene || !_currentScene->isValid())
		return;
	for (int i = 0; i < _tool.size(); i++)
	{
		_tool.at(i)->paintEvent(*e);
	}
	for (int i = 0; i < _stool.size(); i++)
	{
		_stool.at(i)->paintEvent(*e);
	}
	_onPainting = false;
}

void SingleDisplay::resizeEvent(QResizeEvent *e)
{
	if (!_currentScene || !_currentScene->isValid())
		return;

	_currentScene->setViewport(e->size());
	update();

	for (int i = 0; i < _tool.size(); i++)
	{
		_tool.at(i)->resizeEvent(*e);
	}
}

bool SingleDisplay::eventFilter(QObject *o, QEvent *e)
{
	if (o == this)
	{
		switch (e->type())
		{
		case QEvent::HoverEnter :
		{
			QHoverEvent* enter = static_cast<QHoverEvent*>(e);
			enterEvent(enter);
			return true;
		}
		case QEvent::HoverLeave :
		{
			QHoverEvent* leave = static_cast<QHoverEvent*>(e);
			leaveEvent(leave);
			return true;
		}
		case QEvent::HoverMove :
		{
			QHoverEvent* move = static_cast<QHoverEvent*>(e);
			moveEvent(move);
			return true;
		}
		default :
			return QWidget::eventFilter(o, e);
		}
	}
	return false;
}

void SingleDisplay::enterEvent(QHoverEvent *e)
{
	if (!_currentScene || !_currentScene->isValid())
		return;
	for (int i = 0; i < _tool.size(); i++)
	{
		_tool.at(i)->enterEvent(*e);
	}
}

void SingleDisplay::leaveEvent(QHoverEvent *e)
{
	if (!_currentScene || !_currentScene->isValid())
		return;
	for (int i = 0; i < _tool.size(); i++)
	{
		_tool.at(i)->leaveEvent(*e);
	}
}

void SingleDisplay::moveEvent(QHoverEvent *e)
{
	if (!_currentScene || !_currentScene->isValid())
		return;
	for (int i = 0; i < _tool.size(); i++)
	{
		_tool.at(i)->moveEvent(*e);
	}
}

void SingleDisplay::mousePressEvent(QMouseEvent *e)
{
	if (!_currentScene || !_currentScene->isValid())
		return;
	for (int i = 0; i < _tool.size(); i++)
	{
		_tool.at(i)->mousePressed(*e);
	}
}

void SingleDisplay::mouseReleaseEvent(QMouseEvent *e)
{
	if (!_currentScene || !_currentScene->isValid())
		return;
	for (int i = 0; i < _tool.size(); i++)
	{
		_tool.at(i)->mouseReleased(*e);
	}
}

void SingleDisplay::mouseMoveEvent(QMouseEvent *e)
{
	if (!_currentScene || !_currentScene->isValid())
		return;
	for (int i = 0; i < _tool.size(); i++)
	{
		_tool.at(i)->mouseMoved(*e);
	}
}

void SingleDisplay::mouseDoubleClickEvent(QMouseEvent *e)
{
	if (!_currentScene || !_currentScene->isValid())
		return;
	for (int i = 0; i < _tool.size(); i++)
	{
		_tool.at(i)->mouseDblClicked(*e);
	}
}

void SingleDisplay::wheelEvent(QWheelEvent *e)
{
	if (!_currentScene || !_currentScene->isValid())
		return;
	for (int i = 0; i < _tool.size(); i++)
	{
		_tool.at(i)->wheelEvent(*e);
	}
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
