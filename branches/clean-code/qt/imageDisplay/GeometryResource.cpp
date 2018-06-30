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

#include "GeometryResource.h"
#include "SingleDisplay.h"

#include "DemFeatures.h"

//#include <math.h>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

QImage SymbolsResource::getBackGround(QColor color,
                                      QSize size) {
    QImage img(size, QImage::Format_ARGB32);
    img.fill(color.rgba());
    // Falta colocar um ponto sobre o background se necessário
    return img;
}

QImage SymbolsResource::getCross(QColor color, QSize size, unsigned int weigth,
                                 bool pointingCenter) {
    QImage img(size, QImage::Format_ARGB32);
    img.fill(QColor(0, 0, 0, 0).rgba());
    double hotX = size.width() / 2.0;
    double hotY = size.height() / 2.0;
    QPainter painter(&img);
    double diff = weigth;
    painter.setPen(QPen(QBrush(color), weigth));

    if (pointingCenter) {
        painter.drawPoint(hotX, hotY);
        diff = weigth + 2;
    }

    painter.drawLine(QPointF(0, hotY), QPointF(hotX - diff, hotY));
    painter.drawLine(QPointF(hotX + diff, hotY), QPointF(size.width(), hotY));
    painter.drawLine(QPointF(hotX, 0), QPointF(hotX, hotY - diff));
    painter.drawLine(QPointF(hotX, hotY + diff), QPointF(hotX, size.height()));
    painter.end();
    return img;
}

QImage SymbolsResource::getBordedCross(QColor colorBrush, QColor colorPen,
                                       QSize size, unsigned int weigth) {
    QImage img(size, QImage::Format_ARGB32);
    img.fill(QColor(0, 0, 0, 0).rgba());
    int hotX = size.width() / 2;
    int hotY = size.height() / 2;
    QPainter painter(&img);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(weigth % 2, hotY - weigth / 2,
                     size.width() / 2 - (weigth + 1) / 2, weigth, colorPen);
    painter.fillRect(size.width() / 2 + (weigth + 1) / 2, hotY - weigth / 2,
                     size.width() / 2 - weigth / 2, weigth, colorPen);
    painter.fillRect(hotX - weigth / 2, weigth % 2, weigth,
                     size.height() / 2 - (weigth + 1) / 2, colorPen);
    painter.fillRect(hotX - weigth / 2, size.height() / 2 + (weigth + 1) / 2,
                     weigth, size.height() / 2 - weigth / 2, colorPen);
    painter.fillRect(weigth % 2 + 1, hotY - weigth / 2 + 1,
                     size.width() / 2 - (weigth + 1) / 2 - 2, weigth - 2, colorBrush);
    painter.fillRect(size.width() / 2 + (weigth + 1) / 2 + 1, hotY - weigth / 2 + 1,
                     size.width() / 2 - (weigth + 1) / 2 - 2, weigth - 2, colorBrush);
    painter.fillRect(hotX - weigth / 2 + 1, weigth % 2 + 1, weigth - 2,
                     size.height() / 2 - (weigth + 1) / 2 - 2, colorBrush);
    painter.fillRect(hotX - weigth / 2 + 1,
                     size.height() / 2 + (weigth + 1) / 2 + 1, weigth - 2,
                     size.height() / 2 - (weigth + 1) / 2 - 2, colorBrush);
    painter.end();
    return img;
}

QImage SymbolsResource::getBordedX(QColor colorBrush, QColor colorPen,
                                   QSize size, unsigned int weigth) {
    QImage img = SymbolsResource::getBordedCross(colorBrush, colorPen, size,
                 weigth);
    int hotX = size.width() / 2;
    int hotY = size.height() / 2;
    QTransform transf;
    img = img.transformed(transf.translate(-hotX, -hotY).rotate(45).translate(hotX,
                          hotY), Qt::SmoothTransformation);
    return img;
}

QImage SymbolsResource::getTriangle(QColor color, QColor fillcolor, QSize size,
                                    unsigned int weigth, bool pointingCenter) {
    QImage img(size, QImage::Format_ARGB32);
    img.fill(QColor(0, 0, 0, 0).rgba());
    double hotX = size.width() / 2.0;
    double hotY = size.height() / 2.0;
    double a = hotY - 1;
    double b = a / 2.0;
    double c = sqrt(2.0) * a / 2.0;
    QPointF A(hotX, 1);
    QPointF B(hotX + c, hotY + b);
    QPointF C(hotX - c, hotY + b);
    QVector<QPointF> points;
    points.append(A);
    points.append(B);
    points.append(C);
    QPolygonF poly(points);
    QPainter painter(&img);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QBrush(fillcolor));
    painter.drawConvexPolygon(poly);
    painter.setPen(QPen(QBrush(color), weigth));
    painter.setBrush(QBrush(Qt::transparent));

    if (pointingCenter) {
        painter.drawPoint(hotX, hotY);
    }

    painter.drawConvexPolygon(poly);
    painter.end();
    return img;
}

