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

#ifdef WIN32
#define _USE_MATH_DEFINES // for C++
#include <cmath>
#endif
#ifdef unix
#include <math.h>
#endif

#include "StereoTools.h"

#include <QFileDialog>
#include "StereoDisplay.h"

#define NOCURSOR QPixmap::fromImage(SymbolsResource::getBackGround(QColor(0,0,0,0)))

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

void StereoTool::paintEvent(const QPaintEvent& event) {
    event.isAccepted();

    if (display_->painting()) {
#ifdef WIN32
        QPainter painter(_display);
#endif
#ifdef unix
        QPainter painter(display_->getRealDisplay());
#endif
        painter.setRenderHint(QPainter::Antialiasing);

        if (autoPan_ != QPointF(0, 0)) {
            // Draw autoMove feedback
            QPoint endArrow(lastMousePosition_ - (autoPan_ * 5 *
                                                  display_->getCurrentScene()->getScale()).toPoint());
            painter.setPen(QPen(QBrush(Qt::yellow), 7, Qt::SolidLine, Qt::RoundCap));
            painter.drawPoint(lastMousePosition_);
            painter.setPen(QPen(QBrush(Qt::yellow), 2, Qt::SolidLine, Qt::RoundCap));
            painter.drawLine(lastMousePosition_, endArrow);

            if (autoPan_ != QPointF(0, 0)) {
                double tangent = atan2(autoPan_.y(), autoPan_.x());
                QPoint pa(7 * cos (tangent + M_PI / 7) + endArrow.x(),
                          7 * sin (tangent + M_PI / 7) + endArrow.y());
                QPoint pb(7 * cos (tangent - M_PI / 7) + endArrow.x(),
                          7 * sin (tangent - M_PI / 7) + endArrow.y());
                QVector<QPoint> arrow;
                arrow.append(pa);
                arrow.append(endArrow);
                arrow.append(endArrow);
                arrow.append(pb);
                painter.setPen(QPen(QBrush(Qt::yellow), 2, Qt::SolidLine, Qt::SquareCap,
                                    Qt::MiterJoin));
                painter.drawLines(arrow);
            }
        } else if (scale_ > 0) {
            // Draw scaleBar feedback
            QPoint endBar(display_->screenPosition(fixedPointOnImageLeft_).x(),
                          display_->getMouseScreenPosition().y());
            painter.setPen(QPen(QBrush(Qt::yellow), 7, Qt::SolidLine, Qt::RoundCap));
            painter.drawPoint(display_->screenPosition(fixedPointOnImageLeft_));
            painter.setPen(QPen(QBrush(Qt::yellow), 2));
            painter.drawLine(display_->screenPosition(fixedPointOnImageLeft_), endBar);
            painter.drawLine(QPoint(endBar.x() - 3, endBar.y()), QPoint(endBar.x() + 3,
                             endBar.y()));
            painter.drawText(QPoint(endBar.x() + 5, endBar.y() + 5),
                             QString::number(display_->getCurrentScene()->getScale() * 100, 'f',
                                             1).append("%"));
        }

        painter.end();
    }
}

void StereoTool::resizeEvent(const QResizeEvent& event) {
    display_->getLeftDisplay()->resize(event.size());
    display_->getRightDisplay()->resize(event.size());
}

void StereoTool::enterEvent(const QHoverEvent& event) {
    event.isAccepted();
    display_->setCursor(currentCursor_, display_->isStereoCursor());
}

void StereoTool::leaveEvent(const QHoverEvent& event) {
    event.isAccepted();
    display_->setCursor(SymbolsResource::getBackGround(QColor(0, 0, 0, 0)),
                        display_->isStereoCursor());
    display_->updateAll();
}

void StereoTool::moveEvent(const QHoverEvent& event) {
    // Update mouse position only
    display_->updateMousePosition();

    if (actualizePosLabel_) {
        actualizePosLabel();
    }

    if (display_->isStereoCursor()) {
        display_->getCurrentScene()->getLeftScene()->setDetailedPoint(
            display_->getPositionLeft(event.pos() +
                                      display_->getLeftCursorOffset().toPoint()));
        display_->getCurrentScene()->getRightScene()->setDetailedPoint(
            display_->getPositionRight(event.pos() +
                                       display_->getRightCursorOffset().toPoint()));
    } else {
        display_->getCurrentScene()->getLeftScene()->setDetailedPoint(
            display_->getPositionLeft(event.pos()));
        display_->getCurrentScene()->getRightScene()->setDetailedPoint(
            display_->getPositionRight(event.pos()));
    }

    if (!hasButtomPressed_) {
        display_->updateAll();
    }

    emit mouseMoved(display_->getPositionLeft(event.pos() +
                    display_->getLeftCursorOffset().toPoint()),
                    display_->getPositionRight(event.pos() +
                            display_->getRightCursorOffset().toPoint()));
}

