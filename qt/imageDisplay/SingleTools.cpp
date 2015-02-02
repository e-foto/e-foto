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

void SingleTool::paintEvent(const QPaintEvent& event)
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
            QPoint endBar(_display->screenPosition(_fixedPointOnImage).x(), _display->getMouseScreenPosition().y());

            painter.setPen(QPen(QBrush(Qt::yellow), 7, Qt::SolidLine, Qt::RoundCap));
            //painter.drawPoint(_fixedPoint);
            painter.drawPoint(_display->screenPosition(_fixedPointOnImage));

            painter.setPen(QPen(QBrush(Qt::yellow), 2));
            //painter.drawLine(_fixedPoint, endBar);
            painter.drawLine(_display->screenPosition(_fixedPointOnImage), endBar);
            painter.drawLine(QPoint(endBar.x() - 3, endBar.y()), QPoint(endBar.x() + 3, endBar.y()));
            painter.drawText(QPoint(endBar.x() + 5, endBar.y() + 5), QString::number(_display->getCurrentScene()->getScale()*100,'f', 1).append("%"));
        }
        //painter.end();
    }
}

void SingleTool::resizeEvent(const QResizeEvent& event)
{
    event.isAccepted();
    actualizeScaleSpin(_display->getCurrentScene()->getScale());
}

void SingleTool::enterEvent(const QHoverEvent& event)
{
    event.isAccepted();
}

void SingleTool::leaveEvent(const QHoverEvent& event)
{
    event.isAccepted();
}

void SingleTool::moveEvent(const QHoverEvent& event)
{
    // Update mouse position only
    _display->updateMousePosition();

    if (_actualizePosLabel)
        actualizePosLabel(_display, true);

    if (!_display->showDetailedArea())
    {
        SingleScene* scene = (SingleScene*)_display->getCurrentScene();
        scene->setDetailedPoint(_display->getPosition(event.pos()));
        _display->updateDetail();
    }
}

void SingleTool::mousePressed(const QMouseEvent &event)
{
    _currentCursor = _display->cursor();

    // Prepair to zoom default (scaleBar).
    if (event.buttons() & Qt::MidButton)
    {
        _fixedPoint = event.pos();
        _fixedPointOnImage = _display->getPosition(_fixedPoint);
        _scale = _display->getCurrentScene()->getScale();
        _display->setCursor(QCursor(QPixmap::fromImage(SymbolsResource::getLeftArrow())));
        _actualizePosLabel = false;
    }
    // Prepair move reference
    else if (event.buttons() & Qt::RightButton)
    {
        _display->setCursor(QCursor(NOCURSOR));
    }
    _lastMousePosition = event.pos();

    if ((event.button() & Qt::LeftButton) || (event.button() & Qt::MidButton))
    {
        SingleScene* scene = (SingleScene*)_display->getCurrentScene();
        scene->setDetailedPoint(_display->getPosition(event.pos()));
    }
}

void SingleTool::mouseReleased(const QMouseEvent &event)
{
    //if (_autoPan != QPoint(0,0) || _scale != -1)
    _display->setCursor(_currentCursor);

    // Stop move default (autoMove).
    _autoPan = QPoint(0,0);

    // Stop zoom default (scaleBar).
    _scale = -1;

    if (!_actualizePosLabel) // OnScaleBar
    {
        //QCursor::setPos(_display->mapToGlobal(_fixedPoint));
        QCursor::setPos(_display->mapToGlobal(_display->screenPosition(_fixedPointOnImage).toPoint()));
        _actualizePosLabel = true;
        SingleScene* scene = (SingleScene*)_display->getCurrentScene();
        //scene->setDetailedPoint(_display->getPosition(_fixedPoint));
        scene->setDetailedPoint(_fixedPointOnImage);
    }
    else
    {
        SingleScene* scene = (SingleScene*)_display->getCurrentScene();
        scene->setDetailedPoint(_display->getPosition(event.pos()));
    }
    _display->update();
}

void SingleTool::mouseMoved(const QMouseEvent &event)
{
    // Make zoom default (scaleBar).
    if (event.buttons() & Qt::MidButton)
    {
        //int diff = event.pos().y() - _fixedPoint.y();
        int diff = event.pos().y() - _display->screenPosition(_fixedPointOnImage).y();
        double newScale = (_scale*100 - diff)/100;
        //qDebug("screenPositionY = %f, diff = %d, newScale = %f e fixedPoint = (%f, %f)", _display->screenPosition(_fixedPointOnImage).y(), diff, newScale, _fixedPointOnImage.x(), _fixedPointOnImage.y());
        //_display->getCurrentScene()->scaleTo(newScale, _display->getPosition(_fixedPoint));
        _display->getCurrentScene()->scaleTo(newScale, _fixedPointOnImage);
        //if (event.pos().x() < _fixedPoint.x())
        if (event.pos().x() < _display->screenPosition(_fixedPointOnImage).x())
            _display->setCursor(QCursor(QPixmap::fromImage(SymbolsResource::getRightArrow())));
        else
            _display->setCursor(QCursor(QPixmap::fromImage(SymbolsResource::getLeftArrow())));
        actualizeScaleSpin(_display->getCurrentScene()->getScale());
        _display->update();

        if (_propagateScaleTo != NULL)
            _propagateScaleTo->propagateScale(newScale, _display->screenPosition(_fixedPointOnImage).toPoint());
    }

    // Make move default (autoMove).
    if (event.buttons() & Qt::RightButton)
    {
        double scale = _display->getCurrentScene()->getScale();
        QPointF diff = event.posF() - _lastMousePosition;
        _autoPan = -(diff/(5*scale));
        _display->setCursor(QCursor(QPixmap::fromImage(SymbolsResource::getText(QString::fromUtf8("Auto")))));//,_autoPan.y() < 0))));
    }
}

void SingleTool::mouseDblClicked(const QMouseEvent & event)
{
    event.isAccepted();
}

void SingleTool::wheelEvent(const QWheelEvent & event)
{
    event.isAccepted();
}

void SingleTool::autoMove()
{
    // Execute move default (autoMove).
    if (_autoPan != QPointF(0,0))
    {
        _display->getCurrentScene()->pan(-_autoPan);

        SingleScene* scene = (SingleScene*)_display->getCurrentScene();
        scene->setDetailedPoint(_display->getLastMousePosition());
        actualizePosLabel(_display, true);

        _display->blockShowDetailedArea(true);
        _display->update();
        _display->blockShowDetailedArea(false);

        if (_propagateMoveTo != NULL)
            _propagateMoveTo->propagateMove(-_autoPan);
    }
}

