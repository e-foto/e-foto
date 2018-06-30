#ifndef SINGLESCENE_H
#define SINGLESCENE_H

#include <QObject>
#include <QRect>

#include "RasterResource.h"
#include "GeometryResource.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Matrix;

class AbstractScene: public QObject {
  protected:
    explicit AbstractScene(QObject* parent) : QObject(parent) {}

  public:
    virtual void centerContent() = 0;
    virtual void moveTo(QPointF pos) = 0;
    virtual void pan(QPointF dPos) = 0;
    virtual void scaleTo(double newScale, QPointF at = QPointF(-1, -1)) = 0;
    virtual void zoom(double zoomFactor, QPointF at = QPointF(-1, -1)) = 0;
    virtual void setViewport(QSize viewportSize) = 0;

    virtual bool isValid() = 0;
    virtual double getScale() = 0;
    virtual QPointF getViewpoint() = 0;
    virtual unsigned int getWidth() = 0;
    virtual unsigned int getHeight() = 0;

    virtual QImage getFrame(QSize targetSize) = 0;
    virtual QImage getFrame(QSize targetSize, QSize rectSize) = 0;
    virtual QImage getFrame(QSize targetSize, double scale) = 0;
    virtual QImage getThumb(QSize targetSize, QRect* rect = nullptr) = 0;
    virtual QImage getDetail(QSize targetSize, QPointF point, double zoom) = 0;
    virtual QColor getColor(QPoint at) = 0;
    virtual unsigned int getGrayColor(QPointF at, bool linear = false) = 0;

    virtual GeometryResource* geometry() = 0;
};

class SingleScene: public AbstractScene {
    Q_OBJECT

  public:
    explicit SingleScene(QObject* parent, QString filepath);
    ~SingleScene();

    bool loadImage(QString filepath);
    bool loadImage(QImage image);
    bool loadImage(Matrix* image, bool isGrayscale = true);

    QSize imageSize();

    void centerContent();
    void moveTo(QPointF pos);
    void pan(QPointF dPos);
    void scaleTo(double newScale, QPointF at = QPointF(-1, -1));
    void zoom(double zoomFactor, QPointF at = QPointF(-1, -1));
    void setViewport(QSize viewportSize);
    void setLimitScale(double minScale, double maxScale);
    double getMaxScale();

    bool isValid();

    double getScale();
    QPointF getViewpoint();

    double getThumbScale();

    QPointF getDetailedPoint();
    void setDetailedPoint(QPointF point);
    double getDetailZoom();
    void setDetailZoom(double zoom);

    unsigned int getWidth();
    unsigned int getHeight();

    void useSmooth(bool useSmooth);

    QImage getFrame(QSize targetSize);
    QImage getFrame(QSize targetSize, QSize rectSize);
    QImage getFrame(QSize targetSize, double scale);
    QImage getThumb(QSize targetSize, QRect* rect = nullptr);
    QImage getDetail(QSize targetSize, QPointF point, double zoom);
    QColor getColor(QPoint at);
    unsigned int getGrayColor(QPointF at, bool linear = false);

    GeometryResource* geometry();

  private:
    RasterResource* rasterRsrc_{nullptr};
    GeometryResource geometryRsrc_{};
    QPointF viewpoint_{};
    double scale_{0.0};
    double maxScale_{0.0};
    double minScale_{0.0};
    double thumbScale_{0.0};
    double detailZoom_{0.0};
    QPointF detailViewpoint_{};
    QSize viewportSize_{};

    void limitScale();


};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // SINGLESCENE_H