void StereoTool::mousePressed(const QMouseEvent& event) {
    hasButtomPressed_ = true;
    lastCursor_ = display_->getCursor();

    // Prepair to zoom default (scaleBar).
    if (event.buttons() & Qt::MidButton) {
        fixedPoint_ = event.pos();
        fixedPointOnImageLeft_ = display_->getPositionLeft(fixedPoint_);
        fixedPointOnImageRight_ = display_->getPositionRight(fixedPoint_);
        scale_ = display_->getCurrentScene()->getScale();
        currentCursor_ = SymbolsResource::getLeftArrow();
        display_->setCursor(currentCursor_, display_->isStereoCursor());
        actualizePosLabel_ = false;
    }
    // Prepair move reference
    else if (event.buttons() & Qt::RightButton) {
        currentCursor_ = SymbolsResource::getBackGround(QColor(0, 0, 0, 0));
        display_->setCursor(currentCursor_, display_->isStereoCursor());
    }

    lastMousePosition_ = event.pos();

    if ((event.button() & Qt::LeftButton) || (event.button() & Qt::MidButton)) {
        display_->getCurrentScene()->getLeftScene()->setDetailedPoint(
            display_->getPositionLeft(event.pos() +
                                      display_->getLeftCursorOffset().toPoint()));
        display_->getCurrentScene()->getRightScene()->setDetailedPoint(
            display_->getPositionRight(event.pos() +
                                       display_->getRightCursorOffset().toPoint()));
    }
}

void StereoTool::mouseReleased(const QMouseEvent& event) {
    hasButtomPressed_ = false;
    currentCursor_ = lastCursor_;
    display_->setCursor(currentCursor_, display_->isStereoCursor());
    // Stop move default (autoMove).
    autoPan_ = QPoint(0, 0);
    // Stop zoom default (scaleBar).
    scale_ = -1;

    if (!actualizePosLabel_) { // OnScaleBar
    } else {
        display_->getCurrentScene()->getLeftScene()->setDetailedPoint(
            display_->getPositionLeft(event.pos() +
                                      display_->getLeftCursorOffset().toPoint()));
        display_->getCurrentScene()->getRightScene()->setDetailedPoint(
            display_->getPositionRight(event.pos() +
                                       display_->getRightCursorOffset().toPoint()));
    }

    display_->updateAll();
}

void StereoTool::mouseMoved(const QMouseEvent& event) {
    // Make zoom default (scaleBar).
    if (event.buttons() & Qt::MidButton) {
        int diff = event.pos().y() - display_->screenPosition(
                       fixedPointOnImageLeft_).y();
        double newScale = (scale_ * 100 - diff) / 100;
        display_->getCurrentScene()->getLeftScene()->scaleTo(newScale,
                fixedPointOnImageLeft_);
        display_->getCurrentScene()->getRightScene()->scaleTo(newScale,
                fixedPointOnImageRight_);

        if (event.pos().x() < display_->screenPosition(fixedPointOnImageLeft_).x()) {
            currentCursor_ = SymbolsResource::getRightArrow();
            display_->setCursor(currentCursor_, display_->isStereoCursor());
        } else {
            currentCursor_ = SymbolsResource::getLeftArrow();
            display_->setCursor(currentCursor_, display_->isStereoCursor());
        }

        display_->updateAll();
    }

    // Make move default (autoMove).
    if (event.buttons() & Qt::RightButton) {
        double scale = display_->getCurrentScene()->getScale();
        QPointF diff = event.posF() - lastMousePosition_;
        autoPan_ = -(diff / (5 * scale));
        currentCursor_ = SymbolsResource::getText(QString::fromUtf8("Auto"));
        display_->setCursor(currentCursor_, display_->isStereoCursor());
    }
}

