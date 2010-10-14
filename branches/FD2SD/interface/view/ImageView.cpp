#include "ImageView.h"
#include <math.h>

ImageView::ImageView(QWidget* parent) : QGraphicsView( parent )
{
	image = new QImage;
	loadImage("");
	flightDirection = NULL;
	
    graphicsScene = new QGraphicsScene(this);
	view = new QPixmap();
	*view = graphicsScene->addPixmap(QPixmap::fromImage(*image))->pixmap();
	setScene(graphicsScene);
	
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setAlignment(Qt::AlignCenter);
	
	
	createCursors();
}

ImageView::ImageView(QString file, QWidget* parent) : QGraphicsView( parent )
{
	image = new QImage;
	loadImage(file);
	flightDirection = NULL;

	graphicsScene = new QGraphicsScene(this);
	view = new QPixmap();
	*view = graphicsScene->addPixmap(QPixmap::fromImage(*image))->pixmap();
	setScene(graphicsScene);

	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setAlignment(Qt::AlignCenter);


	createCursors();
}

/**
 * Create cursors
 * @bug cursor runtime directory! Solution: create common dir for cursors.
 */
void ImageView::createCursors()
{
	QImage bitmap;
	QImage mask;
	QBitmap bmp;
	QBitmap xbm;
	// Zoom
	bitmap.load("resource/cursors/zoom.bmp");
	mask.load("resource/cursors/zoom.xbm");
	bmp = bmp.fromImage(bitmap);
	xbm = xbm.fromImage(mask);
	zoomCur = new QCursor(bmp,xbm,14,13);
}

void ImageView::setViewMode(int m)
{
	switch (m)
	{
	case 1 : setCursor(Qt::CrossCursor); mode=1; setDragMode(QGraphicsView::NoDrag); break;
	case 2 : setCursor(Qt::OpenHandCursor); mode=2; setDragMode(QGraphicsView::ScrollHandDrag); break;
	case 3 : setCursor(*zoomCur); mode=3; setDragMode(QGraphicsView::RubberBandDrag); break;
	case 4 : setCursor(Qt::CrossCursor); mode=4; setDragMode(QGraphicsView::NoDrag); break;
	default : unsetCursor(); mode=0; setDragMode(QGraphicsView::NoDrag); break;
	}
}

void ImageView::fitView()
{
	fitInView(0,0,image->width(),image->height(),Qt::KeepAspectRatio/*ByExpanding*/);
}

void ImageView::clearGraphicsScene()
{
	for (int i = 0; i < points.size(); i++)
	{
		graphicsScene->removeItem(points.at(i));
	}
	points.clear();
}

void ImageView::drawPoints(QStandardItemModel* points, int mode)
{
	if (mode == 1)
	{
		clearGraphicsScene();
		for (unsigned int row = 0; row < points->rowCount() ;++row)
		{
			if (points->data(points->index(row,4)).toBool())
			{
				int x = points->data(points->index(row,2)).toInt();
				int y = points->data(points->index(row,3)).toInt();
				drawPoint(x,y,1);
			}
		}
	}
	else if (mode == 2)
	{
		clearGraphicsScene();
		for (unsigned int row = 0; row < points->rowCount() ;++row)
		{
			if (points->data(points->index(row,10)).toBool())
			{
				int x = points->data(points->index(row,6)).toInt();
				int y = points->data(points->index(row,7)).toInt();
				if(points->item(row,1)->checkState() == Qt::Checked)
				{
					drawPoint(x,y,2);
				}
				else
				{
					drawPoint(x,y,3);
				}
			}
		}
	}
}

