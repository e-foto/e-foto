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
#include "SingleDisplay.h"

#include "StereoTools.h"

#include <QCursor>
#include <QPainter>
#include <QColor>
#include <QTransform>
#include <QResizeEvent>

//#include <math.h>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

SingleDisplay::SingleDisplay(QWidget* parent, AbstractScene* currentScene):
    QWidget(parent) {
    if (currentScene) {
        currentScene_ = currentScene;
    } else {
        currentScene_ = new SingleScene(this, "");
    }

    displayMode_ = IntermediatedScreen;
    //_onMove = false;
    cloneScale_ = true;
    showDetailArea_ = false;
    blockShowDetailArea_ = false;
    onPainting_ = false;
    over_ = nullptr;
    detail_ = nullptr;
    setMinimumSize(150, 150);
    setAttribute(Qt::WA_Hover, true);
    installEventFilter(this);
    //Cursor cursor(BlackAndWhiteCursor);
    //setCursor(cursor.toQCursor());
}

SingleDisplay::~SingleDisplay() {
}
void SingleDisplay::updateMousePosition() {
    mouseLastPos_ = mapFromGlobal(QCursor::pos());
}

bool SingleDisplay::positionIsVisible(QPointF pos) {
    QPointF screenPos = screenPosition(pos);

    if (screenPos.x() >= 0 && screenPos.y() >= 0 && screenPos.x() <= width()
            && screenPos.y() <= height()) {
        return true;
    } else {
        return false;
    }
}

QPointF SingleDisplay::screenPosition(QPointF pos) {
    QPointF screenPos;
    SingleScene* currentScene = dynamic_cast<SingleScene*>(currentScene_);

    if (displayMode_ == TopViewScreen) {
        screenPos = (pos - QPointF(currentScene->getWidth() / 2.0,
                                   currentScene->getHeight() / 2.0)) * currentScene->getThumbScale() + QPointF(
                        width() / 2, height() / 2);
    } else if (displayMode_ == MostDetailedScreen) {
        screenPos = (pos - currentScene->getDetailedPoint()) * currentScene->getScale()
                    * currentScene->getDetailZoom() + QPointF(width() / 2, height() / 2);
    } else {
        screenPos = (pos - currentScene_->getViewpoint()) * currentScene_->getScale() +
                    QPointF(width() / 2, height() / 2);
    }

    return screenPos;
}

QPointF SingleDisplay::getLastMousePosition() {
    QPointF diffTocenter(mouseLastPos_.x() - size().width() / 2,
                         mouseLastPos_.y() - size().height() / 2);
    double scale;
    SingleScene* currentScene = dynamic_cast<SingleScene*>(currentScene_);
    QPointF mousePos;

    if (displayMode_ == TopViewScreen) {
        scale = currentScene->getThumbScale();
        mousePos = QPointF(currentScene->getWidth() / 2.0,
                           currentScene->getHeight() / 2.0) + diffTocenter / scale;
    } else if (displayMode_ == MostDetailedScreen) {
        scale = currentScene->getScale() * currentScene->getDetailZoom();
        mousePos = currentScene->getDetailedPoint() + diffTocenter / scale;
    } else {
        scale = currentScene_->getScale();
        mousePos = currentScene_->getViewpoint() + diffTocenter / scale;
    }

    return mousePos;
}

QPointF SingleDisplay::getPosition(QPoint screenPosition) {
    QPointF diffTocenter(screenPosition.x() - size().width() / 2,
                         screenPosition.y() - size().height() / 2);
    double scale;
    scale = currentScene_->getScale();
    return currentScene_->getViewpoint() + diffTocenter / scale;
}

QPointF SingleDisplay::getMouseScreenPosition() {
    return mouseLastPos_;
}

bool SingleDisplay::painting() {
    return onPainting_;
}

bool SingleDisplay::showDetailedArea() {
    return showDetailArea_;
}

void SingleDisplay::setShowDetailedArea(bool status) {
    showDetailArea_ = status;
}

