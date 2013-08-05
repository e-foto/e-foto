#ifndef STEREODISPLAY_H
#define STEREODISPLAY_H

#include <QGLWidget>
#include <QGridLayout>

#include "SingleDisplay.h"
#include "StereoScene.h"
#include "StereoTools.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class StereoDisplay;

class GLDisplay : public QGLWidget
{
protected:
	StereoDisplay* stereoDisplay_;
	unsigned int ltexture;
	unsigned int rtexture;
	unsigned int ctexture;
	unsigned int btexture;
	QImage ltext;
	QImage rtext;
	QList< StereoTool* > _tool;

	QPointF _mouseScreenPos;
	QImage _cursor;
        bool _onPainting;
        bool stereo_mode;

        // Anagliph filter
        bool L_Red, L_Green, L_Blue;
        bool R_Red, R_Green, R_Blue;
        bool reverseLensGlasses;

	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *);
	bool eventFilter(QObject *o, QEvent *e);
	void enterEvent(QHoverEvent *e);
	void leaveEvent(QHoverEvent *e);
	void moveEvent(QHoverEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseDoubleClickEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent *e);

public:
	GLDisplay(StereoDisplay* parent);
	~GLDisplay();

	bool _GLDisplayUpdate;

	void updateMousePosition();
	QPointF getMouseScreenPosition();
	void setGLCursor(QImage cursor);
	QImage getGLCursor();
	void setGLBackground(QImage bg);
        void setReverseLensGlasses(bool opt);
        void setColorMaskLeft(bool r, bool g, bool b);
        void setColorMaskRight(bool r, bool g, bool b);
        int setStereoMode(bool mode);

    bool painting();

	void setActivatedTool(StereoTool* tool, bool active = true);
};

class StereoDisplay : public QWidget
{
	Q_OBJECT
protected:
	StereoScene* currentScene_;
	SingleDisplay* leftDisplay_;
	SingleDisplay* rightDisplay_;
	GLDisplay* glDisplay_;
	QPointF leftCursorOffset_;
	QPointF rightCursorOffset_;
	double _currentZ;
    bool _stereoCursor;

    QPointF _centerOnLeft;
    QPointF _centerOnRight;
    double _fitScale;

public:
	StereoDisplay(QWidget * parent, StereoScene* currentScene = NULL);
	~StereoDisplay();

	StereoScene* getCurrentScene();
	SingleDisplay* getLeftDisplay();
	SingleDisplay* getRightDisplay();
	SingleDisplay* getLeftOverDisplay();
	SingleDisplay* getRightOverDisplay();
	SingleDisplay* getLeftNearDisplay();
	SingleDisplay* getRightNearDisplay();
	GLDisplay* getRealDisplay();

	void loadLeftImage(QString filename);
	void loadRightImage(QString filename);
	void loadLeftImage(QImage* image);
	void loadRightImage(QImage* image);
	void loadLeftImage(Matrix* image, bool isGrayscale = true);
	void loadRightImage(Matrix* image, bool isGrayscale = true);
        int setStereoMode(bool mode);

	QPointF getLeftCursorOffset();
	QPointF getRightCursorOffset();
	void setLeftCursorOffset(QPointF offset);
	void setRightCursorOffset(QPointF offset);
    void setCursor(QImage newCursor, bool stereo = true);
	QImage getCursor();
    bool isStereoCursor() {return _stereoCursor;}

	double getCurrentZ();
	void setCurrentZ(double z);

	void updateMousePosition();
	QPointF screenPosition(QPointF position, bool leftChannel = true);
	QPointF getMouseScreenPosition();
	QPointF getPositionLeft(QPoint screenPosition);
	QPointF getPositionRight(QPoint screenPosition);

    void adjustFit(QPointF leftCenter, QPointF rightCenter, double scale) {_centerOnLeft = leftCenter; _centerOnRight = rightCenter; _fitScale = scale; currentScene_->getLeftScene()->setLimitScale(scale, currentScene_->getLeftScene()->getMaxScale()); currentScene_->getRightScene()->setLimitScale(scale, currentScene_->getRightScene()->getMaxScale());}

	virtual void fitView();
	virtual void pan(int dx, int dy);
	virtual void zoom(double zoomFactor, QPoint* atPoint = NULL);

	void updateAll();
	void updateAll(QPointF* left, QPointF* right, bool emitClicked = false);
	void updateDetail(QPointF* left, QPointF* right, bool emitClicked = false);

	void setActivatedTool(StereoTool* tool, bool active = true);

    bool painting();
    void resizeEvent(QResizeEvent *);

    void setReverseLensGlasses(bool opt);
    void setColorMaskLeft(bool r, bool g, bool b);
    void setColorMaskRight(bool r, bool g, bool b);

signals:
    void mousePositionsChanged(QPointF*, QPointF*);
    void mouseClicked(QPointF*, QPointF*);
    void resized(int, int);
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // STEREODISPLAY_H
