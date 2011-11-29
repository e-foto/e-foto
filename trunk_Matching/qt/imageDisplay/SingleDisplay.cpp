#include "SingleDisplay.h"

#include <QCursor>
#include <QPainter>
#include <QColor>
#include <QTransform>
#include <QResizeEvent>

#include <math.h>



Cursor::Cursor(const QImage &image2Cursor, QPoint offset) :
	QCursor(QPixmap::fromImage(image2Cursor), offset.x(), offset.y())
{
}

Cursor& Cursor::noCursor()
{
    QSize size(16,16);
    QImage img(size, QImage::Format_ARGB32);
    img.fill(QColor(0,0,0,0).rgba());
    int hotX = size.width() / 2;
    int hotY = size.height() / 2;

    return *this = Cursor(img, QPoint(hotX, hotY));
}

Cursor& Cursor::toCross(QColor color, QSize size, unsigned int weigth)
{
	QImage img(size, QImage::Format_ARGB32);
	img.fill(QColor(0,0,0,0).rgba());
	int hotX = size.width() / 2;
	int hotY = size.height() / 2;

	QPainter painter(&img);
	painter.fillRect(weigth%2, hotY-weigth/2, size.width()/2-(weigth+1)/2, weigth, color);
	painter.fillRect(size.width()/2+(weigth+1)/2, hotY-weigth/2, size.width()/2-weigth/2, weigth, color);
	painter.fillRect(hotX-weigth/2, weigth%2, weigth, size.height()/2-(weigth+1)/2, color);
	painter.fillRect(hotX-weigth/2, size.height()/2+(weigth+1)/2, weigth, size.height()/2-weigth/2, color);
	painter.end();

	return *this = Cursor(img, QPoint(hotX, hotY));
}

Cursor& Cursor::toCross(QColor colorBrush, QColor colorPen, QSize size, unsigned int weigth)
{
	QImage img(size, QImage::Format_ARGB32);
	img.fill(QColor(0,0,0,0).rgba());
	int hotX = size.width() / 2;
	int hotY = size.height() / 2;

	QPainter painter(&img);
	painter.fillRect(weigth%2, hotY-weigth/2, size.width()/2-(weigth+1)/2, weigth, colorPen);
	painter.fillRect(size.width()/2+(weigth+1)/2, hotY-weigth/2, size.width()/2-weigth/2, weigth, colorPen);
	painter.fillRect(hotX-weigth/2, weigth%2, weigth, size.height()/2-(weigth+1)/2, colorPen);
	painter.fillRect(hotX-weigth/2, size.height()/2+(weigth+1)/2, weigth, size.height()/2-weigth/2, colorPen);

	painter.fillRect(weigth%2+1, hotY-weigth/2+1, size.width()/2-(weigth+1)/2-2, weigth-2, colorBrush);
	painter.fillRect(size.width()/2+(weigth+1)/2+1, hotY-weigth/2+1, size.width()/2-(weigth+1)/2-2, weigth-2, colorBrush);
	painter.fillRect(hotX-weigth/2+1, weigth%2+1, weigth-2, size.height()/2-(weigth+1)/2-2, colorBrush);
	painter.fillRect(hotX-weigth/2+1, size.height()/2+(weigth+1)/2+1, weigth-2, size.height()/2-(weigth+1)/2-2, colorBrush);
	painter.end();

	return *this = Cursor(img, QPoint(hotX, hotY));
}

Cursor& Cursor::toX(QColor color, QSize size, unsigned int weigth)
{
	QImage img(size, QImage::Format_ARGB32);
	img.fill(QColor(0,0,0,0).rgba());
	int hotX = size.width() / 2;
	int hotY = size.height() / 2;

	QPainter painter(&img);
	painter.fillRect(weigth%2, hotY-weigth/2, size.width()/2-(weigth+1)/2, weigth, color);
	painter.fillRect(size.width()/2+(weigth+1)/2, hotY-weigth/2, size.width()/2-weigth/2, weigth, color);
	painter.fillRect(hotX-weigth/2, weigth%2, weigth, size.height()/2-(weigth+1)/2, color);
	painter.fillRect(hotX-weigth/2, size.height()/2+(weigth+1)/2, weigth, size.height()/2-weigth/2, color);
	painter.end();

	QTransform transf;
	img = img.transformed(transf.translate(-hotX,-hotY).rotate(45).translate(hotX,hotY),Qt::SmoothTransformation);

	return *this = Cursor(img, QPoint(hotX, hotY));
}

