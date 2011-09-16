#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QList>
#include <QPointF>
#include <QObject>
#include <QImage>
#include <QPainter>

class Marker : public QImage
{
public:
	Marker();
};

class Coord : public QPointF
{
public:
	Coord();
	Coord(QPointF location, QString label, Marker* mark = NULL);
	QString label_;
	Marker* marker_;
};

class Geometry
{
	int type_;
	QList<Coord> points_;
public:
	Geometry();
	Geometry(const Geometry &other);
	static Geometry createPoint(QPointF coord, QString label, Marker* mark = NULL);
	static Geometry createLine(QList<Coord> coords);
	static Geometry createPolygon(QList<Coord> coords);
	int type() const;
	QList<Coord> listPoints() const;
};

class GeometryResource
{
	QList<Geometry> geometries_;
public:
	GeometryResource();
	void addPoint(QPointF location, QString label, Marker* mark = NULL);
	void clear();
	QImage draw(QImage dst, QSize targetSize, QPointF viewpoint, double scale);
};

#endif // GEOMETRY_H
