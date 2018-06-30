
#ifdef WIN32
#define _USE_MATH_DEFINES // for C++
#include <cmath>
#endif
#ifdef unix
#include <math.h>
#endif

#include "SingleTools.h"
#include "SingleDisplay.h"

#include "math.h"

#include <QFileDialog>
#include <QApplication>

#define NOCURSOR QPixmap::fromImage(SymbolsResource::getBackGround(QColor(0,0,0,0)))

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

void SingleTool::paintEvent(const QPaintEvent& event) {
    event.isAccepted();

    if (display_->painting()) {
        QPainter painter(display_);
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
            QPoint endBar(display_->screenPosition(fixedPointOnImage_).x(),
                          display_->getMouseScreenPosition().y());
            painter.setPen(QPen(QBrush(Qt::yellow), 7, Qt::SolidLine, Qt::RoundCap));
            painter.drawPoint(display_->screenPosition(fixedPointOnImage_));
            painter.setPen(QPen(QBrush(Qt::yellow), 2));
            painter.drawLine(display_->screenPosition(fixedPointOnImage_), endBar);
            painter.drawLine(QPoint(endBar.x() - 3, endBar.y()), QPoint(endBar.x() + 3,
                             endBar.y()));
            painter.drawText(QPoint(endBar.x() + 5, endBar.y() + 5),
                             QString::number(display_->getCurrentScene()->getScale() * 100, 'f',
                                             1).append("%"));
        }
    }
}

void SingleTool::resizeEvent(const QResizeEvent& event) {
    event.isAccepted();
    actualizeScaleSpin(display_->getCurrentScene()->getScale());
}

void SingleTool::enterEvent(const QHoverEvent& event) {
    event.isAccepted();
}

void SingleTool::leaveEvent(const QHoverEvent& event) {
    event.isAccepted();
}

void SingleTool::moveEvent(const QHoverEvent& event) {
    // Update mouse position only
    display_->updateMousePosition();

    if (actualizePosLabel_) {
        actualizePosLabel(display_, true);
    }

    if (!display_->showDetailedArea()) {
        SingleScene* scene = dynamic_cast<SingleScene*>(display_->getCurrentScene());
        scene->setDetailedPoint(display_->getPosition(event.pos()));
        display_->updateDetail();
    }
}

void SingleTool::mousePressed(const QMouseEvent& event) {
    currentCursor_ = display_->cursor();

    // Prepair to zoom default (scaleBar).
    if (event.buttons() & Qt::MidButton) {
        fixedPoint_ = event.pos();
        fixedPointOnImage_ = display_->getPosition(fixedPoint_);
        scale_ = display_->getCurrentScene()->getScale();
        display_->setCursor(QCursor(QPixmap::fromImage(
                                        SymbolsResource::getLeftArrow())));
        actualizePosLabel_ = false;
    }
    // Prepair move reference
    else if (event.buttons() & Qt::RightButton) {
        display_->setCursor(QCursor(NOCURSOR));
    }

    lastMousePosition_ = event.pos();

    if ((event.button() & Qt::LeftButton) || (event.button() & Qt::MidButton)) {
        SingleScene* scene = dynamic_cast<SingleScene*>(display_->getCurrentScene());
        scene->setDetailedPoint(display_->getPosition(event.pos()));
    }
}

void SingleTool::mouseReleased(const QMouseEvent& event) {
    display_->setCursor(currentCursor_);
    // Stop move default (autoMove).
    autoPan_ = QPoint(0, 0);
    // Stop zoom default (scaleBar).
    scale_ = -1;

    if (!actualizePosLabel_) { // OnScaleBar
        QCursor::setPos(display_->mapToGlobal(display_->screenPosition(
                fixedPointOnImage_).toPoint()));
        actualizePosLabel_ = true;
        SingleScene* scene = dynamic_cast<SingleScene*>(display_->getCurrentScene());
        scene->setDetailedPoint(fixedPointOnImage_);
    } else {
        SingleScene* scene = dynamic_cast<SingleScene*>(display_->getCurrentScene());
        scene->setDetailedPoint(display_->getPosition(event.pos()));
    }

    display_->update();
}

void SingleTool::mouseMoved(const QMouseEvent& event) {
    // Make zoom default (scaleBar).
    if (event.buttons() & Qt::MidButton) {
        int diff = event.pos().y() - display_->screenPosition(fixedPointOnImage_).y();
        double newScale = (scale_ * 100 - diff) / 100;
        display_->getCurrentScene()->scaleTo(newScale, fixedPointOnImage_);

        if (event.pos().x() < display_->screenPosition(fixedPointOnImage_).x()) {
            display_->setCursor(QCursor(QPixmap::fromImage(
                                            SymbolsResource::getRightArrow())));
        } else {
            display_->setCursor(QCursor(QPixmap::fromImage(
                                            SymbolsResource::getLeftArrow())));
        }

        actualizeScaleSpin(display_->getCurrentScene()->getScale());
        display_->update();

        if (propagateScaleTo_ != NULL) {
            propagateScaleTo_->propagateScale(newScale,
                                              display_->screenPosition(fixedPointOnImage_).toPoint());
        }
    }

    // Make move default (autoMove).
    if (event.buttons() & Qt::RightButton) {
        double scale = display_->getCurrentScene()->getScale();
        QPointF diff = event.posF() - lastMousePosition_;
        autoPan_ = -(diff / (5 * scale));
        display_->setCursor(QCursor(QPixmap::fromImage(SymbolsResource::getText(
                                        QString::fromUtf8("Auto")))));//,_autoPan.y() < 0))));
    }
}

void SingleTool::wheelEvent(const QWheelEvent& event) {
    event.isAccepted();
}

void SingleTool::autoMove() {
    // Execute move default (autoMove).
    if (autoPan_ != QPointF(0, 0)) {
        display_->getCurrentScene()->pan(-autoPan_);
        SingleScene* scene = dynamic_cast<SingleScene*>(display_->getCurrentScene());
        scene->setDetailedPoint(display_->getLastMousePosition());
        actualizePosLabel(display_, true);
        display_->blockShowDetailedArea(true);
        display_->update();
        display_->blockShowDetailedArea(false);

        if (propagateMoveTo_ != NULL) {
            propagateMoveTo_->propagateMove(-autoPan_);
        }
    }
}

