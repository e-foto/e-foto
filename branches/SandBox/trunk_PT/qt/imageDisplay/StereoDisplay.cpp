#include "StereoDisplay.h"

#include <QTime>
#include <QCursor>
#include <QBitmap>
#include <QPainter>
#include <QWheelEvent>
#include "math.h"



bool GLDisplayUpdate = false;

// Single Display class
MonoDisplay::MonoDisplay(QWidget *parent, MonoView *currentView):
	AbstractDisplay(parent)
{
	currentView_ = currentView;
	parentDisplay_ = 0;
	onMove_ = false;
	cloneScale_ = true;
	setMinimumSize(200,150);
	setMouseTracking(true);

	QPixmap pixmap("../glcursor.png");
	QCursor cursor(pixmap);
	setCursor(cursor);
}
MonoDisplay::MonoDisplay(StereoDisplay *parent, MonoView *currentView):
	AbstractDisplay(parent)
{
	currentView_ = currentView;
	parentDisplay_ = parent;
	onMove_ = false;
	cloneScale_ = true;
	setMinimumSize(200,150);
	setMouseTracking(true);

	QPixmap pixmap("../glcursor.png");
	QCursor cursor(pixmap);
	setCursor(cursor);
}
MonoDisplay::~MonoDisplay()
{
}
QPointF MonoDisplay::getLastMousePosition()
{
	QPointF diffTocenter(moveLastPos_.x() - size().width() / 2, moveLastPos_.y() - size().height() / 2);
	double scale;
	if (parentDisplay_ && !cloneScale_)
	{
		double modscale = width()/(double)parentDisplay_->width() < height()/(double)parentDisplay_->height() ? width()/(double)parentDisplay_->width() : height()/(double)parentDisplay_->height();
		scale = currentView_->getScale() * modscale;
	}
	else
		scale = currentView_->getScale();
	return currentView_->getViewpoint() + diffTocenter / scale;
}
MonoView* MonoDisplay::getCurrentView()
{
	return currentView_;
}
OverviewDisplay* MonoDisplay::getOverviewDisplay()
{
	OverviewDisplay* result = new OverviewDisplay(this);
	overview_ = result;
	return result;
}
DetailDisplay* MonoDisplay::getDetailDisplay()
{
	DetailDisplay* result = new DetailDisplay(this);
	detail_ = result;
	return result;
}
void MonoDisplay::fitView()
{
}
void MonoDisplay::pan(int dx, int dy)
{
}
void MonoDisplay::zoom(double zoomFactor, QPoint* atPoint)
{
}
void MonoDisplay::setDetailZoom(double zoomFactor)
{
	if (detail_)
		detail_->zoom(zoomFactor);
	updateDetail();
}
void MonoDisplay::setCloneScale(bool status)
{
	cloneScale_ = status;
	update();
}
void MonoDisplay::updateAll()
{
	if (parentDisplay_)
		parentDisplay_->updateAll();
	else
		QWidget::update();
}
void MonoDisplay::updateDetail()
{
	if (detail_)
		detail_->update();
	QPointF lastPos = getLastMousePosition();
	emit mousePositionChanged(&lastPos);
}
void MonoDisplay::updateDetail(QPointF* mousePosition, bool emitClicked)
{
	if (!currentView_ || !mousePosition)
		return;
	double scale;
	if (parentDisplay_ && !cloneScale_)
	{
		double modscale = width()/(double)parentDisplay_->width() < height()/(double)parentDisplay_->height() ? width()/(double)parentDisplay_->width() : height()/(double)parentDisplay_->height();
		scale = currentView_->getScale() * modscale;
	}
	else
		scale = currentView_->getScale();
	moveLastPos_.setX(((*mousePosition - currentView_->getViewpoint()) * scale).x() + size().width() / 2);
	moveLastPos_.setY(((*mousePosition - currentView_->getViewpoint()) * scale).y() + size().height() / 2);
	if (detail_)
		detail_->update();
	QPointF lastPos = getLastMousePosition();
	if (emitClicked)
		emit mouseClicked(&lastPos);
}
void MonoDisplay::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	if (currentView_)
	{
		QRect target = rect();
		if (parentDisplay_ && !cloneScale_)
		{
			double modscale = width()/(double)parentDisplay_->width() < height()/(double)parentDisplay_->height() ? width()/(double)parentDisplay_->width() : height()/(double)parentDisplay_->height();
			painter.drawImage(0, 0,currentView_->getFrame(target.size(), currentView_->getScale() * modscale));
		}
		else
			painter.drawImage(0, 0,currentView_->getFrame(target.size()));
		if (detail_)
			detail_->update();
		if (overview_)
			overview_->update();
	}
}
void MonoDisplay::mousePressEvent(QMouseEvent *e)
{
	if (!currentView_ || !currentView_->imageLoaded())
		return;

	moveLastPos_ = e->posF();
	if (e->button() == Qt::RightButton)//:MidButton)
	{
		onMove_ = true;
	}
	else if (e->button() == Qt::LeftButton)
	{
		moveLastPos_ = e->posF();
		QPointF mousePos = getLastMousePosition();
		updateDetail(&mousePos,true);
	}
	QWidget::mousePressEvent(e);
}
void MonoDisplay::mouseReleaseEvent(QMouseEvent *e)
{
	onMove_ = false;
	QWidget::mouseReleaseEvent(e);
}
void MonoDisplay::mouseMoveEvent(QMouseEvent *e)
{
	if (!currentView_ || !currentView_->imageLoaded())
		return;

	QPointF diff = e->posF() - moveLastPos_;
	moveLastPos_ = e->posF();
	if (onMove_ && (e->buttons() & Qt::RightButton))//Qt::MidButton))
	{
		double scale;
		if (parentDisplay_ && !cloneScale_)
		{
			double modscale = width()/(double)parentDisplay_->width() < height()/(double)parentDisplay_->height() ? width()/(double)parentDisplay_->width() : height()/(double)parentDisplay_->height();
			scale = currentView_->getScale() * modscale;
		}
		else
			scale = currentView_->getScale();
		currentView_->pan(-(diff/scale));
		updateAll();
	}
	else
	{
		updateDetail();
	}
	QWidget::mouseMoveEvent(e);
}
void MonoDisplay::wheelEvent(QWheelEvent *e)
{
	if (!currentView_ || !currentView_->imageLoaded())
		return;

	int numDegrees = e->delta() / 8.0;
	int numSteps = numDegrees / 15.0;
	if (e->orientation() == Qt::Vertical)
	{
		double zoomStep;
		if (numSteps>0)
			zoomStep = 1.044273782; // 1*2^(1÷(2^4))
		else if (numSteps<0)
			zoomStep = 0.957603281; // 1/2^(1÷(2^4))
		for (int i = 0; i<abs(numSteps);i++)
			currentView_->zoom(zoomStep);
		updateAll();
		updateDetail();
	}
	QWidget::wheelEvent(e);
}



