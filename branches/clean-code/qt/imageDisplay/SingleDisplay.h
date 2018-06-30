#ifndef SINGLEDISPLAY_H
#define SINGLEDISPLAY_H

#include <QWidget>
#include <QList>

#include "SingleScene.h"
#include "SingleTools.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

enum DisplayMode {IntermediatedScreen, TopViewScreen, MostDetailedScreen};

class AbstractScene;
class StereoTool;

class SingleDisplay : public QWidget {
    Q_OBJECT

  protected:
    AbstractScene* currentScene_{nullptr};
    DisplayMode displayMode_{};
    SingleDisplay* over_{};
    SingleDisplay* detail_{};
    QList< SingleTool* > tool_{};
    QList< StereoTool* > stool_{};

    //bool _onMove;
    bool cloneScale_;
    bool showDetailArea_;
    bool blockShowDetailArea_;
    QPointF mouseLastPos_;
    bool onPainting_;

    void paintEvent(QPaintEvent* e);
    void resizeEvent(QResizeEvent*);
    bool eventFilter(QObject* o, QEvent* e);
    void enterEvent(QHoverEvent* e);
    void leaveEvent(QHoverEvent* e);
    void moveEvent(QHoverEvent* e);
    void mousePressEvent(QMouseEvent* e);
    void wheelEvent(QWheelEvent* e);

  public:
    explicit SingleDisplay(QWidget* parent = 0, AbstractScene* currentScene = nullptr);
    ~SingleDisplay();

    SingleDisplay* getOverDisplay();
    SingleDisplay* getDetailDisplay();

    void updateMousePosition();
    bool positionIsVisible(QPointF);
    QPointF screenPosition(QPointF);
    QPointF getLastMousePosition();
    QPointF getPosition(QPoint screenPosition);
    QPointF getMouseScreenPosition();
    bool painting();

    bool showDetailedArea();
    void setShowDetailedArea(bool status);
    void blockShowDetailedArea(bool status);
    AbstractScene* getCurrentScene();

    virtual void fitView();
    void updateAll();
    void updateDetail();
    void updateDetail(QPointF* newMousePosition, bool emitClicked = false);

    void setOverviewMode(SingleDisplay* display);
    void setDetailMode(SingleDisplay* display);
    void setActivatedTool(SingleTool* tool, bool active = true);
    void setActivatedTool(StereoTool* tool, bool active = true);

  signals:
    void mousePositionChanged(QPointF*);
    void mouseClicked(QPointF*);
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // SINGLEDISPLAY_H
