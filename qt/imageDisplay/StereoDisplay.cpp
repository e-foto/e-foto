#include "StereoDisplay.h"
#include "GLDisplay.h"

#include <QTime>
#include <QCursor>
#include <QBitmap>
#include <QPainter>
#include <QWheelEvent>
#include <QFocusEvent>

#include <GL/gl.h>
//#include <GL/glu.h>

#define NOCURSOR QPixmap::fromImage(SymbolsResource::getBackGround(QColor(0,0,0,0)))

#include <QDebug>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

StereoDisplay::StereoDisplay(QWidget *parent, StereoScene *currentScene):
    QWidget(parent)
{

    leftDisplay_ = NULL;
        rightDisplay_ = NULL;

    if (currentScene)
        currentScene_ = currentScene;
    else
        currentScene_ = new StereoScene(this, "", "");

    _currentZ = 0;
    leftCursorOffset_ = QPointF(-3,0);
    rightCursorOffset_ = QPointF(3,0);
    glDisplay_ = new GLDisplay(this);
    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(glDisplay_);
    setLayout(layout);
    setMinimumSize(400,300);
    //setFocusPolicy(Qt::StrongFocus);
}

StereoDisplay::~StereoDisplay()
{
}

// To Do: 1. Protect methods on close, to avoid crash when fitview by double click and close stereoplotter
//        2. Add keyboard event to move and zoom if display is on focus
//        3. Accept ctrl and shift as modifiers
//void StereoDisplay::focusInEvent(QFocusEvent *event)
//{
//    Q_UNUSED(event);
//    setContentsMargins(1,1,1,1);
//}
//
//void StereoDisplay::focusOutEvent(QFocusEvent *event)
//{
//    Q_UNUSED(event);
//    setContentsMargins(0,0,0,0);
//}
int StereoDisplay::setStereoMode(bool mode)
{
    return glDisplay_->setStereoMode(mode);
}

void StereoDisplay::setReverseLensGlasses(bool opt)
{
    glDisplay_->setReverseLensGlasses(opt);
}

void StereoDisplay::setColorMaskLeft(bool r, bool g, bool b)
{
    glDisplay_->setColorMaskLeft(r, g, b);
}

void StereoDisplay::setColorMaskRight(bool r, bool g, bool b)
{
    glDisplay_->setColorMaskRight(r, g, b);
}

StereoScene* StereoDisplay::getCurrentScene()
{
    return currentScene_;
}

SingleDisplay* StereoDisplay::getLeftDisplay()
{
    if (leftDisplay_ == NULL)
    {
        SingleDisplay* result = new SingleDisplay(0, currentScene_ ? currentScene_->getLeftScene() : NULL);
        leftDisplay_ = result;
    }
    return leftDisplay_;
}

SingleDisplay* StereoDisplay::getRightDisplay()
{
    if (rightDisplay_ == NULL)
    {
        SingleDisplay* result = new SingleDisplay(0, currentScene_ ? currentScene_->getRightScene(): NULL);
        rightDisplay_ = result;
    }
    return rightDisplay_;
}

SingleDisplay* StereoDisplay::getLeftNearDisplay()
{
    if (leftDisplay_ == NULL)
        getLeftDisplay();
    return leftDisplay_->getDetailDisplay();
}

SingleDisplay* StereoDisplay::getRightNearDisplay()
{
    if (rightDisplay_ == NULL)
        getRightDisplay();
    return rightDisplay_->getDetailDisplay();
}

SingleDisplay* StereoDisplay::getLeftOverDisplay()
{
    if (leftDisplay_ == NULL)
        getLeftDisplay();
    return leftDisplay_->getOverDisplay();
}

SingleDisplay* StereoDisplay::getRightOverDisplay()
{
    if (rightDisplay_ == NULL)
        getRightDisplay();
    return rightDisplay_->getOverDisplay();
}

GLDisplay* StereoDisplay::getRealDisplay()
{
    return glDisplay_;
}



void StereoDisplay::loadBackground(QImage image)
{
    glDisplay_->loadBackground(image);
}
void StereoDisplay::loadLeftImage(QString filename)
{
    currentScene_->getLeftScene()->loadImage(filename);
    glDisplay_->changeLeft();
}

void StereoDisplay::loadRightImage(QString filename)
{
    currentScene_->getRightScene()->loadImage(filename);
    glDisplay_->changeRight();
}

void StereoDisplay::loadLeftImage(QImage *image)
{
    currentScene_->getLeftScene()->loadImage(*image);
    glDisplay_->changeLeft();
}

void StereoDisplay::loadRightImage(QImage *image)
{
    currentScene_->getRightScene()->loadImage(*image);
    glDisplay_->changeRight();
}

void StereoDisplay::loadLeftImage(Matrix *image, bool isGrayscale)
{
    currentScene_->getLeftScene()->loadImage(image,isGrayscale);
    glDisplay_->changeLeft();
}

void StereoDisplay::loadRightImage(Matrix *image, bool isGrayscale)
{
    currentScene_->getRightScene()->loadImage(image,isGrayscale);
    glDisplay_->changeRight();
}



