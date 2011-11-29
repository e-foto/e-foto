#include "GeometryResource.h"
#include "SingleDisplay.h"

Marker::Marker(const QImage &image2Mark) :
	QImage(image2Mark)
{
}

Marker::Marker(DefaultMark defaultMarkType)
{
	switch (defaultMarkType)
	{
	case NoMark:
                *this = Cursor(this->toQImage()).noCursor().toQImage();
		break;
                /*
	case RedMark:
		*this = QImage(":/cursors/RedMark");
		break;
	case GreenMark:
		*this = QImage(":/cursors/GreenMark");
		break;
	case BlueMark:
		*this = QImage(":/cursors/BlueMark");
		break;
	case CyanMark:
		*this = QImage(":/cursors/CyanMark");
		break;
	case MagentaMark:
		*this = QImage(":/cursors/MagentaMark");
		break;
	case YellowMark:
		*this = QImage(":/cursors/YellowMark");
		break;
	case DarkRedMark:
		*this = QImage(":/cursors/DarkRedMark");
		break;
	case DarkGreenMark:
		*this = QImage(":/cursors/DarkGreenMark");
		break;
	case DarkBlueMark:
		*this = QImage(":/cursors/DarkBlueMark");
		break;
	case DarkCyanMark:
		*this = QImage(":/cursors/DarkCyanMark");
		break;
	case DarkMagentaMark:
		*this = QImage(":/cursors/DarkMagentaMark");
		break;
	case DarkYellowMark:
		*this = QImage(":/cursors/DarkYellowMark");
		break;
	case BlackMark:
		*this = QImage(":/cursors/BlackMark");
		break;
	case WhiteMark:
		*this = QImage(":/cursors/WhiteMark");
		break;
	case GrayMark:
		*this = QImage(":/cursors/GrayMark");
		break;
	case BlackAndWhiteMark:
		*this = QImage(":/cursors/BlackAndWhiteMark");
		break;
                */
        default:
                *this = Cursor(this->toQImage()).toX(QColor(0,255,0)).toQImage();
                break;
	}
}

QImage Marker::toQImage()
{
        return *this;
}


Coord::Coord()
{
	marker_ = NULL;
}

Coord::Coord(QPointF location, QString label, Marker *mark) :
	QPointF(location.x(),location.y())
{
	label_ = label;
	marker_ = mark;
}

Geometry::Geometry()
{
	type_ = 0;
}

Geometry::Geometry(const Geometry &other)
{
	points_ = other.listPoints();
	type_ = other.type();
}

Geometry Geometry::createPoint(QPointF coord, QString label, Marker *mark)
{
	Geometry result;
	result.type_ = 1;
	result.points_.append(Coord(coord, label, mark));
	return result;
}

Geometry Geometry::createLine(QList<Coord> coords)
{
	Geometry result;
	result.type_ = 2;
	result.points_.append(coords);
	return result;
}

Geometry Geometry::createPolygon(QList<Coord> coords)
{
	Geometry result;
	result.type_ = 3;
	result.points_.append(coords);
	return result;
}

int Geometry::type() const
{
	return type_;
}

QList<Coord> Geometry::listPoints() const
{
	return points_;
}

void Geometry::changePoint(unsigned int key, Coord newCoord)
{
	points_.replace(key, newCoord);
}

GeometryResource::GeometryResource()
{
	linkPointsMode = 4;
}

void GeometryResource::addPoint(QPointF location, QString label, Marker* mark)
{
	geometries_.append(Geometry::createPoint(location, label, mark));
}

void GeometryResource::movePoint(unsigned int key, QPointF location)
{
	Geometry point = geometries_.at(key);
	point.changePoint(0,Coord(location,point.listPoints().at(0).label_,point.listPoints().at(0).marker_));
	geometries_.replace(key,point);
}

void GeometryResource::clear()
{
	geometries_.clear();
}

QImage GeometryResource::draw(QImage dst, QSize targetSize, QPointF viewpoint, double scale)
{
	QImage result(dst);
	QPainter painter(&result);
	for (int i = 0; i<geometries_.size();i++)
	{
		if (geometries_.at(i).type() == 1)
		{
			QList<Coord> points = geometries_.at(i).listPoints();
			for (int j = 0; j<points.size();j++)
			{
				Marker* mark = points.at(j).marker_;
				QString label = points.at(j).label_;

				double x = (points.at(j).x() -(viewpoint.x()-targetSize.width()/(2.0*scale)))*scale;
				double y = (points.at(j).y() -(viewpoint.y()-targetSize.height()/(2.0*scale)))*scale;
				painter.drawImage(x-mark->width()/2, y-mark->height()/2, *mark);
				painter.setPen(QPen(Qt::yellow));
				painter.setFont(QFont("Arial", 10));
				painter.drawText(x+mark->width()/2, y+mark->height()/2, label);
			}
		}
	}
	if (linkPointsMode)
	{
		for (int i = 0; i<geometries_.size();i += 1+linkPointsMode)
		{
			if (geometries_.at(i).type() == 1)
			{
				double x[4], y[4];

				QList<Coord> point0 = geometries_.at(i).listPoints();
				x[0] = (point0.at(0).x() -(viewpoint.x()-targetSize.width()/(2.0*scale)))*scale;
				y[0] = (point0.at(0).y() -(viewpoint.y()-targetSize.height()/(2.0*scale)))*scale;

				if (i+1 < geometries_.size())
				{
					QList<Coord> point1 = geometries_.at(i+1).listPoints();
					x[1] = (point1.at(0).x() -(viewpoint.x()-targetSize.width()/(2.0*scale)))*scale;
					y[1] = (point1.at(0).y() -(viewpoint.y()-targetSize.height()/(2.0*scale)))*scale;
					painter.drawLine(QPointF(x[0],y[0]),QPointF(x[1],y[1]));
				}

				if (linkPointsMode > 1 && i+2 < geometries_.size())
				{
					QList<Coord> point2 = geometries_.at(i+2).listPoints();
					x[2] = (point2.at(0).x() -(viewpoint.x()-targetSize.width()/(2.0*scale)))*scale;
					y[2] = (point2.at(0).y() -(viewpoint.y()-targetSize.height()/(2.0*scale)))*scale;
					painter.drawLine(QPointF(x[1],y[1]),QPointF(x[2],y[2]));
				}

				if (linkPointsMode == 4 && i+3 < geometries_.size())
				{
					QList<Coord> point3 = geometries_.at(i+3).listPoints();
					x[3] = (point3.at(0).x() -(viewpoint.x()-targetSize.width()/(2.0*scale)))*scale;
					y[3] = (point3.at(0).y() -(viewpoint.y()-targetSize.height()/(2.0*scale)))*scale;
					painter.drawLine(QPointF(x[2],y[2]),QPointF(x[3],y[3]));
					painter.drawLine(QPointF(x[3],y[3]),QPointF(x[0],y[0]));
				}
			}
		}
	}
	painter.end();
	return result;
}

void GeometryResource::setLinkPointsMode(int mode)
{
	linkPointsMode = mode;
}