Cursor& Cursor::toX(QColor colorBrush, QColor colorPen, QSize size, unsigned int weigth)
{
	QImage img(size, QImage::Format_ARGB32);
	img.fill(QColor(0,0,0,0).rgba());
	int hotX = size.width() / 2;
	int hotY = size.height() / 2;

	QPainter painter(&img);
	painter.fillRect(weigth%2, hotY-weigth/2, size.width()/2-(weigth+1)/2, weigth, colorPen);
	painter.fillRect(size.width()/2+(weigth+1)/2, hotY-weigth/2, size.width()/2-weigth/2, weigth, colorPen);
	painter.fillRect(hotX-weigth/2, weigth%2, weigth, size.height()/2-(weigth+1)/2, colorPen);
	painter.fillRect(hotX-weigth/2, size.height()/2+(weigth+1)/2, weigth, size.height()/2-weigth/2, colorPen);

	painter.fillRect(weigth%2+1, hotY-weigth/2+1, size.width()/2-(weigth+1)/2-2, weigth-2, colorBrush);
	painter.fillRect(size.width()/2+(weigth+1)/2+1, hotY-weigth/2+1, size.width()/2-(weigth+1)/2-2, weigth-2, colorBrush);
	painter.fillRect(hotX-weigth/2+1, weigth%2+1, weigth-2, size.height()/2-(weigth+1)/2-2, colorBrush);
	painter.fillRect(hotX-weigth/2+1, size.height()/2+(weigth+1)/2+1, weigth-2, size.height()/2-(weigth+1)/2-2, colorBrush);
	painter.end();

	QTransform transf;
	img = img.transformed(transf.translate(-hotX,-hotY).rotate(45).translate(hotX,hotY),Qt::SmoothTransformation);

	return *this = Cursor(img, QPoint(hotX, hotY));
}

Cursor::Cursor(DefaultCursor defaultCursorType)
{
	switch (defaultCursorType)
	{
	case NoCursor:
                *this = Cursor(*this).noCursor();
		break;
        /*case RedCursor:
                *this = Cursor().toCross();
		break;
	case GreenCursor:
		*this = QImage(":/cursors/GreenCursor");
		break;
	case BlueCursor:
		*this = QImage(":/cursors/BlueCursor");
		break;
	case CyanCursor:
		*this = QImage(":/cursors/CyanCursor");
		break;
	case MagentaCursor:
		*this = QImage(":/cursors/MagentaCursor");
		break;
	case YellowCursor:
		*this = QImage(":/cursors/YellowCursor");
		break;
	case DarkRedCursor:
		*this = QImage(":/cursors/DarkRedCursor");
		break;
	case DarkGreenCursor:
		*this = QImage(":/cursors/DarkGreenCursor");
		break;
	case DarkBlueCursor:
		*this = QImage(":/cursors/DarkBlueCursor");
		break;
	case DarkCyanCursor:
		*this = QImage(":/cursors/DarkCyanCursor");
		break;
	case DarkMagentaCursor:
		*this = QImage(":/cursors/DarkMagentaCursor");
		break;
	case DarkYellowCursor:
		*this = QImage(":/cursors/DarkYellowCursor");
		break;
	case BlackCursor:
		*this = QImage(":/cursors/BlackCursor");
		break;
	case WhiteCursor:
		*this = QImage(":/cursors/WhiteCursor");
		break;
	case GrayCursor:
		*this = QImage(":/cursors/GrayCursor");
		break;
	case BlackAndWhiteCursor:
		*this = QImage(":/cursors/BlackAndWhiteCursor");
		break;
                */
                default:
                *this = Cursor(*this).toCross(QColor(255,255,255));
                break;
	}
}

