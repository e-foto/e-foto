#include "StereoDisplay.h"

#include <QTime>
#include <QCursor>
#include <QBitmap>
#include <QPainter>
#include <QWheelEvent>

#include <GL/gl.h>
#include <GL/glu.h>

#define NOCURSOR QPixmap::fromImage(SymbolsResource::getBackGround(QColor(0,0,0,0)))


namespace br {
namespace uerj {
namespace eng {
namespace efoto {

// GL Display class
GLDisplay::GLDisplay(StereoDisplay* parent):
    QGLWidget(parent) {
    stereoDisplay_ = parent;
    setAutoFillBackground(false);
    setAttribute(Qt::WA_Hover, true);
    installEventFilter(this);
    QCursor cursor(NOCURSOR);
    setCursor(cursor);
}

GLDisplay::~GLDisplay() {
    if(glIsTexture((GLuint)ltexture_)) {
        glDeleteTextures(1, (GLuint*)(&ltexture_));
    }

    if(glIsTexture((GLuint)rtexture_)) {
        glDeleteTextures(1, (GLuint*)(&rtexture_));
    }
}

void GLDisplay::setReverseLensGlasses(bool opt) {
    reverseLensGlasses = opt;
}

void GLDisplay::setColorMaskLeft(bool r, bool g, bool b) {
    L_Red_ = r;
    L_Green_ = g;
    L_Blue_ = b;
}

void GLDisplay::setColorMaskRight(bool r, bool g, bool b) {
    R_Red_ = r;
    R_Green_ = g;
    R_Blue_ = b;
}

int GLDisplay::setStereoMode(bool mode) {
    stereo_mode = mode;
    QGLFormat fmt;
    fmt.setAlpha(true);
    fmt.setStereo(stereo_mode);
    fmt.setDoubleBuffer(true);
    fmt.setOverlay(true);
    setFormat(fmt);

    // Test if available
    if ((stereo_mode == 1) && (!format().stereo())) {
        stereo_mode = 0;
        return 0;
    }

    updateGL();
    return 1;
}

QPointF GLDisplay::getMouseScreenPosition() {
    return mouseScreenPos_;
}

void GLDisplay::updateMousePosition() {
    mouseScreenPos_ = mapFromGlobal(QCursor::pos());
}

void GLDisplay::setGLCursor(QImage cursor) {
    if ((ctexture_ != 0) && glIsTexture((GLuint)ctexture_)) {
        glDeleteTextures(1, (GLuint*)(&ctexture_));
    }

    cursor_ = cursor;

    if (cursor.isNull()) {
        (ctexture_ = 0);
        return;
    }

    cursor = QGLWidget::convertToGLFormat(cursor);
    glEnable(GL_TEXTURE_2D);
    glGenTextures( 1, (GLuint*)(&ctexture_) );
    glBindTexture( GL_TEXTURE_2D, (GLuint)ctexture_ );
    glTexImage2D( GL_TEXTURE_2D, 0, 4, cursor.width(), cursor.height(), 0, GL_RGBA,
                  GL_UNSIGNED_BYTE, cursor.bits());
    glDisable(GL_TEXTURE_2D);
}

QImage GLDisplay::getGLCursor() {
    if (ctexture_ != 0) {
        return cursor_;
    } else {
        return QImage();
    }
}

void GLDisplay::setActivatedTool(StereoTool* tool, bool active) {
    for (int i = tool_.size() - 1; i >=  0; i--) {
        if (tool == tool_.at(i)) {
            tool_.removeAt(i);
        }
    }

    if (active) {
        tool_.prepend(tool);
    }
}

bool GLDisplay::painting() {
    return onPainting_;
}

void GLDisplay::paintEvent(QPaintEvent* e) {
    onPainting_ = true;
    QGLWidget::paintEvent(e);

    if (!stereoDisplay_ || !stereoDisplay_->getCurrentScene()
            || !stereoDisplay_->getCurrentScene()->isValid()) {
        return;
    }

    for (int i = 0; i < tool_.size(); i++) {
        tool_.at(i)->paintEvent(*e);
    }

    onPainting_ = false;
}

void GLDisplay::resizeEvent(QResizeEvent* e) {
    if (!stereoDisplay_ || !stereoDisplay_->getCurrentScene()
            || !stereoDisplay_->getCurrentScene()->isValid()) {
        return;
    }

    for (int i = 0; i < tool_.size(); i++) {
        tool_.at(i)->resizeEvent(*e);
    }

    QGLWidget::resizeEvent(e);
}

bool GLDisplay::eventFilter(QObject* o, QEvent* e) {
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

void GLDisplay::enterEvent(QHoverEvent* e) {
    if (!stereoDisplay_->getCurrentScene()
            || !stereoDisplay_->getCurrentScene()->isValid()) {
        return;
    }

    for (int i = 0; i < tool_.size(); i++) {
        tool_.at(i)->enterEvent(*e);
    }
}

void GLDisplay::leaveEvent(QHoverEvent* e) {
    if (!stereoDisplay_->getCurrentScene()
            || !stereoDisplay_->getCurrentScene()->isValid()) {
        return;
    }

    for (int i = 0; i < tool_.size(); i++) {
        tool_.at(i)->leaveEvent(*e);
    }
}

void GLDisplay::moveEvent(QHoverEvent* e) {
    if (!stereoDisplay_->getCurrentScene()
            || !stereoDisplay_->getCurrentScene()->isValid()) {
        return;
    }

    for (int i = 0; i < tool_.size(); i++) {
        tool_.at(i)->moveEvent(*e);
    }
}

void GLDisplay::mousePressEvent(QMouseEvent* e) {
    if (!stereoDisplay_->getCurrentScene()
            || !stereoDisplay_->getCurrentScene()->isValid()) {
        return;
    }

    for (int i = 0; i < tool_.size(); i++) {
        tool_.at(i)->mousePressed(*e);
    }
}

void GLDisplay::wheelEvent(QWheelEvent* e) {
    if (!stereoDisplay_->getCurrentScene()
            || !stereoDisplay_->getCurrentScene()->isValid()) {
        return;
    }

    for (int i = 0; i < tool_.size(); i++) {
        tool_.at(i)->wheelEvent(*e);
    }
}

// Stereo Display class
StereoDisplay::StereoDisplay(QWidget* parent, StereoScene* currentScene):
    QWidget(parent) {
    if (currentScene) {
        currentScene_ = currentScene;
    } else {
        currentScene_ = new StereoScene(this, "", "");
    }

    currentZ_ = 0;
    leftCursorOffset_ = QPointF(-3, 0);
    rightCursorOffset_ = QPointF(3, 0);
    glDisplay_ = new GLDisplay(this);
    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(glDisplay_);
    setLayout(layout);
    setMinimumSize(400, 300);
}

StereoDisplay::~StereoDisplay() {
}

int StereoDisplay::setStereoMode(bool mode) {
    return glDisplay_->setStereoMode(mode);
}

void StereoDisplay::setReverseLensGlasses(bool opt) {
    glDisplay_->setReverseLensGlasses(opt);
}

void StereoDisplay::setColorMaskLeft(bool r, bool g, bool b) {
    glDisplay_->setColorMaskLeft(r, g, b);
}

void StereoDisplay::setColorMaskRight(bool r, bool g, bool b) {
    glDisplay_->setColorMaskRight(r, g, b);
}

StereoScene* StereoDisplay::getCurrentScene() {
    return currentScene_;
}

SingleDisplay* StereoDisplay::getLeftDisplay() {
    if (leftDisplay_ == NULL) {
        SingleDisplay* result = new SingleDisplay(0,
                currentScene_ ? currentScene_->getLeftScene() : NULL);
        leftDisplay_ = result;
    }

    return leftDisplay_;
}

SingleDisplay* StereoDisplay::getRightDisplay() {
    if (rightDisplay_ == NULL) {
        SingleDisplay* result = new SingleDisplay(0,
                currentScene_ ? currentScene_->getRightScene() : NULL);
        rightDisplay_ = result;
    }

    return rightDisplay_;
}

SingleDisplay* StereoDisplay::getLeftNearDisplay() {
    if (leftDisplay_ == NULL) {
        getLeftDisplay();
    }

    return leftDisplay_->getDetailDisplay();
}

SingleDisplay* StereoDisplay::getRightNearDisplay() {
    if (rightDisplay_ == NULL) {
        getRightDisplay();
    }

    return rightDisplay_->getDetailDisplay();
}

SingleDisplay* StereoDisplay::getLeftOverDisplay() {
    if (leftDisplay_ == NULL) {
        getLeftDisplay();
    }

    return leftDisplay_->getOverDisplay();
}

SingleDisplay* StereoDisplay::getRightOverDisplay() {
    if (rightDisplay_ == NULL) {
        getRightDisplay();
    }

    return rightDisplay_->getOverDisplay();
}

GLDisplay* StereoDisplay::getRealDisplay() {
    return glDisplay_;
}

void StereoDisplay::loadLeftImage(QString filename) {
    currentScene_->getLeftScene()->loadImage(filename);
}

void StereoDisplay::loadRightImage(QString filename) {
    currentScene_->getRightScene()->loadImage(filename);
}

void StereoDisplay::loadLeftImage(QImage* image) {
    currentScene_->getLeftScene()->loadImage(*image);
}

void StereoDisplay::loadRightImage(QImage* image) {
    currentScene_->getRightScene()->loadImage(*image);
}

void StereoDisplay::loadLeftImage(Matrix* image, bool isGrayscale) {
    currentScene_->getLeftScene()->loadImage(image, isGrayscale);
}

void StereoDisplay::loadRightImage(Matrix* image, bool isGrayscale) {
    currentScene_->getRightScene()->loadImage(image, isGrayscale);
}



QPointF StereoDisplay::getLeftCursorOffset() {
    return leftCursorOffset_;
}

QPointF StereoDisplay::getRightCursorOffset() {
    return rightCursorOffset_;
}

void StereoDisplay::setLeftCursorOffset(QPointF offset) {
    leftCursorOffset_ = offset;
}

void StereoDisplay::setRightCursorOffset(QPointF offset) {
    rightCursorOffset_ = offset;
}

void StereoDisplay::setCursor(QImage newCursor, bool stereo) {
    if (stereo) {
        glDisplay_->setCursor(QCursor(NOCURSOR));
        glDisplay_->setGLCursor(newCursor);
    } else {
        glDisplay_->setCursor(QCursor(QPixmap::fromImage(newCursor)));
        glDisplay_->setGLCursor(QCursor(NOCURSOR).pixmap().toImage());
    }

    stereoCursor_ = stereo;
}

QImage StereoDisplay::getCursor() {
    if (stereoCursor_) {
        return glDisplay_->getGLCursor();
    } else {
        return glDisplay_->cursor().pixmap().toImage();
    }
}

QPointF StereoDisplay::screenPosition(QPointF pos, bool leftChannel) {
    QPointF screenPos;

    if (leftChannel) {
        screenPos = (pos - currentScene_->getLeftScene()->getViewpoint()) *
                    currentScene_->getLeftScene()->getScale() + QPointF(glDisplay_->width() / 2,
                            glDisplay_->height() / 2);
    } else {
        screenPos = (pos - currentScene_->getRightScene()->getViewpoint()) *
                    currentScene_->getRightScene()->getScale() + QPointF(glDisplay_->width() / 2,
                            glDisplay_->height() / 2);
    }

    return screenPos;
}

QPointF StereoDisplay::getMouseScreenPosition() {
    return glDisplay_->getMouseScreenPosition();
}

void StereoDisplay::updateMousePosition() {
    glDisplay_->updateMousePosition();
}

QPointF StereoDisplay::getPositionLeft(QPoint screenPosition) {
    QPointF diffTocenter(screenPosition.x() - size().width() / 2,
                         screenPosition.y() - size().height() / 2);
    double scale;
    scale = currentScene_->getLeftScene()->getScale();
    return currentScene_->getLeftScene()->getViewpoint() + diffTocenter / scale;
}

QPointF StereoDisplay::getPositionRight(QPoint screenPosition) {
    QPointF diffTocenter(screenPosition.x() - size().width() / 2,
                         screenPosition.y() - size().height() / 2);
    double scale;
    scale = currentScene_->getRightScene()->getScale();
    return currentScene_->getRightScene()->getViewpoint() + diffTocenter / scale;
}

void StereoDisplay::fitView() {
    currentScene_->getLeftScene()->scaleTo(fitScale_);
    currentScene_->getLeftScene()->moveTo(centerOnLeft_);
    currentScene_->getRightScene()->scaleTo(fitScale_);
    currentScene_->getRightScene()->moveTo(centerOnRight_);
    updateAll();
}

/* Method into disuse
void StereoDisplay::pan(int dx, int dy)
{
}
*/

void StereoDisplay::zoom(double zoomFactor/*, QPoint* atPoint*/) {
    currentScene_->getLeftScene()->zoom(zoomFactor);
    currentScene_->getRightScene()->zoom(zoomFactor);
}

void StereoDisplay::updateAll() {
    glDisplay_->_GLDisplayUpdate = true;
    glDisplay_->update();

    if (leftDisplay_) {
        leftDisplay_->updateAll();
    }

    if (rightDisplay_) {
        rightDisplay_->updateAll();
    }
}

void StereoDisplay::updateAll(QPointF* left, QPointF* right, bool emitClicked) {
    glDisplay_->_GLDisplayUpdate = true;
    glDisplay_->update();

    if (leftDisplay_) {
        leftDisplay_->updateDetail(left, emitClicked);
        leftDisplay_->update();
    }

    if (rightDisplay_) {
        rightDisplay_->updateDetail(right, emitClicked);
        rightDisplay_->update();
    }
}

void StereoDisplay::updateDetail(QPointF* left, QPointF* right,
                                 bool emitClicked) {
    if (leftDisplay_) {
        leftDisplay_->updateDetail(left);
    }

    if (rightDisplay_) {
        rightDisplay_->updateDetail(right);
    }

    if (emitClicked) {
        emit mouseClicked(left, right);
    } else {
        emit mousePositionsChanged(left, right);
    }
}

void StereoDisplay::setActivatedTool(StereoTool* tool, bool active) {
    glDisplay_->setActivatedTool(tool, active);
}

bool StereoDisplay::painting() {
    return glDisplay_->painting();
}

void StereoDisplay::resizeEvent(QResizeEvent* e) {
    QWidget::resizeEvent(e);

    if (currentScene_->isValid()) {
        emit resized(glDisplay_->width(), glDisplay_->height());
    }
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