QImage SymbolsResource::getCircle(QColor color, QColor fillcolor, QSize size,
                                  unsigned int weigth, bool pointingCenter) {
    QImage img(size, QImage::Format_ARGB32);
    img.fill(QColor(0, 0, 0, 0).rgba());
    int hotX = size.width() / 2;
    int hotY = size.height() / 2;
    QPainter painter(&img);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QBrush(fillcolor));
    painter.drawEllipse(QPoint(hotX, hotY), size.width() / 2 - 1,
                        size.height() / 2 - 1);
    painter.setPen(QPen(QBrush(color), weigth));
    painter.setBrush(QBrush(Qt::transparent));

    if (pointingCenter) {
        painter.drawPoint(hotX, hotY);
    }

    painter.drawEllipse(QPoint(hotX, hotY), size.width() / 2 - 1,
                        size.height() / 2 - 1);
    painter.end();
    return img;
}

QImage SymbolsResource::getSquare(QColor color, QColor fillcolor, QSize size,
                                  unsigned int weigth, bool pointingCenter) {
    QImage img(size, QImage::Format_ARGB32);
    img.fill(QColor(0, 0, 0, 0).rgba());
    int hotX = size.width() / 2;
    int hotY = size.height() / 2;
    QPainter painter(&img);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QBrush(fillcolor));
    painter.drawRect(hotX - size.width() / 2 + weigth,
                     hotY - size.height() / 2 + weigth, hotX + size.width() / 2 - weigth - 2,
                     hotY + size.height() / 2 - weigth - 2);
    painter.setPen(QPen(QBrush(color), weigth));
    painter.setBrush(QBrush(Qt::transparent));

    if (pointingCenter) {
        painter.drawPoint(hotX, hotY);
    }

    painter.drawRect(hotX - size.width() / 2 + weigth,
                     hotY - size.height() / 2 + weigth, hotX + size.width() / 2 - weigth - 2,
                     hotY + size.height() / 2 - weigth - 2);
    painter.end();
    return img;
}

QImage SymbolsResource::getOpenHand() {
    return QImage(":/icon/OpenHand");
}

QImage SymbolsResource::getClosedHand() {
    return QImage(":/icon/ClosedHand");
}

QImage SymbolsResource::getPointingHand() {
    return QImage(":/icon/PointingHand");
}

QImage SymbolsResource::getLeftArrow() {
    QImage img(":/icon/UpArrow");
    QTransform transf;
    img = img.transformed(transf.translate(-img.width() / 2,
                                           -img.height() / 2).rotate(-90).translate(img.width() / 2, img.height() / 2),
                          Qt::SmoothTransformation);
    return img;
}

QImage SymbolsResource::getRightArrow() {
    QImage img(":/icon/UpArrow");
    QTransform transf;
    img = img.transformed(transf.translate(-img.width() / 2,
                                           -img.height() / 2).rotate(90).translate(img.width() / 2, img.height() / 2),
                          Qt::SmoothTransformation);
    return img;
}

QImage SymbolsResource::getMagnifyGlass(QString text) {
    QImage mag(":/icon/Magnify");

    if (text.isEmpty()) {
        return mag;
    }

    QPainter painter(&mag);
    painter.setPen(QPen(Qt::black));
    painter.drawText(1, 21, 10, 10, Qt::AlignCenter, text);
    painter.end();
    return mag;
}

QImage SymbolsResource::getText(QString text, bool bottom) {
    QImage txt(QSize(64, 64), QImage::Format_ARGB32);
    txt.fill(QColor(0, 0, 0, 0).rgba());
    QPainter painter(&txt);
    painter.setPen(QPen(Qt::yellow));

    if (bottom) {
        painter.drawText(0, 52, text);
    } else {
        painter.drawText(0, 0, text);
    }

    painter.end();
    return txt;
}



Marker::Marker(const QImage& image2Mark) :
    QImage(image2Mark) {
}

Marker::Marker(DefaultMark defaultMarkType) {
    switch (defaultMarkType) {
    case NoMark:
        *this = Marker(SymbolsResource::getBackGround(QColor(0, 0, 0, 0)));
        break;

    default:
        *this = Marker(SymbolsResource::getBordedX(QColor(255, 255, 255, 255), QColor(0,
                       0, 0, 255)));
        break;
    }
}

Coord::Coord() {
    marker_ = NULL;
}

Coord::Coord(QPointF location, QString label, Marker* mark) :
    QPointF(location.x(), location.y()) {
    label_ = label;
    marker_ = mark;
}

