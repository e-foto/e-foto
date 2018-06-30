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
    NoMark, RedMark, GreenMark, BlueMark, CyanMark, MagentaMark,
    YellowMark, DarkRedMark, DarkGreenMark, DarkBlueMark, DarkCyanMark,
    DarkMagentaMark, DarkYellowMark, BlackMark, WhiteMark, GrayMark,
    BlackAndWhiteMark
};

class SymbolsResource {
  public:
    // todo: Method getBackgorund has unused parameters for make a background with dotted pattern.
    static QImage getBackGround(QColor color, QSize size = QSize(24, 24));
    static QImage getCross(QColor color, QSize size = QSize(24, 24),
                           unsigned int weigth = 2, bool pointingCenter = false);
    static QImage getBordedCross(QColor colorBrush, QColor colorPen,
                                 QSize size = QSize(24, 24), unsigned int weigth = 3);
    static QImage getBordedX(QColor colorBrush, QColor colorPen,
                             QSize size = QSize(24, 24), unsigned int weigth = 3);

    static QImage getTriangle(QColor color,
                              QColor fillcolor = QColor(Qt::transparent),
                              QSize size = QSize(24, 24),
                              unsigned int weigth = 2,
                              bool pointingCenter = false);
    static QImage getCircle(QColor color,
                            QColor fillcolor = QColor(Qt::transparent),
                            QSize size = QSize(24, 24),
                            unsigned int weigth = 2,
                            bool pointingCenter = false);
    static QImage getSquare(QColor color,
                            QColor fillcolor = QColor(Qt::transparent),
                            QSize size = QSize(24, 24),
                            unsigned int weigth = 2,
                            bool pointingCenter = false);

    static QImage getOpenHand();
    static QImage getClosedHand();
    static QImage getPointingHand();
    static QImage getLeftArrow();
    static QImage getRightArrow();
    static QImage getMagnifyGlass(QString text = "");
    static QImage getText(QString text = "", bool bottom = true);
};

class Marker : public QImage {
  public:
    explicit Marker(const QImage& image2Mark);
    explicit Marker(DefaultMark defaultMarkType);
};

class Coord : public QPointF {
  public:
    Coord();
    explicit Coord(QPointF location, QString label, Marker* mark = NULL);
    QString label_;
    Marker* marker_;
};

class Geometry {
    int type_{0};
    unsigned int key_{0};
    QList<Coord> points_{};
  public:
    Geometry();
    Geometry(const Geometry& other);
    static Geometry createPoint(QPointF coord, unsigned int pointKey,
                                QString label = "", Marker* mark = NULL);
    int type() const;
    unsigned int key() const;
    QList<Coord> listPoints() const;
};

class GeometryResource {
  public:
    GeometryResource();
    void insertPoint(QPointF location, unsigned int pointKey = 0,
                     QString label = "", Marker* mark = NULL);
    void addPoint(QPointF location, unsigned int pointKey = 0, QString label = "",
                  Marker* mark = NULL);
    void updatePoint(QPointF location, unsigned int pointKey = 0,
                     QString label = "", Marker* mark = NULL);
    void deletePoint(unsigned int pointKey);
    void clear();
    QImage draw(QImage dst, QSize targetSize, QPointF viewpoint, double scale);
    QImage draw(DemFeatures* df_, int projection, QImage dst, QSize targetSize,
                QPointF viewpoint, double scale) const;

    unsigned int generatePointKey() const;
    void setFeatures(DemFeatures* features, int projection) {
        df_ = features;
        featureProjection_ = projection;
    }

  private:
    DemFeatures* df_{nullptr};
    Marker* defaultMark_{nullptr};
    Marker* selectedMark_{nullptr};
    int featureProjection_{-1};
    QList<Geometry> geometries_{};
    int linkPointsMode_{4};

    unsigned int nextPointkey_{1};
    unsigned int nextLinekey_{1};

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // GEOMETRYRESOURCE_H
