#include "StereoDisplay.h"

#include <QTime>
#include <QCursor>
#include <QBitmap>
#include <QPainter>
#include <QWheelEvent>

#define NOCURSOR QPixmap::fromImage(SymbolsResource::getBackGround(QColor(0,0,0,0)))


namespace br {
namespace uerj {
namespace eng {
namespace efoto {

// GL Display class
GLDisplay::GLDisplay(StereoDisplay *parent):
	QGLWidget(parent)
{
	stereoDisplay_ = parent;
	ltexture = 0;
	rtexture = 0;
	ctexture = 0;
	btexture = 0;
	_GLDisplayUpdate = false;
	setAutoFillBackground(false);

	setAttribute(Qt::WA_Hover, true);
	installEventFilter(this);

	QCursor cursor(NOCURSOR);
	setCursor(cursor);
}

GLDisplay::~GLDisplay()
{
	if(glIsTexture((GLuint)ltexture))
		glDeleteTextures(1, (GLuint*)(&ltexture));
	if(glIsTexture((GLuint)rtexture))
		glDeleteTextures(1, (GLuint*)(&rtexture));
}

QPointF GLDisplay::getMouseScreenPosition()
{
	return _mouseScreenPos;
}

void GLDisplay::updateMousePosition()
{
	_mouseScreenPos = mapFromGlobal(QCursor::pos());
}

void GLDisplay::setGLCursor(QImage cursor)
{
	if (ctexture != NULL && glIsTexture((GLuint)ctexture))
		glDeleteTextures(1, (GLuint*)(&ctexture));
	_cursor = cursor;
	if (cursor.isNull())
	{
		ctexture = NULL;
		return;
	}
	cursor = QGLWidget::convertToGLFormat(cursor);
	glEnable(GL_TEXTURE_2D);
	glGenTextures( 1, (GLuint*)(&ctexture) );
	glBindTexture( GL_TEXTURE_2D, (GLuint)ctexture );
	glTexImage2D( GL_TEXTURE_2D, 0, 4, cursor.width(), cursor.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, cursor.bits());
	glDisable(GL_TEXTURE_2D);
}

QImage GLDisplay::getGLCursor()
{
	if (ctexture != NULL)
	{
		return _cursor;
	}
	else
		return QImage();
}

void GLDisplay::setGLBackground(QImage bg)
{
	if (btexture != NULL && glIsTexture((GLuint)btexture))
		glDeleteTextures(1, (GLuint*)(&btexture));
	bg = QGLWidget::convertToGLFormat(bg);
	glEnable(GL_TEXTURE_2D);
	glGenTextures( 1, (GLuint*)(&btexture) );
	glBindTexture( GL_TEXTURE_2D, (GLuint)btexture );
	glTexImage2D( GL_TEXTURE_2D, 0, 4, bg.width(), bg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bg.bits());
	glDisable(GL_TEXTURE_2D);
}

void GLDisplay::setActivatedTool(StereoTool *tool, bool active)
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

void GLDisplay::initializeGL()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_COLOR_MATERIAL);

	glClearColor(0, 0, 0, 0);

	//setGLCursor(SymbolsResource::getBackGround(QColor(0,0,0,0));
	//setGLBackground(QImage(":/cursors/BlackBG"));
	_GLDisplayUpdate = true;
}

void GLDisplay::paintGL()
{
	if (_GLDisplayUpdate)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawBuffer(GL_BACK);

		if (btexture)
		{
			glEnable(GL_TEXTURE_2D);

			glBindTexture( GL_TEXTURE_2D, (GLuint)btexture );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

			glBegin (GL_QUADS);
			{
				glTexCoord2f(0.0, 1.0);
				glVertex2f(0, height());

				glTexCoord2f(1.0, 1.0);
				glVertex2f(width(), height());

				glTexCoord2f(1.0, 0.0);
				glVertex2f(width(), 0);

				glTexCoord2f(0.0, 0.0);
				glVertex2f(0, 0);
			}
			glEnd ();

			glDisable(GL_TEXTURE_2D);
		}
	}

	if (stereoDisplay_->getCurrentScene() == NULL)
		return;

	// Ainda gambiarra pura, mas em breve não será mais
	QRect target = rect();

