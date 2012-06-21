#ifndef GEOMETRYRESOURCE_H
#define GEOMETRYRESOURCE_H

#include <QList>
#include <QPointF>
#include <QPolygonF>
#include <QObject>
#include <QImage>
#include <QPainter>

#include "Features.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

enum DefaultMark {
	NoMark, RedMark, GreenMark, BlueMark, CyanMark, MagentaMark, YellowMark, DarkRedMark, DarkGreenMark, DarkBlueMark, DarkCyanMark, DarkMagentaMark, DarkYellowMark, BlackMark, WhiteMark, GrayMark, BlackAndWhiteMark
};

class SymbolsResource
{
public:
	static QImage getBackGround(QColor color, QSize size = QSize(24,24), QPoint pointedIn = QPoint(-2, -2), QColor pointColor = QColor(255,255,255,255), unsigned int pointWeigth = 2);
		static QImage getCross(QColor color, QSize size = QSize(24,24), unsigned int weigth = 2, bool pointingCenter = false);
	static QImage getBordedCross(QColor colorBrush, QColor colorPen, QSize size = QSize(24,24), unsigned int weigth = 3);
	static QImage getX(QColor color, QSize size = QSize(24,24), unsigned int weigth = 2);
	static QImage getBordedX(QColor colorBrush, QColor colorPen, QSize size = QSize(24,24), unsigned int weigth = 3);

	static QImage getTriangle(QColor color, QColor fillcolor = QColor(Qt::transparent), QSize size = QSize(24,24), unsigned int weigth = 2, bool pointingCenter = false);
	static QImage getCircle(QColor color, QColor fillcolor = QColor(Qt::transparent), QSize size = QSize(24,24), unsigned int weigth = 2, bool pointingCenter = false);
	static QImage getSquare(QColor color, QColor fillcolor = QColor(Qt::transparent), QSize size = QSize(24,24), unsigned int weigth = 2, bool pointingCenter = false);

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
	int key_; // num futuro breve serão feitas alterações para que essa key seja unsigned int
	QList<Coord> points_;
public:
	Geometry();
	Geometry(const Geometry &other);
	static Geometry createPoint(QPointF coord, int pointKey, QString label = "", Marker* mark = NULL);
	static Geometry createLine(QList<Coord> coords, int lineKey);
	static Geometry createPolygon(QList<Coord> coords, int polygonKey);
	int type() const;
	int key() const;
	QList<Coord> listPoints() const;
};

class GeometryResource
{
	Features* df;
	Marker* defaultMark;
	Marker* selectedMark;
    int selected;
	int featureProjection;
	QList<Geometry> geometries_;
	int linkPointsMode;

	unsigned int nextPointkey_;
	unsigned int nextLinekey_;

public:
	GeometryResource();
	void insertPoint(QPointF location, int pointKey = 0, QString label = "", Marker* mark = NULL);
	void addPoint(QPointF location, int pointKey = 0, QString label = "", Marker* mark = NULL);
	void updatePoint(QPointF location, int pointKey = 0, QString label = "", Marker* mark = NULL);
	void deletePoint(int pointKey);
	void addLine(QPointF p0, QPointF p1, int lineKey = 0);
	void clear();
	QImage draw(QImage dst, QSize targetSize, QPointF viewpoint, double scale);
	QImage draw(Features* df, int projection, QImage dst, QSize targetSize, QPointF viewpoint, double scale);

	unsigned int generatePointKey();
	bool hasPoint(unsigned int key);
	int indexOfPoint(unsigned int key);

	void setLinkPointsMode(int mode);
    void setFeatures(Features* features, int projection) {df = features; featureProjection = projection;}
    void setSelectedMark(unsigned int key) {selected = key;} //rever. o draw (sem features) de modo a usar isso.
    void setSelectedMarker(Marker* mark) {selectedMark = mark;}
    void setDefaultMarker(Marker* mark) {defaultMark = mark;}
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // GEOMETRYRESOURCE_H