void SingleTool::actualizeScaleSpin(double scale) {
    if (scaleSpin_) {
        scaleSpin_->blockSignals(true);
        scaleSpin_->setValue(scale * 100);
        scaleSpin_->blockSignals(false);
    }
}

void SingleTool::actualizePosLabel(SingleDisplay* display, bool force) {
    if (force || display->visibleRegion().contains(display->mapFromGlobal(
                QCursor::pos()))) {
        // Actualize X,Y coordinates
        int imageWidth = display->getCurrentScene()->getWidth();
        int imageHeight = display->getCurrentScene()->getHeight();
        display->updateMousePosition();
        QPointF p = display->getLastMousePosition();
        QString info = QString::number(xi_ + p.x() * dx_, 'f', 2);

        if (invertY_) {
            info.append(QString(" x ") + QString::number(yi_ + imageHeight * dy_ - p.y()
                        *dy_, 'f', 2));
        } else {
            info.append(QString(" x ") + QString::number(yi_ + p.y()*dy_, 'f', 2));
        }

        // Append Z if necessary
        unsigned int z = -1;

        if (printZ_) {
            z = display_->getCurrentScene()->getGrayColor(p);
            info.append(QString(" x ") + QString::number(zi_ + (z - 1)*dz_, 'f', 2));
        }

        // Clear if necessary
        if (p.x() < 0 || p.y() < 0 || p.x() > imageWidth || p.y() > imageHeight
                || z == 0) {
            info = "   ";
        }

        // Reset label
        posLabel_->setText(info);
    }
}

void SingleTool::propagateMove(QPointF desloc) {
    display_->getCurrentScene()->pan(desloc);
    display_->update();
}

void SingleTool::propagateScale(double scale, QPoint at, int movementMode) {
    if (movementMode == 3) { // to Overview zooming
        display_->getCurrentScene()->scaleTo(scale);
    }

    if (movementMode == 2) { // to Fitview
        display_->getCurrentScene()->centerContent();
        display_->getCurrentScene()->scaleTo(scale);
    } else if (movementMode == 1) { // to Rubberband zooming
        display_->getCurrentScene()->moveTo(display_->getPosition(at));
        display_->getCurrentScene()->scaleTo(scale);
    } else { // to ScaleBar
        display_->getCurrentScene()->scaleTo(scale, display_->getPosition(at));
    }

    display_->update();
    actualizeScaleSpin(display_->getCurrentScene()->getScale());
}

void SingleTool::setImageMode() {
    xi_ = yi_ = zi_ = 0.0;
    dx_ = dy_ = dz_ = 1.0;
    printZ_ = false;
    invertY_ = false;
}

void SingleTool::setOrtoImageMode(double xi, double dx, double yi, double dy) {
    xi_ = xi;
    yi_ = yi;
    zi_ = 0.0;
    dx_ = dx;
    dy_ = dy;
    dz_ = 1.0;
    printZ_ = false;
    invertY_ = true;
}

void SingleTool::setElevationImageMode(double xi, double dx, double yi,
                                       double dy, double zi, double dz) {
    xi_ = xi;
    yi_ = yi;
    zi_ = zi;
    dx_ = dx;
    dy_ = dy;
    dz_ = dz;
    printZ_ = true;
    invertY_ = true;
}



ZoomTool::ZoomTool(SingleDisplay* display) :
    SingleTool(display) {
    onRubberBand_ = false;
}

ZoomTool::~ZoomTool() {
}

void ZoomTool::paintEvent(const QPaintEvent& event) {
    if (onRubberBand_) {
        QRect rubber(fixedPoint_, display_->getMouseScreenPosition().toPoint());

        if (abs(rubber.width()) < 8 && abs(rubber.height()) < 8) {
            display_->setCursor(QCursor(QPixmap::fromImage(
                                            SymbolsResource::getMagnifyGlass("-"))));
            return;
        } else {
            display_->setCursor(QCursor(QPixmap::fromImage(
                                            SymbolsResource::getMagnifyGlass("+"))));
        }

        QPainter painter(display_);
        painter.setPen(QPen(Qt::yellow));
        painter.drawRect(QRect(fixedPoint_,
                               display_->getMouseScreenPosition().toPoint()));
        painter.end();
    }

    SingleTool::paintEvent(event);
}

void ZoomTool::mousePressed(const QMouseEvent& event) {
    if (event.button() != event.buttons()) {
        return;
    }

    // Rubberband zoom
    if (event.buttons() & Qt::LeftButton) {
        fixedPoint_ = event.pos();
        onRubberBand_ = true;
        currentCursor_ = display_->cursor();
        display_->setCursor(QCursor(QPixmap::fromImage(
                                        SymbolsResource::getMagnifyGlass("-"))));
        display_->blockShowDetailedArea(true);
        display_->update();
        return;
    }

    SingleTool::mousePressed(event);
}

void ZoomTool::mouseReleased(const QMouseEvent& event) {
    QMouseEvent e = event;

    // Rubberband zoom
    if (onRubberBand_) {
        QRect rubber(fixedPoint_, event.pos());

        if (abs(rubber.width()) < 8 && abs(rubber.height()) < 8) {
            display_->getCurrentScene()->zoom(0.8, display_->getPosition(fixedPoint_));

            if (propagateScaleTo_ != NULL) {
                propagateScaleTo_->propagateScale(display_->getCurrentScene()->getScale(),
                                                  fixedPoint_);
            }
        } else {
            double wscale = abs(rubber.width()) == 0 ? 1024 : display_->width() /
                            (double)abs(rubber.width());
            double hscale = abs(rubber.height()) == 0 ? 1024 : display_->height() /
                            (double)abs(rubber.height());
            display_->getCurrentScene()->moveTo(display_->getPosition(rubber.center()));
            display_->getCurrentScene()->zoom(wscale < hscale ? wscale : hscale);
            QCursor::setPos(display_->mapToGlobal(display_->screenPosition(
                    display_->getCurrentScene()->getViewpoint()).toPoint()));
            e = QMouseEvent(QEvent::MouseButtonRelease,
                            display_->screenPosition(display_->getCurrentScene()->getViewpoint()).toPoint(),
                            event.button(), event.buttons(), event.modifiers());
            //_actualizePosLabel = true;

            if (propagateScaleTo_ != NULL) {
                propagateScaleTo_->propagateScale(display_->getCurrentScene()->getScale(),
                                                  rubber.center(), 1);
            }
        }

        onRubberBand_ = false;
        display_->blockShowDetailedArea(false);
        actualizeScaleSpin(display_->getCurrentScene()->getScale());
    }

    SingleTool::mouseReleased(e);
}