	if (stereoDisplay_->getCurrentScene()->getLeftScene() && _GLDisplayUpdate)
		ltext = QGLWidget::convertToGLFormat(stereoDisplay_->getCurrentScene()->getLeftScene()->getFrame(target.size()));
	if (stereoDisplay_->getCurrentScene()->getRightScene() && _GLDisplayUpdate)
		rtext = QGLWidget::convertToGLFormat(stereoDisplay_->getCurrentScene()->getRightScene()->getFrame(target.size()));

	double ll, lr, lt, lb, rl, rr, rt, rb;
	if (ltext.width() < width())
	{
		ll = 0; lr = ltext.width();
	}
	else { ll = 0; lr = width();}
	if (ltext.height() < height())
	{
		lt = 0; lb = ltext.height();
	}
	else { lt = 0; lb = height();}
	if (rtext.width() < width())
	{
		rl = 0; rr = rtext.width();
	}
	else { rl = 0; rr = width();}
	if (rtext.height() < height())
	{
		rt = 0; rb = rtext.height();
	}
	else { rt = 0; rb = height();}

	double cl, cr, ct, cb;
	QPointF mousePos = stereoDisplay_->getMouseScreenPosition();
	cl = mousePos.x() - 16.0;
	cr = mousePos.x() + 16.0;
	cb = mousePos.y() - 16.0;
	ct = mousePos.y() + 16.0;

	glEnable(GL_COLOR_LOGIC_OP);
	glLogicOp(GL_OR);

	//glClear(GL_COLOR_BUFFER_BIT);
	//glDrawBuffer(GL_BACK);

	glEnable(GL_TEXTURE_2D);
	if (_GLDisplayUpdate)
	{
		if(glIsTexture((GLuint)ltexture))
			glDeleteTextures(1, (GLuint*)(&ltexture));
		glGenTextures( 1, (GLuint*)(&ltexture) );
	}
	glBindTexture( GL_TEXTURE_2D, (GLuint)ltexture );
	if (_GLDisplayUpdate)
		glTexImage2D( GL_TEXTURE_2D, 0, 3, ltext.width(), ltext.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, ltext.bits() );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
	glBegin (GL_QUADS);
	{
		glTexCoord2f(0.0, 1.0);
		glVertex2f(ll, lt);

		glTexCoord2f(1.0, 1.0);
		glVertex2f(lr, lt);

		glTexCoord2f(1.0, 0.0);
		glVertex2f(lr, lb);

		glTexCoord2f(0.0, 0.0);
		glVertex2f(ll, lb);
	}
	glEnd ();

	glBindTexture( GL_TEXTURE_2D, (GLuint)ctexture );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBegin (GL_QUADS);
	{
		glTexCoord2f(0.0, 1.0);
		glVertex2f(cl+stereoDisplay_->getLeftCursorOffset().x(), ct+stereoDisplay_->getLeftCursorOffset().y());

		glTexCoord2f(1.0, 1.0);
		glVertex2f(cr+stereoDisplay_->getLeftCursorOffset().x(), ct+stereoDisplay_->getLeftCursorOffset().y());

		glTexCoord2f(1.0, 0.0);
		glVertex2f(cr+stereoDisplay_->getLeftCursorOffset().x(), cb+stereoDisplay_->getLeftCursorOffset().y());

		glTexCoord2f(0.0, 0.0);
		glVertex2f(cl + stereoDisplay_->getLeftCursorOffset().x(), cb+stereoDisplay_->getLeftCursorOffset().y());
	}
	glEnd ();
	//glDisable(GL_TEXTURE_2D);

	//glEnable(GL_TEXTURE_2D);
	if (_GLDisplayUpdate)
	{
		if(glIsTexture((GLuint)rtexture))
			glDeleteTextures(1, (GLuint*)(&rtexture));
		glGenTextures( 1, (GLuint*)(&rtexture) );
	}
	glBindTexture( GL_TEXTURE_2D, (GLuint)rtexture );
	if (_GLDisplayUpdate)
		glTexImage2D( GL_TEXTURE_2D, 0, 3, rtext.width(), rtext.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, rtext.bits() );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
	glBegin (GL_QUADS);
	{
		glTexCoord2f(0.0, 1.0);
		glVertex2f(rl, rt);

		glTexCoord2f(1.0, 1.0);
		glVertex2f(rr, rt);

		glTexCoord2f(1.0, 0.0);
		glVertex2f(rr, rb);

		glTexCoord2f(0.0, 0.0);
		glVertex2f(rl, rb);
	}
	glEnd ();

