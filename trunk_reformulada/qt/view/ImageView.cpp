#include "ImageView.h"
#include <math.h>

#include <QLabel>

ImageView::ImageView(QWidget* parent) : SWidgetQt( parent )
{
}

ImageView::ImageView(QString file, QWidget* parent) : SWidgetQt( parent )
{
	this->createViewport("myViewport");
	if (!this->createImage("myImage",file.toStdString()))
	{
		QMessageBox* msg = new QMessageBox(QMessageBox::Warning,"Unable to open file","The openGL failed to texture image.\n");
		msg->show();
		return;
	}
	this->createPin("myIOPin",QIcon(":/pins/X16x16.png").pixmap(16,16).toImage());
	this->createPin("mySRPin1",QIcon(":/pins/T16x16.png").pixmap(16,16).toImage());
	this->createPin("mySRPin2",QIcon(":/pins/T16x16red.png").pixmap(16,16).toImage());
	this->createPin("mySRPin3",QIcon(":/pins/ARROW32x32.png").pixmap(32,32).toImage());

	this->selectViewport("myViewport");
	this->getSelectedViewport()->addImage("myImage");
	this->selectImage("myImage");
	this->paintGL();
}

ImageView::~ImageView()
{
	this->clearPoints();
	this->destroyPin("myIOPin");
	this->destroyPin("mySRPin1");
	this->destroyPin("mySRPin2");
	this->destroyPin("mySRPin3");
	this->destroyImage("myImage");
	this->destroyViewport("myViewport");
}

void ImageView::setViewMode(int m)
{
	switch (m)
	{
	case 1 : setCursor(Qt::CrossCursor); mode=1; setMarkingCursor(); break;
	case 2 : setCursor(Qt::OpenHandCursor); mode=2; setMoveCursor(); break;
	case 3 : setCursor(Qt::PointingHandCursor); mode=3; setZoomCursor(); break;
	case 4 : setCursor(Qt::CrossCursor); modeBackup = mode; mode=4; setDirectionCursor(); break;
	default : unsetCursor(); mode=0; setMoveCursor(); break;
	}
}

int ImageView::getViewMode()
{
	return mode;
}

void ImageView::fitView()
{
	if (selectedViewport != NULL && selectedImage != NULL)
		selectedViewport->fitView(selectedImage->getNickname());
}

void ImageView::createPoints(QStandardItemModel* points, int mode)
{
	if (mode == 1) //IO use
	{
		for (int row = 0; row < points->rowCount() ;++row)
		{
			string pointName = QString::number(row).toStdString();
			createPoint(pointName,"myIOPin");
		}
	}
	else if (mode == 2) //SR use
	{
		for (int row = 0; row < points->rowCount() ;++row)
		{
			string pointName = QString::number(row).toStdString();
			createPoint(pointName,"mySRPin1");
		}
	}
	createPoint("-1","mySRPin3");
}

void ImageView::drawPoints(QStandardItemModel* points, int mode)
{
	int x;
	int y;
	if (mode == 1) //IO use
	{
		for (int row = 0; row < points->rowCount() ;++row)
		{
			if (points->data(points->index(row,4)).toBool())
			{
				string pointName = QString::number(row).toStdString();
				selectPoint(pointName);
				x = points->data(points->index(row,2)).toInt();
				y = points->data(points->index(row,3)).toInt();
				selectedImage->addMark(selectedPoint, x+0.5, y+0.5, selectedViewport, true);
			}
		}
	}
	else if (mode == 2) //SR use
	{
		for (int row = 0; row < points->rowCount() ;++row)
		{
			if (points->data(points->index(row,10)).toBool())
			{
				string pointName = QString::number(row).toStdString();
				selectPoint(pointName);
				x = points->data(points->index(row,6)).toInt();
				y = points->data(points->index(row,7)).toInt();
				if(points->item(row,1)->checkState() == Qt::Checked)
				{
					selectedPoint->setNewMarker(getPin("mySRPin1"));
				}
				else
				{
					selectedPoint->setNewMarker(getPin("mySRPin2"));
				}
				selectedImage->addMark(selectedPoint, x, y, selectedViewport, true);
			}
		}
	}
}

void ImageView::clearPoints()
{
	for (int i = -1; i < countPoints(); ++i)
	{
		destroyPoint(QString::number(i).toStdString());
	}
}

