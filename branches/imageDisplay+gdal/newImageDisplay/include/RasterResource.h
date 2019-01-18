#ifndef RASTERRESOURCE_H
#define RASTERRESOURCE_H

#include <QString>
#include <QImage>
#include <QColor>
#include "gdal.h"
#include "gdal_priv.h"



class RasterResource {
protected:
    //QImage** _pyramid;
    //int _levels;
    GDALDataset* _dataset;
    QSize _imageDim;
    bool _isValid;
    bool _useSmoothIn;
    bool _useSmoothOut;

    int _nBands;
    int _nViewBands;
    int* _viewBands;

    QImage::Format _qtFormat;
    QVector<QRgb>  _cTable;

public:
    RasterResource(QString filepath, bool withSmoothIn = false, bool withSmoothOut = true);
    ~RasterResource();

    void emitLoadError();
    bool load(QImage image);
    bool load(QString filepath);
    bool save(QString filepath, QString format);

    bool isValid();
    //int levels();
    int height();
    int width();
    QSize size();
    QPointF center();
    QImage getImageCut(QSize targetSize, QRectF imageCut);
    QColor getColor(QPoint at);
    unsigned int getGrayColor(QPointF at, bool linear = false);
    void useSmoothIn(bool useSmooth);
    void transformImage(double H[9]);
};

#endif // RASTERRESOURCE_H
