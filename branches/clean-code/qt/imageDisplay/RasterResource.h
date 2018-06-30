#ifndef RASTERRESOURCE_H
#define RASTERRESOURCE_H

#include <QString>
#include <QImage>
#include <QColor>

class RasterResource {
  protected:
    QImage** pyramid_{nullptr};
    QSize imageDim_{};
    int levels_{0};
    bool isValid_{false};
    bool useSmoothIn_{false};
    bool useSmoothOut_{false};

  public:
    RasterResource(QString filepath, bool withSmoothIn = false,
                   bool withSmoothOut = true);
    ~RasterResource();

    static void emitLoadError();
    bool load(QImage image);
    bool load(QString filepath);
    bool save(QString filepath, QString format);

    bool isValid() const;

    int width();
    int height();
    QSize size() const;
    QPointF center();

    QImage getImageCut(QSize targetSize, QRectF imageCut);
    QColor getColor(QPoint at);
    unsigned int getGrayColor(QPointF at, bool linear = false);
    void useSmoothIn(bool useSmooth);
};

#endif // RASTERRESOURCE_H