void StereoTool::wheelEvent(const QWheelEvent& event) {
    double numSteps = event.delta() / (15.0 * 8.0);

    if (event.orientation() == Qt::Vertical) {
        if (display_->isStereoCursor()) {
            if (event.modifiers() == Qt::ShiftModifier) {
                display_->getCurrentScene()->getRightScene()->pan(QPointF(numSteps, 0));
            } else if (event.modifiers() == Qt::ControlModifier) {
                display_->getCurrentScene()->getRightScene()->pan(QPointF(0, numSteps));
            } else {
                display_->setLeftCursorOffset(QPointF(display_->getLeftCursorOffset().x() +
                                                      numSteps / fabs(numSteps), 0));
                display_->setRightCursorOffset(QPointF(display_->getRightCursorOffset().x() -
                                                       numSteps / fabs(numSteps), 0));
            }

            actualizePosLabel();
            emit mouseMoved(display_->getPositionLeft(event.pos() +
                            display_->getLeftCursorOffset().toPoint()),
                            display_->getPositionRight(event.pos() +
                                    display_->getRightCursorOffset().toPoint()));
            display_->updateAll();
        }
    }
}

void StereoTool::autoMove() {
    // Execute move default (autoMove).
    if (autoPan_ != QPointF(0, 0)) {
        display_->getCurrentScene()->getLeftScene()->pan(-autoPan_);
        display_->getCurrentScene()->getRightScene()->pan(-autoPan_);
        actualizePosLabel();
        display_->updateAll();
    }
}

void StereoTool::actualizePosLabel() {
    {
        // Actualize X,Y coordinates
        int leftImageWidth = display_->getCurrentScene()->getLeftScene()->getWidth();
        int leftImageHeight = display_->getCurrentScene()->getLeftScene()->getHeight();
        int rightImageWidth = display_->getCurrentScene()->getRightScene()->getWidth();
        int rightImageHeight =
            display_->getCurrentScene()->getRightScene()->getHeight();
        display_->updateMousePosition();
        QPointF mp = display_->getMouseScreenPosition();
        QPointF pl = display_->getPositionLeft((mp +
                                                display_->getLeftCursorOffset()).toPoint());
        QPointF pr = display_->getPositionRight((mp +
                                                display_->getRightCursorOffset()).toPoint());
        QString leftInfo = QString("Left: ") + QString::number(pl.x(), 'f',
                           2) + QString(" x ") + QString::number(pl.y(), 'f', 2);
        QString rightInfo = QString("Right: ") + QString::number(pr.x(), 'f',
                            2) + QString(" x ") + QString::number(pr.y(), 'f', 2);

        // Clear if necessary
        if (pl.x() < 0 || pl.y() < 0 || pl.x() > leftImageWidth
                || pl.y() > leftImageHeight) {
            leftInfo = "   ";
        }

        if (pr.x() < 0 || pr.y() < 0 || pr.x() > rightImageWidth
                || pr.y() > rightImageHeight) {
            rightInfo = "   ";
        }

        // Reset label
        leftPosLabel_->setText(leftInfo);
        rightPosLabel_->setText(rightInfo);
    }
}

void StereoTool::setCursor(QImage cursor, bool stereo) {
    currentCursor_ = cursor;
    display_->setCursor(currentCursor_, stereo);
}

ZoomStereoTool::ZoomStereoTool(StereoDisplay* display) :
    StereoTool(display) {
    onRubberBand_ = false;
}

ZoomStereoTool::~ZoomStereoTool() {
}

void ZoomStereoTool::paintEvent(const QPaintEvent& event) {
    if (onRubberBand_) {
        QRect rubber(fixedPoint_, display_->getMouseScreenPosition().toPoint());

        if (abs(rubber.width()) < 8 && abs(rubber.height()) < 8) {
            currentCursor_ = SymbolsResource::getMagnifyGlass("-");
            display_->setCursor(currentCursor_, false);
            return;
        } else {
            currentCursor_ = SymbolsResource::getMagnifyGlass("+");
            display_->setCursor(currentCursor_, false);
        }

        display_->getRealDisplay()->updateGL();
        QPainter painter(display_->getRealDisplay());
        painter.setPen(QPen(Qt::yellow));
        painter.drawRect(QRect(fixedPoint_,
                               display_->getMouseScreenPosition().toPoint()));
        painter.end();
    }

    StereoTool::paintEvent(event);
}

