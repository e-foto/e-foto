#include "GLDisplay.h"
#include "StereoTools.h"
#include "StereoDisplay.h"

#include <QMessageBox>
#include <QMouseEvent>
#include <QApplication>

#include <QFileDialog>

#include <QtDebug>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

GLDisplay::GLDisplay(StereoDisplay *parent)
    : QOpenGLWidget( (QWidget*) parent),
      stereoDisplay_(parent),
      cursorOn(false),
      isPainting(false),
      reverseLensGlasses(false),
      lineAdded(false),
      modelLoaded(false),
      leftNeedsChange(false),
      rightNeedsChange(false)
{
    // Default anagliph
    L_Red = R_Green = R_Blue = true;
    R_Red = L_Green = L_Blue = false;

    // Initialize context with GLFormat
    setStereoMode(false);

    // Filtering hover events
    setAttribute(Qt::WA_Hover, true);
    installEventFilter(this);
}

GLDisplay::~GLDisplay()
{
    // Set the opengl context to this widget
    makeCurrent();

    // remove pointed opengl objects here
    if (!vbo.isCreated())
        vbo.destroy();
    for (int i = 0; i < 5; ++i)
        if(texture[i])
            delete texture[i];
    if (fbo)
        delete fbo;

    // work in opengl is done
    doneCurrent();
}


void GLDisplay::initializeGL()
{
    // QOpenGLFunctions super class need initialization
    initializeOpenGLFunctions();

    // Set clear color
    glClearColor(0, 0, 0, 1);
    glEnable(GL_BLEND);
    //glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        exit("Compile vertex shader error!");

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        exit("Compile fragment shader error!");

    // Link shader pipeline
    if (!program.link())
        exit("Program linking error!");

    // Bind shader pipeline for use
    if (!program.bind())
        exit("Program binding error!");

    // Get attributes location
    vertexLocation = program.attributeLocation("a_vertex");
    textureLocation = program.attributeLocation("a_texture");
}

void GLDisplay::paintGL()
{
    // Bind shader program
    program.bind();

    // Load data if needed
    if (!modelLoaded)
        changeModel(width(),height());

    // Bind data
    vbo.bind();

    // Clear display
    glClear(GL_COLOR_BUFFER_BIT);

    // Enable and set attributes
    program.enableAttributeArray(vertexLocation);
    program.setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
    program.enableAttributeArray(textureLocation);
    program.setAttributeBuffer(textureLocation, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));

    // This pipeline uses one active texture per draw
    static const int u_text = 0;
    program.setUniformValue("u_texture", u_text);

    // Draw background
    if (texture[0]) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        program.setUniformValue("u_filter", 1, 1, 1);
        program.setUniformValue("u_projection", projection);
        texture[0]->bind();
        texture[0]->setWrapMode(QOpenGLTexture::Repeat);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    // If the reverse lens mode is chosen we invert the filters applied with an xor operation
    bool i = reverseLensGlasses;
