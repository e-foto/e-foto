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

class SymbolsResource
{
public:
	static QImage getBackGround(QColor color, QSize size = QSize(24,24), QPoint pointedIn = QPoint(-2, -2), QColor pointColor = QColor(255,255,255,255), unsigned int pointWeigth = 2);
	static QImage getCross(QColor color, QSize size = QSize(24,24), unsigned int weigth = 2);
	static QImage getBordedCross(QColor colorBrush, QColor colorPen, QSize size = QSize(24,24), unsigned int weigth = 3);
	static QImage getX(QColor color, QSize size = QSize(24,24), unsigned int weigth = 2);
	static QImage getBordedX(QColor colorBrush, QColor colorPen, QSize size = QSize(24,24), unsigned int weigth = 3);

	static QImage getTriangle(QColor color, QSize size = QSize(24,24), unsigned int weigth = 2, bool pointingCenter = false);
	static QImage getCircle(QColor color, QSize size = QSize(24,24), unsigned int weigth = 2, bool pointingCenter = false);
	static QImage getSquare(QColor color, QSize size = QSize(24,24), unsigned int weigth = 2, bool pointingCenter = false);

	static QImage getOpenHand();
	static QImage getClosedHand();
	static QImage getPointingHand();
	static QImage getUpArrow();
	static QImage getDownArrow();
	static QImage getLeftArrow();
	static QImage getRightArrow();
	static QImage getMagnifyGlass(QString text = "");
	static QImage getText(QString text = "", bool bottom = true);
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