	glBindTexture( GL_TEXTURE_2D, (GLuint)ctexture );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBegin (GL_QUADS);
	{
		glTexCoord2f(0.0, 1.0);
		glVertex2f(cl+stereoDisplay_->getRightCursorOffset().x(), ct+stereoDisplay_->getRightCursorOffset().y());

		glTexCoord2f(1.0, 1.0);
		glVertex2f(cr+stereoDisplay_->getRightCursorOffset().x(), ct+stereoDisplay_->getRightCursorOffset().y());

		glTexCoord2f(1.0, 0.0);
		glVertex2f(cr+stereoDisplay_->getRightCursorOffset().x(), cb+stereoDisplay_->getRightCursorOffset().y());

		glTexCoord2f(0.0, 0.0);
		glVertex2f(cl+stereoDisplay_->getRightCursorOffset().x(), cb+stereoDisplay_->getRightCursorOffset().y());
	}
	glEnd ();
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_COLOR_LOGIC_OP);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	_GLDisplayUpdate = false;
	//swapBuffers();
}

void GLDisplay::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	_GLDisplayUpdate = true;
	stereoDisplay_->getCurrentScene()->setViewport(QSize(w, h));
}

void GLDisplay::paintEvent(QPaintEvent *e)
{
	QGLWidget::paintEvent(e);
	if (!stereoDisplay_ || !stereoDisplay_->getCurrentScene() || !stereoDisplay_->getCurrentScene()->isValid())
		return;
	for (int i = 0; i < _tool.size(); i++)
	{
		_tool.at(i)->paintEvent(*e);
	}
}

void GLDisplay::resizeEvent(QResizeEvent *e)
{
	if (!stereoDisplay_ || !stereoDisplay_->getCurrentScene() || !stereoDisplay_->getCurrentScene()->isValid())
		return;
	for (int i = 0; i < _tool.size(); i++)
	{
		_tool.at(i)->resizeEvent(*e);
	}
	QGLWidget::resizeEvent(e);
}

bool GLDisplay::eventFilter(QObject *o, QEvent *e)
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

void GLDisplay::enterEvent(QHoverEvent *e)
{
	if (!stereoDisplay_->getCurrentScene() || !stereoDisplay_->getCurrentScene()->isValid())
		return;
	for (int i = 0; i < _tool.size(); i++)
	{
		_tool.at(i)->enterEvent(*e);
	}
}

void GLDisplay::leaveEvent(QHoverEvent *e)
{
	if (!stereoDisplay_->getCurrentScene() || !stereoDisplay_->getCurrentScene()->isValid())
		return;
	for (int i = 0; i < _tool.size(); i++)
	{
		_tool.at(i)->leaveEvent(*e);
	}
}

void GLDisplay::moveEvent(QHoverEvent *e)
{
	if (!stereoDisplay_->getCurrentScene() || !stereoDisplay_->getCurrentScene()->isValid())
		return;
	for (int i = 0; i < _tool.size(); i++)
	{
		_tool.at(i)->moveEvent(*e);
	}
}

void GLDisplay::mousePressEvent(QMouseEvent *e)
{
	if (!stereoDisplay_->getCurrentScene() || !stereoDisplay_->getCurrentScene()->isValid())
		return;
	for (int i = 0; i < _tool.size(); i++)
	{
		_tool.at(i)->mousePressed(*e);
	}
}

void GLDisplay::mouseReleaseEvent(QMouseEvent *e)
{
	if (!stereoDisplay_->getCurrentScene() || !stereoDisplay_->getCurrentScene()->isValid())
		return;
	for (int i = 0; i < _tool.size(); i++)
	{
		_tool.at(i)->mouseReleased(*e);
	}
}

void GLDisplay::mouseMoveEvent(QMouseEvent *e)
{
	if (!stereoDisplay_->getCurrentScene() || !stereoDisplay_->getCurrentScene()->isValid())
		return;
	for (int i = 0; i < _tool.size(); i++)
	{
		_tool.at(i)->mouseMoved(*e);
	}
}