QCursor Cursor::toQCursor()
{
	return QCursor(pixmap(), hotSpot().x(), hotSpot().y());
}

QImage Cursor::toQImage()
{
	return pixmap().toImage();
}





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
	_over = NULL;
	_detail = NULL;
	setMinimumSize(150,150);

	setAttribute(Qt::WA_Hover, true);
	installEventFilter(this);

	Cursor cursor(BlackAndWhiteCursor);
	setCursor(cursor.toQCursor());
}

SingleDisplay::~SingleDisplay()
{
}
void SingleDisplay::updateMousePosition()
{
	_mouseLastPos = mapFromGlobal(QCursor::pos());
}

QPointF SingleDisplay::getLastMousePosition()
{
	QPointF diffTocenter(_mouseLastPos.x() - size().width() / 2, _mouseLastPos.y() - size().height() / 2);
	double scale;
	scale = _currentScene->getScale();
	return _currentScene->getViewpoint() + diffTocenter / scale;
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

void SingleDisplay::setCurrentScene(AbstractScene *newScene)
{
	_currentScene = newScene;
	updateAll();
}

AbstractScene* SingleDisplay::getCurrentScene()
{
	return _currentScene;
}

SingleDisplay* SingleDisplay::getOverDisplay()
{
	SingleDisplay* result = new SingleDisplay(this);
	result->setOverviewMode(this);
	_over = result;
	return result;
}

SingleDisplay* SingleDisplay::getDetailDisplay()
{
	SingleDisplay* result = new SingleDisplay(this);
	result->setDetailMode(this);
	_detail = result;
	return result;
}

void SingleDisplay::fitView()
{
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

void SingleDisplay::setDetailZoom(double zoomFactor)
{
	if (_detail)
		_detail->zoom(zoomFactor);
	updateDetail();
}

void SingleDisplay::setCloneScale(bool status)
{
	_cloneScale = status;
	update();
}

void SingleDisplay::updateAll()
{
	QWidget::update();
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
}

void SingleDisplay::setDetailMode(SingleDisplay *display)
{
	_displayMode = MostDetailedScreen;
	_over = display;
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

void SingleDisplay::paintEvent(QPaintEvent *e)
{
	if (_displayMode == IntermediatedScreen)
	{
		QPainter painter(this);
		painter.fillRect(rect(),QBrush(QImage(":/cursors/DarkBG")));
		if (_currentScene->isValid())
		{
			QRect target = rect();
			painter.drawImage(0, 0,_currentScene->getFrame(target.size()));
			if (_detail)
				_detail->update();
			if (_over)
				_over->update();
		}
		painter.end();
	}
	else if (_displayMode == TopViewScreen)
	{
		QPainter painter(this);
		painter.fillRect(rect(),QBrush(QImage(":/cursors/DarkBG")));
		AbstractScene* currentScene;
		if (_detail && (currentScene = _detail->getCurrentScene()) && currentScene->isValid())
		{
			QRect target = rect();
			QSize targetSize = target.size();

			QImage thumb = currentScene->getThumb(targetSize,&(_detail->rect()));
			painter.drawImage((targetSize.width()-thumb.width())/2, (targetSize.height()-thumb.height())/2, thumb);
		}
		painter.end();
	}
	else if (_displayMode == MostDetailedScreen)
	{
		QPainter painter(this);
		painter.fillRect(rect(),QBrush(QImage(":/cursors/DarkBG")));
		AbstractScene* currentScene;
		if (_over && (currentScene = _over->getCurrentScene()) && currentScene->isValid())
		{
			QRect target = rect();
			QSize targetSize = target.size();
			double zoom = ceil(currentScene->getScale()*4.0);

			QImage detail = currentScene->getDetail(targetSize, _over->getLastMousePosition(), zoom);
			painter.drawImage(0, 0, detail);
		}
		painter.end();
	}
	if (!_currentScene || !_currentScene->isValid())
		return;
	for (int i = 0; i < _tool.size(); i++)
	{
		_tool.at(i)->paintEvent(*e);
	}
}

void SingleDisplay::resizeEvent(QResizeEvent *e)
{
	_currentScene->setViewport(e->size());
	updateAll();
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