// Detail Display class
DetailDisplay::DetailDisplay(MonoDisplay *parent):
	AbstractDisplay(parent),
	parentDisplay_(parent)
{
	setMinimumSize(100,75);
	zoom_ = 2.0;
}
DetailDisplay::~DetailDisplay()
{
}
void DetailDisplay::fitView()
{
}
void DetailDisplay::pan(int dx, int dy)
{
}
void DetailDisplay::zoom(double zoomFactor, QPoint* atPoint)
{
	zoom_ = zoomFactor;
}
void DetailDisplay::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	MonoView* currentView;
	if (parentDisplay_ && (currentView = parentDisplay_->getCurrentView()))
	{
		QRect target = rect();

		QSize targetSize = target.size();

		QImage detail = currentView->getDetail(targetSize,parentDisplay_->getLastMousePosition(),zoom_);
		painter.drawImage(0, 0, detail);
	}
	// Código do Marcelo para gerar ponteiros
	int cursor_x = rect().center().x();
	int cursor_y =  rect().center().y();
	int cur_width = 3;
	int cur_height = 10;
	int hcw = cur_width / 2;
	int ch = cur_height - hcw;
	QColor cursor_color = QColor(255,255,0);
	painter.fillRect(cursor_x-hcw, cursor_y-cur_height, cur_width-1, ch, cursor_color);
	painter.fillRect(cursor_x-hcw, cursor_y+hcw, cur_width-1, ch, cursor_color);
	painter.fillRect(cursor_x-cur_height, cursor_y-hcw, ch, cur_width-1, cursor_color);
	painter.fillRect(cursor_x+hcw, cursor_y-hcw, ch, cur_width-1, cursor_color);
}
void DetailDisplay::mousePressEvent(QMouseEvent *e)
{
}
void DetailDisplay::mouseReleaseEvent(QMouseEvent *e)
{
}
void DetailDisplay::mouseMoveEvent(QMouseEvent *e)
{
}
void DetailDisplay::wheelEvent(QWheelEvent *e)
{
}



