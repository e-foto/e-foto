#ifndef GEOMETRYRESOURCE_H
#define GEOMETRYRESOURCE_H
/*Copyright 2002-2014 e-foto team (UERJ)
  This file is part of e-foto.

    e-foto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    e-foto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with e-foto.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QList>
#include <QPointF>
#include <QPolygonF>
#include <QObject>
#include <QImage>
#include <QPainter>


namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class DemFeatures;

enum DefaultMark {
    NoMark, RedMark, GreenMark, BlueMark, CyanMark, MagentaMark, YellowMark, DarkRedMark, DarkGreenMark, DarkBlueMark, DarkCyanMark, DarkMagentaMark, DarkYellowMark, BlackMark, WhiteMark, GrayMark, BlackAndWhiteMark
};

class SymbolsResource
{
public:
    // todo: Method getBackgorund has unused parameters for make a background with dotted pattern.
    static QImage getBackGround(QColor color, QSize size = QSize(24,24)/*, QPoint pointedIn = QPoint(-2, -2), QColor pointColor = QColor(255,255,255,255), unsigned int pointWeigth = 2*/);
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
    explicit Marker(const QImage &image2Mark);
    explicit Marker(DefaultMark defaultMarkType);

    QImage toQImage();
};

class Coord : public QPointF
{
public:
    Coord();
    explicit Coord(QPointF location, QString label, Marker* mark = NULL);
    QString label_;
    Marker* marker_;
};

class Geometry
{
    int type_;
    unsigned int key_;
    QList<Coord> points_;
public:
    Geometry();
    Geometry(const Geometry &other);
    static Geometry createPoint(QPointF coord, unsigned int pointKey, QString label = "", Marker* mark = NULL);
    static Geometry createLine(QList<Coord> coords, unsigned int lineKey);
    static Geometry createPolygon(QList<Coord> coords, unsigned int polygonKey);
    int type() const;
    unsigned int key() const;
    QList<Coord> listPoints() const;
};

class GeometryResource
{
    DemFeatures* df;
    Marker* defaultMark;
    Marker* selectedMark;
    int featureProjection;
    QList<Geometry> geometries_;
    int linkPointsMode;

    unsigned int nextPointkey_;
    unsigned int nextLinekey_;

public:
    GeometryResource();
    void insertPoint(QPointF location, unsigned int pointKey = 0, QString label = "", Marker* mark = NULL);
    void addPoint(QPointF location, unsigned int pointKey = 0, QString label = "", Marker* mark = NULL);
    void updatePoint(QPointF location, unsigned int pointKey = 0, QString label = "", Marker* mark = NULL);
    void deletePoint(unsigned int pointKey);
    void addLine(QPointF p0, QPointF p1, unsigned int lineKey = 0);
    void clear();
    QImage draw(QImage dst, QSize targetSize, QPointF viewpoint, double scale);
    QImage draw(DemFeatures* df, int projection, QImage dst, QSize targetSize, QPointF viewpoint, double scale);

    unsigned int generatePointKey();
    bool hasPoint(unsigned int key);
    int indexOfPoint(unsigned int key);

    void setLinkPointsMode(int mode);
    void setFeatures(DemFeatures* features, int projection) {df = features; featureProjection = projection;}
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // GEOMETRYRESOURCE_H
