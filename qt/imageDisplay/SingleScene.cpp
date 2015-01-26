#include "SingleScene.h"
#include <math.h>
#include "Matrix.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

SingleScene::SingleScene(QObject* parent, QString filepath):
    AbstractScene(parent),
    viewpoint_(0,0),
    viewportSize_(800,600)
{
    detailZoom_ = 4.0;
    if (filepath.isEmpty())
    {
        rasterRsrc_ = NULL;
        minScale_ = 1;
        maxScale_ = 8;
    }
    else
    {
        scale_ = 1.0;
        rasterRsrc_ = new RasterResource(filepath);
        if (!rasterRsrc_->isValid())
        {
            delete(rasterRsrc_);
            rasterRsrc_ = NULL;
            return;
        }
        minScale_ = 1;
        maxScale_ = 8;
        viewpoint_ = QPointF(rasterRsrc_->center().toPoint());
    }
}

SingleScene::~SingleScene()
{
    if (rasterRsrc_ != NULL)
        delete(rasterRsrc_);
}

QPointF SingleScene::getViewpoint()
{
    return viewpoint_;
}

unsigned int SingleScene::getWidth()
{
    if (rasterRsrc_ && rasterRsrc_->isValid())
        return rasterRsrc_->width();
    return 0;
}

unsigned int SingleScene::getHeight()
{
    if (rasterRsrc_ && rasterRsrc_->isValid())
        return rasterRsrc_->height();
    return 0;
}

void SingleScene::useSmooth(bool useSmooth)
{
    if (rasterRsrc_ && rasterRsrc_->isValid())
        rasterRsrc_->useSmoothIn(useSmooth);
}

void SingleScene::centerContent()
{
    if (rasterRsrc_ && rasterRsrc_->isValid())
        viewpoint_ = rasterRsrc_->center();
}

void SingleScene::moveTo(QPointF pos)
{
    viewpoint_ = pos;
    if (rasterRsrc_ && rasterRsrc_->isValid())
    {
        if (viewpoint_.x() > rasterRsrc_->width())
            viewpoint_.setX(rasterRsrc_->width());
        if (viewpoint_.x() < 0)
            viewpoint_.setX(0);
        if (viewpoint_.y() > rasterRsrc_->height())
            viewpoint_.setY(rasterRsrc_->height());
        if (viewpoint_.y() < 0)
            viewpoint_.setY(0);
    }
}

void SingleScene::pan(QPointF dPos)
{
    viewpoint_ += dPos;
    if (rasterRsrc_ && rasterRsrc_->isValid())
    {
        if (viewpoint_.x() > rasterRsrc_->width())
            viewpoint_.setX(rasterRsrc_->width());
        if (viewpoint_.x() < 0)
            viewpoint_.setX(0);
        if (viewpoint_.y() > rasterRsrc_->height())
            viewpoint_.setY(rasterRsrc_->height());
        if (viewpoint_.y() < 0)
            viewpoint_.setY(0);
    }
}

void SingleScene::scaleTo(double newScale, QPointF at)
{
    double oldScale = scale_;
    scale_ = newScale;

    limitScale();

    //if (at == QPoint(-1,-1) || rasterRsrc_ == NULL)
    if (at.x() < 0 || at.y() < 0 || !rasterRsrc_ || !(rasterRsrc_->isValid()))
        return;
    if (!((at.x() >= 0 && at.x() <= rasterRsrc_->width()) && (at.y() >= 0 && at.y() <= rasterRsrc_->height())))
        at = rasterRsrc_->center();
    moveTo((viewpoint_-at)*oldScale/scale_ + at);
}

void SingleScene::zoom(double zoomFactor, QPointF at)
{
    double oldScale = scale_;
    scale_ *= zoomFactor;

    limitScale();

    if (at == QPointF(-1,-1) || !rasterRsrc_ || !(rasterRsrc_->isValid()))
        return;
    if (!((at.x() >= 0 && at.x() <= rasterRsrc_->width()) && (at.y() >= 0 && at.y() <= rasterRsrc_->height())))
        at = rasterRsrc_->center();
    moveTo((viewpoint_-at)*oldScale/scale_ + at);
}

void SingleScene::setViewport(QSize viewportSize)
{
    viewportSize_ = viewportSize;
    setLimitScale(viewportSize_.width()/(double)getWidth() < viewportSize_.height()/(double)getHeight() ? viewportSize_.width()/(double)getWidth() : viewportSize_.height()/(double)getHeight(), getMaxScale());
    limitScale();
}

void SingleScene::transformImage(double H[9])
{
    if (rasterRsrc_ && rasterRsrc_->isValid())
        rasterRsrc_->transformImage(H);
}