void ZoomStereoTool::mousePressed(const QMouseEvent& event) {
    if (event.button() != event.buttons()) {
        return;
    }

    // Rubberband zoom
    if (event.buttons() & Qt::LeftButton) {
        fixedPoint_ = event.pos();
        onRubberBand_ = true;
        lastCursor_ = display_->getCursor();
        currentCursor_ = SymbolsResource::getMagnifyGlass("-");
        display_->setCursor(currentCursor_, false);
        display_->updateAll();
        return;
    }

    StereoTool::mousePressed(event);
}

void ZoomStereoTool::mouseReleased(const QMouseEvent& event) {
    // Rubberband zoom
    if (onRubberBand_) {
        QRect rubber(fixedPoint_, event.pos());

        if (abs(rubber.width()) < 8 && abs(rubber.height()) < 8) {
            display_->getCurrentScene()->getLeftScene()->zoom(0.8,
                    display_->getPositionLeft(fixedPoint_));
            display_->getCurrentScene()->getRightScene()->zoom(0.8,
                    display_->getPositionRight(fixedPoint_));
        } else {
            double wscale = abs(rubber.width()) == 0 ? 1024 : display_->width() /
                            (double)abs(rubber.width());
            double hscale = abs(rubber.height()) == 0 ? 1024 : display_->height() /
                            (double)abs(rubber.height());
            display_->getCurrentScene()->getLeftScene()->moveTo(display_->getPositionLeft(
                        rubber.center()));
            display_->getCurrentScene()->getLeftScene()->zoom(wscale < hscale ? wscale :
                    hscale);
            display_->getCurrentScene()->getRightScene()->moveTo(display_->getPositionRight(
                        rubber.center()));
            display_->getCurrentScene()->getRightScene()->zoom(wscale < hscale ? wscale :
                    hscale);
        }

        onRubberBand_ = false;
    }

    StereoTool::mouseReleased(event);
}

void ZoomStereoTool::mouseMoved(const QMouseEvent& event) {
    // Rubberband zoom.
    if (event.buttons() & Qt::LeftButton) {
        display_->updateAll();
        return;
    }

    StereoTool::mouseMoved(event);
}

MoveStereoTool::MoveStereoTool(StereoDisplay* display) :
    StereoTool(display) {
}

MoveStereoTool::~MoveStereoTool() {
}

void MoveStereoTool::mousePressed(const QMouseEvent& event) {
    if (event.button() != event.buttons()) {
        return;
    }

    StereoTool::mousePressed(event);

    if (event.buttons() & Qt::LeftButton) {
        currentCursor_ = SymbolsResource::getClosedHand();
        display_->setCursor(currentCursor_, false);
    }

    display_->updateAll();
}

void MoveStereoTool::mouseReleased(const QMouseEvent& event) {
    StereoTool::mouseReleased(event);
    currentCursor_ = SymbolsResource::getOpenHand();
    display_->setCursor(currentCursor_, false);
}

void MoveStereoTool::mouseMoved(const QMouseEvent& event) {
    // Move
    if (event.buttons() & Qt::LeftButton) {
        QPointF diff = event.posF() - lastMousePosition_;
        lastMousePosition_ = event.pos();
        double scale;
        scale = display_->getCurrentScene()->getScale();

        if (event.modifiers() != Qt::ShiftModifier) {
            display_->getCurrentScene()->getLeftScene()->pan(-(diff / scale));
        }

        if (event.modifiers() != Qt::ControlModifier) {
            display_->getCurrentScene()->getRightScene()->pan(-(diff / scale));
        }

        display_->updateAll();
        return;
    }

    StereoTool::mouseMoved(event);
}

MarkStereoTool::MarkStereoTool(StereoDisplay* display) :
    StereoTool{display},
    mark_(WhiteMark) {
}

MarkStereoTool::~MarkStereoTool() {
}

void MarkStereoTool::changeMarker(Marker marker) {
    mark_ = marker;
}

Marker* MarkStereoTool::getMarker() {
    return &mark_;
}

void MarkStereoTool::addMark(QPointF lPos, QPointF rPos, int key, QString label,
                             Marker* marker) {
    display_->getCurrentScene()->getLeftScene()->geometry()->addPoint(lPos, key,
            label, marker == NULL ? &mark_ : marker);
    display_->getCurrentScene()->getRightScene()->geometry()->addPoint(rPos, key,
            label, marker == NULL ? &mark_ : marker);
}