void ZoomTool::mouseMoved(const QMouseEvent& event) {
    // Rubberband zoom.
    if (event.buttons() & Qt::LeftButton) {
        display_->update();
        return;
    }

    SingleTool::mouseMoved(event);
}

MoveTool::MoveTool(SingleDisplay* display) :
    SingleTool(display) {
}

MoveTool::~MoveTool() {
}

void MoveTool::mousePressed(const QMouseEvent& event) {
    if (event.button() != event.buttons()) {
        return;
    }

    SingleTool::mousePressed(event);

    if (event.buttons() & Qt::LeftButton) {
        display_->setCursor(QPixmap::fromImage(SymbolsResource::getClosedHand()));
    }

    display_->update();
}

void MoveTool::mouseReleased(const QMouseEvent& event) {
    SingleTool::mouseReleased(event);
    display_->setCursor(QPixmap::fromImage(SymbolsResource::getOpenHand()));
}

void MoveTool::mouseMoved(const QMouseEvent& event) {
    // Move
    if (event.buttons() & Qt::LeftButton) {
        QPointF diff = event.posF() - lastMousePosition_;
        lastMousePosition_ = event.pos();
        double scale;
        scale = display_->getCurrentScene()->getScale();
        display_->getCurrentScene()->pan(-(diff / scale));
        display_->update();

        if (propagateMoveTo_ != NULL) {
            propagateMoveTo_->propagateMove(-(diff / scale));
        }

        return;
    }

    SingleTool::mouseMoved(event);
}

MarkTool::MarkTool(SingleDisplay* display) :
    SingleTool(display),
    mark_(GreenMark) {
    onlyEmitClickedMode_ = false;
    nextMarkItem_ = 1;
}

MarkTool::~MarkTool() {
}

void MarkTool::changeMarker(Marker marker) {
    mark_ = marker;
}

Marker* MarkTool::getMarker() {
    return &mark_;
}

void MarkTool::addMark(QPointF location, int key, QString label,
                       Marker* marker) {
    display_->getCurrentScene()->geometry()->addPoint(location, key, label,
            marker == NULL ? &mark_ : marker);
}

void MarkTool::insertMark(QPointF location, int key, QString label,
                          Marker* marker) {
    display_->getCurrentScene()->geometry()->insertPoint(location, key, label,
            marker == NULL ? &mark_ : marker);
}

void MarkTool::deleteMark(int key) {
    display_->getCurrentScene()->geometry()->deletePoint(key);
}

void MarkTool::clear() {
    display_->getCurrentScene()->geometry()->clear();
}

void MarkTool::setToOnlyEmitClickedMode() {
    onlyEmitClickedMode_ = true;
}

void MarkTool::putClickOn(QPointF& pos) {
    if (onlyEmitClickedMode_) {
        emit clicked(pos);
    } else {
        display_->getCurrentScene()->geometry()->insertPoint(pos, nextMarkItem_,
                QString::number(nextMarkItem_), &mark_);
        display_->update();
        nextMarkItem_++;
    }
}

void MarkTool::mousePressed(const QMouseEvent& event) {
    // Add mark
    if (event.buttons() & Qt::LeftButton) {
        QPointF local = display_->getPosition(event.pos());

        if (local.x() >= 0 && local.y() >= 0
                && local.x() <= display_->getCurrentScene()->getWidth()
                && local.y() <= display_->getCurrentScene()->getHeight()) {
            putClickOn(local);
        }
    }

    SingleTool::mousePressed(event);
}