void ImageView::drawFlightDirection(int x, int y)
{
	// Em breve os valores width/2 e height/2 usados neste claculo serão substituidos pelas coordenadas do centro ótico
	// do sensor corrigido na Orientação Interior. Isto tornará mais fiel o cálculo do ângulo aqui realizado.
	double halfW = selectedImage->getWidth()/2.0;
	double halfH = selectedImage->getHeight()/2.0;
	double cx = (x - halfW);//halfW;
	double cy = -(y - halfH);//halfH;
	double ang;
	if (cx != 0 && cy != 0)
		ang = atan(cy/cx);
	else if (cx == 0 && cy > 0)
		ang = 1.570796327;
	else if (cx == 0 && cy < 0)
		ang = -1.570796327;
	else if (cy == 0 && cx > 0)
		ang = 0.0;
	else if (cy == 0 && cx < 0)
		ang = 3.141592654;
	if (cx < 0)
	{
		if (cy > 0)
		{
			ang = 3.141592654+ang;
		}
		else
		{
			ang = -3.141592654+ang;
		}
	}
	double dx = (halfW-16)*cx/sqrt(pow(cx,2)) - cx;//*halfW;
	double dy = (halfH-16)*cy/sqrt(pow(cy,2)) - cy;//*halfH;
	dy *= sin(ang)*cy/sqrt(pow(cy,2));
	dx *= cos(ang)*cx/sqrt(pow(cx,2));
	ang *= 57.2957795; //rad2Degree
	QImage* aux = new QImage(QIcon(":/pins/ARROW32x32.png").pixmap(32,32).toImage());
	getPin("mySRPin3")->rotate(ang,(void*)aux, CM::QtMethods);
	getPoint("-1")->panInImage(selectedImage->getNickname(),dx,dy);
	repaint();
	setViewMode(modeBackup);
	//qDebug("w = %f; h = %f; cx = %f; cy = %f; dx = %f; dy = %f",halfW,halfH,cx,cy,dx,dy);
}

QPoint ImageView::getPointCoords()
{
	int x = 0;
	int y = 0;
	if (selectedPoint != NULL && selectedImage != NULL)
	{
		x = selectedPoint->getXInImage(selectedImage->getNickname())+selectedImage->getWidth()/2;
		y = -(selectedPoint->getYInImage(selectedImage->getNickname())-selectedImage->getHeight()/2);
	}
	return QPoint(x,y);
}

/****************************
   LOAD IMAGE INTO MEMORY
*****************************/

bool ImageView::loadImage(QString file)
{
	this->createViewport("myViewport");
	if (!this->createImage("myImage",file.toStdString()))
	{
		QMessageBox* msg = new QMessageBox(QMessageBox::Warning,"Unable to open file","The openGL failed to texture image.\n");
		msg->show();
		return false;
	}
	this->createPin("myIOPin",QIcon(":/pins/X16x16.png").pixmap(16,16).toImage());
	this->createPin("mySRPin1",QIcon(":/pins/T16x16.png").pixmap(16,16).toImage());
	this->createPin("mySRPin2",QIcon(":/pins/T16x16red.png").pixmap(16,16).toImage());
	this->createPin("mySRPin3",QIcon(":/pins/ARROW32x32.png").pixmap(32,32).toImage());

	this->selectViewport("myViewport");
	this->getSelectedViewport()->addImage("myImage");
	this->selectImage("myImage");
	this->paintGL();
	return true;
}

void ImageView::mousePressEvent(QMouseEvent* e)
{
	emit mousePressed();
	SWidgetQt::mousePressEvent(e);
	if (e->button() == Qt::LeftButton)
	{
		GLdouble mousePosX = (double)e->x();
		GLdouble mousePosY = height() - (double)e->y();
		double mousePos[2];
		getMousePos(mousePosX, mousePosY, mousePos);
		if (!selectedImage->isInsideImage(mousePos[0], mousePos[1],selectedViewport->getNickname()))
			return;
		switch (mode)
		{
		case 1 :
			emit markClicked(getPointCoords());
			break;
		case 2 :
			setCursor(Qt::ClosedHandCursor);
			break;
		case 4 :
			emit flightDirectionClicked(getPointCoords());
			break;
		}
	}
	emit changed();
}
void ImageView::mouseReleaseEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		switch (mode)
		{
		case 2 :
			setCursor(Qt::OpenHandCursor);
			break;
		}
	}
	SWidgetQt::mouseReleaseEvent(e);
}