#define XOR(X) (i ^ X ? 1 : 0)

    // Change image textures if is needed
    QRect target = rect();
    auto leftScene = stereoDisplay_->getCurrentScene()->getLeftScene();
    if (leftNeedsChange && leftScene){
        QImage img = leftScene->getFrame(target.size());
        if (img.isNull())
            return;
        loadTexture( img.convertToFormat(QImage::Format_RGBA8888).mirrored(), 1, false );
        leftNeedsChange = false;
    }
    auto rightScene = stereoDisplay_->getCurrentScene()->getRightScene();
    if (rightNeedsChange && rightScene){
        QImage img = stereoDisplay_->getCurrentScene()->getRightScene()->getFrame(target.size());
        if (img.isNull())
            return;
        loadTexture( img.convertToFormat(QImage::Format_RGBA8888).mirrored(), 2, false );
        rightNeedsChange = false;
    }

    // Draw images
    if (texture[1] && texture[2]) {
        // Redirect draw to frame buffer object
        if (!fbo) {
            QOpenGLFramebufferObjectFormat format;
            format.setAttachment(QOpenGLFramebufferObject::NoAttachment);
            fbo = new QOpenGLFramebufferObject(width(), height(), format);
        }
        fbo->bind();

        // Draw images
        glClearColor(0, 0, 0, 0);
        program.setUniformValue("u_projection", projection);
        glClear(GL_COLOR_BUFFER_BIT);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        program.setUniformValue("u_filter", XOR(L_Red), XOR(L_Green), XOR(L_Blue) );
        texture[1]->bind();
        glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

        glBlendFunc(GL_ONE, GL_ONE);
        program.setUniformValue("u_filter", XOR(R_Red), XOR(R_Green), XOR(R_Blue) );
        texture[2]->bind();
        glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

        // Redirect draw to default frame buffer
        fbo->release();

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, fbo->texture());
        program.setUniformValue("u_filter", 1, 1, 1);
        glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
    }


    // draw cursor
    if (cursorOn && stereoDisplay_->isStereoCursor() && texture[3]) {
        QMatrix4x4 lmodel, rmodel;
        glBlendFunc(GL_ONE, GL_ONE);

        qreal lx = stereoDisplay_->getLeftCursorOffset().x();
        qreal ly = stereoDisplay_->getLeftCursorOffset().y();
        lmodel.translate(cursorPos.x() + lx, cursorPos.y() + ly);
        program.setUniformValue("u_filter", XOR(L_Red), XOR(L_Green), XOR(L_Blue) );
        program.setUniformValue("u_projection", projection * lmodel);
        texture[3]->bind();
        glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

        qreal rx = stereoDisplay_->getRightCursorOffset().x();
        qreal ry = stereoDisplay_->getRightCursorOffset().y();
        rmodel.translate(cursorPos.x() + rx, cursorPos.y() + ry);
        program.setUniformValue("u_filter", XOR(R_Red), XOR(R_Green), XOR(R_Blue) );
        program.setUniformValue("u_projection", projection * rmodel);
        texture[3]->bind();
        glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
    }

    // Draw line
    if (texture[4] && lineAdded) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        program.setUniformValue("u_filter", 1, 1, 1);
        program.setUniformValue("u_projection", projection);
        texture[4]->bind();
        glDrawArrays(GL_LINE_LOOP, 12, 4);
        lineAdded = false;
    }

    // Unbind shader program and data
    program.release();
    vbo.release();
}

void GLDisplay::resizeGL(int w, int h)
{
    // Update viewport
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, w * retinaScale, h * retinaScale);

    // Update projection
    projection.setToIdentity();
    projection.ortho(QRect(0, 0, w * retinaScale, h * retinaScale));

    // Update model
    changeModel(w, h);

    // Update frame buffer object
    if (fbo) {
        delete fbo;
        fbo = nullptr;
    }

    // Update render
    update();
}

bool GLDisplay::eventFilter(QObject *o, QEvent *e)
{
    // Catch events from this object
    if (o == this)
    {
        // Call actions from hover events
        switch (e->type())
        {
        case QEvent::HoverEnter :
        {
            QHoverEvent* he = static_cast<QHoverEvent*>(e);
            hoverEntered(he);
            return true;
        }
        case QEvent::HoverLeave :
        {
            QHoverEvent* he = static_cast<QHoverEvent*>(e);
            hoverLeaved(he);
            return true;
        }
        case QEvent::HoverMove :
        {
            QHoverEvent* he = static_cast<QHoverEvent*>(e);
            hoverMoved(he);
            return true;
        }
        default :
            return QOpenGLWidget::eventFilter(o, e);
        }
    }
    return false;
}

void GLDisplay::exit(QString msg)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();

    // Todo: review this
    qApp->quit();
}

void GLDisplay::drawLine(int coords[8], QColor color)
{
    QImage pen(16, 16, QImage::Format_RGBA8888);
    pen.fill(color);
    loadTexture(pen, 4);
    memcpy(line, coords, 8 * sizeof(int));
    changeModel(width(), height());
    lineAdded = true;
    update();
}

