#include "StereoDisplay.h"

#include <QTime>
#include <QPainter>
#include <QWheelEvent>
#include "math.h"

// Single Display class
MonoDisplay::MonoDisplay(QWidget *parent, MonoView *currentView):
	AbstractDisplay(parent)
{
	currentView_ = currentView;
	parentDisplay_ = 0;
	onMove_ = false;
	setMinimumSize(200,150);
	setMouseTracking(true);
}
MonoDisplay::MonoDisplay(StereoDisplay *parent, MonoView *currentView):
	AbstractDisplay(parent)
{
	currentView_ = currentView;
	parentDisplay_ = parent;
	onMove_ = false;
	setMinimumSize(200,150);
	setMouseTracking(true);
}
MonoDisplay::~MonoDisplay()
{
}
QPointF MonoDisplay::getLastMousePosition()
{
	QPointF diffTocenter(moveLastPos_.x() - size().width() / 2, moveLastPos_.y() - size().height() / 2);
	return currentView_->getViewpoint() + diffTocenter / currentView_->getScale();
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
	moveLastPos_.setX(((*mousePosition - currentView_->getViewpoint()) * currentView_->getScale()).x() + size().width() / 2);
	moveLastPos_.setY(((*mousePosition - currentView_->getViewpoint()) * currentView_->getScale()).y() + size().height() / 2);
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
		double scale = currentView_->getScale();
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
	}
	QWidget::wheelEvent(e);
}



// Detail Display class
DetailDisplay::DetailDisplay(MonoDisplay *parent):
	AbstractDisplay(parent),
	parentDisplay_(parent)
{
	setMinimumSize(100,75);
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
}
void DetailDisplay::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	MonoView* currentView;
	if (parentDisplay_ && (currentView = parentDisplay_->getCurrentView()))
	{
		QRect target = rect();

		QSize targetSize = target.size();

		QImage detail = currentView->getDetail(targetSize,parentDisplay_->getLastMousePosition(),8.0);
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
	setMouseTracking(true);
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
}
void GLDisplay::paintGL()
{
	if (stereoDisplay_->getCurrentView() == NULL)
		return;

	// Ainda gambiarra pura, mas em breve não será mais
	QRect target = rect();

	QImage ltext;
	QImage rtext;

	if (stereoDisplay_->getCurrentView()->getLeftView())
		ltext = QGLWidget::convertToGLFormat(stereoDisplay_->getCurrentView()->getLeftView()->getFrame(target.size()));
	if (stereoDisplay_->getCurrentView()->getRightView())
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

	glClear(GL_COLOR_BUFFER_BIT);
	glDrawBuffer(GL_BACK);
	glEnable(GL_TEXTURE_2D);
	if(glIsTexture((GLuint)ltexture))
		glDeleteTextures(1, (GLuint*)(&ltexture));
	glGenTextures( 1, (GLuint*)(&ltexture) );
	glBindTexture( GL_TEXTURE_2D, (GLuint)ltexture );
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
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_COLOR_LOGIC_OP);
	glLogicOp(GL_OR);

	glEnable(GL_TEXTURE_2D);
	if(glIsTexture((GLuint)rtexture))
		glDeleteTextures(1, (GLuint*)(&rtexture));
	glGenTextures( 1, (GLuint*)(&rtexture) );
	glBindTexture( GL_TEXTURE_2D, (GLuint)rtexture );
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
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_COLOR_LOGIC_OP);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
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
				leftDetail = new QPointF(stereoDisplay_->getCurrentView()->getLeftView()->getViewpoint() + diffTocenter / stereoDisplay_->getCurrentView()->getLeftView()->getScale());
			if (stereoDisplay_->getCurrentView()->getRightView() && stereoDisplay_->getCurrentView()->getRightView()->imageLoaded())
				rightDetail = new QPointF(stereoDisplay_->getCurrentView()->getRightView()->getViewpoint() + diffTocenter / stereoDisplay_->getCurrentView()->getRightView()->getScale());
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
			leftDetail = new QPointF(stereoDisplay_->getCurrentView()->getLeftView()->getViewpoint() + diffTocenter / stereoDisplay_->getCurrentView()->getLeftView()->getScale());
		if (stereoDisplay_->getCurrentView()->getRightView() && stereoDisplay_->getCurrentView()->getRightView()->imageLoaded())
			rightDetail = new QPointF(stereoDisplay_->getCurrentView()->getRightView()->getViewpoint() + diffTocenter / stereoDisplay_->getCurrentView()->getRightView()->getScale());


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
	glDisplay_->update();
	if (leftDisplay_)
		leftDisplay_->update();
	if (rightDisplay_)
		rightDisplay_->update();
}
void StereoDisplay::updateAll(QPointF* left, QPointF* right, bool emitClicked)
{
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