bool SingleScene::createImage(QSize size, QColor color)
{
    RasterResource* rsrc = new RasterResource("");
    QImage image(size,QImage::Format_ARGB32);
    image.fill(color.rgba());

    rsrc->load(image);
    if (rsrc->isValid())
    {
        if (rasterRsrc_)
            delete(rasterRsrc_);
        rasterRsrc_ = rsrc;
        viewpoint_ = QPointF(rasterRsrc_->center().toPoint());
        scale_ = 1;
        setLimitScale(viewportSize_.width()/(double)getWidth() < viewportSize_.height()/(double)getHeight() ? viewportSize_.width()/(double)getWidth() : viewportSize_.height()/(double)getHeight(), getMaxScale());
        return true;
    }
    else
        delete(rsrc);
    return false;
}

bool SingleScene::saveImage(QString filepath, QString format)
{
    if (rasterRsrc_ && rasterRsrc_->isValid())
    {
        return rasterRsrc_->save(filepath,format);
    }
    return false;
}

bool SingleScene::loadImage(QString filepath)
{
    RasterResource* rsrc = new RasterResource(filepath);
    if (rsrc->isValid())
    {
        if (rasterRsrc_)
            delete(rasterRsrc_);
        rasterRsrc_ = rsrc;
        detailViewpoint_ = viewpoint_ = QPointF(rasterRsrc_->center().toPoint());
        scale_ = 1;
        setLimitScale(viewportSize_.width()/(double)getWidth() < viewportSize_.height()/(double)getHeight() ? viewportSize_.width()/(double)getWidth() : viewportSize_.height()/(double)getHeight(), getMaxScale());
        return true;
    }
    else
        delete(rsrc);
    return false;
}

bool SingleScene::loadImage(QImage image)
{
    RasterResource* rsrc = new RasterResource("");
    rsrc->load(image);
    if (rsrc->isValid())
    {
        if (rasterRsrc_)
            delete(rasterRsrc_);
        rasterRsrc_ = rsrc;
        detailViewpoint_ = viewpoint_ = QPointF(rasterRsrc_->center().toPoint());
        scale_ = 1;
        setLimitScale(viewportSize_.width()/(double)getWidth() < viewportSize_.height()/(double)getHeight() ? viewportSize_.width()/(double)getWidth() : viewportSize_.height()/(double)getHeight(), getMaxScale());
        return true;
    }
    else
        delete(rsrc);
    return false;
}

bool SingleScene::loadImage(Matrix *image, bool isGrayscale)
{
    // Transforma a matrix em QImage
    QImage img(image->getCols(), image->getRows(), QImage::Format_ARGB32);
    unsigned int pixel;

    // Convert Matrix to QImage
    for (unsigned int i=1; i<=image->getRows(); i++)
    {
        for (unsigned int j=1; j<=image->getCols(); j++)
        {
            if (isGrayscale)
            {
                pixel = int(image->get(i,j)*255.0);
                pixel = (pixel << 16) + (pixel << 8) + pixel;
            }
            else
                pixel = int(image->get(i,j)*double(0xFFFFFF));

            pixel = pixel | 0XFF000000;
            img.setPixel(j-1, i-1, pixel);
        }
    }

    // Procede o load por QImage;
    return loadImage(img);
}

QSize SingleScene::imageSize()
{
    if (rasterRsrc_ && rasterRsrc_->isValid())
        return QSize(rasterRsrc_->width(),rasterRsrc_->height());
    return QSize(0,0);
}

void SingleScene::setLimitScale(double minScale, double maxScale)
{
    minScale_ = minScale;
    maxScale_ = maxScale;
}

double SingleScene::getMinScale()
{
    return minScale_;
}

double SingleScene::getMaxScale()
{
    return maxScale_;
}

void SingleScene::limitScale()
{
    if (scale_ < minScale_)
        scale_ = minScale_;
    else if (scale_ > maxScale_)
        scale_ = maxScale_;
}

QImage SingleScene::getImage()
{
    if (!rasterRsrc_ || !(rasterRsrc_->isValid()))
        return QImage();

    QRectF imageCut;
    imageCut.setSize(rasterRsrc_->size());
    imageCut.moveCenter(rasterRsrc_->center());

    return rasterRsrc_->getImageCut(rasterRsrc_->size(), imageCut);
}

QImage SingleScene::getFrame(QSize targetSize)
{
    if (!rasterRsrc_ || !(rasterRsrc_->isValid()))
        return QImage();

    QRectF imageCut;
    QSizeF newSize;
    newSize.setWidth(targetSize.width()/scale_);
    newSize.setHeight(targetSize.height()/scale_);
    imageCut.setSize(newSize);
    imageCut.moveCenter(viewpoint_);

    return geometryRsrc_.draw(rasterRsrc_->getImageCut(targetSize, imageCut),targetSize, viewpoint_, scale_);
}