void SingleDisplay::blockShowDetailedArea(bool status) {
    blockShowDetailArea_ = status;
}

AbstractScene* SingleDisplay::getCurrentScene() {
    return currentScene_;
}

SingleDisplay* SingleDisplay::getOverDisplay() {
    if (over_ == nullptr) {
        SingleDisplay* result = new SingleDisplay(0);
        result->setOverviewMode(this);
        over_ = result;
    }

    return over_;
}

SingleDisplay* SingleDisplay::getDetailDisplay() {
    if (detail_ == nullptr) {
        SingleDisplay* result = new SingleDisplay(0);
        result->setDetailMode(this);
        detail_ = result;
    }

    return detail_;
}

void SingleDisplay::fitView() {
    currentScene_->setViewport(size());
    double wscale = width() / (double)currentScene_->getWidth();
    double hscale = height() / (double)currentScene_->getHeight();
    currentScene_->scaleTo(wscale < hscale ? wscale : hscale);
    currentScene_->centerContent();
    update();
}

void SingleDisplay::updateAll() {
    QWidget::update();

    if(displayMode_ == IntermediatedScreen) {
        if (detail_) {
            detail_->update();
        }

        if (over_) {
            over_->update();
        }
    }
}

void SingleDisplay::updateDetail() {
    if (detail_) {
        detail_->update();
    }
}

void SingleDisplay::updateDetail(QPointF* mousePosition, bool emitClicked) {
    if (!currentScene_ || !mousePosition) {
        return;
    }

    double scale;
    scale = currentScene_->getScale();
    mouseLastPos_.setX(((*mousePosition - currentScene_->getViewpoint()) *
                        scale).x() + size().width() / 2);
    mouseLastPos_.setY(((*mousePosition - currentScene_->getViewpoint()) *
                        scale).y() + size().height() / 2);

    if (detail_) {
        detail_->update();
    }

    if (emitClicked) {
        QPointF lastPos = getLastMousePosition();
        emit mouseClicked(&lastPos);
    }
}

void SingleDisplay::setOverviewMode(SingleDisplay* display) {
    displayMode_ = TopViewScreen;
    detail_ = display;
    currentScene_ = detail_->getCurrentScene();
}

void SingleDisplay::setDetailMode(SingleDisplay* display) {
    displayMode_ = MostDetailedScreen;
    over_ = display;
    currentScene_ = over_->getCurrentScene();
}

void SingleDisplay::setActivatedTool(SingleTool* tool, bool active) {
    for (int i = tool_.size() - 1; i >=  0; i--) {
        if (tool == tool_.at(i)) {
            tool_.removeAt(i);
        }
    }

    if (active) {
        tool_.prepend(tool);
    }
}

void SingleDisplay::setActivatedTool(StereoTool* tool, bool active) {
    for (int i = stool_.size() - 1; i >=  0; i--) {
        if (tool == stool_.at(i)) {
            stool_.removeAt(i);
        }
    }

    if (active) {
        stool_.prepend(tool);
    }
}