void MarkStereoTool::insertMark(QPointF lPos, QPointF rPos, int key,
                                QString label, Marker* marker) {
    display_->getCurrentScene()->getLeftScene()->geometry()->insertPoint(lPos, key,
            label, marker == NULL ? &mark_ : marker);
    display_->getCurrentScene()->getRightScene()->geometry()->insertPoint(rPos, key,
            label, marker == NULL ? &mark_ : marker);
}

void MarkStereoTool::deleteMark(int key) {
    display_->getCurrentScene()->getLeftScene()->geometry()->deletePoint(key);
    display_->getCurrentScene()->getRightScene()->geometry()->deletePoint(key);
}

void MarkStereoTool::clear() {
    display_->getCurrentScene()->getLeftScene()->geometry()->clear();
    display_->getCurrentScene()->getRightScene()->geometry()->clear();
}

void MarkStereoTool::setToOnlyEmitClickedMode() {
    onlyEmitClickedMode_ = true;
}

void MarkStereoTool::putClickOn(QPointF& lPos, QPointF& rPos) {
    if (onlyEmitClickedMode_) {
        emit clicked(lPos, rPos);
    } else {
        display_->getCurrentScene()->getLeftScene()->geometry()->insertPoint(lPos,
                nextMarkItem_, QString::number(nextMarkItem_), &mark_);
        display_->getCurrentScene()->getRightScene()->geometry()->insertPoint(rPos,
                nextMarkItem_, QString::number(nextMarkItem_), &mark_);
        display_->updateAll();
        nextMarkItem_++;
    }
}

void MarkStereoTool::mousePressed(const QMouseEvent& event) {
    // Add mark
    if (event.buttons() & Qt::LeftButton) {
        QPointF lLocal = display_->getPositionLeft(event.pos() +
                         display_->getLeftCursorOffset().toPoint());
        QPointF rLocal = display_->getPositionRight(event.pos() +
                         display_->getRightCursorOffset().toPoint());

        if ((lLocal.x() >= 0 && lLocal.y() >= 0
                && lLocal.x() <= display_->getCurrentScene()->getLeftScene()->getWidth()
                && lLocal.y() <= display_->getCurrentScene()->getLeftScene()->getHeight())
                && (rLocal.x() >= 0 && rLocal.y() >= 0
                    && rLocal.x() <= display_->getCurrentScene()->getRightScene()->getWidth()
                    && rLocal.y() <= display_->getCurrentScene()->getRightScene()->getHeight())) {
            putClickOn(lLocal, rLocal);
        }
    }

    StereoTool::mousePressed(event);
}

NearStereoTool::NearStereoTool(StereoDisplay* display) :
    StereoTool(display) {
    nearDock_ = new QDockWidget("Detailview");
    nearDock_->setFeatures(QDockWidget::DockWidgetClosable |
                           QDockWidget::NoDockWidgetFeatures | QDockWidget::DockWidgetFloatable |
                           QDockWidget::DockWidgetMovable);
    leftNear_ = display_->getLeftNearDisplay();
    rightNear_ = display_->getRightNearDisplay();
    leftNear_->setActivatedTool(this);
    rightNear_->setActivatedTool(this);
    leftNear_->setCursor(QPixmap::fromImage(SymbolsResource::getBordedCross(QColor(
            255, 255, 255, 255), QColor(0, 0, 0, 255), QSize(25, 25))));
    rightNear_->setCursor(QPixmap::fromImage(SymbolsResource::getBordedCross(QColor(
                              255, 255, 255, 255), QColor(0, 0, 0, 255), QSize(25, 25))));
    QWidget* widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(widget);
    layout->addWidget(leftNear_);
    layout->addWidget(rightNear_);
    widget->setLayout(layout);
    nearDock_->setWidget(widget);
}

NearStereoTool::~NearStereoTool() {
}

QDockWidget* NearStereoTool::getNearDock() {
    return nearDock_;
}

SingleDisplay* NearStereoTool::getLeftNear() {
    return leftNear_;
}

SingleDisplay* NearStereoTool::getRightNear() {
    return rightNear_;
}

bool NearStereoTool::nearIsVisible() {
    return nearDock_->isVisible();
}