QImage SingleScene::getFrame(QSize targetSize, QSize rectSize)
{
    if (!rasterRsrc_ || !(rasterRsrc_->isValid()))
        return QImage();

    QRectF imageCut;
    QSizeF newSize;
    newSize.setWidth(targetSize.width()/scale_);
    newSize.setHeight(targetSize.height()/scale_);
    imageCut.setSize(newSize);
    imageCut.moveCenter(viewpoint_);

    QImage result(geometryRsrc_.draw(rasterRsrc_->getImageCut(targetSize, imageCut),targetSize, viewpoint_, scale_));

    QPointF detailTopLeft = (detailViewpoint_ - QPointF(rectSize.width()/(detailZoom_*scale_*2.0), rectSize.height()/(detailZoom_*scale_*2.0)));
    QPointF toScreen = (detailTopLeft - viewpoint_)*scale_ + QPointF(targetSize.width()/2, targetSize.height()/2);
    QRect detailedRect( toScreen.toPoint(), ((scale_*QSizeF(rectSize))/(scale_*detailZoom_)).toSize());

    QPainter painter(&result);
    painter.setPen(QPen(QColor(Qt::yellow)));
    painter.drawRect(detailedRect);
    painter.end();

    return result;
}

QImage SingleScene::getFrame(QSize targetSize, double scale)
{
    if (!rasterRsrc_ || !(rasterRsrc_->isValid()))
        return QImage();

    QRectF imageCut;
    QSizeF newSize;
    newSize.setWidth(targetSize.width()/scale);
    newSize.setHeight(targetSize.height()/scale);
    imageCut.setSize(newSize);
    imageCut.moveCenter(viewpoint_);

    return geometryRsrc_.draw(rasterRsrc_->getImageCut(targetSize, imageCut),targetSize, viewpoint_, scale);
}

QImage SingleScene::getThumb(QSize targetSize, QRect* rect)
{
    if (!rasterRsrc_ || !(rasterRsrc_->isValid()))
        return QImage();

    QRectF imageCut;
    imageCut.setSize(rasterRsrc_->size());
    imageCut.moveCenter(rasterRsrc_->center());
    QSize newTargetSize = rasterRsrc_->size();
    newTargetSize.scale(targetSize,Qt::KeepAspectRatio);

    double scale = thumbScale_ = newTargetSize.width()/(double)rasterRsrc_->size().width();

    QImage result( geometryRsrc_.draw( rasterRsrc_->getImageCut(newTargetSize, imageCut), newTargetSize, imageCut.center(), scale));

    if (rect)
    {
        QRect viewedRect(((viewpoint_-QPointF(rect->width()/(scale_*2.0), rect->height()/(scale_*2.0)))*scale).toPoint(), ((scale*QRectF(*rect).size())/scale_).toSize());
        QPainter painter(&result);
        painter.setPen(QPen(QColor(Qt::yellow)));
        painter.drawRect(viewedRect);
        painter.end();
        *rect = viewedRect;
    }
    return result;
}

QImage SingleScene::getDetail(QSize targetSize, QPointF point, double zoom)
{
    if (!rasterRsrc_ || !(rasterRsrc_->isValid()))
        return QImage();

    QRectF imageCut;
    QSizeF newSize;
    newSize.setWidth(targetSize.width()/zoom);
    newSize.setHeight(targetSize.height()/zoom);
    imageCut.setSize(newSize);
    imageCut.moveCenter(point);

    //detailViewpoint_ = point;

    return geometryRsrc_.draw(rasterRsrc_->getImageCut(targetSize, imageCut),targetSize, point, zoom);
}

QColor SingleScene::getColor(QPoint at)
{
    if (!rasterRsrc_ || !(rasterRsrc_->isValid()))
        return QColor();
    return rasterRsrc_->getColor(at);
}

unsigned int SingleScene::getGrayColor(QPointF at, bool linear)
{
    if (!rasterRsrc_ || !(rasterRsrc_->isValid()))
        return 0;
    return rasterRsrc_->getGrayColor(at, linear);
}

bool SingleScene::isValid()
{
    return rasterRsrc_ ? (rasterRsrc_->isValid() ? true : false) : false;
}

double SingleScene::getScale()
{
    return scale_;
}

double SingleScene::getThumbScale()
{
    return thumbScale_;
}

QPointF SingleScene::getDetailedPoint()
{
    return detailViewpoint_;
}

void SingleScene::setDetailedPoint(QPointF point)
{
    detailViewpoint_ = point;
}

double SingleScene::getDetailZoom()
{
    return detailZoom_;
}

void SingleScene::setDetailZoom(double zoom)
{
    detailZoom_ = zoom;
}

RasterResource* SingleScene::rasters(int &rastersCount)
{
    rastersCount = 1;
    return rasterRsrc_;
}

GeometryResource* SingleScene::geometries(int &geometriesCount)
{
    geometriesCount = 1;
    return &geometryRsrc_;
}

RasterResource* SingleScene::raster(/*int at*/)
{
    // Num futuro em que existam vários rasterResources associados a uma cena o at será o indice do raster buscado na cena
    // O código -1 poderá ser usado para indicar o raster corrente
    return rasterRsrc_;
}

GeometryResource* SingleScene::geometry(/*int at*/)
{
    // Num futuro em que existam vários geometryResources associados a uma cena o at será o indice do geometry buscado na cena
    // O código -1 poderá ser usado para indicar o geometry corrente
    return &geometryRsrc_;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
