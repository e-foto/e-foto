#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class StereoDisplay;
class StereoTool;

class GLDisplay : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLDisplay(StereoDisplay *parent = nullptr);
    ~GLDisplay();

    bool painting();
    void setActivatedTool(StereoTool *tool, bool active);
    QImage getGLCursor();
    void setGLCursor(QImage newCursor);
    bool setStereoMode(bool opt);
    void setReverseLensGlasses(bool opt);
    void setColorMaskLeft(bool r, bool g, bool b);
    void setColorMaskRight(bool r, bool g, bool b);
    QPointF getMouseScreenPosition();
    void updateMousePosition();
    void changeLeft();
    void changeRight();
    void drawLine(int coords[8], QColor color = QColor(255,255,0));
    void loadBackground(QImage image);
    void loadLeftImage(QString filename);
    void loadRightImage(QString filename);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void paintEvent(QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;
    bool eventFilter(QObject *o, QEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;
    void hoverEntered(QHoverEvent *event);
    void hoverLeaved(QHoverEvent *event);
    void hoverMoved(QHoverEvent *event);
    void refreshCursor(bool hover, QPoint pos);
    void changeModel(int w, int h);
    void loadTexture(QImage image, int i, bool outOfContext = true);
    void exit(QString msg);


private:
    StereoDisplay* stereoDisplay_;
    QList< StereoTool* > tool;
    bool stereoMode;
    bool cursorOn;
    bool isPainting;
    bool reverseLensGlasses;
    bool L_Red, L_Green, L_Blue;
    bool R_Red, R_Green, R_Blue;
    bool lineAdded;
    int line[8] = {-1,-1,-1,-1,-1,-1,-1,-1};
    QPoint cursorPos;
    QImage currentCursor;

    QOpenGLBuffer vbo;
    QOpenGLShaderProgram program;
    QOpenGLFramebufferObject *fbo = nullptr;
    QOpenGLTexture *texture[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};
    QMatrix4x4 projection;
    int vertexLocation, textureLocation;
    bool modelLoaded;
    bool leftNeedsChange, rightNeedsChange;
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // WIDGET_H