void SingleTool::actualizeScaleSpin(double scale)
{
    if (_scaleSpin)
    {
        _scaleSpin->blockSignals(true);
        _scaleSpin->setValue(scale*100);
        _scaleSpin->blockSignals(false);
    }
}

void SingleTool::actualizePosLabel(SingleDisplay* display, bool force)
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

void SingleTool::propagateMove(QPointF desloc)
{
    _display->getCurrentScene()->pan(desloc);
    _display->update();
}

void SingleTool::propagateScale(double scale, QPoint at, int movementMode)
{
    if (movementMode == 3) // to Overview zooming
    {
        _display->getCurrentScene()->scaleTo(scale);
    }
    if (movementMode == 2) // to Fitview
    {
        _display->getCurrentScene()->centerContent();
        _display->getCurrentScene()->scaleTo(scale);
    }
    else if (movementMode == 1) // to Rubberband zooming
    {
        _display->getCurrentScene()->moveTo(_display->getPosition(at));
        _display->getCurrentScene()->scaleTo(scale);
    }
    else // to ScaleBar
        _display->getCurrentScene()->scaleTo(scale, _display->getPosition(at));

    _display->update();
    actualizeScaleSpin(_display->getCurrentScene()->getScale());
}

void SingleTool::setImageMode()
{
    _xi = _yi = _zi = 0.0;
    _dx = _dy = _dz = 1.0;
    _printZ = false;
    _invertY = false;
}

void SingleTool::setOrtoImageMode(double xi, double dx, double yi, double dy)
{
    _xi = xi;
    _yi = yi;
    _zi = 0.0;
    _dx = dx;
    _dy = dy;
    _dz = 1.0;
    _printZ = false;
    _invertY = true;
}

void SingleTool::setElevationImageMode(double xi, double dx, double yi, double dy, double zi, double dz)
{
    _xi = xi;
    _yi = yi;
    _zi = zi;
    _dx = dx;
    _dy = dy;
    _dz = dz;
    _printZ = true;
    _invertY = true;
}



ZoomTool::ZoomTool(SingleDisplay* display) :
    SingleTool(display)
{
    _onRubberBand = false;
}

ZoomTool::~ZoomTool()
{
}

void ZoomTool::paintEvent(const QPaintEvent& event)
{
    if (_onRubberBand)
    {
        QRect rubber(_fixedPoint, _display->getMouseScreenPosition().toPoint());
        if (abs(rubber.width()) < 8 && abs(rubber.height()) < 8)
        {
            _display->setCursor(QCursor(QPixmap::fromImage(SymbolsResource::getMagnifyGlass("-"))));
            return;
        }
        else
            _display->setCursor(QCursor(QPixmap::fromImage(SymbolsResource::getMagnifyGlass("+"))));
        QPainter painter(_display);
        painter.setPen(QPen(Qt::yellow));
        painter.drawRect(QRect(_fixedPoint,_display->getMouseScreenPosition().toPoint()));
        painter.end();
    }
    SingleTool::paintEvent(event);
}

//void ZoomTool::paintEvent(const QPaintEvent& event)
//{
//}

//void ZoomTool::resizeEvent(const QResizeEvent &event)
//{
//}

//void ZoomTool::enterEvent(const QHoverEvent& event)
//{
//}

//void ZoomTool::leaveEvent(const QHoverEvent& event)
//{
//}

//void ZoomTool::moveEvent(const QHoverEvent& event)
//{
//}

void ZoomTool::mousePressed(const QMouseEvent & event)
{
    if (event.button() != event.buttons())
        return;

    // Rubberband zoom
    if (event.buttons() & Qt::LeftButton)
    {
        _fixedPoint = event.pos();
        _onRubberBand = true;
        _currentCursor = _display->cursor();
        _display->setCursor(QCursor(QPixmap::fromImage(SymbolsResource::getMagnifyGlass("-"))));
        _display->blockShowDetailedArea(true);
        _display->update();
        return;
    }
    SingleTool::mousePressed(event);
}

void ZoomTool::mouseReleased(const QMouseEvent & event)
{
    QMouseEvent e = event;

    // Rubberband zoom
    if (_onRubberBand)
    {
        QRect rubber(_fixedPoint, event.pos());
        if (abs(rubber.width()) < 8 && abs(rubber.height()) < 8)
        {
            _display->getCurrentScene()->zoom(0.8, _display->getPosition(_fixedPoint));
            if (_propagateScaleTo != NULL)
                _propagateScaleTo->propagateScale(_display->getCurrentScene()->getScale(), _fixedPoint);
        }
        else
        {
            double wscale = abs(rubber.width()) == 0 ? 1024 : _display->width()/(double)abs(rubber.width());
            double hscale = abs(rubber.height()) == 0 ? 1024 : _display->height()/(double)abs(rubber.height());
            _display->getCurrentScene()->moveTo(_display->getPosition(rubber.center()));
            _display->getCurrentScene()->zoom(wscale < hscale ? wscale : hscale);

            QCursor::setPos(_display->mapToGlobal(_display->screenPosition(_display->getCurrentScene()->getViewpoint()).toPoint()));
            e = QMouseEvent(QEvent::MouseButtonRelease, _display->screenPosition(_display->getCurrentScene()->getViewpoint()).toPoint(), event.button(), event.buttons(), event.modifiers());
            //_actualizePosLabel = true;

            if (_propagateScaleTo != NULL)
                _propagateScaleTo->propagateScale(_display->getCurrentScene()->getScale(), rubber.center(), 1);
        }
        _onRubberBand = false;
        _display->blockShowDetailedArea(false);
        //_display->setCursor(QCursor(QPixmap::fromImage(SymbolsResource::getMagnifyGlass())));
        actualizeScaleSpin(_display->getCurrentScene()->getScale());
    }
    SingleTool::mouseReleased(e);
}

void ZoomTool::mouseMoved(const QMouseEvent & event)
{
    // Rubberband zoom.
    if (event.buttons() & Qt::LeftButton)
    {
        _display->update();
        return;
    }
    SingleTool::mouseMoved(event);
}

void ZoomTool::mouseDblClicked(const QMouseEvent & event)
{
    event.isAccepted();
    _display->fitView();
    actualizeScaleSpin(_display->getCurrentScene()->getScale());

    if (_propagateScaleTo != NULL)
        _propagateScaleTo->propagateScale(_display->getCurrentScene()->getScale(), QPoint(), 2);
}

//void ZoomTool::wheelEvent(const QWheelEvent & event)
//{
//}



MoveTool::MoveTool(SingleDisplay* display) :
    SingleTool(display)
{
}

MoveTool::~MoveTool()
{
}

//void MoveTool::paintEvent(const QPaintEvent& event)
//{
//}

