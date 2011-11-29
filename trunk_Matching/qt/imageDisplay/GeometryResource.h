#ifndef GEOMETRYRESOURCE_H
#define GEOMETRYRESOURCE_H

#include <QList>
#include <QPointF>
#include <QObject>
#include <QImage>
#include <QPainter>

enum DefaultMark {
NoMark, RedMark, GreenMark, BlueMark, CyanMark, MagentaMark, YellowMark, DarkRedMark, DarkGreenMark, DarkBlueMark, DarkCyanMark, DarkMagentaMark, DarkYellowMark, BlackMark, WhiteMark, GrayMark, BlackAndWhiteMark
};

class Marker : public QImage
{
public:
	Marker(const QImage &image2Mark);
	Marker(DefaultMark defaultMarkType);

        QImage toQImage();
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
	void changePoint(unsigned int key, Coord);
};

class GeometryResource
{
	QList<Geometry> geometries_;
	int linkPointsMode;
public:
	GeometryResource();
	void addPoint(QPointF location, QString label, Marker* mark = NULL);
	void movePoint(unsigned int key, QPointF location);
	void clear();
	QImage draw(QImage dst, QSize targetSize, QPointF viewpoint, double scale);

	void setLinkPointsMode(int mode);
};

#endif // GEOMETRYRESOURCE_H