OverTool::OverTool(SingleDisplay* display) :
    SingleTool(display) {
    onMove_ = false;
    over_ = display_->getOverDisplay();
    over_->setHidden(true);
    over_->setActivatedTool(this);
    overDock_ = new QDockWidget("Overview");
    overDock_->setWidget(over_);
    overDock_->setFeatures(QDockWidget::NoDockWidgetFeatures |
                           QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
}

OverTool::~OverTool() {
}

QDockWidget* OverTool::getOverDock() {
    return overDock_;
}

bool OverTool::overIsVisible() {
    return overDock_->isVisible();
}

void OverTool::setOverVisible(bool status) {
    if (status && !overDock_->isVisible()) {
        overDock_->show();
    }

    overDock_->setHidden(!status);
}

void OverTool::paintEvent(const QPaintEvent& event) {
    event.isAccepted();
}

void OverTool::resizeEvent(const QResizeEvent& event) {
    event.isAccepted();
}

void OverTool::enterEvent(const QHoverEvent& event) {
    event.isAccepted();
}

void OverTool::leaveEvent(const QHoverEvent& event) {
    event.isAccepted();
}

void OverTool::moveEvent(const QHoverEvent& event) {
    event.isAccepted();
    over_->updateMousePosition();

    if (display_->positionIsVisible(over_->getLastMousePosition())) {
        if (!onMove_) {
            over_->setCursor(QPixmap::fromImage(SymbolsResource::getOpenHand()));
        }
    } else {
        if (!onMove_) {
            over_->setCursor(QPixmap::fromImage(SymbolsResource::getPointingHand()));
        }
    }

    actualizePosLabel(over_);
}

void OverTool::mousePressed(const QMouseEvent& event) {
    if (event.buttons() & Qt::LeftButton) {
        over_->updateMousePosition();

        if (display_->positionIsVisible(over_->getLastMousePosition())) {
            onMove_ = true;
            lastPos_ = over_->getLastMousePosition();
            over_->setCursor(QPixmap::fromImage(SymbolsResource::getClosedHand()));
        } else {
            display_->getCurrentScene()->moveTo(over_->getLastMousePosition());
            over_->setCursor(QPixmap::fromImage(SymbolsResource::getOpenHand()));
            display_->update();
        }
    }
}

void OverTool::mouseReleased(const QMouseEvent& event) {
    event.isAccepted();
    onMove_ = false;
    over_->setCursor(QPixmap::fromImage(SymbolsResource::getOpenHand()));
}

void OverTool::mouseMoved(const QMouseEvent& event) {
    if (event.buttons() & Qt::LeftButton) {
        if (onMove_) {
            over_->updateMousePosition();
            display_->getCurrentScene()->pan(over_->getLastMousePosition() - lastPos_);
            display_->update();
            lastPos_ = over_->getLastMousePosition();
        }
    }
}

void OverTool::wheelEvent(const QWheelEvent& event) {
    over_->updateMousePosition();

    if (!(onMove_)
            && (display_->positionIsVisible(over_->getLastMousePosition()))) {
        int numDegrees = event.delta() / 8.0;
        int numSteps = numDegrees / 15.0;

        if (event.orientation() == Qt::Vertical) {
            double zoomStep;

            if (numSteps > 0) {
                zoomStep = 1.044273782;    // 1*2^(1÷(2^4))
            } else if (numSteps < 0) {
                zoomStep = 0.957603281;    // 1/2^(1÷(2^4))
            }

            for (int i = 0; i < abs(numSteps); i++) {
                display_->getCurrentScene()->zoom(zoomStep, over_->getLastMousePosition());
            }

            display_->update();
            actualizeScaleSpin(display_->getCurrentScene()->getScale());

            if (propagateScaleTo_ != NULL) {
                propagateScaleTo_->propagateScale(display_->getCurrentScene()->getScale(),
                                                  display_->screenPosition(over_->getLastMousePosition()).toPoint());
            }
        }
    }
}



NearTool::NearTool(SingleDisplay* display) :
    SingleTool(display) {
    near_ = display_->getDetailDisplay();
    near_->setHidden(true);
    near_->setActivatedTool(this);
    nearDock_ = new QDockWidget("Detailview");
    nearDock_->setWidget(near_);
    nearDock_->setFeatures(QDockWidget::NoDockWidgetFeatures |
                           QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    cursorIsVisible_ = false;
    marker_ = NULL;
    near_->setCursor(QPixmap::fromImage(SymbolsResource::getBordedCross(QColor(255,
                                        255, 255, 255), QColor(0, 0, 0, 255), QSize(25, 25))));
}

NearTool::~NearTool() {
}

QDockWidget* NearTool::getNearDock() {
    return nearDock_;
}

bool NearTool::nearIsVisible() {
    return nearDock_->isVisible();
}

void NearTool::setNearVisible(bool status) {
    if (status && !nearDock_->isVisible()) {
        nearDock_->show();
    }

    nearDock_->setHidden(!status);
}

void NearTool::setMarker(MarkTool* marker) {
    marker_ = marker;
}

void NearTool::setNearCursor(QCursor cursor) {
    near_->setCursor(cursor);
}

void NearTool::paintEvent(const QPaintEvent& event) {
    event.isAccepted();
    QPoint pos = near_->screenPosition(display_->getLastMousePosition()).toPoint();
    QPixmap ico(display_->cursor().pixmap());
    QRect reg(QPoint(), ico.size());

    if (near_->painting()) {
        QPainter painter(near_);

        if (display_->showDetailedArea() && cursorIsVisible_) {
            reg.moveCenter(pos);
            painter.drawPixmap(reg, ico);
            painter.end();
        } else if (!display_->showDetailedArea() && cursorIsVisible_) {
            reg.moveCenter(QPoint(near_->width() / 2, near_->height() / 2));
            painter.drawPixmap(reg, ico);
            painter.end();
        }
    }

    cursorIsVisible_ = reg.intersects(near_->rect())
                       && display_->visibleRegion().contains(display_->mapFromGlobal(QCursor::pos()))
                       && !near_->visibleRegion().contains(near_->mapFromGlobal(QCursor::pos()));
}

void NearTool::resizeEvent(const QResizeEvent& event) {
    event.isAccepted();
}

void NearTool::enterEvent(const QHoverEvent& event) {
    event.isAccepted();

    if (display_->visibleRegion().contains(display_->mapFromGlobal(
            QCursor::pos()))) {
        cursorIsVisible_ = true;
        near_->update();
    }
}

void NearTool::leaveEvent(const QHoverEvent& event) {
    event.isAccepted();

    if (!display_->visibleRegion().contains(display_->mapFromGlobal(
            QCursor::pos()))) {
        cursorIsVisible_ = false;
        near_->update();
    }
}

void NearTool::moveEvent(const QHoverEvent& event) {
    event.isAccepted();

    if (near_->positionIsVisible(display_->getLastMousePosition())
            || cursorIsVisible_) {
        near_->update();
    }

    near_->updateMousePosition();
    actualizePosLabel(near_);
}

void NearTool::mousePressed(const QMouseEvent& event) {
    // Add mark
    if (near_->visibleRegion().contains(near_->mapFromGlobal(QCursor::pos()))
            && event.buttons() & Qt::LeftButton) {
        QPointF local = near_->getLastMousePosition();

        if (marker_ && local.x() >= 0 && local.y() >= 0
                && local.x() <= display_->getCurrentScene()->getWidth()
                && local.y() <= display_->getCurrentScene()->getHeight()) {
            marker_->putClickOn(local);
            //_display->updateAll();
        }
    }
}

void NearTool::mouseReleased(const QMouseEvent& event) {
    event.isAccepted();
}

void NearTool::mouseMoved(const QMouseEvent& event) {
    event.isAccepted();
}

void NearTool::wheelEvent(const QWheelEvent& event) {
    event.isAccepted();
}



SingleToolsBar::SingleToolsBar(SingleDisplay* display, QWidget* parent) :
    QToolBar("Display Tools", parent),
    zoom(display),
    move(display),
    mark(display),
    near_(display),
    over(display)//,
    //info(display)
{
    display_ = display;
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
    setFitView = new QAction(QIcon(":/icon/fit.png"),
                             "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN"
                             "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Fit View</span></p></body></html>",
                             this);
    showOverview = new QAction(QIcon(":/icon/overIcon"),
                               "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN"
                               "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Overview</span></p></body></html>",
                               this);
    showNearview = new QAction(QIcon(":/icon/detailIcon"),
                               "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN"
                               "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Detailview</span></p></body></html>",
                               this);
    useAntialias = new QAction(QIcon(":/icon/aliasingIcon"),
                               "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN"
                               "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Antialias</span></p></body></html>",
                               this);
    useFixedNearview = new QAction(QIcon(":/icon/fixdetailviewIcon.png"),
                                   "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN"
                                   "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Fixed Detailview</span></p></body></html>",
                                   this);
    openImage = new QAction(QIcon(":/icon/fileopen.png"),
                            "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN"
                            "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Open</span></p></body></html>",
                            this);
    saveImage = new QAction(QIcon(":/icon/disquette.png"),
                            "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN"
                            "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Save</span></p></body></html>",
                            this);
    scaleSpinBox = new QDoubleSpinBox(this);
    scaleSpinBox->setSuffix(" %");
    scaleSpinBox->setDecimals(0);
    scaleSpinBox->setRange(0, 800);
    scaleSpinBox->setSingleStep(0);
    scaleSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    connect(scaleSpinBox, SIGNAL(editingFinished()), this, SLOT(rescaleDisplay()));
    zoom.setScaleSpin(scaleSpinBox);
    move.setScaleSpin(scaleSpinBox);
    mark.setScaleSpin(scaleSpinBox);
    near_.setScaleSpin(scaleSpinBox);
    over.setScaleSpin(scaleSpinBox);
    infoLabel_ = new QLabel("   ", 0);
    zoom.setPosLabel(infoLabel_);
    move.setPosLabel(infoLabel_);
    mark.setPosLabel(infoLabel_);
    near_.setPosLabel(infoLabel_);
    over.setPosLabel(infoLabel_);
    detailComboBox = new QComboBox(this);
    detailComboBox->addItems(QString("1x 2x 4x 8x").split(" "));
    detailComboBox->setCurrentIndex(1);
    SingleScene* scene = dynamic_cast<SingleScene*>(display_->getCurrentScene());
    scene->setDetailZoom(2.0);
    connect(detailComboBox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(changeDetailZoom(int)));
    addAction(openImage);
    addAction(saveImage);
    addSeparator();
    addAction(setMarkTool);
    addAction(setMoveTool);
    addAction(setZoomTool);
    addSeparator();
    addAction(setFitView);
    addSeparator();
    addAction(useAntialias);
    addWidget(scaleSpinBox);
    addSeparator();
    addAction(showOverview);
    addAction(showNearview);
    addAction(useFixedNearview);
    addWidget(detailComboBox);
    setZoomTool->setCheckable(true);
    setMoveTool->setCheckable(true);
    setMarkTool->setCheckable(true);
    showOverview->setCheckable(true);
    showNearview->setCheckable(true);
    useAntialias->setCheckable(true);
    useFixedNearview->setCheckable(true);
    showOverview->setChecked(true);
    showNearview->setChecked(true);
    display_->setActivatedTool(&near_);
    near_.setMarker(&mark);
    QActionGroup* navegation = new QActionGroup(this);
    navegation->addAction(setZoomTool);
    navegation->addAction(setMoveTool);
    navegation->addAction(setMarkTool);
    navegation->setExclusive(true);
    connect(this, SIGNAL(actionTriggered(QAction*)), this,
            SLOT(executeAction(QAction*)));
    currentTool_ = &move;
    setMoveTool->trigger();
}

void  SingleToolsBar::setOpenVisible(bool status) {
    openImage->setVisible(status);
}

void  SingleToolsBar::setSaveVisible(bool status) {
    saveImage->setVisible(status);
}

void  SingleToolsBar::setMarkVisible(bool status) {
    setMarkTool->setVisible(status);
}

QLabel* SingleToolsBar::getInfo() {
    return infoLabel_;
}

QDockWidget* SingleToolsBar::getOverview() {
    return over.getOverDock();
}

QDockWidget* SingleToolsBar::getNearview() {
    return near_.getNearDock();
}

void SingleToolsBar::executeAction(QAction* action) {
    if (action ==  setZoomTool ) {
        display_->setActivatedTool(currentTool_, false);
        display_->setActivatedTool(currentTool_ = &zoom);
        display_->setCursor(QCursor(QPixmap::fromImage(
                                        SymbolsResource::getMagnifyGlass())));
    }

    if (action ==  setMoveTool ) {
        display_->setActivatedTool(currentTool_, false);
        display_->setActivatedTool(currentTool_ = &move);
        display_->setCursor(QCursor(QPixmap::fromImage(
                                        SymbolsResource::getOpenHand())));
    }

    if (action ==  setMarkTool ) {
        display_->setActivatedTool(currentTool_, false);
        display_->setActivatedTool(currentTool_ = &mark);
        display_->setCursor(QCursor(QPixmap::fromImage(SymbolsResource::getBordedCross(
                                        QColor(255, 255, 255, 255), QColor(0, 0, 0, 255), QSize(25, 25)))));
    }

    if (action ==  setFitView ) {
        display_->fitView();
        currentTool_->actualizeScaleSpin(display_->getCurrentScene()->getScale());
    }

    if (action ==  showOverview ) {
        over.setOverVisible(!over.overIsVisible());
    }

    if (action ==  showNearview ) {
        near_.setNearVisible(!near_.nearIsVisible());
        display_->setActivatedTool(&near_, near_.nearIsVisible());
        detailComboBox->setEnabled(near_.nearIsVisible());
    }

    if (action == useAntialias) {
        SingleScene* scene = dynamic_cast<SingleScene*>(display_->getCurrentScene());
        scene->useSmooth(useAntialias->isChecked());
        display_->update();
    }

    if (action == useFixedNearview) {
        display_->setShowDetailedArea(useFixedNearview->isChecked());
        display_->update();
    }

    if (action ==  openImage ) {
        QString filename = QFileDialog::getOpenFileName(0, "Open Image", ".",
                           "*.bmp *.png *.tif *.ppm *.jpg");

        if (!filename.isEmpty()) {
            SingleScene* scene = dynamic_cast<SingleScene*>(display_->getCurrentScene());
            scene->loadImage(filename);
            display_->update();
        }
    }

    if (action ==  saveImage ) {
    }
}

void SingleToolsBar::rescaleDisplay() {
    int diff = display_->getCurrentScene()->getScale() * 100 -
               scaleSpinBox->value();

    if (diff) {
        display_->getCurrentScene()->scaleTo(scaleSpinBox->value() / 100);
        display_->update();
        currentTool_->actualizeScaleSpin(display_->getCurrentScene()->getScale());
    }
}

void SingleToolsBar::changeDetailZoom(int nz) {
    SingleScene* scene = dynamic_cast<SingleScene*>(display_->getCurrentScene());

    switch (nz) {
    case 0:
        scene->setDetailZoom(1.0);
        break;

    case 1:
        scene->setDetailZoom(2.0);
        break;

    case 2:
        scene->setDetailZoom(4.0);
        break;

    case 3:
        scene->setDetailZoom(8.0);
        break;

    default :
        scene->setDetailZoom(2.0);
        break;
    }
}

void SingleToolsBar::setImageMode() {
    zoom.setImageMode();
    move.setImageMode();
    mark.setImageMode();
    over.setImageMode();
    near_.setImageMode();
}

void SingleToolsBar::setOrtoImageMode(double xi, double dx, double yi,
                                      double dy) {
    zoom.setOrtoImageMode(xi, dx, yi, dy);
    move.setOrtoImageMode(xi, dx, yi, dy);
    mark.setOrtoImageMode(xi, dx, yi, dy);
    over.setOrtoImageMode(xi, dx, yi, dy);
    near_.setOrtoImageMode(xi, dx, yi, dy);
}

void SingleToolsBar::setElevationImageMode(double xi, double dx, double yi,
        double dy, double zi, double dz) {
    zoom.setElevationImageMode(xi, dx, yi, dy, zi, dz);
    move.setElevationImageMode(xi, dx, yi, dy, zi, dz);
    mark.setElevationImageMode(xi, dx, yi, dy, zi, dz);
    over.setElevationImageMode(xi, dx, yi, dy, zi, dz);
    near_.setElevationImageMode(xi, dx, yi, dy, zi, dz);
}






SeparatedStereoToolsBar::SeparatedStereoToolsBar(SingleDisplay* leftDisplay,
        SingleDisplay* rightDisplay, QWidget* parent) :
    QToolBar("Display Tools", parent),
    leftZoom(leftDisplay),
    leftMove(leftDisplay),
    leftMark(leftDisplay),
    leftNear(leftDisplay),
    leftOver(leftDisplay),
    rightZoom(rightDisplay),
    rightMove(rightDisplay),
    rightMark(rightDisplay),
    rightNear(rightDisplay),
    rightOver(rightDisplay) { //,
    _leftDisplay = leftDisplay;
    _rightDisplay = rightDisplay;
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
    setFitLeftView = new QAction(QIcon(":/icon/fitleftIcon.png"),
                                 "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN"
                                 "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Fit Left</span></p></body></html>",
                                 this);
    setFitRightView = new QAction(QIcon(":/icon/fitrightIcon.png"),
                                  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN"
                                  "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Fit Right</span></p></body></html>",
                                  this);
    setFitBothView = new QAction(QIcon(":/icon/fitbothIcon.png"),
                                 "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN"
                                 "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Fit Both</span></p></body></html>",
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
    setEqualMovements = new QAction(QIcon(":/icon/equalmoveIcon.png"),
                                    "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN"
                                    "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Equal Movements</span></p></body></html>",
                                    this);
    setEqualScales = new QAction(QIcon(":/icon/equalzoomIcon.png"),
                                 "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN"
                                 "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Equal Scales</span></p></body></html>",
                                 this);
    scaleLeftSpinBox = new QDoubleSpinBox(this);
    scaleLeftSpinBox->setSuffix(" %");
    scaleLeftSpinBox->setDecimals(0);
    scaleLeftSpinBox->setRange(0, 800);
    scaleLeftSpinBox->setSingleStep(0);
    scaleLeftSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    connect(scaleLeftSpinBox, SIGNAL(editingFinished()), this,
            SLOT(rescaleLeftDisplay()));
    scaleRightSpinBox = new QDoubleSpinBox(this);
    scaleRightSpinBox->setSuffix(" %");
    scaleRightSpinBox->setDecimals(0);
    scaleRightSpinBox->setRange(0, 800);
    scaleRightSpinBox->setSingleStep(0);
    scaleRightSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    connect(scaleRightSpinBox, SIGNAL(editingFinished()), this,
            SLOT(rescaleRightDisplay()));
    leftZoom.setScaleSpin(scaleLeftSpinBox);
    leftMove.setScaleSpin(scaleLeftSpinBox);
    leftMark.setScaleSpin(scaleLeftSpinBox);
    leftNear.setScaleSpin(scaleLeftSpinBox);
    leftOver.setScaleSpin(scaleLeftSpinBox);
    rightZoom.setScaleSpin(scaleRightSpinBox);
    rightMove.setScaleSpin(scaleRightSpinBox);
    rightMark.setScaleSpin(scaleRightSpinBox);
    rightNear.setScaleSpin(scaleRightSpinBox);
    rightOver.setScaleSpin(scaleRightSpinBox);
    _leftInfoLabel = new QLabel("   ", 0);
    _rightInfoLabel = new QLabel("   ", 0);
    leftZoom.setPosLabel(_leftInfoLabel);
    leftMove.setPosLabel(_leftInfoLabel);
    leftMark.setPosLabel(_leftInfoLabel);
    leftNear.setPosLabel(_leftInfoLabel);
    leftOver.setPosLabel(_leftInfoLabel);
    rightZoom.setPosLabel(_rightInfoLabel);
    rightMove.setPosLabel(_rightInfoLabel);
    rightMark.setPosLabel(_rightInfoLabel);
    rightNear.setPosLabel(_rightInfoLabel);
    rightOver.setPosLabel(_rightInfoLabel);
    detailComboBox = new QComboBox(this);
    detailComboBox->addItems(QString("1x 2x 4x 8x").split(" "));
    detailComboBox->setCurrentIndex(1);
    SingleScene* leftScene = dynamic_cast<SingleScene*>
                             (_leftDisplay->getCurrentScene());
    leftScene->setDetailZoom(2.0);
    SingleScene* rightScene = dynamic_cast<SingleScene*>
                              (_rightDisplay->getCurrentScene());
    rightScene->setDetailZoom(2.0);
    connect(detailComboBox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(changeDetailZoom(int)));
    addAction(openLeftImage);
    addAction(openRightImage);
    addAction(saveLeftImage);
    addAction(saveRightImage);
    addSeparator();
    addAction(setMarkTool);
    addAction(setMoveTool);
    addAction(setZoomTool);
    addSeparator();
    addAction(useAntialias);
    addAction(setEqualMovements);
    addAction(setEqualScales);
    addWidget(scaleLeftSpinBox);
    addWidget(scaleRightSpinBox);
    addSeparator();
    addAction(setFitLeftView);
    addAction(setFitRightView);
    addAction(setFitBothView);
    addSeparator();
    addAction(showOverview);
    addAction(showNearview);
    addWidget(detailComboBox);
    setZoomTool->setCheckable(true);
    setMoveTool->setCheckable(true);
    setMarkTool->setCheckable(true);
    showOverview->setCheckable(true);
    showNearview->setCheckable(true);
    useAntialias->setCheckable(true);
    setEqualMovements->setCheckable(true);
    setEqualScales->setCheckable(true);
    showOverview->setChecked(true);
    showNearview->setChecked(true);
    _leftDisplay->setActivatedTool(&leftNear);
    _rightDisplay->setActivatedTool(&rightNear);
    _leftDisplay->setShowDetailedArea(true);
    _rightDisplay->setShowDetailedArea(true);
    leftNear.setMarker(&leftMark);
    rightNear.setMarker(&rightMark);
    QActionGroup* navegation = new QActionGroup(this);
    navegation->addAction(setZoomTool);
    navegation->addAction(setMoveTool);
    navegation->addAction(setMarkTool);
    navegation->setExclusive(true);
    connect(this, SIGNAL(actionTriggered(QAction*)), this,
            SLOT(executeAction(QAction*)));
    currentLeftTool = &leftMove;
    currentRightTool = &rightMove;
    setMoveTool->trigger();
}

void  SeparatedStereoToolsBar::setOpenVisible(bool status) {
    openLeftImage->setVisible(status);
    openRightImage->setVisible(status);
}

void  SeparatedStereoToolsBar::setSaveVisible(bool status) {
    saveLeftImage->setVisible(status);
    saveRightImage->setVisible(status);
}

void  SeparatedStereoToolsBar::setMarkVisible(bool status) {
    setMarkTool->setVisible(status);
}

QLabel* SeparatedStereoToolsBar::getLeftInfo() {
    return _leftInfoLabel;
}

QLabel* SeparatedStereoToolsBar::getRightInfo() {
    return _rightInfoLabel;
}

QDockWidget* SeparatedStereoToolsBar::getLeftOverview() {
    return leftOver.getOverDock();
}

QDockWidget* SeparatedStereoToolsBar::getRightOverview() {
    return rightOver.getOverDock();
}

QDockWidget* SeparatedStereoToolsBar::getLeftNearview() {
    return leftNear.getNearDock();
}

QDockWidget* SeparatedStereoToolsBar::getRightNearview() {
    return rightNear.getNearDock();
}

void SeparatedStereoToolsBar::executeAction(QAction* action) {
    if (action ==  setZoomTool ) {
        _leftDisplay->setActivatedTool(currentLeftTool, false);
        _leftDisplay->setActivatedTool(currentLeftTool = &leftZoom);
        _leftDisplay->setCursor(QCursor(QPixmap::fromImage(
                                            SymbolsResource::getMagnifyGlass())));
        _rightDisplay->setActivatedTool(currentRightTool, false);
        _rightDisplay->setActivatedTool(currentRightTool = &rightZoom);
        _rightDisplay->setCursor(QCursor(QPixmap::fromImage(
                                             SymbolsResource::getMagnifyGlass())));
    }

    if (action ==  setMoveTool ) {
        _leftDisplay->setActivatedTool(currentLeftTool, false);
        _leftDisplay->setActivatedTool(currentLeftTool = &leftMove);
        _leftDisplay->setCursor(QCursor(QPixmap::fromImage(
                                            SymbolsResource::getOpenHand())));
        _rightDisplay->setActivatedTool(currentRightTool, false);
        _rightDisplay->setActivatedTool(currentRightTool = &rightMove);
        _rightDisplay->setCursor(QCursor(QPixmap::fromImage(
                                             SymbolsResource::getOpenHand())));
    }

    if (action ==  setMarkTool ) {
        _leftDisplay->setActivatedTool(currentLeftTool, false);
        _leftDisplay->setActivatedTool(currentLeftTool = &leftMark);
        _leftDisplay->setCursor(QCursor(QPixmap::fromImage(
                                            SymbolsResource::getBordedCross(QColor(255, 255, 255, 255), QColor(0, 0, 0,
                                                    255), QSize(25, 25)))));
        _rightDisplay->setActivatedTool(currentRightTool, false);
        _rightDisplay->setActivatedTool(currentRightTool = &rightMark);
        _rightDisplay->setCursor(QCursor(QPixmap::fromImage(
                                             SymbolsResource::getBordedCross(QColor(255, 255, 255, 255), QColor(0, 0, 0,
                                                     255), QSize(25, 25)))));
    }

    if (action ==  setFitLeftView ) {
        _leftDisplay->fitView();
        currentLeftTool->actualizeScaleSpin(
            _leftDisplay->getCurrentScene()->getScale());
    }

    if (action ==  setFitRightView ) {
        _rightDisplay->fitView();
        currentRightTool->actualizeScaleSpin(
            _rightDisplay->getCurrentScene()->getScale());
    }

    if (action ==  setFitBothView ) {
        _leftDisplay->fitView();
        _rightDisplay->fitView();
        currentLeftTool->actualizeScaleSpin(
            _leftDisplay->getCurrentScene()->getScale());
        currentRightTool->actualizeScaleSpin(
            _rightDisplay->getCurrentScene()->getScale());
    }

    if (action ==  showOverview ) {
        leftOver.setOverVisible(!leftOver.overIsVisible());
        rightOver.setOverVisible(!rightOver.overIsVisible());
    }

    if (action ==  showNearview ) {
        leftNear.setNearVisible(!leftNear.nearIsVisible());
        rightNear.setNearVisible(!rightNear.nearIsVisible());
        _leftDisplay->setActivatedTool(&leftNear, leftNear.nearIsVisible());
        _rightDisplay->setActivatedTool(&rightNear, rightNear.nearIsVisible());
        detailComboBox->setEnabled(leftNear.nearIsVisible());
    }

    if (action ==  setEqualMovements ) {
        if (setEqualMovements->isChecked()) {
            leftZoom.propagateMoveTo(&rightZoom);
            rightZoom.propagateMoveTo(&leftZoom);
            leftMove.propagateMoveTo(&rightMove);
            rightMove.propagateMoveTo(&leftMove);
            leftMark.propagateMoveTo(&rightMark);
            rightMark.propagateMoveTo(&leftMark);
        } else {
            leftZoom.propagateMoveTo();
            rightZoom.propagateMoveTo();
            leftMove.propagateMoveTo();
            rightMove.propagateMoveTo();
            leftMark.propagateMoveTo();
            rightMark.propagateMoveTo();
        }
    }

    if (action ==  setEqualScales ) {
        if (setEqualScales->isChecked()) {
            leftZoom.propagateScaleTo(&rightZoom);
            rightZoom.propagateScaleTo(&leftZoom);
            leftMove.propagateScaleTo(&rightMove);
            rightMove.propagateScaleTo(&leftMove);
            leftMark.propagateScaleTo(&rightMark);
            rightMark.propagateScaleTo(&leftMark);
            leftOver.propagateScaleTo(&rightOver);
            rightOver.propagateScaleTo(&leftOver);
            scaleRightSpinBox->setValue(scaleLeftSpinBox->value());
            _rightDisplay->getCurrentScene()->scaleTo(scaleLeftSpinBox->value() / 100);
            _rightDisplay->update();
            currentRightTool->actualizeScaleSpin(
                _rightDisplay->getCurrentScene()->getScale());
            scaleRightSpinBox->setDisabled(true);
        } else {
            leftZoom.propagateScaleTo();
            rightZoom.propagateScaleTo();
            leftMove.propagateScaleTo();
            rightMove.propagateScaleTo();
            leftMark.propagateScaleTo();
            rightMark.propagateScaleTo();
            leftOver.propagateScaleTo();
            rightOver.propagateScaleTo();
            scaleRightSpinBox->setEnabled(true);
        }
    }

    if (action == useAntialias) {
        SingleScene* leftScene = dynamic_cast<SingleScene*>
                                 (_leftDisplay->getCurrentScene());
        SingleScene* rightScene = dynamic_cast<SingleScene*>
                                  (_rightDisplay->getCurrentScene());
        leftScene->useSmooth(useAntialias->isChecked());
        rightScene->useSmooth(useAntialias->isChecked());
        _leftDisplay->update();
        _rightDisplay->update();
    }

    if (action ==  openLeftImage ) {
        QString filename = QFileDialog::getOpenFileName(0, "Open Left Image", ".",
                           "*.bmp *.png *.tif *.ppm *.jpg");

        if (!filename.isEmpty()) {
            SingleScene* scene = dynamic_cast<SingleScene*>
                                 (_leftDisplay->getCurrentScene());
            scene->loadImage(filename);
            _leftDisplay->update();
        }
    }

    if (action ==  openRightImage ) {
        QString filename = QFileDialog::getOpenFileName(0, "Open Right Image", ".",
                           "*.bmp *.png *.tif *.ppm *.jpg");

        if (!filename.isEmpty()) {
            SingleScene* scene = dynamic_cast<SingleScene*>
                                 (_rightDisplay->getCurrentScene());
            scene->loadImage(filename);
            _rightDisplay->update();
        }
    }

    if (action ==  saveLeftImage ) {
    }

    if (action ==  saveRightImage ) {
    }
}

void SeparatedStereoToolsBar::changeMode(int action) {
    if (action ==  1) {
        setMarkTool->trigger();
    }

    if (action ==  2) {
        setMoveTool->trigger();
    }

    if (action ==  3) {
        setZoomTool->trigger();
    }
}

void SeparatedStereoToolsBar::rescaleLeftDisplay() {
    int diff = _leftDisplay->getCurrentScene()->getScale() * 100 -
               scaleLeftSpinBox->value();

    if (diff) {
        _leftDisplay->getCurrentScene()->scaleTo(scaleLeftSpinBox->value() / 100);
        _leftDisplay->update();
        currentLeftTool->actualizeScaleSpin(
            _leftDisplay->getCurrentScene()->getScale());

        if (setEqualScales->isChecked()) {
            _rightDisplay->getCurrentScene()->scaleTo(scaleLeftSpinBox->value() / 100);
            _rightDisplay->update();
            currentRightTool->actualizeScaleSpin(
                _rightDisplay->getCurrentScene()->getScale());
        }
    }
}

void SeparatedStereoToolsBar::rescaleRightDisplay() {
    int diff = _rightDisplay->getCurrentScene()->getScale() * 100 -
               scaleRightSpinBox->value();

    if (diff) {
        _rightDisplay->getCurrentScene()->scaleTo(scaleRightSpinBox->value() / 100);
        _rightDisplay->update();
        currentRightTool->actualizeScaleSpin(
            _rightDisplay->getCurrentScene()->getScale());
    }
}

void SeparatedStereoToolsBar::changeDetailZoom(int nz) {
    SingleScene* leftScene = dynamic_cast<SingleScene*>
                             (_leftDisplay->getCurrentScene());
    SingleScene* rightScene = dynamic_cast<SingleScene*>
                              (_rightDisplay->getCurrentScene());

    switch (nz) {
    case 0:
        leftScene->setDetailZoom(1.0);
        rightScene->setDetailZoom(1.0);
        break;

    case 1:
        leftScene->setDetailZoom(2.0);
        rightScene->setDetailZoom(2.0);
        break;

    case 2:
        leftScene->setDetailZoom(4.0);
        rightScene->setDetailZoom(4.0);
        break;

    case 3:
        leftScene->setDetailZoom(8.0);
        rightScene->setDetailZoom(8.0);
        break;

    default :
        leftScene->setDetailZoom(2.0);
        rightScene->setDetailZoom(2.0);
        break;
    }
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