void GLDisplay::changeModel(int w, int h)
{
    // Set the opengl context to this widget
    makeCurrent();

    // Set vertices for background, image and cursor
    int *l = line;
    int coords[4][4][3] = {
        { {   0,   0, -1 }, {   w,   0, -1 }, {   w,   h, -1 }, {   0,   h, -1 } }, // Background vertices
        { {   0,   0,  0 }, {   w,   0,  0 }, {   w,   h,  0 }, {   0,   h,  0 } }, // Image vertices
        { { -16, -16, +1 }, { +16, -16, +1 }, { +16, +16, +1 }, { -16, +16, +1 } }, // Cursor vertices
        { {l[0],l[1], +1 }, {l[2],l[3], +1 }, {l[4],l[5], +1 }, {l[6],l[7], +1 } }  // Line vertices
    };

    // Map coordinates of vertices and texture
    QVector<GLfloat> map;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            // vertex position
            map.append(coords[i][j][0]);
            map.append(coords[i][j][1]);
            map.append(coords[i][j][2]);
            // texture coordinate
            map.append(j == 1 || j == 2);
            map.append(j == 2 || j == 3);
        }
    }

    // Remap texture coordinates from background if is needed
    if (texture[0]) {
        GLfloat s = w / (float) texture[0]->width();
        GLfloat t = h / (float) texture[0]->height();
        map[ 3] = 0; map[ 4] = 0;
        map[ 8] = s; map[ 9] = 0;
        map[13] = s; map[14] = t;
        map[18] = 0; map[19] = t;
    }

    // Create buffer if is needed
    if (!vbo.isCreated()) {
        vbo.create();
        vbo.bind();
        vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
        vbo.allocate(map.count() * sizeof(GLfloat));
        vbo.release();
    }
    vbo.bind();
    void* data = vbo.map(QOpenGLBuffer::WriteOnly);
    if (data)
        memcpy(data, map.constData(), map.count() * sizeof(GLfloat));
    vbo.unmap();
    vbo.release();

    modelLoaded = true;

    // work in opengl is done
    doneCurrent();
}

void GLDisplay::loadLeftImage(QString filename)
{
    loadTexture(QImage(filename), 1);
}

void GLDisplay::loadRightImage(QString filename)
{
    loadTexture(QImage(filename), 2);
}

void GLDisplay::loadBackground(QImage image)
{
    loadTexture(image, 0);
    changeModel(width(), height());
}

void GLDisplay::loadTexture(QImage image, int i, bool outOfContext)
{
    // Set the opengl context to this widget
    if (outOfContext)
        makeCurrent();

    // Destroy if is needed
    if (texture[i]) {
        texture[i]->destroy();
        delete texture[i];
    }

    // Load new texture
    if (context() && context()->isValid())
        texture[i] = new QOpenGLTexture(image);

    // work in opengl is done
    if (outOfContext)
        doneCurrent();
}

void GLDisplay::mousePressEvent(QMouseEvent *e)
{
    // Events are not handled if there is no valid scene
    if (!stereoDisplay_->getCurrentScene() || !stereoDisplay_->getCurrentScene()->isValid())
        return;
    for (int i = 0; i < tool.size(); i++)
    {
        // We transmit the treatment to the associated tools
        tool.at(i)->mousePressed(*e);
    }
}

void GLDisplay::mouseReleaseEvent(QMouseEvent *e)
{
    // Events are not handled if there is no valid scene
    if (!stereoDisplay_->getCurrentScene() || !stereoDisplay_->getCurrentScene()->isValid())
        return;
    for (int i = 0; i < tool.size(); i++)
    {
        // We transmit the treatment to the associated tools
        tool.at(i)->mouseReleased(*e);
    }
}

void GLDisplay::mouseMoveEvent(QMouseEvent *e)
{
    // Events are not handled if there is no valid scene
    if (!stereoDisplay_->getCurrentScene() || !stereoDisplay_->getCurrentScene()->isValid())
        return;
    for (int i = 0; i < tool.size(); i++)
    {
        // We transmit the treatment to the associated tools
        tool.at(i)->mouseMoved(*e);
    }
}

void GLDisplay::mouseDoubleClickEvent(QMouseEvent *e)
{
    // Events are not handled if there is no valid scene
    if (!stereoDisplay_->getCurrentScene() || !stereoDisplay_->getCurrentScene()->isValid())
        return;
    for (int i = 0; i < tool.size(); i++)
    {
        // We transmit the treatment to the associated tools
        tool.at(i)->mouseDblClicked(*e);
    }
}

void GLDisplay::wheelEvent(QWheelEvent *e)
{
    // Events are not handled if there is no valid scene
    if (!stereoDisplay_->getCurrentScene() || !stereoDisplay_->getCurrentScene()->isValid())
        return;

    for (int i = 0; i < tool.size(); i++)
    {
        // We transmit the treatment to the associated tools
        tool.at(i)->wheelEvent(*e);
    }
}