Geometry::Geometry() {
    type_ = 0;
}

Geometry::Geometry(const Geometry& other) {
    points_ = other.listPoints();
    type_ = other.type();
    key_ = other.key();
}

Geometry Geometry::createPoint(QPointF coord, unsigned int pointKey,
                               QString label, Marker* mark) {
    Geometry result;
    result.type_ = 1;
    result.points_.append(Coord(coord, label, mark));
    result.key_ = pointKey;
    return result;
}

int Geometry::type() const {
    return type_;
}

unsigned int Geometry::key() const {
    return key_;
}

QList<Coord> Geometry::listPoints() const {
    return points_;
}

GeometryResource::GeometryResource() {
    defaultMark_ = new Marker(SymbolsResource::getCircle(QColor(0, 0, 0),
                              QColor(128,
                                     255, 128), QSize(8, 8), 1, false));
    selectedMark_  = new Marker(SymbolsResource::getCircle(QColor(0, 0, 0),
                                QColor(128, 255, 128, 32), QSize(10, 10), 2, true));
}

void GeometryResource::insertPoint(QPointF location, unsigned int pointKey,
                                   QString label, Marker* mark) {
    if (pointKey == 0) {
        unsigned int key = generatePointKey();
        geometries_.append(Geometry::createPoint(location, key, label, mark));

        if (nextPointkey_ <= key) {
            nextPointkey_ = key++;
        }
    } else {
        unsigned int key = pointKey;

        for (int i = geometries_.size() - 1; i >= 0; i--) {
            if (geometries_.at(i).key() == key) {
                geometries_.removeAt(i);
            }
        }

        geometries_.append(Geometry::createPoint(location, key, label, mark));

        if (nextPointkey_ <= key) {
            nextPointkey_ = key++;
        }
    }
}

void GeometryResource::addPoint(QPointF location, unsigned int pointKey,
                                QString label, Marker* mark) {
    if (pointKey == 0) {
        pointKey = nextPointkey_;
    }

    geometries_.append(Geometry::createPoint(location, pointKey, label, mark));

    if (nextPointkey_ <= unsigned(pointKey)) {
        nextPointkey_ = pointKey++;
    }
}

void GeometryResource::updatePoint(QPointF location, unsigned int pointKey,
                                   QString label, Marker* mark) {
    if (pointKey == 0) {
        return;
    } else {
        unsigned int key = pointKey;
        bool removed = false;

        for (int i = geometries_.size() - 1; i >= 0; i--) {
            if (geometries_.at(i).key() == key) {
                geometries_.removeAt(i);
                removed = true;
            }
        }

        if (removed) {
            geometries_.append(Geometry::createPoint(location, key, label, mark));

            if (nextPointkey_ <= key) {
                nextPointkey_ = key++;
            }
        }
    }
}

void GeometryResource::deletePoint(unsigned int pointKey) {
    unsigned int key = pointKey;

    for (int i = geometries_.size() - 1; i >= 0; i--) {
        if (geometries_.at(i).key() == key) {
            geometries_.removeAt(i);
        }
    }
}

void GeometryResource::clear() {
    geometries_.clear();
}

QImage GeometryResource::draw(QImage dst, QSize targetSize, QPointF viewpoint,
                              double scale) {
    if (df_ != NULL) {
        dst = draw(df_, featureProjection_, dst, targetSize, viewpoint, scale);
    }

    QImage result(dst);
    QPainter painter(&result);

    for (int i = 0; i < geometries_.size(); i++) {
        if (geometries_.at(i).type() == 1) {
            QList<Coord> points = geometries_.at(i).listPoints();

            for (int j = 0; j < points.size(); j++) {
                Marker* mark = points.at(j).marker_;
                QString label = points.at(j).label_;
                double x = (points.at(j).x() - (viewpoint.x() - targetSize.width() /
                                                (2.0 * scale))) * scale;
                double y = (points.at(j).y() - (viewpoint.y() - targetSize.height() /
                                                (2.0 * scale))) * scale;
                painter.drawImage(x - mark->width() / 2, y - mark->height() / 2, *mark);
                painter.setPen(QPen(Qt::yellow));
                painter.setFont(QFont("Arial", 10));
                painter.drawText(x + mark->width() / 2, y + mark->height() / 2, label);
            }
        } else if  (geometries_.at(i).type() == 2) {
            painter.setPen(QPen(Qt::green));
            QList<Coord> points = geometries_.at(i).listPoints();
            size_t nsize = points.size();
            double* x = new double[nsize];
            double* y = new double[nsize];

            for (int j = 0; j < points.size(); j++) {
                x[j] = (points.at(j).x() - (viewpoint.x() - targetSize.width() /
                                            (2.0 * scale))) * scale;
                y[j] = (points.at(j).y() - (viewpoint.y() - targetSize.height() /
                                            (2.0 * scale))) * scale;
            }

            for (int j = 0; j < points.size() - 1; j++) {
                int k = (j + 1) % points.size();
                painter.drawLine(QPointF(x[j], y[j]), QPointF(x[k], y[k]));
            }

            delete[] x;
            delete[] y;
        }
    }

    painter.end();
    return result;
}

