#include "Geometry.h"


Marker::Marker()
{

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

GeometryResource::GeometryResource()
{
}

void GeometryResource::addPoint(QPointF location, QString label, Marker* mark)
{
	geometries_.append(Geometry::createPoint(location, label, mark));
}

void GeometryResource::clear()
{
	geometries_.clear();
}
#include <QDebug>
QImage GeometryResource::draw(QImage dst, QSize targetSize, QPointF viewpoint, double scale)
{

	QImage result(targetSize, QImage::Format_ARGB32);
	//QImage mark("../X16x16.png");
	QPainter painter(&result);
	painter.drawImage(0,0,dst);
	//painter.drawImage(targetSize.width()/2-8,targetSize.height()/2-8,mark);
	for (int i = 0; i<geometries_.size();i++)
	{
		if (geometries_.at(i).type() == 1)
		{
			QList<Coord> points = geometries_.at(i).listPoints();
			for (int j = 0; j<points.size();j++)
			{
				Marker* mark = points.at(j).marker_;
				QString label = points.at(j).label_;

				//qDebug("canto = %fx%f",viewpoint.x()-targetSize.width()/(2.0*scale),viewpoint.y()-targetSize.height()/(2.0*scale));//isso dá o leftTop de mundo desta janela
				//qDebug("ponto = %fx%f",points.at(j).x(),points.at(j).y());
				//qDebug("(ponto - canto) * scale = %fx%f", (points.at(j).x() -(viewpoint.x()-targetSize.width()/(2.0*scale)))*scale, (points.at(j).y() -(viewpoint.y()-targetSize.height()/(2.0*scale)))*scale);

				double x = (points.at(j).x() -(viewpoint.x()-targetSize.width()/(2.0*scale)))*scale;
				double y = (points.at(j).y() -(viewpoint.y()-targetSize.height()/(2.0*scale)))*scale;
				painter.drawImage(x-mark->width()/2, y-mark->height()/2, *mark);
				painter.setPen(QPen(QColor(Qt::yellow)));
				painter.drawText(x+mark->width()/2, y+mark->height()/2, label);
			}
		}
	}
	painter.end();
	return result;
}