void ImageView::drawPoint(int x, int y, int mode)
{
	if (mode == 1)
	{
		QGraphicsItem* part;
		part = graphicsScene->addLine(x-1.5,y-1.5,x,y,QPen(QColor(0,255,0,255)));
		part->setZValue(2);
		points.push_back(part);
		part = graphicsScene->addLine(x+1,y+1,x+2.5,y+2.5,QPen(QColor(0,255,0,255)));
		part->setZValue(2);
		points.push_back(part);
		part = graphicsScene->addLine(x-1.5,y+2.5,x,y+1,QPen(QColor(0,255,0,255)));
		part->setZValue(2);
		points.push_back(part);
		part = graphicsScene->addLine(x+1,y,x+2.5,y-1.5,QPen(QColor(0,255,0,255)));
		part->setZValue(2);
		points.push_back(part);
		part = graphicsScene->addEllipse(x-1.5,y-1.5,4,4,QPen(QColor(0,255,0,255)));
		part->setZValue(2);
		points.push_back(part);
	}
	else if (mode == 2)
	{
		QGraphicsItem* part;
		part = graphicsScene->addLine(x-1.5,y-1.5,x,y,QPen(QColor(0,255,0,255)));
		part->setZValue(2);
		points.push_back(part);
		part = graphicsScene->addLine(x+1,y+1,x+2.5,y+2.5,QPen(QColor(0,255,0,255)));
		part->setZValue(2);
		points.push_back(part);
		part = graphicsScene->addLine(x-1.5,y+2.5,x,y+1,QPen(QColor(0,255,0,255)));
		part->setZValue(2);
		points.push_back(part);
		part = graphicsScene->addLine(x+1,y,x+2.5,y-1.5,QPen(QColor(0,255,0,255)));
		part->setZValue(2);
		points.push_back(part);
		part = graphicsScene->addEllipse(x-1.5,y-1.5,4,4,QPen(QColor(0,255,0,255)));
		part->setZValue(2);
		points.push_back(part);
	}
	else if (mode == 3)
	{
		QGraphicsItem* part;
		part = graphicsScene->addLine(x-1.5,y-1.5,x,y,QPen(QColor(255,0,0,255)));
		part->setZValue(2);
		points.push_back(part);
		part = graphicsScene->addLine(x+1,y+1,x+2.5,y+2.5,QPen(QColor(255,0,0,255)));
		part->setZValue(2);
		points.push_back(part);
		part = graphicsScene->addLine(x-1.5,y+2.5,x,y+1,QPen(QColor(255,0,0,255)));
		part->setZValue(2);
		points.push_back(part);
		part = graphicsScene->addLine(x+1,y,x+2.5,y-1.5,QPen(QColor(255,0,0,255)));
		part->setZValue(2);
		points.push_back(part);
		part = graphicsScene->addEllipse(x-1.5,y-1.5,4,4,QPen(QColor(255,0,0,255)));
		part->setZValue(2);
		points.push_back(part);
	}
}

void ImageView::drawFlightDirection(int x, int y)
{
	if (flightDirection != NULL)
		graphicsScene->removeItem(flightDirection);
	flightDirection = graphicsScene->addLine(x,y,view->width()/2,view->height()/2,QPen(QColor(0,255,0,255)));
	flightDirection->setZValue(2);

}

void ImageView::scaleView(qreal scaleFactor)
{
	qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor < 0.07 || factor > 100)
		return;

	scale(scaleFactor, scaleFactor);
}

/****************************
   LOAD IMAGE INTO MEMORY
*****************************/

bool ImageView::loadImage(QString file)
{
	bool load = image->load(file);
	if (!load)
		QMessageBox::information( this, tr("Image drawing"),tr("Could not open file %1").arg(file));
	windowChanged=true;
	return load;
}


/****************************
   MOUSE EVENTS
*****************************/
void ImageView::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton) 
	{
		switch (mode)
		{
		case 1 :
			currentPos = mapToScene(e->pos()).toPoint();
			emit markClicked(currentPos);
			break;
		case 3 : 
			rubberBandRect.setTopLeft(e->pos());
			setCursor(Qt::CrossCursor);			
			break;
		case 4 :
			currentPos = mapToScene(e->pos()).toPoint();
			emit flightDirectionClicked(currentPos);
			break;
		}
	}
	else
	{
		switch (mode)
		{
                case 3 :
			scaleView(0.5);
			break;
		}
	}
	QGraphicsView::mousePressEvent(e);
	update();
}

void ImageView::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton) 
	{
		switch (mode)
		{
		case 1 :
			{
				emit mouseReleased();
				break;
			}
		case 3 :
			{
				rubberBandRect.setBottomRight(e->pos());
				if (rubberBandRect.topLeft() == rubberBandRect.bottomRight())
					scaleView(2.0);
				else
					fitInView(mapToScene(rubberBandRect).boundingRect(),Qt::KeepAspectRatio/*ByExpanding*/);
				setCursor(*zoomCur);
				break;
			}
		case 4 :
			{
				emit mouseReleased();
				break;
			}
		}
	}
	QGraphicsView::mouseReleaseEvent(e);
}

void ImageView::wheelEvent(QWheelEvent *e)
{
	scaleView(pow((double)2, e->delta() / 240.0));
}