//void MoveTool::resizeEvent(const QResizeEvent &event)
//{
//}

//void MoveTool::enterEvent(const QHoverEvent& event)
//{
//}

//void MoveTool::leaveEvent(const QHoverEvent& event)
//{
//}

//void MoveTool::moveEvent(const QHoverEvent& event)
//{
//}

void MoveTool::mousePressed(const QMouseEvent & event)
{
    if (event.button() != event.buttons())
        return;
    SingleTool::mousePressed(event);
    if (event.buttons() & Qt::LeftButton)
        _display->setCursor(QPixmap::fromImage(SymbolsResource::getClosedHand()));
    _display->update();
}

void MoveTool::mouseReleased(const QMouseEvent & event)
{
    SingleTool::mouseReleased(event);
    _display->setCursor(QPixmap::fromImage(SymbolsResource::getOpenHand()));
}

void MoveTool::mouseMoved(const QMouseEvent & event)
{
    // Move
    if (event.buttons() & Qt::LeftButton)
    {
        QPointF diff = event.posF() - _lastMousePosition;
        _lastMousePosition = event.pos();
        double scale;
        scale = _display->getCurrentScene()->getScale();
        _display->getCurrentScene()->pan(-(diff/scale));
        _display->update();

        if (_propagateMoveTo != NULL)
            _propagateMoveTo->propagateMove(-(diff/scale));
        return;
    }
    SingleTool::mouseMoved(event);
}

//void MoveTool::mouseDblClicked(const QMouseEvent & event)
//{
//}

//void MoveTool::wheelEvent(const QWheelEvent & event)
//{
//}



MarkTool::MarkTool(SingleDisplay* display) :
    SingleTool(display),
    mark(GreenMark)
{
    onlyEmitClickedMode = false;
    nextMarkItem = 1;
}

MarkTool::~MarkTool()
{
}

void MarkTool::changeMarker(Marker marker)
{
    mark = marker;
}

Marker* MarkTool::getMarker()
{
    return &mark;
}

void MarkTool::addMark(QPointF location, int key, QString label, Marker *marker)
{
    _display->getCurrentScene()->geometry()->addPoint(location, key, label, marker == NULL ? &mark : marker);
}

void MarkTool::insertMark(QPointF location, int key, QString label, Marker *marker)
{
    _display->getCurrentScene()->geometry()->insertPoint(location, key, label, marker == NULL ? &mark : marker);
}

void MarkTool::deleteMark(int key)
{
    _display->getCurrentScene()->geometry()->deletePoint(key);
}

void MarkTool::clear()
{
    _display->getCurrentScene()->geometry()->clear();
}

void MarkTool::setToOnlyEmitClickedMode()
{
    onlyEmitClickedMode = true;
}

void MarkTool::setToAutoCreateMarkFrom(unsigned int start)
{
    onlyEmitClickedMode = false;
    nextMarkItem = start;
}

void MarkTool::putClickOn(QPointF& pos)
{
    if (onlyEmitClickedMode)
    {
        emit clicked(pos);
    }
    else
    {
        _display->getCurrentScene()->geometry()->insertPoint(pos, nextMarkItem, QString::number(nextMarkItem), &mark);
        _display->update();
        nextMarkItem++;
    }
}

//void MarkTool::paintEvent(const QPaintEvent& event)
//{
//}

//void MarkTool::resizeEvent(const QResizeEvent &event)
//{
//}

//void MarkTool::enterEvent(const QHoverEvent& event)
//{
//}

//void MarkTool::leaveEvent(const QHoverEvent& event)
//{
//}

//void MarkTool::moveEvent(const QHoverEvent& event)
//{
//}

void MarkTool::mousePressed(const QMouseEvent & event)
{
    // Add mark
    if (event.buttons() & Qt::LeftButton)
    {
        QPointF local = _display->getPosition(event.pos());
        if (local.x() >= 0 && local.y() >= 0 && local.x() <= _display->getCurrentScene()->getWidth() && local.y() <= _display->getCurrentScene()->getHeight())
        {
            putClickOn(local);
        }
    }
    SingleTool::mousePressed(event);
}

/* Methods into disuse:
 *
void MarkTool::editMark(int key, QPointF location, Marker *marker)
{
    //_display->getCurrentScene()->geometry()->editPoint(key, location, marker == NULL ? &mark : marker);
}

void MarkTool::editMark(int key, QPointF location, QString label, Marker *marker)
{
    //_display->getCurrentScene()->geometry()->editPoint(key, location, label, marker == NULL ? &mark : marker);
}
*/

//void MarkTool::mouseReleased(const QMouseEvent & event)
//{
//}

//void MarkTool::mouseMoved(const QMouseEvent & event)
//{
//}

//void MarkTool::mouseDblClicked(const QMouseEvent & event)
//{
//}

//void MarkTool::wheelEvent(const QWheelEvent & event)
//{
//}


/*
InfoTool::InfoTool(SingleDisplay* display) :
        SingleTool(display)
{
        setImageMode();
}

InfoTool::~InfoTool()
{
}

void InfoTool::paintEvent(const QPaintEvent& event)
{
}

void InfoTool::resizeEvent(const QResizeEvent &event)
{
}

void InfoTool::enterEvent(const QHoverEvent& event)
{
}

void InfoTool::leaveEvent(const QHoverEvent& event)
{
}

void InfoTool::moveEvent(const QHoverEvent& event)
{
        SingleTool::moveEvent(event);

        actualizePosLabel();
}

void InfoTool::mousePressed(const QMouseEvent & event)
{
}

void InfoTool::mouseReleased(const QMouseEvent & event)
{
}

void InfoTool::mouseMoved(const QMouseEvent & event)
{
}

void InfoTool::mouseDblClicked(const QMouseEvent & event)
{
}

void InfoTool::wheelEvent(const QWheelEvent & event)
{
}
*/