// Overview Display class
OverviewDisplay::OverviewDisplay(MonoDisplay *parent):
	AbstractDisplay(parent),
	parentDisplay_(parent)
{
	setMinimumSize(100,75);
}
OverviewDisplay::~OverviewDisplay()
{
}
void OverviewDisplay::fitView()
{
}
void OverviewDisplay::pan(int dx, int dy)
{
}
void OverviewDisplay::zoom(double zoomFactor, QPoint* atPoint)
{
}
void OverviewDisplay::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	MonoView* currentView;
	if (parentDisplay_ && (currentView = parentDisplay_->getCurrentView()))
	{
		QRect target = rect();

		QSize targetSize = target.size();

		QImage thumb = currentView->getThumb(targetSize);
		painter.drawImage((targetSize.width()-thumb.width())/2, (targetSize.height()-thumb.height())/2, thumb);
	}
}
void OverviewDisplay::mousePressEvent(QMouseEvent *e)
{
}
void OverviewDisplay::mouseReleaseEvent(QMouseEvent *e)
{
}
void OverviewDisplay::mouseMoveEvent(QMouseEvent *e)
{
}
void OverviewDisplay::wheelEvent(QWheelEvent *e)
{
}



// GL Display class
GLDisplay::GLDisplay(StereoDisplay *parent):
	QGLWidget(parent)
{
	stereoDisplay_ = parent;
	ltexture = 0;
	rtexture = 0;
	setAutoFillBackground(false);
	setMouseTracking(true);

	QPixmap pixmap("../nocursor.png");
	QCursor cursor(pixmap);
	setCursor(cursor);
}
GLDisplay::~GLDisplay()
{
	if(glIsTexture((GLuint)ltexture))
		glDeleteTextures(1, (GLuint*)(&ltexture));
	if(glIsTexture((GLuint)rtexture))
		glDeleteTextures(1, (GLuint*)(&rtexture));
}
void GLDisplay::initializeGL()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_COLOR_MATERIAL);

	glClearColor(0, 0, 0, 0);

	QImage* cursor = new QImage("../glcursor.png");
	*cursor = QGLWidget::convertToGLFormat(*cursor);
	glEnable(GL_TEXTURE_2D);
	glGenTextures( 1, (GLuint*)(&ctexture) );
	glBindTexture( GL_TEXTURE_2D, (GLuint)ctexture );
	glTexImage2D( GL_TEXTURE_2D, 0, 4, cursor->width(), cursor->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, cursor->bits());
	glDisable(GL_TEXTURE_2D);
	GLDisplayUpdate = true;
}
void GLDisplay::paintGL()
{
	if (stereoDisplay_->getCurrentView() == NULL)
		return;

	// Ainda gambiarra pura, mas em breve não será mais
	QRect target = rect();

	if (stereoDisplay_->getCurrentView()->getLeftView() && GLDisplayUpdate)
		ltext = QGLWidget::convertToGLFormat(stereoDisplay_->getCurrentView()->getLeftView()->getFrame(target.size()));
	if (stereoDisplay_->getCurrentView()->getRightView() && GLDisplayUpdate)
		rtext = QGLWidget::convertToGLFormat(stereoDisplay_->getCurrentView()->getRightView()->getFrame(target.size()));

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
	cl = moveLastPos_.x() - 16.0;
	cr = moveLastPos_.x() + 16.0;
	cb = moveLastPos_.y() - 16.0;
	ct = moveLastPos_.y() + 16.0;

	glEnable(GL_COLOR_LOGIC_OP);
	glLogicOp(GL_OR);

	glClear(GL_COLOR_BUFFER_BIT);
	glDrawBuffer(GL_BACK);

	glEnable(GL_TEXTURE_2D);
	if (GLDisplayUpdate)
	{
		if(glIsTexture((GLuint)ltexture))
			glDeleteTextures(1, (GLuint*)(&ltexture));
		glGenTextures( 1, (GLuint*)(&ltexture) );
	}
	glBindTexture( GL_TEXTURE_2D, (GLuint)ltexture );
	if (GLDisplayUpdate)
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
	if (GLDisplayUpdate)
	{
		if(glIsTexture((GLuint)rtexture))
			glDeleteTextures(1, (GLuint*)(&rtexture));
		glGenTextures( 1, (GLuint*)(&rtexture) );
	}
	glBindTexture( GL_TEXTURE_2D, (GLuint)rtexture );
	if (GLDisplayUpdate)
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
	GLDisplayUpdate = false;
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
	GLDisplayUpdate = true;
}
void GLDisplay::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::RightButton)//:MidButton)
	{
		moveLastPos_ = e->posF();
		onMove_ = true;
	}
	else if (e->button() == Qt::LeftButton)
	{
		moveLastPos_ = e->posF();
		if (stereoDisplay_->getCurrentView())
		{
			QPointF diffTocenter(moveLastPos_.x() - size().width() / 2, moveLastPos_.y() - size().height() / 2);
			QPointF* leftDetail = NULL;
			QPointF* rightDetail = NULL;
			if (stereoDisplay_->getCurrentView()->getLeftView() && stereoDisplay_->getCurrentView()->getLeftView()->imageLoaded())
				leftDetail = new QPointF(stereoDisplay_->getCurrentView()->getLeftView()->getViewpoint() + (diffTocenter+stereoDisplay_->getLeftCursorOffset()) / stereoDisplay_->getCurrentView()->getLeftView()->getScale());
			if (stereoDisplay_->getCurrentView()->getRightView() && stereoDisplay_->getCurrentView()->getRightView()->imageLoaded())
				rightDetail = new QPointF(stereoDisplay_->getCurrentView()->getRightView()->getViewpoint() + (diffTocenter+stereoDisplay_->getRightCursorOffset()) / stereoDisplay_->getCurrentView()->getRightView()->getScale());
			stereoDisplay_->updateDetail(leftDetail, rightDetail, true);
			if (leftDetail)
				delete(leftDetail);
			if (rightDetail)
				delete(rightDetail);
		}
	}
	QWidget::mousePressEvent(e);
}
void GLDisplay::mouseReleaseEvent(QMouseEvent *e)
{
	onMove_ = false;
	QWidget::mouseReleaseEvent(e);
}
void GLDisplay::mouseMoveEvent(QMouseEvent *e)
{
	QPointF diff = e->posF() - moveLastPos_;
	moveLastPos_ = e->posF();
	if (stereoDisplay_->getCurrentView())
	{
		QPointF diffTocenter(moveLastPos_.x() -diff.x() - size().width() / 2, moveLastPos_.y() -diff.y() - size().height() / 2);
		QPointF* leftDetail = NULL;
		QPointF* rightDetail = NULL;
		if (stereoDisplay_->getCurrentView()->getLeftView() && stereoDisplay_->getCurrentView()->getLeftView()->imageLoaded())
			leftDetail = new QPointF(stereoDisplay_->getCurrentView()->getLeftView()->getViewpoint() + (diffTocenter+stereoDisplay_->getLeftCursorOffset()) / stereoDisplay_->getCurrentView()->getLeftView()->getScale());
		if (stereoDisplay_->getCurrentView()->getRightView() && stereoDisplay_->getCurrentView()->getRightView()->imageLoaded())
			rightDetail = new QPointF(stereoDisplay_->getCurrentView()->getRightView()->getViewpoint() + (diffTocenter+stereoDisplay_->getRightCursorOffset()) / stereoDisplay_->getCurrentView()->getRightView()->getScale());


		if (onMove_ && (e->buttons() & Qt::RightButton))//Qt::MidButton))
		{
			if (stereoDisplay_->getCurrentView()->getLeftView())
			{
				double lscale = stereoDisplay_->getCurrentView()->getLeftView()->getScale();
				stereoDisplay_->getCurrentView()->getLeftView()->pan(-(diff/lscale));
			}
			if (stereoDisplay_->getCurrentView()->getRightView())
			{
				double rscale = stereoDisplay_->getCurrentView()->getRightView()->getScale();
				stereoDisplay_->getCurrentView()->getRightView()->pan(-(diff/rscale));
			}
			stereoDisplay_->updateAll(leftDetail, rightDetail);
		}
		else if (!onMove_)
		{
			update();
			stereoDisplay_->updateDetail(leftDetail, rightDetail);
		}

		if (leftDetail)
			delete(leftDetail);
		if (rightDetail)
			delete(rightDetail);
	}
	QWidget::mouseMoveEvent(e);
}
void GLDisplay::wheelEvent(QWheelEvent *e)
{
	int numDegrees = e->delta() / 8.0;
	int numSteps = numDegrees / 15.0;
	if (stereoDisplay_->getCurrentView() && stereoDisplay_->getCurrentView()->getLeftView() && stereoDisplay_->getCurrentView()->getRightView())
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
				stereoDisplay_->getCurrentView()->getLeftView()->zoom(zoomStep);
				stereoDisplay_->getCurrentView()->getRightView()->zoom(zoomStep);
			}
			stereoDisplay_->updateAll();
		}
	}
	QWidget::wheelEvent(e);
}



