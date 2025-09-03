#ifndef RASTERRESOURCE_H
#define RASTERRESOURCE_H

#include <gdal_priv.h>

#include <QColor>
#include <QImage>
#include <QString>

#include "CommonDef.h"
#include "Matrix.h"

#include <vector>

class RasterResource {
 protected:
  GDALDataset** _pyramid;
  QSize _imageDim;
  int _levels;
  bool _isValid;
  bool _useSmoothIn;
  bool _useSmoothOut;

  QImage gdalDatasetToQImage(GDALDataset* dataset,
                             const QRect& rect = QRect()) const;
  GDALDataset* createPyramidLevel(GDALDataset* srcDataset, int level) const;

 public:
  RasterResource(QString filepath, bool withSmoothIn = false,
                 bool withSmoothOut = true, bool metadataOnly = false);
  ~RasterResource();

  void emitLoadError(const QString& filepath = "");
  bool load(QImage image);
  bool load(std::vector<br::uerj::eng::efoto::Matrix> &image);
  bool load(QString filepath, bool metadataOnly = false);
  bool save(QString filepath, QString format);

  bool isValid();
  int levels();

  int width();
  int height();
  QSize size();
  QPointF center();

  QImage getImageCut(QSize targetSize, QRectF imageCut);
  QColor getColor(QPoint at);
  unsigned int getGrayColor(QPoint at, bool linear = false);
  void useSmoothIn(bool useSmooth);
  void transformImage(double H[9]);
};

#endif  // RASTERRESOURCE_H