OverTool::OverTool(SingleDisplay* display) :
    SingleTool(display)
{
    _onMove = false;
    _over = _display->getOverDisplay();
    _over->setHidden(true);
    _over->setActivatedTool(this);
    _overDock = new QDockWidget("Overview");
    _overDock->setWidget(_over);
    _overDock->setFeatures(QDockWidget::NoDockWidgetFeatures | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
}

OverTool::~OverTool()
{
}

QDockWidget* OverTool::getOverDock()
{
    return _overDock;
}

bool OverTool::overIsVisible()
{
    return _overDock->isVisible();
}

void OverTool::setOverVisible(bool status)
{
    if (status && !_overDock->isVisible())
        _overDock->show();
    _overDock->setHidden(!status);
}

void OverTool::paintEvent(const QPaintEvent& event)
{
    event.isAccepted();
}

void OverTool::resizeEvent(const QResizeEvent &event)
{
    event.isAccepted();
}

void OverTool::enterEvent(const QHoverEvent& event)
{
    event.isAccepted();
}

void OverTool::leaveEvent(const QHoverEvent& event)
{
    event.isAccepted();
}

void OverTool::moveEvent(const QHoverEvent& event)
{
    event.isAccepted();
    _over->updateMousePosition();
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

    actualizePosLabel(_over);
}

void OverTool::mousePressed(const QMouseEvent & event)
{
    if (event.buttons() & Qt::LeftButton)
    {
        _over->updateMousePosition();
        if (_display->positionIsVisible(_over->getLastMousePosition()))
        {
            _onMove = true;
            _lastPos = _over->getLastMousePosition();
            _over->setCursor(QPixmap::fromImage(SymbolsResource::getClosedHand()));
        }
        else
        {
            _display->getCurrentScene()->moveTo(_over->getLastMousePosition());
            _over->setCursor(QPixmap::fromImage(SymbolsResource::getOpenHand()));
            _display->update();
        }
    }
}

void OverTool::mouseReleased(const QMouseEvent & event)
{
    event.isAccepted();
    _onMove = false;
    _over->setCursor(QPixmap::fromImage(SymbolsResource::getOpenHand()));
}

void OverTool::mouseMoved(const QMouseEvent & event)
{
    if (event.buttons() & Qt::LeftButton)
    {
        if (_onMove)
        {
            _over->updateMousePosition();
            _display->getCurrentScene()->pan(_over->getLastMousePosition() - _lastPos);
            _display->update();
            _lastPos = _over->getLastMousePosition();
        }
    }
}

void OverTool::mouseDblClicked(const QMouseEvent & event)
{
    event.isAccepted();
}

void OverTool::wheelEvent(const QWheelEvent & event)
{
    _over->updateMousePosition();
    if (!(_onMove) && (_display->positionIsVisible(_over->getLastMousePosition())))
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
                _display->getCurrentScene()->zoom(zoomStep, _over->getLastMousePosition());
            _display->update();
            actualizeScaleSpin(_display->getCurrentScene()->getScale());

            if (_propagateScaleTo != NULL)
                _propagateScaleTo->propagateScale(_display->getCurrentScene()->getScale(), _display->screenPosition(_over->getLastMousePosition()).toPoint());
        }
    }
}