void GLDisplay::paintEvent(QPaintEvent *e)
{
    // We paint this object first
    QOpenGLWidget::paintEvent(e);

    // Scape if there is no valid scene
    if (!stereoDisplay_ || !stereoDisplay_->getCurrentScene() || !stereoDisplay_->getCurrentScene()->isValid())
        return;

    isPainting = true;
    for (int i = 0; i < tool.size(); i++)
    {
        // And we transmit the painting to the associated tools
        tool.at(i)->paintEvent(*e);
    }
    isPainting = false;
}

void GLDisplay::resizeEvent(QResizeEvent *e)
{
    // We resize this object
    QOpenGLWidget::resizeEvent(e);

    // Events are not handled if there is no valid scene
    if (!stereoDisplay_ || !stereoDisplay_->getCurrentScene() || !stereoDisplay_->getCurrentScene()->isValid())
        return;

    // We transmit the resize to the associated tools
    for (int i = 0; i < tool.size(); i++)
    {
        tool.at(i)->resizeEvent(*e);
    }
}

void GLDisplay::hoverEntered(QHoverEvent *event)
{
    // We refresh cursor and paint this object first   <= Todo: review this
    refreshCursor(true, event->pos());

    // Events are not handled if there is no valid scene
    if (!stereoDisplay_->getCurrentScene() || !stereoDisplay_->getCurrentScene()->isValid())
        return;

    for (int i = 0; i < tool.size(); i++)
    {
        // We transmit the treatment to the associated tools
        tool.at(i)->enterEvent(*event);
    }
}

void GLDisplay::hoverLeaved(QHoverEvent *event)
{
    // We refresh cursor and paint this object first   <= Todo: review this
    refreshCursor(false, event->pos());

    // Events are not handled if there is no valid scene
    if (!stereoDisplay_->getCurrentScene() || !stereoDisplay_->getCurrentScene()->isValid())
        return;

    for (int i = 0; i < tool.size(); i++)
    {
        // We transmit the treatment to the associated tools
        tool.at(i)->leaveEvent(*event);
    }
}

void GLDisplay::hoverMoved(QHoverEvent *event)
{
    // We refresh cursor and paint this object first   <= Todo: review this
    refreshCursor(true, event->pos());

    // Events are not handled if there is no valid scene
    if (!stereoDisplay_->getCurrentScene() || !stereoDisplay_->getCurrentScene()->isValid())
        return;

    for (int i = 0; i < tool.size(); i++)
    {
        // We transmit the treatment to the associated tools
        tool.at(i)->moveEvent(*event);
    }
}

void GLDisplay::refreshCursor(bool hover, QPoint pos)
{
    cursorOn = hover;
    cursorPos = pos;
    //update();
}

bool GLDisplay::painting()
{
    return isPainting;
}

void GLDisplay::setActivatedTool(StereoTool *newTool, bool active)
{
    // Search for removal avoiding duplicates
    for (int i = tool.size() - 1; i >=  0; i--)
    {
        if (newTool == tool.at(i))
        {
            tool.removeAt(i);
        }
    }

    // Activate the tool when applicable
    if (active)
        tool.prepend(newTool);
}

QImage GLDisplay::getGLCursor()
{
    return currentCursor;
}

void GLDisplay::setGLCursor(QImage newCursor)
{
    currentCursor = newCursor;
    loadTexture(newCursor, 3);
}

void GLDisplay::setReverseLensGlasses(bool opt)
{
    reverseLensGlasses = opt;
}

bool GLDisplay::setStereoMode(bool mode)
{
    /// \todo <This method has been forced to return false until we have the
    ///        necessary hardware to test the implementation with active stereo
    ///        mode, that is, its correction and testing requires an active
    ///        polarizated glasses, a 120 Hz monitor, and a quad-buffered video
    ///        card. SPUserInterface_Qt::onStereoModeChanged() uses this method
    ///        and should be updated when a solution is feasible.>
    Q_UNUSED(mode)
    return stereoMode = false;
}

void GLDisplay::setColorMaskLeft(bool r, bool g, bool b)
{
    L_Red = r;
    L_Green = g;
    L_Blue = b;
}

void GLDisplay::setColorMaskRight(bool r, bool g, bool b)
{
    R_Red = r;
    R_Green = g;
    R_Blue = b;
}

QPointF GLDisplay::getMouseScreenPosition()
{
    return cursorPos;
}

void GLDisplay::updateMousePosition()
{
    cursorPos = mapFromGlobal(QCursor::pos());
}

void GLDisplay::changeLeft()
{
    leftNeedsChange = true;
}

void GLDisplay::changeRight()
{
    rightNeedsChange = true;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
