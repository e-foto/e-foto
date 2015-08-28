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

#define _USE_MATH_DEFINES // for C++
#include <cmath>

#include "StereoTools.h"

#include <QFileDialog>
#include "StereoDisplay.h"
//#include <math.h>

#define NOCURSOR QPixmap::fromImage(SymbolsResource::getBackGround(QColor(0,0,0,0)))

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

void StereoTool::paintEvent(const QPaintEvent& event)
{
    event.isAccepted();
    if (_display->painting())
    {
        QPainter painter(_display);
        painter.setRenderHint(QPainter::Antialiasing);

        if (_autoPan != QPointF(0,0))
        {
            // Draw autoMove feedback
            QPoint endArrow(_lastMousePosition - (_autoPan*5*_display->getCurrentScene()->getScale()).toPoint());

            painter.setPen(QPen(QBrush(Qt::yellow), 7, Qt::SolidLine, Qt::RoundCap));
            painter.drawPoint(_lastMousePosition);

            painter.setPen(QPen(QBrush(Qt::yellow), 2, Qt::SolidLine, Qt::RoundCap));
            painter.drawLine(_lastMousePosition, endArrow);

            if (_autoPan != QPointF(0,0))
            {
                double tangent = atan2(_autoPan.y(), _autoPan.x());
                QPoint pa(7 * cos (tangent + M_PI / 7) + endArrow.x(), 7 * sin (tangent + M_PI / 7) + endArrow.y());
                QPoint pb(7 * cos (tangent - M_PI / 7) + endArrow.x(), 7 * sin (tangent - M_PI / 7) + endArrow.y());
                QVector<QPoint> arrow;
                arrow.append(pa);
                arrow.append(endArrow);
                arrow.append(endArrow);
                arrow.append(pb);

                painter.setPen(QPen(QBrush(Qt::yellow), 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
                painter.drawLines(arrow);
            }
        }
        else if (_scale > 0)
        {
            // Draw scaleBar feedback
            //QPoint endBar(_fixedPoint.x(), _display->getMouseScreenPosition().y());
            QPoint endBar(_display->screenPosition(_fixedPointOnImageLeft).x(), _display->getMouseScreenPosition().y());

            painter.setPen(QPen(QBrush(Qt::yellow), 7, Qt::SolidLine, Qt::RoundCap));
            //painter.drawPoint(_fixedPoint);
            painter.drawPoint(_display->screenPosition(_fixedPointOnImageLeft));

            painter.setPen(QPen(QBrush(Qt::yellow), 2));
            //painter.drawLine(_fixedPoint, endBar);
            painter.drawLine(_display->screenPosition(_fixedPointOnImageLeft), endBar);
            painter.drawLine(QPoint(endBar.x() - 3, endBar.y()), QPoint(endBar.x() + 3, endBar.y()));
            painter.drawText(QPoint(endBar.x() + 5, endBar.y() + 5), QString::number(_display->getCurrentScene()->getScale()*100,'f', 1).append("%"));
        }
        painter.end();
    }
}

void StereoTool::resizeEvent(const QResizeEvent &event)
{
    _display->getLeftDisplay()->resize(event.size());
    _display->getRightDisplay()->resize(event.size());
    //actualizeScaleSpin(_display->getCurrentScene()->getScale());
}

void StereoTool::enterEvent(const QHoverEvent& event)
{
    event.isAccepted();
    _display->setCursor(_currentCursor, _display->isStereoCursor());
}

void StereoTool::leaveEvent(const QHoverEvent& event)
{
    event.isAccepted();
    _display->setCursor(SymbolsResource::getBackGround(QColor(0,0,0,0)), _display->isStereoCursor());
    _display->updateAll();
}

void StereoTool::moveEvent(const QHoverEvent& event)
{
    // Update mouse position only
    _display->updateMousePosition();

    if (_actualizePosLabel)
        actualizePosLabel();

    _display->getCurrentScene()->getLeftScene()->setDetailedPoint(_display->getPositionLeft(event.pos()+_display->getLeftCursorOffset().toPoint()));
    _display->getCurrentScene()->getRightScene()->setDetailedPoint(_display->getPositionRight(event.pos()+_display->getRightCursorOffset().toPoint()));

    if (!_hasButtomPressed)
        _display->updateAll();

    emit mouseMoved(_display->getPositionLeft(event.pos()+_display->getLeftCursorOffset().toPoint()),_display->getPositionRight(event.pos()+_display->getRightCursorOffset().toPoint()));
}

void StereoTool::mousePressed(const QMouseEvent &event)
{
    _hasButtomPressed = true;

    _lastCursor = _display->getCursor();

    // Prepair to zoom default (scaleBar).
    if (event.buttons() & Qt::MidButton)
    {
        _fixedPoint = event.pos();
        _fixedPointOnImageLeft = _display->getPositionLeft(_fixedPoint);
        _fixedPointOnImageRight = _display->getPositionRight(_fixedPoint);
        _scale = _display->getCurrentScene()->getScale();
        _currentCursor = SymbolsResource::getLeftArrow();
        _display->setCursor(_currentCursor, _display->isStereoCursor());
        _actualizePosLabel = false;
    }
    // Prepair move reference
    else if (event.buttons() & Qt::RightButton)
    {
        _currentCursor = SymbolsResource::getBackGround(QColor(0,0,0,0));
        _display->setCursor(_currentCursor, _display->isStereoCursor());
    }
    _lastMousePosition = event.pos();

    if ((event.button() & Qt::LeftButton) || (event.button() & Qt::MidButton))
    {
        _display->getCurrentScene()->getLeftScene()->setDetailedPoint(_display->getPositionLeft(event.pos()+_display->getLeftCursorOffset().toPoint()));
        _display->getCurrentScene()->getRightScene()->setDetailedPoint(_display->getPositionRight(event.pos()+_display->getRightCursorOffset().toPoint()));
    }
}

void StereoTool::mouseReleased(const QMouseEvent &event)
{
    _hasButtomPressed = false;

    //if (_autoPan != QPoint(0,0) || _scale != -1)
    _currentCursor = _lastCursor;
    _display->setCursor(_currentCursor, _display->isStereoCursor());

    // Stop move default (autoMove).
    _autoPan = QPoint(0,0);

    // Stop zoom default (scaleBar).
    _scale = -1;

    if (!_actualizePosLabel) // OnScaleBar
    {
        /*
        QCursor::setPos(_display->mapToGlobal(_display->screenPosition(_fixedPointOnImage).toPoint()));
        _actualizePosLabel = true;
        SingleScene* scene = (SingleScene*)_display->getCurrentScene();
        scene->setDetailedPoint(_fixedPointOnImage);
        */
    }
    else
    {
        _display->getCurrentScene()->getLeftScene()->setDetailedPoint(_display->getPositionLeft(event.pos()+_display->getLeftCursorOffset().toPoint()));
        _display->getCurrentScene()->getRightScene()->setDetailedPoint(_display->getPositionRight(event.pos()+_display->getRightCursorOffset().toPoint()));
    }

    _display->updateAll();
}

void StereoTool::mouseMoved(const QMouseEvent &event)
{
    // Make zoom default (scaleBar).
    if (event.buttons() & Qt::MidButton)
    {

        int diff = event.pos().y() - _display->screenPosition(_fixedPointOnImageLeft).y();
        double newScale = (_scale*100 - diff)/100;
        //qDebug("screenPositionY = %f, diff = %d, newScale = %f e fixedPoint = (%f, %f)", _display->screenPosition(_fixedPointOnImage).y(), diff, newScale, _fixedPointOnImage.x(), _fixedPointOnImage.y());
        _display->getCurrentScene()->getLeftScene()->scaleTo(newScale, _fixedPointOnImageLeft);
        _display->getCurrentScene()->getRightScene()->scaleTo(newScale, _fixedPointOnImageRight);
        if (event.pos().x() < _display->screenPosition(_fixedPointOnImageLeft).x())
        {
            _currentCursor = SymbolsResource::getRightArrow();
            _display->setCursor(_currentCursor, _display->isStereoCursor());
        }
        else
        {
            _currentCursor = SymbolsResource::getLeftArrow();
            _display->setCursor(_currentCursor, _display->isStereoCursor());
        }

        //actualizeScaleSpin(_display->getCurrentScene()->getScale());
        _display->updateAll();
    }

    // Make move default (autoMove).
    if (event.buttons() & Qt::RightButton)
    {
        double scale = _display->getCurrentScene()->getScale();
        QPointF diff = event.posF() - _lastMousePosition;
        _autoPan = -(diff/(5*scale));
        _currentCursor = SymbolsResource::getText(QString::fromUtf8("Auto"));
        _display->setCursor(_currentCursor, _display->isStereoCursor());
    }
}

void StereoTool::mouseDblClicked(const QMouseEvent & event)
{
    event.isAccepted();
}

void StereoTool::wheelEvent(const QWheelEvent & event)
{
    double numSteps = event.delta() / (15.0 * 8.0);
    if (event.orientation() == Qt::Vertical)
    {
        if (event.modifiers() == Qt::ShiftModifier)
            _display->getCurrentScene()->getRightScene()->pan(QPointF(numSteps, 0));
        else if (event.modifiers() == Qt::ControlModifier)
            _display->getCurrentScene()->getRightScene()->pan(QPointF(0, numSteps));
        else
        {
            _display->setLeftCursorOffset(QPointF(_display->getLeftCursorOffset().x()+numSteps/fabs(numSteps),0));
            _display->setRightCursorOffset(QPointF(_display->getRightCursorOffset().x()-numSteps/fabs(numSteps),0));
        }
        _display->updateAll();
    }
}

void StereoTool::autoMove()
{
    // Execute move default (autoMove).
    if (_autoPan != QPointF(0,0))
    {
        _display->getCurrentScene()->getLeftScene()->pan(-_autoPan);
        _display->getCurrentScene()->getRightScene()->pan(-_autoPan);

        //SingleScene* scene = (SingleScene*)_display->getCurrentScene();
        //scene->setDetailedPoint(_display->getLastMousePosition());
        actualizePosLabel();

        //_display->blockShowDetailedArea(true);
        _display->updateAll();
        //_display->blockShowDetailedArea(false);
    }
}

void StereoTool::actualizePosLabel()
{
    //if (_display->visibleRegion().contains(_display->mapFromGlobal(QCursor::pos())))
    {
        // Actualize X,Y coordinates
        int leftImageWidth = _display->getCurrentScene()->getLeftScene()->getWidth();
        int leftImageHeight = _display->getCurrentScene()->getLeftScene()->getHeight();
        int rightImageWidth = _display->getCurrentScene()->getRightScene()->getWidth();
        int rightImageHeight = _display->getCurrentScene()->getRightScene()->getHeight();
        _display->updateMousePosition();

        QPointF mp = _display->getMouseScreenPosition();
        QPointF pl = _display->getPositionLeft((mp + _display->getLeftCursorOffset()).toPoint());
        QPointF pr = _display->getPositionRight((mp + _display->getRightCursorOffset()).toPoint());

        QString leftInfo = QString("Left: ") + QString::number(pl.x(),'f',2) + QString(" x ") + QString::number(pl.y(),'f',2);
        QString rightInfo = QString("Right: ") + QString::number(pr.x(),'f',2) + QString(" x ") + QString::number(pr.y(),'f',2);

        //double X; double Y;	double Z;
        //computeIntersection(pl.x(), pl.y(), pr.x(), pr.y(), X, Y, Z);
        //QString stereoInfo;

        // Clear if necessary
        if (pl.x() < 0 || pl.y() < 0 || pl.x() > leftImageWidth || pl.y() > leftImageHeight)
            leftInfo = "   ";
        if (pr.x() < 0 || pr.y() < 0 || pr.x() > rightImageWidth || pr.y() > rightImageHeight)
            rightInfo = "   ";

        // Reset label
        _leftPosLabel->setText(leftInfo);
        _rightPosLabel->setText(rightInfo);
    }
}

void StereoTool::setCursor(QImage cursor, bool stereo)
{
    _currentCursor = cursor;
    _display->setCursor(_currentCursor, stereo);
}
/*
void StereoTool::actualizePosLabel(SingleDisplay* display, bool force)
{

    if (force || display->visibleRegion().contains(display->mapFromGlobal(QCursor::pos())))
    {
        // Actualize X,Y coordinates
        int imageWidth = display->getCurrentScene()->getWidth();
        int imageHeight = display->getCurrentScene()->getHeight();
        display->updateMousePosition();
        QPointF p = display->getLastMousePosition();
        QString info = QString::number(_xi + p.x()*_dx,'f',2);
        if (_invertY)
            info.append(QString(" x ") + QString::number(_yi+imageHeight*_dy - p.y()*_dy,'f',2));
        else
            info.append(QString(" x ") + QString::number(_yi + p.y()*_dy,'f',2));

        // Append Z if necessary
        unsigned int z = -1;
        if (_printZ)
        {
            z = _display->getCurrentScene()->getGrayColor(p);
            info.append(QString(" x ") + QString::number(_zi + (z-1)*_dz,'f',2));
        }

        // Clear if necessary
        if (p.x() < 0 || p.y() < 0 || p.x() > imageWidth || p.y() > imageHeight || z == 0)
            info = "   ";

        // Reset label
        _posLabel->setText(info);
    }

}
*/


ZoomStereoTool::ZoomStereoTool(StereoDisplay* display) :
    StereoTool(display)
{
    _onRubberBand = false;
}

ZoomStereoTool::~ZoomStereoTool()
{
}

void ZoomStereoTool::paintEvent(const QPaintEvent &event)
{
    if (_onRubberBand)
    {
        QRect rubber(_fixedPoint, _display->getMouseScreenPosition().toPoint());
        if (abs(rubber.width()) < 8 && abs(rubber.height()) < 8)
        {
            _currentCursor = SymbolsResource::getMagnifyGlass("-");
            _display->setCursor(_currentCursor, false);
            return;
        }
        else
        {	_currentCursor = SymbolsResource::getMagnifyGlass("+");
            _display->setCursor(_currentCursor, false);
        }
        QPainter painter(_display->getRealDisplay());
        painter.setPen(QPen(Qt::yellow));
        painter.drawRect(QRect(_fixedPoint,_display->getMouseScreenPosition().toPoint()));
        painter.end();
    }
    StereoTool::paintEvent(event);
}

//void ZoomStereoTool::enterEvent(const QHoverEvent& event)
//{
//}

//void ZoomStereoTool::leaveEvent(const QHoverEvent& event)
//{
//}

//void ZoomStereoTool::moveEvent(const QHoverEvent& event)
//{
//}

void ZoomStereoTool::mousePressed(const QMouseEvent & event)
{
    if (event.button() != event.buttons())
        return;

    // Rubberband zoom
    if (event.buttons() & Qt::LeftButton)
    {
        _fixedPoint = event.pos();
        _onRubberBand = true;
        _lastCursor = _display->getCursor();
        _currentCursor = SymbolsResource::getMagnifyGlass("-");
        _display->setCursor(_currentCursor, false);
        //_display->blockShowDetailedArea(true);
        _display->updateAll();
        return;
    }
    StereoTool::mousePressed(event);
}

void ZoomStereoTool::mouseReleased(const QMouseEvent & event)
{
    // Rubberband zoom
    if (_onRubberBand)
    {
        QRect rubber(_fixedPoint, event.pos());
        if (abs(rubber.width()) < 8 && abs(rubber.height()) < 8)
        {
            _display->getCurrentScene()->getLeftScene()->zoom(0.8, _display->getPositionLeft(_fixedPoint));
            _display->getCurrentScene()->getRightScene()->zoom(0.8, _display->getPositionRight(_fixedPoint));
        }
        else
        {
            double wscale = abs(rubber.width()) == 0 ? 1024 : _display->width()/(double)abs(rubber.width());
            double hscale = abs(rubber.height()) == 0 ? 1024 : _display->height()/(double)abs(rubber.height());
            _display->getCurrentScene()->getLeftScene()->moveTo(_display->getPositionLeft(rubber.center()));
            _display->getCurrentScene()->getLeftScene()->zoom(wscale < hscale ? wscale : hscale);
            _display->getCurrentScene()->getRightScene()->moveTo(_display->getPositionRight(rubber.center()));
            _display->getCurrentScene()->getRightScene()->zoom(wscale < hscale ? wscale : hscale);

            //QCursor::setPos(_display->mapToGlobal(_display->screenPosition(_display->getCurrentScene()->getViewpoint()).toPoint()));
            //e = QMouseEvent(QEvent::MouseButtonRelease, _display->screenPosition(_display->getCurrentScene()->getViewpoint()).toPoint(), event.button(), event.buttons(), event.modifiers());
        }
        _onRubberBand = false;
        //actualizeScaleSpin(_display->getCurrentScene()->getScale());
    }
    StereoTool::mouseReleased(event);
}

void ZoomStereoTool::mouseMoved(const QMouseEvent & event)
{
    // Rubberband zoom.
    if (event.buttons() & Qt::LeftButton)
    {
        _display->updateAll();
        return;
    }
    StereoTool::mouseMoved(event);
}

void ZoomStereoTool::mouseDblClicked(const QMouseEvent & event)
{
    event.isAccepted();
    _display->fitView();
    //actualizeScaleSpin(_display->getCurrentScene()->getScale());
}

/*
void ZoomStereoTool::wheelEvent(const QWheelEvent & event)
{
 int numDegrees = event.delta() / 8.0;
 int numSteps = numDegrees / 15.0;
 if (event.orientation() == Qt::Vertical)
 {
  double zoomStep;
  if (numSteps>0)
   zoomStep = 1.044273782; // 1*2^(1÷(2^4))
  else if (numSteps<0)
   zoomStep = 0.957603281; // 1/2^(1÷(2^4))
  for (int i = 0; i<abs(numSteps);i++)
  {
   _display->getCurrentScene()->getLeftScene()->zoom(zoomStep, _display->getPositionLeft(event.pos()));
   _display->getCurrentScene()->getRightScene()->zoom(zoomStep, _display->getPositionRight(event.pos()));
  }
  _display->updateAll();
 }
}
*/


MoveStereoTool::MoveStereoTool(StereoDisplay* display) :
    StereoTool(display)
{
}

MoveStereoTool::~MoveStereoTool()
{
}

//void MoveStereoTool::enterEvent(const QHoverEvent& event)
//{
//}

//void MoveStereoTool::leaveEvent(const QHoverEvent& event)
//{
//}

//void MoveStereoTool::moveEvent(const QHoverEvent& event)
//{
//	_display->updateMousePosition();
//}

void MoveStereoTool::mousePressed(const QMouseEvent & event)
{
    if (event.button() != event.buttons())
        return;
    StereoTool::mousePressed(event);
    if (event.buttons() & Qt::LeftButton)
    {
        _currentCursor = SymbolsResource::getClosedHand();
        _display->setCursor(_currentCursor, false);
    }
    _display->updateAll();
}

void MoveStereoTool::mouseReleased(const QMouseEvent & event)
{
    StereoTool::mouseReleased(event);
    _currentCursor = SymbolsResource::getOpenHand();
    _display->setCursor(_currentCursor, false);
}

void MoveStereoTool::mouseMoved(const QMouseEvent & event)
{
    // Move
    if (event.buttons() & Qt::LeftButton)
    {
        QPointF diff = event.posF() - _lastMousePosition;
        _lastMousePosition = event.pos();
        double scale;
        scale = _display->getCurrentScene()->getScale();
        if (event.modifiers() != Qt::ShiftModifier)
            _display->getCurrentScene()->getLeftScene()->pan(-(diff/scale));
        if (event.modifiers() != Qt::ControlModifier)
            _display->getCurrentScene()->getRightScene()->pan(-(diff/scale));
        _display->updateAll();
        return;
    }
    StereoTool::mouseMoved(event);
}

//void MoveStereoTool::mouseDblClicked(const QMouseEvent & event)
//{
//}

//void MoveStereoTool::wheelEvent(const QWheelEvent & event)
//{
//}



MarkStereoTool::MarkStereoTool(StereoDisplay* display) :
    StereoTool(display),
    mark(WhiteMark)
{
}

MarkStereoTool::~MarkStereoTool()
{
}

void MarkStereoTool::changeMarker(Marker marker)
{
    mark = marker;
}

Marker* MarkStereoTool::getMarker()
{
    return &mark;
}

void MarkStereoTool::addMark(QPointF lPos, QPointF rPos, int key, QString label, Marker *marker)
{
    _display->getCurrentScene()->getLeftScene()->geometry()->addPoint(lPos, key, label, marker == NULL ? &mark : marker);
    _display->getCurrentScene()->getRightScene()->geometry()->addPoint(rPos, key, label, marker == NULL ? &mark : marker);
}

void MarkStereoTool::insertMark(QPointF lPos, QPointF rPos, int key, QString label, Marker *marker)
{
    _display->getCurrentScene()->getLeftScene()->geometry()->insertPoint(lPos, key, label, marker == NULL ? &mark : marker);
    _display->getCurrentScene()->getRightScene()->geometry()->insertPoint(rPos, key, label, marker == NULL ? &mark : marker);
}

/*
void MarkStereoTool::editMark(int key, QPointF lPos, QPointF rPos, Marker *marker)
{
}

void MarkStereoTool::editMark(int key, QPointF lPos, QPointF rPos, QString label, Marker *marker)
{
}
*/

void MarkStereoTool::deleteMark(int key)
{
    _display->getCurrentScene()->getLeftScene()->geometry()->deletePoint(key);
    _display->getCurrentScene()->getRightScene()->geometry()->deletePoint(key);
}

void MarkStereoTool::clear()
{
    _display->getCurrentScene()->getLeftScene()->geometry()->clear();
    _display->getCurrentScene()->getRightScene()->geometry()->clear();
}

void MarkStereoTool::setToOnlyEmitClickedMode()
{
    onlyEmitClickedMode = true;
}

void MarkStereoTool::setToAutoCreateMarkFrom(unsigned int start)
{
    onlyEmitClickedMode = false;
    nextMarkItem = start;
}

void MarkStereoTool::putClickOn(QPointF& lPos, QPointF& rPos)
{
    if (onlyEmitClickedMode)
    {
        emit clicked(lPos, rPos);
    }
    else
    {
        _display->getCurrentScene()->getLeftScene()->geometry()->insertPoint(lPos, nextMarkItem, QString::number(nextMarkItem), &mark);
        _display->getCurrentScene()->getRightScene()->geometry()->insertPoint(rPos, nextMarkItem, QString::number(nextMarkItem), &mark);
        _display->updateAll();
        nextMarkItem++;
    }
}

//void MarkStereoTool::enterEvent(const QHoverEvent& event)
//{
//}

//void MarkStereoTool::leaveEvent(const QHoverEvent& event)
//{
//}

//void MarkStereoTool::moveEvent(const QHoverEvent& event)
//{
//}

void MarkStereoTool::mousePressed(const QMouseEvent & event)
{
    // Add mark
    if (event.buttons() & Qt::LeftButton)
    {
        QPointF lLocal = _display->getPositionLeft(event.pos()+_display->getLeftCursorOffset().toPoint());
        QPointF rLocal = _display->getPositionRight(event.pos()+_display->getRightCursorOffset().toPoint());
        if ((lLocal.x() >= 0 && lLocal.y() >= 0 && lLocal.x() <= _display->getCurrentScene()->getLeftScene()->getWidth() && lLocal.y() <= _display->getCurrentScene()->getLeftScene()->getHeight())
            && (rLocal.x() >= 0 && rLocal.y() >= 0 && rLocal.x() <= _display->getCurrentScene()->getRightScene()->getWidth() && rLocal.y() <= _display->getCurrentScene()->getRightScene()->getHeight()))
        {
            putClickOn(lLocal, rLocal);
        }
    }
    StereoTool::mousePressed(event);
}

//void MarkStereoTool::mouseReleased(const QMouseEvent & event)
//{
//}

//void MarkStereoTool::mouseMoved(const QMouseEvent & event)
//{
//}

//void MarkStereoTool::mouseDblClicked(const QMouseEvent & event)
//{
//}

//void MarkStereoTool::wheelEvent(const QWheelEvent & event)
//{
//}



NearStereoTool::NearStereoTool(StereoDisplay* display) :
    StereoTool(display)
{
    _nearDock = new QDockWidget("Detailview");
    _nearDock->setFeatures(QDockWidget::NoDockWidgetFeatures | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);

    _leftNear = _display->getLeftNearDisplay();
    _rightNear = _display->getRightNearDisplay();
    //_leftNear->setHidden(true);
    //_rightNear->setHidden(true);
    _leftNear->setActivatedTool(this);
    _rightNear->setActivatedTool(this);
    _leftNear->setCursor(QPixmap::fromImage(SymbolsResource::getBordedCross(QColor(255,255,255,255),QColor(0,0,0,255),QSize(25,25))));
    _rightNear->setCursor(QPixmap::fromImage(SymbolsResource::getBordedCross(QColor(255,255,255,255),QColor(0,0,0,255),QSize(25,25))));

    QWidget* widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(widget);
    layout->addWidget(_leftNear);
    layout->addWidget(_rightNear);
    widget->setLayout(layout);
    _nearDock->setWidget(widget);

    _cursorIsVisible = false;
    //_marker = NULL;
}

NearStereoTool::~NearStereoTool()
{
}

QDockWidget* NearStereoTool::getNearDock()
{
    return _nearDock;
}

SingleDisplay* NearStereoTool::getLeftNear()
{
    return _leftNear;
}

SingleDisplay* NearStereoTool::getRightNear()
{
    return _rightNear;
}

bool NearStereoTool::nearIsVisible()
{
    return _nearDock->isVisible();
}

void NearStereoTool::setNearVisible(bool status)
{
    if (status && !_nearDock->isVisible())
        _nearDock->show();
    _nearDock->setHidden(!status);
}

/* Methods into disuse:
void NearStereoTool::setMarker(MarkTool *marker)
{
    _marker = marker;
}

void NearStereoTool::setNearCursor(QCursor cursor)
{
    //_leftNear->setCursor(cursor);
    //_rightNear->setCursor(cursor);
}
*/

void NearStereoTool::paintEvent(const QPaintEvent &event)
{
    event.isAccepted();
    QPixmap ico = QPixmap::fromImage(_display->getCursor());
    QRect reg(QPoint(), ico.size());
    if (_leftNear->painting())
    {
        QPainter painter(_leftNear);
        {
            reg.moveCenter(QPoint(_leftNear->width()/2, _leftNear->height()/2));
            painter.drawPixmap(reg,ico);
            //painter.end();
        }
    }
    if (_rightNear->painting())
    {
        QPainter painter(_rightNear);
        {
            reg.moveCenter(QPoint(_rightNear->width()/2, _rightNear->height()/2));
            painter.drawPixmap(reg,ico);
            //painter.end();
        }
    }
}

void NearStereoTool::resizeEvent(const QResizeEvent &event)
{
    event.isAccepted();
}

void NearStereoTool::enterEvent(const QHoverEvent& event)
{
    event.isAccepted();
}

void NearStereoTool::leaveEvent(const QHoverEvent& event)
{
    event.isAccepted();
}

void NearStereoTool::moveEvent(const QHoverEvent& event)
{
    event.isAccepted();
    /*
    if (_near->positionIsVisible(_display->getLastMousePosition()) || _cursorIsVisible)
    {
        _near->update();
    }
    _near->updateMousePosition();

    actualizePosLabel(_near);*/
}

void NearStereoTool::mousePressed(const QMouseEvent & event)
{
    event.isAccepted();
}

void NearStereoTool::mouseReleased(const QMouseEvent & event)
{
    event.isAccepted();
}

void NearStereoTool::mouseMoved(const QMouseEvent & event)
{
    event.isAccepted();
}

void NearStereoTool::mouseDblClicked(const QMouseEvent & event)
{
    event.isAccepted();
}

void NearStereoTool::wheelEvent(const QWheelEvent & event)
{
    event.isAccepted();
}



OverStereoTool::OverStereoTool(StereoDisplay* display) :
    StereoTool(display)
{
    _overDock = new QDockWidget("Overview");
    _overDock->setFeatures(QDockWidget::NoDockWidgetFeatures | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);

    _leftOver = _display->getLeftOverDisplay();
    _rightOver = _display->getRightOverDisplay();
    //_leftOver->setHidden(true);
    //_rightOver->setHidden(true);
    //_leftOver->setActivatedTool(this);
    //_rightOver->setActivatedTool(this);

    QWidget* widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(widget);
    layout->addWidget(_leftOver);
    layout->addWidget(_rightOver);
    widget->setLayout(layout);
    _overDock->setWidget(widget);

    _onMove = false;
}

OverStereoTool::~OverStereoTool()
{
}

QDockWidget* OverStereoTool::getOverDock()
{
    return _overDock;
}

SingleDisplay* OverStereoTool::getLeftOver()
{
    return _leftOver;
}

SingleDisplay* OverStereoTool::getRightOver()
{
    return _rightOver;
}

bool OverStereoTool::overIsVisible()
{
    return _overDock->isVisible();
}

void OverStereoTool::setOverVisible(bool status)
{
    if (status && !_overDock->isVisible())
        _overDock->show();
    _overDock->setHidden(!status);
}

void OverStereoTool::paintEvent(const QPaintEvent &event)
{
    event.isAccepted();
}

void OverStereoTool::resizeEvent(const QResizeEvent &event)
{
    event.isAccepted();
}

void OverStereoTool::enterEvent(const QHoverEvent& event)
{
    event.isAccepted();
}

void OverStereoTool::leaveEvent(const QHoverEvent& event)
{
    event.isAccepted();
}

void OverStereoTool::moveEvent(const QHoverEvent& event)
{
    event.isAccepted();
    _leftOver->updateMousePosition();
    _rightOver->updateMousePosition();
    /*
    if (_display->positionIsVisible(_over->getLastMousePosition()))
    {
        if (!_onMove)//_over->cursor().pixmap() != QPixmap::fromImage(SymbolsResource::getPointingHand()))
            _over->setCursor(QPixmap::fromImage(SymbolsResource::getOpenHand()));
    }
    else
    {
        if (!_onMove)//_over->cursor().pixmap() != QPixmap::fromImage(SymbolsResource::getOpenHand()))
            _over->setCursor(QPixmap::fromImage(SymbolsResource::getPointingHand()));
    }
    */
    //actualizePosLabel(_leftOver);
    //actualizePosLabel(_rightOver);
}

void OverStereoTool::mousePressed(const QMouseEvent & event)
{
    event.isAccepted();
}

void OverStereoTool::mouseReleased(const QMouseEvent & event)
{
    event.isAccepted();
}

void OverStereoTool::mouseMoved(const QMouseEvent & event)
{
    event.isAccepted();
}

void OverStereoTool::mouseDblClicked(const QMouseEvent & event)
{
    event.isAccepted();
}

void OverStereoTool::wheelEvent(const QWheelEvent & event)
{
    event.isAccepted();
}



StereoToolsBar::StereoToolsBar(StereoDisplay *display, QWidget *parent) :
    QToolBar(parent),
    _zoom(display),
    _move(display),
    _mark(display),
    _near(display),
    _over(display)
{
    _display = display;

    setZoomTool = new QAction(QIcon(":/icon/zoomIcon"),                  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Zoom</span></p></body></html>", this);
    setMoveTool = new QAction(QIcon(":/icon/moveIcon"),                  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Move</span></p></body></html>", this);
    setMarkTool = new QAction(QIcon(":/icon/markIcon"),                  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Mark</span></p></body></html>", this);
    showOverview = new QAction(QIcon(":/icon/overIcon"),                 "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Overview</span></p></body></html>", this);
    showNearview = new QAction(QIcon(":/icon/detailIcon"),               "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Nearview</span></p></body></html>", this);
    useAntialias = new QAction(QIcon(":/icon/aliasingIcon"),             "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Antialias</span></p></body></html>", this);
    openLeftImage = new QAction(QIcon(":/icon/fileopen.png"),            "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Open Left</span></p></body></html>", this);
    openRightImage = new QAction(QIcon(":/icon/fileopen.png"),           "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Open Right</span></p></body></html>", this);
    saveLeftImage = new QAction(QIcon(":/icon/disquette.png"),           "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Save Left</span></p></body></html>", this);
    saveRightImage = new QAction(QIcon(":/icon/disquette.png"),          "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Save Right</span></p></body></html>", this);
    setFitView = new QAction(QIcon(":/icon/fit.png"),                    "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Fit View</span></p></body></html>", this);

    //scaleSpinBox = new QDoubleSpinBox(this);
    //scaleSpinBox->setSuffix(" %");
    //scaleSpinBox->setDecimals(0);
    //scaleSpinBox->setRange(0,800);
    //scaleSpinBox->setSingleStep(0);
    //scaleSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    //connect(scaleSpinBox,SIGNAL(editingFinished()),this,SLOT(rescaleDisplay()));

    //_zoom.setScaleSpin(scaleSpinBox);
    //_move.setScaleSpin(scaleSpinBox);
    //_mark.setScaleSpin(scaleSpinBox);
    //_near.setScaleSpin(scaleSpinBox);
    //_over.setScaleSpin(scaleSpinBox);

    _leftInfoLabel = new QLabel("   ", 0);
    _rightInfoLabel = new QLabel("   ", 0);
    _stereoInfoLabel = new QLabel("   ", 0);

    _zoom.setPosLabel(_leftInfoLabel, _rightInfoLabel, _stereoInfoLabel);
    _move.setPosLabel(_leftInfoLabel, _rightInfoLabel, _stereoInfoLabel);
    _mark.setPosLabel(_leftInfoLabel, _rightInfoLabel, _stereoInfoLabel);
    _near.setPosLabel(_leftInfoLabel, _rightInfoLabel, _stereoInfoLabel);
    _over.setPosLabel(_leftInfoLabel, _rightInfoLabel, _stereoInfoLabel);

    //detailComboBox = new QComboBox(this);
    //detailComboBox->addItems(QString("1x 2x 4x 8x").split(" "));
    //detailComboBox->setCurrentIndex(1);
    //_display->getCurrentScene()->setDetailZoom(2.0);
    //connect(detailComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeDetailZoom(int)));

        setMarkTool->setShortcut(Qt::CTRL + Qt::Key_Z);
        setMoveTool->setShortcut(Qt::CTRL + Qt::Key_X);

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
    //addWidget(scaleSpinBox);
    addSeparator();
    addAction(showOverview);
    addAction(showNearview);
    //addWidget(detailComboBox);

    setZoomTool->setCheckable(true);
    setMoveTool->setCheckable(true);
    setMarkTool->setCheckable(true);
    showOverview->setCheckable(true);
    showNearview->setCheckable(true);
    useAntialias->setCheckable(true);
    showOverview->setChecked(true);
    showNearview->setChecked(true);

    _display->setActivatedTool(&_near);

    QActionGroup* navegation = new QActionGroup(this);
    navegation->addAction(setZoomTool);
    navegation->addAction(setMoveTool);
    navegation->addAction(setMarkTool);
    navegation->setExclusive(true);

    connect(this, SIGNAL(actionTriggered(QAction*)), this, SLOT(executeAction(QAction*)));
    currentTool = &_move;
    setMoveTool->trigger();
}

SingleDisplay* StereoToolsBar::getLeftNearview()
{
    return _near.getLeftNear();
}

SingleDisplay* StereoToolsBar::getRightNearview()
{
    return _near.getRightNear();
}

QDockWidget* StereoToolsBar::getNearviews()
{
    return _near.getNearDock();
}

SingleDisplay* StereoToolsBar::getLeftOverview()
{
    return _over.getLeftOver();
}

SingleDisplay* StereoToolsBar::getRightOverview()
{
    return _over.getRightOver();
}

QDockWidget* StereoToolsBar::getOverviews()
{
    return _over.getOverDock();
}

void StereoToolsBar::clearStereoInfoLabel()
{
    QString stereoInfo = QString("   ");
    _stereoInfoLabel->setText(stereoInfo);
}

void StereoToolsBar::actualizeStereoInfoLabel(double X, double Y, double Z)
{
    QString stereoInfo = QString("Object: ") + QString::number(X,'f',2) + QString(" x ") + QString::number(Y,'f',2) + QString(" x ") + QString::number(Z,'f',2);
    _stereoInfoLabel->setText(stereoInfo);
}

void  StereoToolsBar::setOpenVisible(bool status)
{
    openLeftImage->setVisible(status);
    openRightImage->setVisible(status);
}

void  StereoToolsBar::setSaveVisible(bool status)
{
    saveLeftImage->setVisible(status);
    saveRightImage->setVisible(status);
}

void  StereoToolsBar::setMarkVisible(bool status)
{
    setMarkTool->setVisible(status);
}

void StereoToolsBar::changeMode(int mode)
{
    if (mode ==  1)
    {
        setMarkTool->trigger();
    }
    if (mode ==  2)
    {
        setMoveTool->trigger();
    }
    if (mode ==  3)
    {
        setZoomTool->trigger();
    }
}

void StereoToolsBar::executeAction(QAction *action)
{
    if (action ==  setZoomTool )
    {
        _display->setActivatedTool(currentTool, false);
        _display->setActivatedTool(currentTool = &_zoom);
        _zoom.setCursor(SymbolsResource::getMagnifyGlass(), false);
        _leftInfoLabel->setHidden(true);
        _rightInfoLabel->setHidden(true);
        _stereoInfoLabel->setHidden(true);
        //_display->setCursor(QCursor(QPixmap::fromImage(SymbolsResource::getMagnifyGlass())));
    }
    if (action ==  setMoveTool )
    {
        _display->setActivatedTool(currentTool, false);
        _display->setActivatedTool(currentTool = &_move);
        _move.setCursor(SymbolsResource::getOpenHand(), false);
        _leftInfoLabel->setHidden(true);
        _rightInfoLabel->setHidden(true);
        _stereoInfoLabel->setHidden(true);
        //_display->setCursor(QCursor(QPixmap::fromImage(SymbolsResource::getOpenHand())));
    }
    if (action ==  setMarkTool )
    {
        _display->setActivatedTool(currentTool, false);
        _display->setActivatedTool(currentTool = &_mark);
        _mark.setCursor(SymbolsResource::getBordedCross(QColor(255,255,255,255), QColor(0,0,0,255), QSize(25, 25)),true);
        _leftInfoLabel->setHidden(false);
        _rightInfoLabel->setHidden(false);
        _stereoInfoLabel->setHidden(false);
        //_display->setCursor(QCursor(QPixmap::fromImage(SymbolsResource::getBordedCross(QColor(255,255,255,255), QColor(0,0,0,255), QSize(25, 25)))));
    }
    if (action ==  setFitView )
    {
        _display->fitView();
        //currentTool->actualizeScaleSpin(_display->getCurrentScene()->getScale());
    }
    if (action ==  showOverview )
    {
        _over.setOverVisible(!_over.overIsVisible());
    }
    if (action ==  showNearview )
    {
        _near.setNearVisible(!_near.nearIsVisible());
        _display->setActivatedTool(&_near, _near.nearIsVisible());
        //detailComboBox->setEnabled(_near.nearIsVisible());
    }
    if (action == useAntialias)
    {
        //_display->getCurrentScene()->useSmooth(useAntialias->isChecked());
        _display->updateAll();
    }
    if (action ==  openLeftImage )
    {
        QString filename = QFileDialog::getOpenFileName(0, "Open Image", ".", "*.bmp *.png *.tif *.ppm *.jpg");
        if (!filename.isEmpty())
        {
            SingleScene* scene = (SingleScene*)_display->getCurrentScene()->getLeftScene();
            scene->loadImage(filename);
            _display->updateAll();
        }
    }
    if (action ==  saveLeftImage )
    {
    }
    if (action ==  openRightImage )
    {
        QString filename = QFileDialog::getOpenFileName(0, "Open Image", ".", "*.bmp *.png *.tif *.ppm *.jpg");
        if (!filename.isEmpty())
        {
            SingleScene* scene = (SingleScene*)_display->getCurrentScene()->getRightScene();
            scene->loadImage(filename);
            _display->updateAll();
        }
    }
    if (action ==  saveRightImage )
    {
    }
}

/* mousePressed
if (e->button() == Qt::RightButton)//:MidButton)
{
 moveLastPos_ = e->posF();
 onMove_ = true;
}
else if (e->button() == Qt::LeftButton)
{
 moveLastPos_ = e->posF();
 if (stereoDisplay_->getCurrentScene())
 {
  QPointF diffTocenter(moveLastPos_.x() - size().width() / 2, moveLastPos_.y() - size().height() / 2);
  QPointF* leftDetail = NULL;
  QPointF* rightDetail = NULL;
  if (stereoDisplay_->getCurrentScene()->getLeftScene() && stereoDisplay_->getCurrentScene()->getLeftScene()->imageLoaded())
   leftDetail = new QPointF(stereoDisplay_->getCurrentScene()->getLeftScene()->getViewpoint() + (diffTocenter+stereoDisplay_->getLeftCursorOffset()) / stereoDisplay_->getCurrentScene()->getLeftScene()->getScale());
  if (stereoDisplay_->getCurrentScene()->getRightScene() && stereoDisplay_->getCurrentScene()->getRightScene()->imageLoaded())
   rightDetail = new QPointF(stereoDisplay_->getCurrentScene()->getRightScene()->getViewpoint() + (diffTocenter+stereoDisplay_->getRightCursorOffset()) / stereoDisplay_->getCurrentScene()->getRightScene()->getScale());
  stereoDisplay_->updateDetail(leftDetail, rightDetail, true);
  if (leftDetail)
   delete(leftDetail);
  if (rightDetail)
   delete(rightDetail);
 }
}
QWidget::mousePressEvent(e);
*/

/* mouseReleased
onMove_ = false;
QWidget::mouseReleaseEvent(e);
*/

/* mouseMoved
QPointF diff = e->posF() - moveLastPos_;
moveLastPos_ = e->posF();
if (stereoDisplay_->getCurrentScene())
{
 QPointF diffTocenter(moveLastPos_.x() -diff.x() - size().width() / 2, moveLastPos_.y() -diff.y() - size().height() / 2);
 QPointF* leftDetail = NULL;
 QPointF* rightDetail = NULL;*/
/*
 if (stereoDisplay_->getCurrentScene()->getLeftScene() && stereoDisplay_->getCurrentScene()->getLeftScene()->imageLoaded())
  leftDetail = new QPointF(stereoDisplay_->getCurrentScene()->getLeftScene()->getViewpoint() + (diffTocenter+stereoDisplay_->getLeftCursorOffset()) / stereoDisplay_->getCurrentScene()->getLeftScene()->getScale());
 if (stereoDisplay_->getCurrentScene()->getRightScene() && stereoDisplay_->getCurrentScene()->getRightScene()->imageLoaded())
  rightDetail = new QPointF(stereoDisplay_->getCurrentScene()->getRightScene()->getViewpoint() + (diffTocenter+stereoDisplay_->getRightCursorOffset()) / stereoDisplay_->getCurrentScene()->getRightScene()->getScale());

 if (onMove_ && (e->buttons() & Qt::RightButton))//Qt::MidButton))
 {
  if (stereoDisplay_->getCurrentScene()->getLeftScene())
  {
   double lscale = stereoDisplay_->getCurrentScene()->getLeftScene()->getScale();
   stereoDisplay_->getCurrentScene()->getLeftScene()->pan(-(diff/lscale));
  }
  if (stereoDisplay_->getCurrentScene()->getRightScene())
  {
   double rscale = stereoDisplay_->getCurrentScene()->getRightScene()->getScale();
   stereoDisplay_->getCurrentScene()->getRightScene()->pan(-(diff/rscale));
  }
  stereoDisplay_->updateAll(leftDetail, rightDetail);
 }
 else */ /* if (!onMove_)
 {
  update();
  stereoDisplay_->updateDetail(leftDetail, rightDetail);
 }

 if (leftDetail)
  delete(leftDetail);
 if (rightDetail)
  delete(rightDetail);
}
QWidget::mouseMoveEvent(e); */

/* wheelEvent
int numDegrees = e->delta() / 8.0;
int numSteps = numDegrees / 15.0;
if (stereoDisplay_->getCurrentScene() && stereoDisplay_->getCurrentScene()->getLeftScene() && stereoDisplay_->getCurrentScene()->getRightScene())
{
 if (e->orientation() == Qt::Vertical)
 {
  double zoomStep;
  if (numSteps>0)
   zoomStep = 1.044273782; // 1*2^(1÷(2^4))
  else if (numSteps<0)
   zoomStep = 0.957603281; // 1/2^(1÷(2^4))
  for (int i = 0; i<abs(numSteps);i++)
  {
   stereoDisplay_->getCurrentScene()->getLeftScene()->zoom(zoomStep);
   stereoDisplay_->getCurrentScene()->getRightScene()->zoom(zoomStep);
  }
  stereoDisplay_->updateAll();
 }
}
QWidget::wheelEvent(e);
*/

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