NearTool::NearTool(SingleDisplay* display) :
    SingleTool(display)
{
    _near = _display->getDetailDisplay();
    _near->setHidden(true);
    _near->setActivatedTool(this);
    _nearDock = new QDockWidget("Detailview");
    _nearDock->setWidget(_near);
    _nearDock->setFeatures(QDockWidget::NoDockWidgetFeatures | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    _cursorIsVisible = false;
    _marker = NULL;
    _near->setCursor(QPixmap::fromImage(SymbolsResource::getBordedCross(QColor(255,255,255,255),QColor(0,0,0,255),QSize(25,25))));
}

NearTool::~NearTool()
{
}

QDockWidget* NearTool::getNearDock()
{
    return _nearDock;
}

bool NearTool::nearIsVisible()
{
    return _nearDock->isVisible();
}

void NearTool::setNearVisible(bool status)
{
    if (status && !_nearDock->isVisible())
        _nearDock->show();
    _nearDock->setHidden(!status);
}

void NearTool::setMarker(MarkTool *marker)
{
    _marker = marker;
}

void NearTool::setNearCursor(QCursor cursor)
{
    _near->setCursor(cursor);
}

//void NearTool::setActivatedToolOnNear(SingleTool *tool, bool activate)
//{
//	_near->setActivatedTool(tool, activate);
//}

void NearTool::paintEvent(const QPaintEvent& event)
{
    event.isAccepted();
    //if (_near->painting())
    //{
    //QPainter painter(_near);
    //QPixmap ico(_display->cursor().pixmap());
    //QRect reg(QPoint(),ico.size());
    //reg.moveCenter(QPoint(_near->width()/2, _near->height()/2));
    //painter.drawPixmap(reg,ico);
    //painter.end();
    //}

    QPoint pos = _near->screenPosition(_display->getLastMousePosition()).toPoint();
    QPixmap ico(_display->cursor().pixmap());
    QRect reg(QPoint(),ico.size());
    if (_near->painting())
    {
        QPainter painter(_near);
        if (_display->showDetailedArea() && _cursorIsVisible)
        {
            reg.moveCenter(pos);
            painter.drawPixmap(reg,ico);
            painter.end();
        }
        else if (!_display->showDetailedArea() && _cursorIsVisible)
        {
            reg.moveCenter(QPoint(_near->width()/2, _near->height()/2));
            painter.drawPixmap(reg,ico);
            painter.end();
        }
    }
    _cursorIsVisible = reg.intersects(_near->rect()) && _display->visibleRegion().contains(_display->mapFromGlobal(QCursor::pos())) && !_near->visibleRegion().contains(_near->mapFromGlobal(QCursor::pos()));
}

void NearTool::resizeEvent(const QResizeEvent &event)
{
    event.isAccepted();
}

void NearTool::enterEvent(const QHoverEvent& event)
{
    event.isAccepted();
    if (_display->visibleRegion().contains(_display->mapFromGlobal(QCursor::pos())))// && !_near->visibleRegion().contains(_near->mapFromGlobal(QCursor::pos())))
    {
        _cursorIsVisible = true;
        _near->update();
    }
    //else
    //_near->setCursor(_display->cursor());
}

void NearTool::leaveEvent(const QHoverEvent& event)
{
    event.isAccepted();
    if (!_display->visibleRegion().contains(_display->mapFromGlobal(QCursor::pos())))// || _near->visibleRegion().contains(_near->mapFromGlobal(QCursor::pos())))
    {
        _cursorIsVisible = false;
        _near->update();
    }
}

void NearTool::moveEvent(const QHoverEvent& event)
{
    event.isAccepted();
    //if (!_display->showDetailedArea())
    //_display->updateDetail();

    if (_near->positionIsVisible(_display->getLastMousePosition()) || _cursorIsVisible)
    {
        _near->update();
    }
    _near->updateMousePosition();

    actualizePosLabel(_near);
}

void NearTool::mousePressed(const QMouseEvent & event)
{
    //if ((event.button() & Qt::LeftButton) || (event.button() & Qt::MidButton))
    //{
    //SingleScene* scene = (SingleScene*)_display->getCurrentScene();
    //scene->setDetailedPoint(_display->getPosition(event.pos()));
    //}

    // Add mark
    if (_near->visibleRegion().contains(_near->mapFromGlobal(QCursor::pos())) && event.buttons() & Qt::LeftButton)
    {
        QPointF local = _near->getLastMousePosition();
        if (_marker && local.x() >= 0 && local.y() >= 0 && local.x() <= _display->getCurrentScene()->getWidth() && local.y() <= _display->getCurrentScene()->getHeight())
        {
            _marker->putClickOn(local);
            //_display->updateAll();
        }
    }
}

void NearTool::mouseReleased(const QMouseEvent & event)
{
    event.isAccepted();
    //SingleScene* scene = (SingleScene*)_display->getCurrentScene();
    //scene->setDetailedPoint(_display->getPosition(event.pos()));
}

void NearTool::mouseMoved(const QMouseEvent & event)
{
    event.isAccepted();
}

void NearTool::mouseDblClicked(const QMouseEvent & event)
{
    event.isAccepted();
}

void NearTool::wheelEvent(const QWheelEvent & event)
{
    event.isAccepted();
}



SingleToolsBar::SingleToolsBar(SingleDisplay *display, QWidget *parent) :
    QToolBar("Display Tools", parent),
    zoom(display),
    move(display),
    mark(display),
    near_(display),
    over(display)//,
  //info(display)
{
    _display = display;

    setZoomTool = new QAction(QIcon(":/icon/zoomIcon"),                  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Zoom</span></p></body></html>", this);
    setMoveTool = new QAction(QIcon(":/icon/moveIcon"),                  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Move</span></p></body></html>", this);
    setMarkTool = new QAction(QIcon(":/icon/markIcon"),                  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Measure mark</span></p></body></html>", this);
    setFitView = new QAction(QIcon(":/icon/fit.png"),                    "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Fit View</span></p></body></html>", this);
    showOverview = new QAction(QIcon(":/icon/overIcon"),                 "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Overview</span></p></body></html>", this);
    showNearview = new QAction(QIcon(":/icon/detailIcon"),               "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Detailview</span></p></body></html>", this);
    useAntialias = new QAction(QIcon(":/icon/aliasingIcon"),             "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Antialias</span></p></body></html>", this);
    useFixedNearview = new QAction(QIcon(":/icon/fixdetailviewIcon.png"),"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Fixed Detailview</span></p></body></html>", this);
    openImage = new QAction(QIcon(":/icon/fileopen.png"),                "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Open</span></p></body></html>", this);
    saveImage = new QAction(QIcon(":/icon/disquette.png"),               "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Save</span></p></body></html>", this);

    scaleSpinBox = new QDoubleSpinBox(this);
    scaleSpinBox->setSuffix(" %");
    scaleSpinBox->setDecimals(0);
    scaleSpinBox->setRange(0,800);
    scaleSpinBox->setSingleStep(0);
    scaleSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    connect(scaleSpinBox,SIGNAL(editingFinished()),this,SLOT(rescaleDisplay()));

    zoom.setScaleSpin(scaleSpinBox);
    move.setScaleSpin(scaleSpinBox);
    mark.setScaleSpin(scaleSpinBox);
    near_.setScaleSpin(scaleSpinBox);
    over.setScaleSpin(scaleSpinBox);

    _infoLabel = new QLabel("   ", 0);

    zoom.setPosLabel(_infoLabel);
    move.setPosLabel(_infoLabel);
    mark.setPosLabel(_infoLabel);
    near_.setPosLabel(_infoLabel);
    over.setPosLabel(_infoLabel);

    detailComboBox = new QComboBox(this);
    detailComboBox->addItems(QString("1x 2x 4x 8x").split(" "));
    detailComboBox->setCurrentIndex(1);
    SingleScene* scene = (SingleScene*)_display->getCurrentScene();
    scene->setDetailZoom(2.0);
    connect(detailComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeDetailZoom(int)));

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

    _display->setActivatedTool(&near_);

    near_.setMarker(&mark);

    QActionGroup* navegation = new QActionGroup(this);
    navegation->addAction(setZoomTool);
    navegation->addAction(setMoveTool);
    navegation->addAction(setMarkTool);
    navegation->setExclusive(true);

    connect(this, SIGNAL(actionTriggered(QAction*)), this, SLOT(executeAction(QAction*)));
    currentTool = &move;
    setMoveTool->trigger();
}

void  SingleToolsBar::setOpenVisible(bool status)
{
    openImage->setVisible(status);
}

void  SingleToolsBar::setSaveVisible(bool status)
{
    saveImage->setVisible(status);
}

void  SingleToolsBar::setMarkVisible(bool status)
{
    setMarkTool->setVisible(status);
}

QLabel* SingleToolsBar::getInfo()
{
    return _infoLabel;
}

QDockWidget* SingleToolsBar::getOverview()
{
    return over.getOverDock();
}

QDockWidget* SingleToolsBar::getNearview()
{
    return near_.getNearDock();
}

void SingleToolsBar::executeAction(QAction *action)
{
    if (action ==  setZoomTool )
    {
        _display->setActivatedTool(currentTool, false);
        _display->setActivatedTool(currentTool = &zoom);
        _display->setCursor(QCursor(QPixmap::fromImage(SymbolsResource::getMagnifyGlass())));
    }
    if (action ==  setMoveTool )
    {
        _display->setActivatedTool(currentTool, false);
        _display->setActivatedTool(currentTool = &move);
        _display->setCursor(QCursor(QPixmap::fromImage(SymbolsResource::getOpenHand())));
    }
    if (action ==  setMarkTool )
    {
        _display->setActivatedTool(currentTool, false);
        _display->setActivatedTool(currentTool = &mark);
        _display->setCursor(QCursor(QPixmap::fromImage(SymbolsResource::getBordedCross(QColor(255,255,255,255), QColor(0,0,0,255), QSize(25, 25)))));
    }
    if (action ==  setFitView )
    {
        _display->fitView();
        currentTool->actualizeScaleSpin(_display->getCurrentScene()->getScale());
    }
    if (action ==  showOverview )
    {
        over.setOverVisible(!over.overIsVisible());
    }
    if (action ==  showNearview )
    {
        near_.setNearVisible(!near_.nearIsVisible());
        _display->setActivatedTool(&near_, near_.nearIsVisible());
        detailComboBox->setEnabled(near_.nearIsVisible());
    }
    if (action == useAntialias)
    {
        SingleScene* scene = (SingleScene*)_display->getCurrentScene();
        scene->useSmooth(useAntialias->isChecked());
        _display->update();
    }
    if (action == useFixedNearview)
    {
        _display->setShowDetailedArea(useFixedNearview->isChecked());
        _display->update();
    }
    if (action ==  openImage )
    {
        QString filename = QFileDialog::getOpenFileName(0, "Open Image", ".", "*.bmp *.png *.tif *.ppm *.jpg");
        if (!filename.isEmpty())
        {
            SingleScene* scene = (SingleScene*)_display->getCurrentScene();
            scene->loadImage(filename);
            _display->update();
        }
    }
    if (action ==  saveImage )
    {
    }
}

void SingleToolsBar::rescaleDisplay()
{
    int diff = _display->getCurrentScene()->getScale()*100 - scaleSpinBox->value();
    if (diff)
    {
        _display->getCurrentScene()->scaleTo(scaleSpinBox->value()/100);
        _display->update();
        currentTool->actualizeScaleSpin(_display->getCurrentScene()->getScale());
    }
}

void SingleToolsBar::changeDetailZoom(int nz)
{
    SingleScene* scene = (SingleScene*)_display->getCurrentScene();
    switch (nz){
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

void SingleToolsBar::setImageMode()
{
    zoom.setImageMode();
    move.setImageMode();
    mark.setImageMode();
    over.setImageMode();
    near_.setImageMode();
}

void SingleToolsBar::setOrtoImageMode(double xi, double dx, double yi, double dy)
{
    zoom.setOrtoImageMode(xi, dx, yi, dy);
    move.setOrtoImageMode(xi, dx, yi, dy);
    mark.setOrtoImageMode(xi, dx, yi, dy);
    over.setOrtoImageMode(xi, dx, yi, dy);
    near_.setOrtoImageMode(xi, dx, yi, dy);
}

void SingleToolsBar::setElevationImageMode(double xi, double dx, double yi, double dy, double zi, double dz)
{
    zoom.setElevationImageMode(xi, dx, yi, dy, zi, dz);
    move.setElevationImageMode(xi, dx, yi, dy, zi, dz);
    mark.setElevationImageMode(xi, dx, yi, dy, zi, dz);
    over.setElevationImageMode(xi, dx, yi, dy, zi, dz);
    near_.setElevationImageMode(xi, dx, yi, dy, zi, dz);
}






SeparatedStereoToolsBar::SeparatedStereoToolsBar(SingleDisplay *leftDisplay, SingleDisplay *rightDisplay, QWidget *parent) :
    QToolBar("Display Tools",parent),
    leftZoom(leftDisplay),
    leftMove(leftDisplay),
    leftMark(leftDisplay),
    leftNear(leftDisplay),
    leftOver(leftDisplay),
    //leftInfo(leftDisplay),
    rightZoom(rightDisplay),
    rightMove(rightDisplay),
    rightMark(rightDisplay),
    rightNear(rightDisplay),
    rightOver(rightDisplay)//,
  //rightInfo(rightDisplay)
{
    _leftDisplay = leftDisplay;
    _rightDisplay = rightDisplay;
    //_leftDisplay->setActivatedTool(&leftInfo);
    //_rightDisplay->setActivatedTool(&rightInfo);

    /* Modificado Paulo :Tool Tips
        setZoomTool = new QAction(QIcon(":/icon/zoomIcon"),"Zoom", this);
        setMoveTool = new QAction(QIcon(":/icon/moveIcon"),"Move", this);
        setMarkTool = new QAction(QIcon(":/icon/markIcon"),"Mark", this);
        setFitLeftView = new QAction(QIcon(":/icon/fitleftIcon.png"),"Fit Left", this);
        setFitRightView = new QAction(QIcon(":/icon/fitrightIcon.png"),"Fit Right", this);
        setFitBothView = new QAction(QIcon(":/icon/fitbothIcon.png"),"Fit Both", this);
        showOverview = new QAction(QIcon(":/icon/overIcon"),"Overview", this);
        showNearview = new QAction(QIcon(":/icon/detailIcon"),"Nearview", this);
        useAntialias = new QAction(QIcon(":/icon/aliasingIcon"),"Antialias", this);
        openLeftImage = new QAction(QIcon(":/icon/fileopen.png"),"Open Left", this);
        openRightImage = new QAction(QIcon(":/icon/fileopen.png"),"Open Left", this);
        saveLeftImage = new QAction(QIcon(":/icon/disquette.png"),"Save Left", this);
        saveRightImage = new QAction(QIcon(":/icon/disquette.png"),"Save Right", this);
        setEqualMovements = new QAction(QIcon(":/icon/equalmoveIcon.png"),"Equal Movements", this);
        setEqualScales = new QAction(QIcon(":/icon/equalzoomIcon.png"),"Equal Scales", this);
        */

    setZoomTool = new QAction(QIcon(":/icon/zoomIcon"),                  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Zoom</span></p></body></html>", this);
    setMoveTool = new QAction(QIcon(":/icon/moveIcon"),                  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Move</span></p></body></html>", this);
    setMarkTool = new QAction(QIcon(":/icon/markIcon"),                  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Mark</span></p></body></html>", this);
    setFitLeftView = new QAction(QIcon(":/icon/fitleftIcon.png"),        "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Fit Left</span></p></body></html>", this);
    setFitRightView = new QAction(QIcon(":/icon/fitrightIcon.png"),      "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Fit Right</span></p></body></html>", this);
    setFitBothView = new QAction(QIcon(":/icon/fitbothIcon.png"),        "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Fit Both</span></p></body></html>", this);
    showOverview = new QAction(QIcon(":/icon/overIcon"),                 "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Overview</span></p></body></html>", this);
    showNearview = new QAction(QIcon(":/icon/detailIcon"),               "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Nearview</span></p></body></html>", this);
    useAntialias = new QAction(QIcon(":/icon/aliasingIcon"),             "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Antialias</span></p></body></html>", this);
    openLeftImage = new QAction(QIcon(":/icon/fileopen.png"),            "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Open Left</span></p></body></html>", this);
    openRightImage = new QAction(QIcon(":/icon/fileopen.png"),           "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Open Right</span></p></body></html>", this);
    saveLeftImage = new QAction(QIcon(":/icon/disquette.png"),           "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Save Left</span></p></body></html>", this);
    saveRightImage = new QAction(QIcon(":/icon/disquette.png"),          "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Save Right</span></p></body></html>", this);
    setEqualMovements = new QAction(QIcon(":/icon/equalmoveIcon.png"),   "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Equal Movements</span></p></body></html>", this);
    setEqualScales = new QAction(QIcon(":/icon/equalzoomIcon.png"),      "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;font-weight:600; color:#000000;\">Equal Scales</span></p></body></html>", this);

    scaleLeftSpinBox = new QDoubleSpinBox(this);
    scaleLeftSpinBox->setSuffix(" %");
    scaleLeftSpinBox->setDecimals(0);
    scaleLeftSpinBox->setRange(0,800);
    scaleLeftSpinBox->setSingleStep(0);
    scaleLeftSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    connect(scaleLeftSpinBox,SIGNAL(editingFinished()),this,SLOT(rescaleLeftDisplay()));

    scaleRightSpinBox = new QDoubleSpinBox(this);
    scaleRightSpinBox->setSuffix(" %");
    scaleRightSpinBox->setDecimals(0);
    scaleRightSpinBox->setRange(0,800);
    scaleRightSpinBox->setSingleStep(0);
    scaleRightSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    connect(scaleRightSpinBox,SIGNAL(editingFinished()),this,SLOT(rescaleRightDisplay()));

    leftZoom.setScaleSpin(scaleLeftSpinBox);
    leftMove.setScaleSpin(scaleLeftSpinBox);
    leftMark.setScaleSpin(scaleLeftSpinBox);
    leftNear.setScaleSpin(scaleLeftSpinBox);
    leftOver.setScaleSpin(scaleLeftSpinBox);
    //leftInfo.setScaleSpin(scaleLeftSpinBox);
    rightZoom.setScaleSpin(scaleRightSpinBox);
    rightMove.setScaleSpin(scaleRightSpinBox);
    rightMark.setScaleSpin(scaleRightSpinBox);
    rightNear.setScaleSpin(scaleRightSpinBox);
    rightOver.setScaleSpin(scaleRightSpinBox);
    //rightInfo.setScaleSpin(scaleRightSpinBox);

    _leftInfoLabel = new QLabel("   ", 0);
    _rightInfoLabel = new QLabel("   ", 0);

    leftZoom.setPosLabel(_leftInfoLabel);
    leftMove.setPosLabel(_leftInfoLabel);
    leftMark.setPosLabel(_leftInfoLabel);
    leftNear.setPosLabel(_leftInfoLabel);
    leftOver.setPosLabel(_leftInfoLabel);
    //leftInfo.setPosLabel(_leftInfoLabel);
    rightZoom.setPosLabel(_rightInfoLabel);
    rightMove.setPosLabel(_rightInfoLabel);
    rightMark.setPosLabel(_rightInfoLabel);
    rightNear.setPosLabel(_rightInfoLabel);
    rightOver.setPosLabel(_rightInfoLabel);
    //rightInfo.setPosLabel(_rightInfoLabel);

    detailComboBox = new QComboBox(this);
    detailComboBox->addItems(QString("1x 2x 4x 8x").split(" "));
    detailComboBox->setCurrentIndex(1);
    SingleScene* leftScene = (SingleScene*)_leftDisplay->getCurrentScene();
    leftScene->setDetailZoom(2.0);
    SingleScene* rightScene = (SingleScene*)_rightDisplay->getCurrentScene();
    rightScene->setDetailZoom(2.0);
    connect(detailComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeDetailZoom(int)));

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

    connect(this, SIGNAL(actionTriggered(QAction*)), this, SLOT(executeAction(QAction*)));
    currentLeftTool = &leftMove;
    currentRightTool = &rightMove;
    setMoveTool->trigger();
}

void  SeparatedStereoToolsBar::setOpenVisible(bool status)
{
    openLeftImage->setVisible(status);
    openRightImage->setVisible(status);
}

void  SeparatedStereoToolsBar::setSaveVisible(bool status)
{
    saveLeftImage->setVisible(status);
    saveRightImage->setVisible(status);
}

void  SeparatedStereoToolsBar::setMarkVisible(bool status)
{
    setMarkTool->setVisible(status);
}

QLabel* SeparatedStereoToolsBar::getLeftInfo()
{
    return _leftInfoLabel;
}

QLabel* SeparatedStereoToolsBar::getRightInfo()
{
    return _rightInfoLabel;
}

QDockWidget* SeparatedStereoToolsBar::getLeftOverview()
{
    return leftOver.getOverDock();
}

QDockWidget* SeparatedStereoToolsBar::getRightOverview()
{
    return rightOver.getOverDock();
}

QDockWidget* SeparatedStereoToolsBar::getLeftNearview()
{
    return leftNear.getNearDock();
}

QDockWidget* SeparatedStereoToolsBar::getRightNearview()
{
    return rightNear.getNearDock();
}

void SeparatedStereoToolsBar::executeAction(QAction *action)
{
    if (action ==  setZoomTool )
    {
        /*
                leftNear.setActivatedToolOnNear(currentLeftTool, false);
                leftNear.setActivatedToolOnNear(&leftZoom);
                rightNear.setActivatedToolOnNear(currentRightTool, false);
                rightNear.setActivatedToolOnNear(&rightZoom);
                */

        _leftDisplay->setActivatedTool(currentLeftTool, false);
        _leftDisplay->setActivatedTool(currentLeftTool = &leftZoom);
        _leftDisplay->setCursor(QCursor(QPixmap::fromImage(SymbolsResource::getMagnifyGlass())));

        _rightDisplay->setActivatedTool(currentRightTool, false);
        _rightDisplay->setActivatedTool(currentRightTool = &rightZoom);
        _rightDisplay->setCursor(QCursor(QPixmap::fromImage(SymbolsResource::getMagnifyGlass())));

    }
    if (action ==  setMoveTool )
    {
        /*
                leftNear.setActivatedToolOnNear(currentLeftTool, false);
                leftNear.setActivatedToolOnNear(&leftMove);
                rightNear.setActivatedToolOnNear(currentRightTool, false);
                rightNear.setActivatedToolOnNear(&rightMove);
                */

        _leftDisplay->setActivatedTool(currentLeftTool, false);
        _leftDisplay->setActivatedTool(currentLeftTool = &leftMove);
        _leftDisplay->setCursor(QCursor(QPixmap::fromImage(SymbolsResource::getOpenHand())));

        _rightDisplay->setActivatedTool(currentRightTool, false);
        _rightDisplay->setActivatedTool(currentRightTool = &rightMove);
        _rightDisplay->setCursor(QCursor(QPixmap::fromImage(SymbolsResource::getOpenHand())));
    }
    if (action ==  setMarkTool )
    {
        /*
                leftNear.setActivatedToolOnNear(currentLeftTool, false);
                leftNear.setActivatedToolOnNear(&leftMark);
                rightNear.setActivatedToolOnNear(currentRightTool, false);
                rightNear.setActivatedToolOnNear(&rightMark);
                */

        _leftDisplay->setActivatedTool(currentLeftTool, false);
        _leftDisplay->setActivatedTool(currentLeftTool = &leftMark);
        _leftDisplay->setCursor(QCursor(QPixmap::fromImage(SymbolsResource::getBordedCross(QColor(255,255,255,255), QColor(0,0,0,255), QSize(25, 25)))));

        _rightDisplay->setActivatedTool(currentRightTool, false);
        _rightDisplay->setActivatedTool(currentRightTool = &rightMark);
        _rightDisplay->setCursor(QCursor(QPixmap::fromImage(SymbolsResource::getBordedCross(QColor(255,255,255,255), QColor(0,0,0,255), QSize(25, 25)))));
    }
    if (action ==  setFitLeftView )
    {
        _leftDisplay->fitView();
        currentLeftTool->actualizeScaleSpin(_leftDisplay->getCurrentScene()->getScale());
    }
    if (action ==  setFitRightView )
    {
        _rightDisplay->fitView();
        currentRightTool->actualizeScaleSpin(_rightDisplay->getCurrentScene()->getScale());
    }
    if (action ==  setFitBothView )
    {
        _leftDisplay->fitView();
        _rightDisplay->fitView();
        currentLeftTool->actualizeScaleSpin(_leftDisplay->getCurrentScene()->getScale());
        currentRightTool->actualizeScaleSpin(_rightDisplay->getCurrentScene()->getScale());
    }
    if (action ==  showOverview )
    {
        leftOver.setOverVisible(!leftOver.overIsVisible());
        rightOver.setOverVisible(!rightOver.overIsVisible());
    }
    if (action ==  showNearview )
    {
        leftNear.setNearVisible(!leftNear.nearIsVisible());
        rightNear.setNearVisible(!rightNear.nearIsVisible());
        _leftDisplay->setActivatedTool(&leftNear, leftNear.nearIsVisible());
        _rightDisplay->setActivatedTool(&rightNear, rightNear.nearIsVisible());
        detailComboBox->setEnabled(leftNear.nearIsVisible());
    }
    if (action ==  setEqualMovements )
    {
        if (setEqualMovements->isChecked())
        {
            leftZoom.propagateMoveTo(&rightZoom);
            rightZoom.propagateMoveTo(&leftZoom);
            leftMove.propagateMoveTo(&rightMove);
            rightMove.propagateMoveTo(&leftMove);
            leftMark.propagateMoveTo(&rightMark);
            rightMark.propagateMoveTo(&leftMark);
        }
        else
        {
            leftZoom.propagateMoveTo();
            rightZoom.propagateMoveTo();
            leftMove.propagateMoveTo();
            rightMove.propagateMoveTo();
            leftMark.propagateMoveTo();
            rightMark.propagateMoveTo();
        }
    }
    if (action ==  setEqualScales )
    {
        if (setEqualScales->isChecked())
        {
            leftZoom.propagateScaleTo(&rightZoom);
            rightZoom.propagateScaleTo(&leftZoom);
            leftMove.propagateScaleTo(&rightMove);
            rightMove.propagateScaleTo(&leftMove);
            leftMark.propagateScaleTo(&rightMark);
            rightMark.propagateScaleTo(&leftMark);
            leftOver.propagateScaleTo(&rightOver);
            rightOver.propagateScaleTo(&leftOver);
            scaleRightSpinBox->setValue(scaleLeftSpinBox->value());
            _rightDisplay->getCurrentScene()->scaleTo(scaleLeftSpinBox->value()/100);
            _rightDisplay->update();
            currentRightTool->actualizeScaleSpin(_rightDisplay->getCurrentScene()->getScale());
            scaleRightSpinBox->setDisabled(true);
        }
        else
        {
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
    if (action == useAntialias)
    {
        SingleScene* leftScene = (SingleScene*)_leftDisplay->getCurrentScene();
        SingleScene* rightScene = (SingleScene*)_rightDisplay->getCurrentScene();
        leftScene->useSmooth(useAntialias->isChecked());
        rightScene->useSmooth(useAntialias->isChecked());
        _leftDisplay->update();
        _rightDisplay->update();
    }
    if (action ==  openLeftImage )
    {
        QString filename = QFileDialog::getOpenFileName(0, "Open Left Image", ".", "*.bmp *.png *.tif *.ppm *.jpg");
        if (!filename.isEmpty())
        {
            SingleScene* scene = (SingleScene*)_leftDisplay->getCurrentScene();
            scene->loadImage(filename);
            _leftDisplay->update();
        }
    }
    if (action ==  openRightImage )
    {
        QString filename = QFileDialog::getOpenFileName(0, "Open Right Image", ".", "*.bmp *.png *.tif *.ppm *.jpg");
        if (!filename.isEmpty())
        {
            SingleScene* scene = (SingleScene*)_rightDisplay->getCurrentScene();
            scene->loadImage(filename);
            _rightDisplay->update();
        }
    }
    if (action ==  saveLeftImage )
    {
    }
    if (action ==  saveRightImage )
    {
    }
}

void SeparatedStereoToolsBar::changeMode(int action)
{
    if (action ==  1)
    {
        setMarkTool->trigger();
    }
    if (action ==  2)
    {
        setMoveTool->trigger();
    }
    if (action ==  3)
    {
        setZoomTool->trigger();
    }
}

void SeparatedStereoToolsBar::rescaleLeftDisplay()
{
    int diff = _leftDisplay->getCurrentScene()->getScale()*100 - scaleLeftSpinBox->value();
    if (diff)
    {
        _leftDisplay->getCurrentScene()->scaleTo(scaleLeftSpinBox->value()/100);
        _leftDisplay->update();
        currentLeftTool->actualizeScaleSpin(_leftDisplay->getCurrentScene()->getScale());

        if (setEqualScales->isChecked())
        {
            _rightDisplay->getCurrentScene()->scaleTo(scaleLeftSpinBox->value()/100);
            _rightDisplay->update();
            currentRightTool->actualizeScaleSpin(_rightDisplay->getCurrentScene()->getScale());
        }
    }
}

void SeparatedStereoToolsBar::rescaleRightDisplay()
{
    int diff = _rightDisplay->getCurrentScene()->getScale()*100 - scaleRightSpinBox->value();
    if (diff)
    {
        _rightDisplay->getCurrentScene()->scaleTo(scaleRightSpinBox->value()/100);
        _rightDisplay->update();
        currentRightTool->actualizeScaleSpin(_rightDisplay->getCurrentScene()->getScale());
        //currentLeftTool->actualizeScaleSpin(_leftDisplay->getCurrentScene()->getScale());
    }
}

void SeparatedStereoToolsBar::changeDetailZoom(int nz)
{
    SingleScene* leftScene = (SingleScene*)_leftDisplay->getCurrentScene();
    SingleScene* rightScene = (SingleScene*)_rightDisplay->getCurrentScene();
    switch (nz){
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