QImage GeometryResource::draw(DemFeatures* dfs, int projection, QImage dst,
                              QSize targetSize, QPointF viewpoint, double scale) const {
    QImage result(dst);
    QPainter painter(&result);

    for (int i = 0; i < dfs->getNumFeatures(); i++) {
        if (dfs->selectedFeature() == i + 1) {
            continue;
        }

        DemFeature* df = dfs->getFeatureLink(i + 1);
        QVector<QPointF> poly;

        for (unsigned int j = 0; j < df->points.size(); j++) {
            double x = 0;
            double y = 0;

            if (projection == 0) {
                x = df->points.at(j).left_x;
                y = df->points.at(j).left_y;
            } else if (projection == 1) {
                x = df->points.at(j).right_x;
                y = df->points.at(j).right_y;
            }

            x = (x - (viewpoint.x() - targetSize.width() / (2.0 * scale))) * scale;
            y = (y - (viewpoint.y() - targetSize.height() / (2.0 * scale))) * scale;
            poly.append(QPointF(x, y));
        }

        painter.setPen(QPen(QColor(Qt::darkGreen)));
        painter.setBrush(QBrush(QColor(128, 128, 0, 128)));

        if (df->feature_type == 3) { // preenchimento e fechamento
            painter.drawPolygon(poly);
        }

        if (df->feature_type == 2) { // linhas
            painter.drawPolyline(poly);
        }

        if (true) { // pontos
            Marker* mark = defaultMark_;

            for (int j = 0; j < poly.size(); j++) {
                QString label = QString::number(j + 1);
                double x = poly.at(j).x();
                double y = poly.at(j).y();
                painter.drawImage(x - mark->width() / 2, y - mark->height() / 2, *mark);
                painter.setPen(QPen(Qt::yellow));
                painter.setFont(QFont("Arial", 10));
                painter.drawText(x + mark->width() / 2 + 2, y + mark->height() / 2, label);
            }
        }
    }

    if (dfs->selectedFeature() > 0) {
        DemFeature* df = dfs->getFeatureLink(dfs->selectedFeature());
        QVector<QPointF> poly;

        if (df == NULL) {
            return result;
        }

        for (unsigned int j = 0; j < df->points.size(); j++) {
            double x = 0;
            double y = 0;

            if (projection == 0) {
                x = df->points.at(j).left_x;
                y = df->points.at(j).left_y;
            } else if (projection == 1) {
                x = df->points.at(j).right_x;
                y = df->points.at(j).right_y;
            }

            x = (x - (viewpoint.x() - targetSize.width() / (2.0 * scale))) * scale;
            y = (y - (viewpoint.y() - targetSize.height() / (2.0 * scale))) * scale;
            poly.append(QPointF(x, y));
        }

        painter.setPen(QPen(QBrush(QColor(Qt::darkGreen)), 3));
        painter.setBrush(QBrush(QColor(Qt::darkYellow), Qt::Dense7Pattern));

        if (df->feature_type == 3) { // preenchimento e fechamento
            painter.drawPolygon(poly);
        }

        if (df->feature_type == 2) { // linhas
            painter.drawPolyline(poly);
        }

        if (true) { // pontos
            Marker* mark = defaultMark_;

            for (int j = 0; j < poly.size(); j++) {
                if (dfs->selectedPoint() - 1 == j) {
                    Marker* mark = selectedMark_;
                    QString label = QString::number(j + 1);
                    double x = poly.at(j).x();
                    double y = poly.at(j).y();
                    painter.drawImage(x - mark->width() / 2, y - mark->height() / 2, *mark);
                    painter.setPen(QPen(Qt::yellow));
                    painter.setFont(QFont("Arial", 10));
                    painter.drawText(x + mark->width() / 2 + 2, y + mark->height() / 2, label);
                } else {
                    QString label = QString::number(j + 1);
                    double x = poly.at(j).x();
                    double y = poly.at(j).y();
                    painter.drawImage(x - mark->width() / 2, y - mark->height() / 2, *mark);
                    painter.setPen(QPen(Qt::yellow));
                    painter.setFont(QFont("Arial", 10));
                    painter.drawText(x + mark->width() / 2 + 2, y + mark->height() / 2, label);
                }
            }
        }
    }

    painter.end();
    return result;
}

unsigned int GeometryResource::generatePointKey() const {
    return nextPointkey_;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