void GLDisplay::mouseDoubleClickEvent(QMouseEvent *e)
{
	if (!stereoDisplay_->getCurrentScene() || !stereoDisplay_->getCurrentScene()->isValid())
		return;
	for (int i = 0; i < _tool.size(); i++)
	{
		_tool.at(i)->mouseDblClicked(*e);
	}
}

void GLDisplay::wheelEvent(QWheelEvent *e)
{
	if (!stereoDisplay_->getCurrentScene() || !stereoDisplay_->getCurrentScene()->isValid())
		return;
	for (int i = 0; i < _tool.size(); i++)
	{
		_tool.at(i)->wheelEvent(*e);
	}
}



// Stereo Display class
StereoDisplay::StereoDisplay(QWidget *parent, StereoScene *currentScene):
	QWidget(parent)
{
	//QGLFormat fmt;
	//fmt.setAlpha(true);
	//fmt.setStereo(true);
	//fmt.setDoubleBuffer(true);
	//fmt.setOverlay(true);
	//QGLFormat::setDefaultFormat(fmt);

	leftDisplay_ = NULL;
	rightDisplay_ = NULL;

	if (currentScene)
		currentScene_ = currentScene;
	else
		currentScene_ = new StereoScene(this, "", "");

	_currentZ = 0;
	leftCursorOffset_ = QPointF(-4,0);
	rightCursorOffset_ = QPointF(4,0);
	glDisplay_ = new GLDisplay(this);
	QGridLayout* layout = new QGridLayout(this);
	layout->setContentsMargins(0,0,0,0);
	layout->addWidget(glDisplay_);
	setLayout(layout);
	setMinimumSize(400,300);
}

StereoDisplay::~StereoDisplay()
{
}

StereoScene* StereoDisplay::getCurrentScene()
{
	return currentScene_;
}

SingleDisplay* StereoDisplay::getLeftDisplay()
{
	if (leftDisplay_ == NULL)
	{
		SingleDisplay* result = new SingleDisplay(0, currentScene_ ? currentScene_->getLeftScene() : NULL);
		leftDisplay_ = result;
	}
	return leftDisplay_;
}

SingleDisplay* StereoDisplay::getRightDisplay()
{
	if (rightDisplay_ == NULL)
	{
		SingleDisplay* result = new SingleDisplay(0, currentScene_ ? currentScene_->getRightScene(): NULL);
		rightDisplay_ = result;
	}
	return rightDisplay_;
}

SingleDisplay* StereoDisplay::getLeftNearDisplay()
{
	if (leftDisplay_ == NULL)
		getLeftDisplay();
	return leftDisplay_->getDetailDisplay();
}

SingleDisplay* StereoDisplay::getRightNearDisplay()
{
	if (rightDisplay_ == NULL)
		getRightDisplay();
	return rightDisplay_->getDetailDisplay();
}

SingleDisplay* StereoDisplay::getLeftOverDisplay()
{
	if (leftDisplay_ == NULL)
		getLeftDisplay();
	return leftDisplay_->getOverDisplay();
}

SingleDisplay* StereoDisplay::getRightOverDisplay()
{
	if (rightDisplay_ == NULL)
		getRightDisplay();
	return rightDisplay_->getOverDisplay();
}

GLDisplay* StereoDisplay::getRealDisplay()
{
	return glDisplay_;
}



void StereoDisplay::loadLeftImage(QString filename)
{
	currentScene_->getLeftScene()->loadImage(filename);
}

void StereoDisplay::loadRightImage(QString filename)
{
	currentScene_->getRightScene()->loadImage(filename);
}

void StereoDisplay::loadLeftImage(QImage *image)
{
	currentScene_->getLeftScene()->loadImage(*image);
}

void StereoDisplay::loadRightImage(QImage *image)
{
	currentScene_->getRightScene()->loadImage(*image);
}

void StereoDisplay::loadLeftImage(Matrix *image, bool isGrayscale)
{
	currentScene_->getLeftScene()->loadImage(image,isGrayscale);
}

void StereoDisplay::loadRightImage(Matrix *image, bool isGrayscale)
{
	currentScene_->getRightScene()->loadImage(image,isGrayscale);
}