// Stereo Display class
StereoDisplay::StereoDisplay(QWidget *parent, StereoView *currentView):
	AbstractDisplay(parent),
	currentView_(currentView),
	leftDisplay_(0),
	rightDisplay_(0)
{
	QGLFormat fmt;
	fmt.setAlpha(true);
	//fmt.setStereo(true);
	fmt.setDoubleBuffer(true);
	QGLFormat::setDefaultFormat(fmt);

	leftCursorOffset_ = QPointF(-4,0);
	rightCursorOffset_ = QPointF(4,0);
	glDisplay_ = new GLDisplay(this);
	QGridLayout* layout = new QGridLayout(this);
	layout->addWidget(glDisplay_);
	setLayout(layout);
	setMinimumSize(400,300);
}
StereoDisplay::~StereoDisplay()
{
}
StereoView* StereoDisplay::getCurrentView()
{
	return currentView_;
}
MonoDisplay* StereoDisplay::getLeftDisplay()
{
	MonoDisplay* result = new MonoDisplay(this, currentView_ ? currentView_->getLeftView() : NULL);
	leftDisplay_ = result;
	return result;
}
MonoDisplay* StereoDisplay::getRightDisplay()
{
	MonoDisplay* result = new MonoDisplay(this, currentView_ ? currentView_->getRightView(): NULL);
	rightDisplay_ = result;
	return result;
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
void StereoDisplay::fitView()
{
}
void StereoDisplay::pan(int dx, int dy)
{
}
void StereoDisplay::zoom(double zoomFactor, QPoint* atPoint)
{
}
void StereoDisplay::updateAll()
{
	GLDisplayUpdate = true;
	glDisplay_->update();
	if (leftDisplay_)
		leftDisplay_->update();
	if (rightDisplay_)
		rightDisplay_->update();
}
void StereoDisplay::updateAll(QPointF* left, QPointF* right, bool emitClicked)
{
	GLDisplayUpdate = true;
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
