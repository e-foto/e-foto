#include "SWidgetQt.h"

SWidgetQt::SWidgetQt(QWidget* parent) : QGLWidget(parent)
{
	selectedViewport = 0;
	selectedImage = 0;
	selectedPin = 0;
	selectedPoint = 0;
	/*
 selectedPen = 0;
 selectedBrush = 0;
 selectedPoint = 0;
 selectedLine = 0;
 selectedPolygon = 0;
 */
	cursorType = -1;
	onMove = false;
}

/** Implementation to abstract methods **/

int SWidgetQt::getWidth()
{
	return width();
}
int SWidgetQt::getHeight()
{
	return height();
}
void SWidgetQt::setWidth(int w)
{
	resize(w,height());
}
void SWidgetQt::setHeight(int h)
{
	resize(width(),h);
}
void SWidgetQt::resize(int w, int h)
{
	QGLWidget::resize(w, h);
	repaint();
}
bool SWidgetQt::createPin(string nickname, QImage loadedPin)
{
	// Impedimos o nickname vazio ou com ponto e virgula e a replica de nicknames
	if (nickname == "" || string::npos != nickname.find(";") || containsPin(nickname))
		return false;
	QImage* img = new QImage(loadedPin);
	sclass::SPin p(this, nickname, (void*)img, CM::QtMethods);
	// E o pin que não consegue fazer o load não entra na estrutura da SWidget.
	if (p.loaded())
	{
		pin.push_back(p);
		return true;
	}
	else
		return false;
}

void SWidgetQt::paintGL()
{
	for (unsigned int i = 0; i < viewport.size(); i++)
	{
		viewport.at(i).show();
	}
	if (selectedViewport != NULL)
	{
		int left, bottom, width, height;
		selectedViewport->informDimensions(left, bottom, width, height);
		setViewport(left, bottom, width, height);
	}
	//QGLWidget::paintGL();
}
void SWidgetQt::repaint()
{
	clearGL();
	paintGL();
	QGLWidget::repaint();
}

void SWidgetQt::mouseMoveEvent(QMouseEvent* event)
{
	if (selectedViewport != NULL)
	{
		if (onMove && ((event->buttons() & Qt::LeftButton) ||(event->buttons() & Qt::MidButton)))
		{
			int xDistance = (event->globalX() - moveStartPos.x());
			int yDistance = -(event->globalY() - moveStartPos.y());
			moveStartPos = event->globalPos();
			pan(xDistance,yDistance);
			QGLWidget::repaint();
		}
	}
	QGLWidget::mouseMoveEvent(event);
}
void SWidgetQt::mousePressEvent(QMouseEvent* event)
{
	GLdouble mousePosX;
	GLdouble mousePosY;
	double mousePos[2];

	mousePosX = (double)event->x();
	mousePosY = height() - (double)event->y();
	getMousePos(mousePosX, mousePosY, mousePos);

	if (selectedViewport != NULL)
	{
		if (!selectedViewport->isInsideViewport(mousePosX, mousePosY))
			return;
		if (event->button() == Qt::RightButton)
		{
			if (cursorType == 0)
			{
				if (selectedImage == NULL)
				{
					zoomOut();
				}
				else
				{
					if (selectedImage->isInsideImage(mousePos[0], mousePos[1],selectedViewport->getNickname()))
						zoomOut(mousePos[0], mousePos[1]);
				}
			}
		}
		else if (event->button() == Qt::LeftButton)
		{
			if (cursorType == 0)
			{
				if (selectedImage == NULL)
				{
					zoomIn();
				}
				else
				{
					if (selectedImage->isInsideImage(mousePos[0], mousePos[1],selectedViewport->getNickname()))
						zoomIn(mousePos[0], mousePos[1]);
				}
			}
			else if (cursorType == 1 && selectedPoint != NULL && selectedImage != NULL)
			{
				if (selectedImage->isInsideImage(mousePos[0], mousePos[1],selectedViewport->getNickname()))
					selectedImage->addMark(selectedPoint, mousePos[0], mousePos[1], selectedViewport);
			}
			else if (cursorType == 2)
			{
				moveStartPos = event->globalPos();
				onMove = true;
			}
			else if (cursorType == 3)
			{
				if (selectedImage->isInsideImage(mousePos[0], mousePos[1],selectedViewport->getNickname()))
					selectedImage->addMark(selectedPoint, mousePos[0], mousePos[1], selectedViewport);
			}
		}
		else if (event->button() == Qt::MidButton)
		{
			moveStartPos = event->globalPos();
			onMove = true;
		}
		QGLWidget::repaint();
	}
	else
	{
		cout << "Error: Invalid image or viewport, please choose another image or viewport." << endl;
	}
	QGLWidget::mousePressEvent(event);
}
void SWidgetQt::mouseReleaseEvent(QMouseEvent* event)
{
	onMove = false;
	QGLWidget::mouseReleaseEvent(event);
}
void SWidgetQt::wheelEvent(QWheelEvent* event)
{
	GLdouble mousePosX;
	GLdouble mousePosY;
	double mousePos[2];

	mousePosX = (double)event->x();
	mousePosY = height() - (double)event->y();
	getMousePos(mousePosX, mousePosY, mousePos);

	if (selectedViewport != NULL && selectedImage != NULL)
	{
		if (!selectedViewport->isInsideViewport(mousePosX, mousePosY))
			return;
		if (!selectedImage->isInsideImage(mousePos[0], mousePos[1],selectedViewport->getNickname()))
			return;
		int numDegrees = event->delta() / 8;
		int numSteps = numDegrees / 15;
		if (event->orientation() == Qt::Vertical)
		{
			if (numSteps>0)
			{
				zoom(selectedViewport->getZoomFactor()*pow(1.071773463,numSteps),mousePos[0],mousePos[1]);
			}
			else
			{
				zoom(selectedViewport->getZoomFactor()/pow(1.071773463,-numSteps),mousePos[0],mousePos[1]);
			}
		}
	}
	QGLWidget::wheelEvent(event);
}
void SWidgetQt::resizeEvent(QResizeEvent* event)
{
	if (selectedViewport != NULL)
		selectedViewport->redefineZoomLimits();
	QGLWidget::resizeEvent(event);
}