QPointF StereoDisplay::getLeftCursorOffset()
{
	return leftCursorOffset_;
}

QPointF StereoDisplay::getRightCursorOffset()
{
	return rightCursorOffset_;
}

void StereoDisplay::setLeftCursorOffset(QPointF offset)
{
	leftCursorOffset_ = offset;
}

void StereoDisplay::setRightCursorOffset(QPointF offset)
{
	rightCursorOffset_ = offset;
}

void StereoDisplay::setCursor(QImage newCursor)
{
	glDisplay_->setGLCursor(newCursor);
}

QImage StereoDisplay::getCursor()
{
	return glDisplay_->getGLCursor();
}

double StereoDisplay::getCurrentZ()
{
	return _currentZ;
}

void StereoDisplay::setCurrentZ(double z)
{
	_currentZ = z;
}

QPointF StereoDisplay::screenPosition(QPointF position, bool leftChannel)
{
	//REFAZER
	return QPointF(0,0);
}

QPointF StereoDisplay::getMouseScreenPosition()
{
	return glDisplay_->getMouseScreenPosition();
}

void StereoDisplay::updateMousePosition()
{
	glDisplay_->updateMousePosition();
}

QPointF StereoDisplay::getPositionLeft(QPoint screenPosition)
{
	QPointF diffTocenter(screenPosition.x() - size().width() / 2, screenPosition.y() - size().height() / 2);
	double scale;
	scale = currentScene_->getLeftScene()->getScale();
	return currentScene_->getLeftScene()->getViewpoint() + diffTocenter / scale;
}

QPointF StereoDisplay::getPositionRight(QPoint screenPosition)
{
	QPointF diffTocenter(screenPosition.x() - size().width() / 2, screenPosition.y() - size().height() / 2);
	double scale;
	scale = currentScene_->getRightScene()->getScale();
	return currentScene_->getRightScene()->getViewpoint() + diffTocenter / scale;
}

void StereoDisplay::fitView()
{
	double wscale = glDisplay_->width() / (double)currentScene_->getWidth();
	double hscale = glDisplay_->height() / (double)currentScene_->getHeight();
	currentScene_->getLeftScene()->scaleTo(wscale < hscale ? wscale : hscale);
	currentScene_->getLeftScene()->centerContent();
	currentScene_->getLeftScene()->pan(-currentScene_->getChannelsOffset()/2.0);
	currentScene_->getRightScene()->scaleTo(wscale < hscale ? wscale : hscale);
	currentScene_->getRightScene()->centerContent();
	currentScene_->getRightScene()->pan(currentScene_->getChannelsOffset()/2.0);
	updateAll();
}

void StereoDisplay::pan(int dx, int dy)
{
}

void StereoDisplay::zoom(double zoomFactor, QPoint* atPoint)
{
	currentScene_->getLeftScene()->zoom(zoomFactor);
	currentScene_->getRightScene()->zoom(zoomFactor);
}

void StereoDisplay::updateAll()
{
	glDisplay_->_GLDisplayUpdate = true;
	glDisplay_->update();
	if (leftDisplay_)
		leftDisplay_->updateAll();
	if (rightDisplay_)
		rightDisplay_->updateAll();
}

void StereoDisplay::updateAll(QPointF* left, QPointF* right, bool emitClicked)
{
	glDisplay_->_GLDisplayUpdate = true;
	glDisplay_->update();
	if (leftDisplay_)
	{
		leftDisplay_->updateDetail(left,emitClicked);
		leftDisplay_->update();
	}
	if (rightDisplay_)
	{
		rightDisplay_->updateDetail(right,emitClicked);
		rightDisplay_->update();
	}
}

void StereoDisplay::updateDetail(QPointF* left, QPointF* right, bool emitClicked)
{
	if (leftDisplay_)
		leftDisplay_->updateDetail(left);
	if (rightDisplay_)
		rightDisplay_->updateDetail(right);
	if (emitClicked)
		emit mouseClicked(left, right);
	else
		emit mousePositionsChanged(left, right);
}

void StereoDisplay::setActivatedTool(StereoTool *tool, bool active)
{
	glDisplay_->setActivatedTool(tool, active);
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