void SingleDisplay::paintEvent(QPaintEvent* e) {
    onPainting_ = true;

    if (displayMode_ == IntermediatedScreen) {
        QPainter painter(this);
        painter.fillRect(rect(), QBrush(SymbolsResource::getBackGround(Qt::darkGray)));

        if (currentScene_->isValid()) {
            QRect target = rect();

            if (showDetailArea_ && !blockShowDetailArea_ && detail_
                    && detail_->isVisible()) {
                painter.drawImage(0, 0, currentScene_->getFrame(target.size(),
                                  detail_->size()));
            } else {
                painter.drawImage(0, 0, currentScene_->getFrame(target.size()));
            }

            if (detail_) {
                detail_->update();
            }

            if (over_) {
                over_->update();
            }
        }
    } else if (displayMode_ == TopViewScreen) {
        QPainter painter(this);
        painter.fillRect(rect(), QBrush(SymbolsResource::getBackGround(Qt::darkGray)));
        AbstractScene* currentScene;

        if (detail_ && (currentScene = detail_->getCurrentScene())
                && currentScene->isValid()) {
            QRect target = rect();
            QSize targetSize = target.size();
            QRect temp = detail_->rect();
            QImage thumb = currentScene->getThumb(targetSize, &temp);
            painter.drawImage((targetSize.width() - thumb.width()) / 2,
                              (targetSize.height() - thumb.height()) / 2, thumb);
        }

        painter.end();
    } else if (displayMode_ == MostDetailedScreen) {
        QPainter painter(this);
        painter.fillRect(rect(), QBrush(SymbolsResource::getBackGround(Qt::darkGray)));
        SingleScene* currentScene;

        if (over_
                && (currentScene = dynamic_cast<SingleScene*>(over_->getCurrentScene()))
                && currentScene->isValid()) {
            QRect target = rect();
            QSize targetSize = target.size();
            double zoom = currentScene->getScale() * currentScene->getDetailZoom();
            QImage detail = currentScene->getDetail(targetSize,
                                                    currentScene->getDetailedPoint(), zoom);
            painter.drawImage(0, 0, detail);
        }

        painter.end();
    }

    if (!currentScene_ || !currentScene_->isValid()) {
        return;
    }

    for (int i = 0; i < tool_.size(); i++) {
        tool_.at(i)->paintEvent(*e);
    }

    for (int i = 0; i < stool_.size(); i++) {
        stool_.at(i)->paintEvent(*e);
    }

    onPainting_ = false;
}

void SingleDisplay::resizeEvent(QResizeEvent* e) {
    if (!currentScene_ || !currentScene_->isValid()) {
        return;
    }

    currentScene_->setViewport(e->size());
    update();

    for (int i = 0; i < tool_.size(); i++) {
        tool_.at(i)->resizeEvent(*e);
    }
}

bool SingleDisplay::eventFilter(QObject* o, QEvent* e) {
    if (o == this) {
        switch (e->type()) {
        case QEvent::HoverEnter : {
            QHoverEvent* enter = static_cast<QHoverEvent*>(e);
            enterEvent(enter);
            return true;
        }

        case QEvent::HoverLeave : {
            QHoverEvent* leave = static_cast<QHoverEvent*>(e);
            leaveEvent(leave);
            return true;
        }

        case QEvent::HoverMove : {
            QHoverEvent* move = static_cast<QHoverEvent*>(e);
            moveEvent(move);
            return true;
        }

        default :
            return QWidget::eventFilter(o, e);
        }
    }

    return false;
}

void SingleDisplay::enterEvent(QHoverEvent* e) {
    if (!currentScene_ || !currentScene_->isValid()) {
        return;
    }

    for (int i = 0; i < tool_.size(); i++) {
        tool_.at(i)->enterEvent(*e);
    }
}

void SingleDisplay::leaveEvent(QHoverEvent* e) {
    if (!currentScene_ || !currentScene_->isValid()) {
        return;
    }

    for (int i = 0; i < tool_.size(); i++) {
        tool_.at(i)->leaveEvent(*e);
    }
}

void SingleDisplay::moveEvent(QHoverEvent* e) {
    if (!currentScene_ || !currentScene_->isValid()) {
        return;
    }

    for (int i = 0; i < tool_.size(); i++) {
        tool_.at(i)->moveEvent(*e);
    }
}

void SingleDisplay::mousePressEvent(QMouseEvent* e) {
    if (!currentScene_ || !currentScene_->isValid()) {
        return;
    }

    for (int i = 0; i < tool_.size(); i++) {
        tool_.at(i)->mousePressed(*e);
    }
}

void SingleDisplay::wheelEvent(QWheelEvent* e) {
    if (!currentScene_ || !currentScene_->isValid()) {
        return;
    }

    for (int i = 0; i < tool_.size(); i++) {
        tool_.at(i)->wheelEvent(*e);
    }
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