void NearStereoTool::setNearVisible(bool status) {
    if (status && !nearDock_->isVisible()) {
        nearDock_->show();
    }

    nearDock_->setHidden(!status);
}

void NearStereoTool::paintEvent(const QPaintEvent& event) {
    event.isAccepted();
    //* this Code Cause Paint Warnings!
    QPixmap ico = QPixmap::fromImage(display_->getCursor());
    QRect reg(QPoint(), ico.size());

    if (leftNear_->painting()) {
        QPainter painter(leftNear_);
        {
            reg.moveCenter(QPoint(leftNear_->width() / 2, leftNear_->height() / 2));
            painter.drawPixmap(reg, ico);
        }
    }

    if (rightNear_->painting()) {
        QPainter painter(rightNear_);
        {
            reg.moveCenter(QPoint(rightNear_->width() / 2, rightNear_->height() / 2));
            painter.drawPixmap(reg, ico);
        }
    }
}

void NearStereoTool::resizeEvent(const QResizeEvent& event) {
    event.isAccepted();
}

void NearStereoTool::enterEvent(const QHoverEvent& event) {
    event.isAccepted();
}

void NearStereoTool::leaveEvent(const QHoverEvent& event) {
    event.isAccepted();
}

void NearStereoTool::moveEvent(const QHoverEvent& event) {
    event.isAccepted();
}

void NearStereoTool::mousePressed(const QMouseEvent& event) {
    event.isAccepted();
}

void NearStereoTool::mouseReleased(const QMouseEvent& event) {
    event.isAccepted();
}

void NearStereoTool::mouseMoved(const QMouseEvent& event) {
    event.isAccepted();
}

void NearStereoTool::wheelEvent(const QWheelEvent& event) {
    if (display_->isStereoCursor()) {
        display_->getCurrentScene()->getLeftScene()->setDetailedPoint(
            display_->getPositionLeft(event.pos() +
                                      display_->getLeftCursorOffset().toPoint()));
        display_->getCurrentScene()->getRightScene()->setDetailedPoint(
            display_->getPositionRight(event.pos() +
                                       display_->getRightCursorOffset().toPoint()));
    }

    leftNear_->update();
    rightNear_->update();
    event.isAccepted();
}



OverStereoTool::OverStereoTool(StereoDisplay* display) :
    StereoTool(display) {
    overDock_ = new QDockWidget("Overview");
    overDock_->setFeatures(QDockWidget::DockWidgetClosable |
                           QDockWidget::NoDockWidgetFeatures | QDockWidget::DockWidgetFloatable |
                           QDockWidget::DockWidgetMovable);
    leftOver_ = display_->getLeftOverDisplay();
    rightOver_ = display_->getRightOverDisplay();
    QWidget* widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(widget);
    layout->addWidget(leftOver_);
    layout->addWidget(rightOver_);
    widget->setLayout(layout);
    overDock_->setWidget(widget);
}

OverStereoTool::~OverStereoTool() {
}

QDockWidget* OverStereoTool::getOverDock() {
    return overDock_;
}

SingleDisplay* OverStereoTool::getLeftOver() {
    return leftOver_;
}

SingleDisplay* OverStereoTool::getRightOver() {
    return rightOver_;
}

bool OverStereoTool::overIsVisible() {
    return overDock_->isVisible();
}

void OverStereoTool::setOverVisible(bool status) {
    if (status && !overDock_->isVisible()) {
        overDock_->show();
    }

    overDock_->setHidden(!status);
}

void OverStereoTool::paintEvent(const QPaintEvent& event) {
    event.isAccepted();
}

void OverStereoTool::resizeEvent(const QResizeEvent& event) {
    event.isAccepted();
}

void OverStereoTool::enterEvent(const QHoverEvent& event) {
    event.isAccepted();
}

void OverStereoTool::leaveEvent(const QHoverEvent& event) {
    event.isAccepted();
}

void OverStereoTool::moveEvent(const QHoverEvent& event) {
    event.isAccepted();
    leftOver_->updateMousePosition();
    rightOver_->updateMousePosition();
}

void OverStereoTool::mousePressed(const QMouseEvent& event) {
    event.isAccepted();
}

void OverStereoTool::mouseReleased(const QMouseEvent& event) {
    event.isAccepted();
}

void OverStereoTool::mouseMoved(const QMouseEvent& event) {
    event.isAccepted();
}