QPointF StereoDisplay::getLeftCursorOffset()
{
    return leftCursorOffset_;
}

QPointF StereoDisplay::getRightCursorOffset()
{
    return rightCursorOffset_;
}

void StereoDisplay::setLeftCursorOffset(QPointF offset)
{
    leftCursorOffset_ = offset;
}

void StereoDisplay::setRightCursorOffset(QPointF offset)
{
    rightCursorOffset_ = offset;
}

void StereoDisplay::setCursor(QImage newCursor, bool stereo)
{
    if (stereo)
    {
        glDisplay_->setCursor(QCursor(NOCURSOR));
        glDisplay_->setGLCursor(newCursor);
    }
    else
    {
        glDisplay_->setCursor(QCursor(QPixmap::fromImage(newCursor)));
        glDisplay_->setGLCursor(QCursor(NOCURSOR).pixmap().toImage());
    }
    _stereoCursor = stereo;
}

QImage StereoDisplay::getCursor()
{
    if (_stereoCursor)
        return glDisplay_->getGLCursor();
    else
        return glDisplay_->cursor().pixmap().toImage();
}

double StereoDisplay::getCurrentZ()
{
    return _currentZ;
}

void StereoDisplay::setCurrentZ(double z)
{
    _currentZ = z;
}

QPointF StereoDisplay::screenPosition(QPointF pos, bool leftChannel)
{
    //REFAZER
    //return QPointF(0,0);

    QPointF screenPos;
    if (leftChannel)
        screenPos = (pos - currentScene_->getLeftScene()->getViewpoint())*currentScene_->getLeftScene()->getScale() + QPointF(glDisplay_->width()/2, glDisplay_->height()/2);
    else
        screenPos = (pos - currentScene_->getRightScene()->getViewpoint())*currentScene_->getRightScene()->getScale() + QPointF(glDisplay_->width()/2, glDisplay_->height()/2);

    return screenPos;
}

QPointF StereoDisplay::getMouseScreenPosition()
{
    return glDisplay_->getMouseScreenPosition();
}

void StereoDisplay::updateMousePosition()
{
    glDisplay_->updateMousePosition();
}

QPointF StereoDisplay::getPositionLeft(QPoint screenPosition)
{
    QPointF diffTocenter(screenPosition.x() - size().width() / 2, screenPosition.y() - size().height() / 2);
    double scale;
    scale = currentScene_->getLeftScene()->getScale();
    return currentScene_->getLeftScene()->getViewpoint() + diffTocenter / scale;
}

QPointF StereoDisplay::getPositionRight(QPoint screenPosition)
{
    QPointF diffTocenter(screenPosition.x() - size().width() / 2, screenPosition.y() - size().height() / 2);
    double scale;
    scale = currentScene_->getRightScene()->getScale();
    return currentScene_->getRightScene()->getViewpoint() + diffTocenter / scale;
}

void StereoDisplay::fitView()
{
    currentScene_->getLeftScene()->scaleTo(_fitScale);
    currentScene_->getLeftScene()->moveTo(_centerOnLeft);
    currentScene_->getRightScene()->scaleTo(_fitScale);
    currentScene_->getRightScene()->moveTo(_centerOnRight);
        updateAll();
}

/* Method into disuse
void StereoDisplay::pan(int dx, int dy)
{
}
*/

void StereoDisplay::zoom(double zoomFactor/*, QPoint* atPoint*/)
{
    currentScene_->getLeftScene()->zoom(zoomFactor);
    currentScene_->getRightScene()->zoom(zoomFactor);
}

void StereoDisplay::updateAll(std::pair<bool, bool> scene)
{
    // For integrated (opengl) mode
    if (scene.first)
        glDisplay_->changeLeft();
    if (scene.second)
        glDisplay_->changeRight();
    glDisplay_->update();
    // For separeted mode
    if (leftDisplay_)
        leftDisplay_->updateAll();
    if (rightDisplay_)
        rightDisplay_->updateAll();
}

void StereoDisplay::updateAll(QPointF* left, QPointF* right, bool emitClicked)
{
    //glDisplay_->_GLDisplayUpdate = true;
    glDisplay_->update();
    if (leftDisplay_)
    {
        leftDisplay_->updateDetail(left,emitClicked);
        leftDisplay_->update();
    }
    if (rightDisplay_)
    {
        rightDisplay_->updateDetail(right,emitClicked);
        rightDisplay_->update();
    }
}

void StereoDisplay::updateDetail(QPointF* left, QPointF* right, bool emitClicked)
{
    if (leftDisplay_)
        leftDisplay_->updateDetail(left);
    if (rightDisplay_)
        rightDisplay_->updateDetail(right);
    if (emitClicked)
        emit mouseClicked(left, right);
    else
        emit mousePositionsChanged(left, right);
}

void StereoDisplay::setActivatedTool(StereoTool *tool, bool active)
{
    glDisplay_->setActivatedTool(tool, active);
}

bool StereoDisplay::painting()
{
    return glDisplay_->painting();
}

void StereoDisplay::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    if (currentScene_->isValid())
        emit resized(glDisplay_->width(), glDisplay_->height());
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