void OverStereoTool::wheelEvent(const QWheelEvent& event) {
    event.isAccepted();
}



StereoToolsBar::StereoToolsBar(StereoDisplay* display, QWidget* parent) :
    QToolBar(parent),
    _zoom(display),
    _move(display),
    _mark(display),
    _near(display),
    _over(display) {
    display_ = display;
    setWindowTitle("Display tools");
    setZoomTool = new QAction(QIcon(":/icon/zoomIcon"),
                              "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN"
                              "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Zoom</span></p></body></html>",
                              this);
    setMoveTool = new QAction(QIcon(":/icon/moveIcon"),
                              "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN"
                              "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Move</span></p></body></html>",
                              this);
    setMarkTool = new QAction(QIcon(":/icon/markIcon"),
                              "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN"
                              "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Measure</span></p></body></html>",
                              this);
    showOverview = new QAction(QIcon(":/icon/overIcon"),
                               "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN"
                               "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Overview</span></p></body></html>",
                               this);
    showNearview = new QAction(QIcon(":/icon/detailIcon"),
                               "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN"
                               "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Nearview</span></p></body></html>",
                               this);
    useAntialias = new QAction(QIcon(":/icon/aliasingIcon"),
                               "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN"
                               "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Antialias</span></p></body></html>",
                               this);
    openLeftImage = new QAction(QIcon(":/icon/fileopen.png"),
                                "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN"
                                "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Open Left</span></p></body></html>",
                                this);
    openRightImage = new QAction(QIcon(":/icon/fileopen.png"),
                                 "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN"
                                 "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Open Right</span></p></body></html>",
                                 this);
    saveLeftImage = new QAction(QIcon(":/icon/disquette.png"),
                                "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN"
                                "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Save Left</span></p></body></html>",
                                this);
    saveRightImage = new QAction(QIcon(":/icon/disquette.png"),
                                 "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN"
                                 "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Save Right</span></p></body></html>",
                                 this);
    setFitView = new QAction(QIcon(":/icon/fit.png"),
                             "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN"
                             "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Fit View</span></p></body></html>",
                             this);
    leftInfoLabel_ = new QLabel("   ", 0);
    rightInfoLabel_ = new QLabel("   ", 0);
    stereoInfoLabel_ = new QLabel("   ", 0);
    _zoom.setPosLabel(leftInfoLabel_, rightInfoLabel_, stereoInfoLabel_);
    _move.setPosLabel(leftInfoLabel_, rightInfoLabel_, stereoInfoLabel_);
    _mark.setPosLabel(leftInfoLabel_, rightInfoLabel_, stereoInfoLabel_);
    _near.setPosLabel(leftInfoLabel_, rightInfoLabel_, stereoInfoLabel_);
    _over.setPosLabel(leftInfoLabel_, rightInfoLabel_, stereoInfoLabel_);
    setMarkTool->setShortcut(Qt::CTRL + Qt::Key_Z);
    setMoveTool->setShortcut(Qt::CTRL + Qt::Key_X);
    setZoomTool->setShortcut(Qt::CTRL + Qt::Key_C);
    addAction(openLeftImage);
    addAction(openRightImage);
    addAction(saveLeftImage);
    addAction(saveRightImage);
    addSeparator();
    addAction(setMarkTool);
    addAction(setMoveTool);
    addAction(setZoomTool);
    addSeparator();
    addAction(setFitView);
    addSeparator();
    addAction(useAntialias);
    addSeparator();
    addAction(showOverview);
    addAction(showNearview);
    setZoomTool->setCheckable(true);
    setMoveTool->setCheckable(true);
    setMarkTool->setCheckable(true);
    showOverview->setCheckable(true);
    showNearview->setCheckable(true);
    useAntialias->setCheckable(true);
    showOverview->setChecked(true);
    showNearview->setChecked(true);
    display_->setActivatedTool(&_near);
    QActionGroup* navegation = new QActionGroup(this);
    navegation->addAction(setZoomTool);
    navegation->addAction(setMoveTool);
    navegation->addAction(setMarkTool);
    navegation->setExclusive(true);
    connect(this, SIGNAL(actionTriggered(QAction*)), this,
            SLOT(executeAction(QAction*)));
    currentTool_ = &_move;
    setMoveTool->trigger();
}

SingleDisplay* StereoToolsBar::getLeftNearview() {
    return _near.getLeftNear();
}

SingleDisplay* StereoToolsBar::getRightNearview() {
    return _near.getRightNear();
}

QDockWidget* StereoToolsBar::getNearviews() {
    return _near.getNearDock();
}

SingleDisplay* StereoToolsBar::getLeftOverview() {
    return _over.getLeftOver();
}

SingleDisplay* StereoToolsBar::getRightOverview() {
    return _over.getRightOver();
}

QDockWidget* StereoToolsBar::getOverviews() {
    return _over.getOverDock();
}

void StereoToolsBar::actualizeStereoInfoLabel(double X, double Y, double Z) {
    QString stereoInfo = QString("Object: ") + QString::number(X, 'f',
                         2) + QString(" x ") + QString::number(Y, 'f',
                                 2) + QString(" x ") + QString::number(Z, 'f', 2);
    stereoInfoLabel_->setText(stereoInfo);
}

void  StereoToolsBar::setOpenVisible(bool status) {
    openLeftImage->setVisible(status);
    openRightImage->setVisible(status);
}

void  StereoToolsBar::setSaveVisible(bool status) {
    saveLeftImage->setVisible(status);
    saveRightImage->setVisible(status);
}

void  StereoToolsBar::setMarkVisible(bool status) {
    setMarkTool->setVisible(status);
}

void StereoToolsBar::changeMode(int mode) {
    if (mode ==  1) {
        setMarkTool->trigger();
    }

    if (mode ==  2) {
        setMoveTool->trigger();
    }

    if (mode ==  3) {
        setZoomTool->trigger();
    }
}

void StereoToolsBar::executeAction(QAction* action) {
    if (action ==  setZoomTool ) {
        display_->setActivatedTool(currentTool_, false);
        display_->setActivatedTool(currentTool_ = &_zoom);
        _zoom.setCursor(SymbolsResource::getMagnifyGlass(), false);
        leftInfoLabel_->setHidden(true);
        rightInfoLabel_->setHidden(true);
        stereoInfoLabel_->setHidden(true);
    }

    if (action ==  setMoveTool ) {
        display_->setActivatedTool(currentTool_, false);
        display_->setActivatedTool(currentTool_ = &_move);
        _move.setCursor(SymbolsResource::getOpenHand(), false);
        leftInfoLabel_->setHidden(true);
        rightInfoLabel_->setHidden(true);
        stereoInfoLabel_->setHidden(true);
    }

    if (action ==  setMarkTool ) {
        display_->setActivatedTool(currentTool_, false);
        display_->setActivatedTool(currentTool_ = &_mark);
        _mark.setCursor(SymbolsResource::getBordedCross(QColor(255, 255, 255, 255),
                        QColor(0, 0, 0, 255), QSize(25, 25)), true);
        leftInfoLabel_->setHidden(false);
        rightInfoLabel_->setHidden(false);
        stereoInfoLabel_->setHidden(false);
    }

    if (action ==  setFitView ) {
        display_->fitView();
    }

    if (action ==  showOverview ) {
        _over.setOverVisible(!_over.overIsVisible());
    }

    if (action ==  showNearview ) {
        _near.setNearVisible(!_near.nearIsVisible());
        display_->setActivatedTool(&_near, _near.nearIsVisible());
    }

    if (action == useAntialias) {
        display_->updateAll();
    }

    if (action ==  openLeftImage ) {
        QString filename = QFileDialog::getOpenFileName(0, "Open Image", ".",
                           "*.bmp *.png *.tif *.ppm *.jpg");

        if (!filename.isEmpty()) {
            SingleScene* scene = dynamic_cast<SingleScene*>
                                 (display_->getCurrentScene()->getLeftScene());
            scene->loadImage(filename);
            display_->updateAll();
        }
    }

    if (action ==  saveLeftImage ) {
    }

    if (action ==  openRightImage ) {
        QString filename = QFileDialog::getOpenFileName(0, "Open Image", ".",
                           "*.bmp *.png *.tif *.ppm *.jpg");

        if (!filename.isEmpty()) {
            SingleScene* scene = dynamic_cast<SingleScene*>
                                 (display_->getCurrentScene()->getRightScene());
            scene->loadImage(filename);
            display_->updateAll();
        }
    }

    if (action ==  saveRightImage ) {
    }
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
