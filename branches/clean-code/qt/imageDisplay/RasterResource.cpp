#include "RasterResource.h"
#include "math.h"

#include <QMessageBox>
#include <QDebug>

RasterResource::RasterResource(QString filepath, bool withSmoothIn,
                               bool withSmoothOut) {
    // Abrir a imagem e testar se é válida
    QImage* srcImage = new QImage();
    isValid_ = srcImage->load(filepath);
    levels_ = 0;
    useSmoothIn_ = withSmoothIn;
    useSmoothOut_ = withSmoothOut;

    if (!isValid_ || srcImage->width() == 0 || srcImage->height() == 0) {
        if (filepath != "") {
            emitLoadError();
        }

        return;
    }

    // Calcular o numero de niveis da piramide
    imageDim_ = srcImage->size();
    levels_ =  log(imageDim_.width() < imageDim_.height() ? imageDim_.width() :
                   imageDim_.height() ) / log(2) - 4;
    // Alocando espaço para a piramide
    pyramid_ = (QImage**) calloc(levels_, sizeof(QImage*));
    // Atribui a imagem original ao primeiro nível
    pyramid_[0] = new QImage(srcImage->convertToFormat(QImage::Format_ARGB32));
    delete(srcImage);

    if (pyramid_[0]->width() == 0 || pyramid_[0]->height() == 0) {
        isValid_ = false;
        delete(pyramid_[0]);
        free(pyramid_);
        emitLoadError();
        return;
    }

    // Construindo imagens
    for(int l = 1; l < levels_; l++) {
        // Cada imagem do novo nível é igual ao nível anterior reduzida pela metade
        int w = pyramid_[l - 1]->width() / 2;
        int h = pyramid_[l - 1]->height() / 2;
        pyramid_[l] = new QImage(pyramid_[l - 1]->scaled(w, h,
                                 Qt::KeepAspectRatioByExpanding,
                                 useSmoothOut_ ? Qt::SmoothTransformation : Qt::FastTransformation));

        if (pyramid_[l]->width() == 0 || pyramid_[l]->height() == 0) {
            isValid_ = false;

            for (int k = l; l >= 0; l--) {
                delete(pyramid_[k]);
            }

            free(pyramid_);
            emitLoadError();
            return;
        }
    }
}

RasterResource::~RasterResource() {
    if (!isValid_) {
        return;
    }

    for(int l = 0; l < levels_; l++) {
        delete(pyramid_[l]);
    }

    free(pyramid_);
}

void RasterResource::emitLoadError() {
    QMessageBox* msgBox = new QMessageBox();
    msgBox->setText("Error: The image loading process.");
    msgBox->exec();
}

void RasterResource::useSmoothIn(bool useSmooth) {
    useSmoothIn_ = useSmooth;
}

bool RasterResource::load(QImage image) {
    // Impede carga de imagem nula
    if (image.isNull()) {
        emitLoadError();
        return false;
    }

    // Remove imagem pré-existente
    if (isValid_) {
        for(int l = 0; l < levels_; l++) {
            delete(pyramid_[l]);
        }

        free(pyramid_);
    }

    isValid_ = true;
    // Calcular o numero de niveis da piramide
    imageDim_ = image.size();
    levels_ =  log(imageDim_.width() < imageDim_.height() ? imageDim_.width() :
                   imageDim_.height() ) / log(2) - 4;

    if (levels_ < 0) {
        levels_ = 0;
    }

    // Alocando espaço para a piramide
    pyramid_ = (QImage**) calloc(levels_, sizeof(QImage*));
    // Atribui a imagem original ao primeiro nível
    pyramid_[0] = new QImage(image.convertToFormat(QImage::Format_ARGB32));

    if (pyramid_[0]->width() == 0 || pyramid_[0]->height() == 0) {
        isValid_ = false;
        delete(pyramid_[0]);
        free(pyramid_);
        emitLoadError();
        return false;
    }

    // Construindo imagens
    for(int l = 1; l < levels_; l++) {
        // Cada imagem do novo nível é igual ao nível anterior reduzida pela metade
        int w = pyramid_[l - 1]->width() / 2;
        int h = pyramid_[l - 1]->height() / 2;
        pyramid_[l] = new QImage(pyramid_[l - 1]->scaled(w, h, Qt::IgnoreAspectRatio,
                                 useSmoothOut_ ? Qt::SmoothTransformation : Qt::FastTransformation));

        if (pyramid_[l]->width() == 0 || pyramid_[l]->height() == 0) {
            isValid_ = false;

            for (int k = l; l >= 0; l--) {
                delete(pyramid_[k]);
            }

            free(pyramid_);
            emitLoadError();
            return false;
        }
    }

    return isValid_;
}

bool RasterResource::load(QString filepath) {
    // Abrir a imagem e testar se é válida
    QImage image;

    if (image.load(filepath)) {
        for(int l = 0; l < levels_; l++) {
            delete(pyramid_[l]);
        }

        free(pyramid_);
        isValid_ = true;
    } else {
        return false;
    }

    // Calcular o numero de niveis da piramide
    imageDim_ = image.size();
    levels_ =  log(imageDim_.width() < imageDim_.height() ? imageDim_.width() :
                   imageDim_.height() ) / log(2) - 4;

    if (levels_ < 0) {
        levels_ = 0;
    }

    // Alocando espaço para a piramide
    pyramid_ = (QImage**) calloc(levels_, sizeof(QImage*));
    // Atribui a imagem original ao primeiro nível
    pyramid_[0] = new QImage(image.convertToFormat(QImage::Format_ARGB32));

    if (pyramid_[0]->width() == 0 || pyramid_[0]->height() == 0) {
        isValid_ = false;
        delete(pyramid_[0]);
        free(pyramid_);
        emitLoadError();
        return false;
    }

    // Construindo imagens
    for(int l = 1; l < levels_; l++) {
        // Cada imagem do novo nível é igual ao nível anterior reduzida pela metade
        int w = pyramid_[l - 1]->width() / 2;
        int h = pyramid_[l - 1]->height() / 2;
        pyramid_[l] = new QImage(pyramid_[l - 1]->scaled(w, h, Qt::IgnoreAspectRatio,
                                 useSmoothOut_ ? Qt::SmoothTransformation : Qt::FastTransformation));

        if (pyramid_[l]->width() == 0 || pyramid_[l]->height() == 0) {
            isValid_ = false;

            for (int k = l; l >= 0; l--) {
                delete(pyramid_[k]);
            }

            free(pyramid_);
            emitLoadError();
            return false;
        }
    }

    return isValid_;
}

bool RasterResource::save(QString filepath, QString format) {
    if (pyramid_ && pyramid_[0]->save(filepath, format.toLocal8Bit().constData())) {
        return true;
    }

    return false;
}

bool RasterResource::isValid() const {
    return isValid_;
}

int RasterResource::width() {
    return imageDim_.width();
}

int RasterResource::height() {
    return imageDim_.height();
}

QSize RasterResource::size() const {
    return imageDim_;
}

QPointF RasterResource::center() {
    return QPointF(width() / 2.0, height() / 2.0);
}

QImage RasterResource::getImageCut(QSize targetSize, QRectF imageCut) {
    // Este método poderia possuir a assinatura getImageCut(QSize targetSize, QPointF viewPoint, double scale)
    // Contudo devido a possível incompatibilidade com o TeCanvas ele ficou da forma atual
    QImage result;

    if (isValid_) {
        // Inicialização de variáveis para o recorte na imagem do topo da pilha
        QRect rectToCut = imageCut.toRect();
        QImage* img = pyramid_[0];
        int l = 0;
        // Na prática, qualquer corte deveria possuir a mesma escala em width e height
        // mas isso não ocorre para o recorte de Overview. Por isso recuperamos a escala assim:
        double wscale = targetSize.width() / imageCut.width();
        double hscale = targetSize.height() / imageCut.height();
        double scale = (wscale > hscale) ? wscale : hscale;

        // Seleciona a imagem correta a recortar, faz o recorte e ajustes para perfeito encaixe do centro do recorte na imagem com a imagem resultante
        if (scale > 1.0) {
            // Este caso precisa ser tratado com atenção.
            // Precisamos manter o aspécto da imagem, o zoom correto e o ponto central do recorte corretamente alinhado
            // Primeiro evitamos problemas com o espaço recortado aumentando ligeiramente a área de recorte
            rectToCut.setLeft(rectToCut.left() - 1);
            rectToCut.setTop(rectToCut.top() - 1);
            rectToCut.setWidth(rectToCut.width() + 2);
            rectToCut.setHeight(rectToCut.height() + 2);
            // Criamos um newTargetSize para a nova escala com o aspécto do primeiro recorte
            QSize newTargetSize = rectToCut.size() * scale;
            // Um recorte final vai garantir a posição corretamente alinhada e o targetSize pedido
            QRect finalCut(((imageCut.topLeft() - QPointF(rectToCut.topLeft())) *
                            scale).toPoint(), targetSize);
            result = img->copy(rectToCut).scaled(newTargetSize,
                                                 Qt::KeepAspectRatioByExpanding,
                                                 useSmoothIn_ ?
                                                 Qt::SmoothTransformation
                                                 : Qt::FastTransformation).copy(finalCut);
        } else if (scale > 0.5) {
            // Corta e reduz a imagem a partir do primeiro nível da pirâmide. Este é o caso mais simples
            result = img->copy(rectToCut).scaled(targetSize, Qt::KeepAspectRatioByExpanding,
                                                 useSmoothOut_ ? Qt::SmoothTransformation : Qt::FastTransformation);
        } else {
            // Procura o nível correto da pirâmide que será utilizado e o novo imageCut
            while (scale <= 0.5 && l < levels_ - 1) {
                scale *= 2;
                l++;
                QPointF center(QPointF(imageCut.center().x() / 2.0,
                                       imageCut.center().y() / 2.0));
                imageCut.setSize(imageCut.size() / 2.0);
                imageCut.moveCenter(center);
            }

            // Troca a imagem pela imagem do nível correto, seleciona o novo corte e recorta.
            img = pyramid_[l];
            rectToCut = imageCut.toRect();
            result = img->copy(rectToCut).scaled(targetSize, Qt::KeepAspectRatioByExpanding,
                                                 useSmoothOut_ ? Qt::SmoothTransformation : Qt::FastTransformation);
        }
    }

    result.setDotsPerMeterX(3780);
    result.setDotsPerMeterY(3780);
    return result;
}

QColor RasterResource::getColor(QPoint at) {
    if (isValid_ && at.x() >= 0
            && at.y() >= 0 && at.x() < width() - 1
            && at.y() < height() - 1) {
        return QColor(pyramid_[0]->pixel(at));
    }

    return QColor();
}

unsigned int RasterResource::getGrayColor(QPointF at, bool linear) {
    unsigned int result = 0;

    if (isValid_ && at.x() >= 0 && at.y() >= 0 && at.x() < width() - 1
            && at.y() < height() - 1) {
        if (linear) {
            // adicionar a parte linear aqui depois.
        } else {
            result = qGray(pyramid_[0]->pixel((int)floor(at.x()), (int)floor(at.y())));
        }
    }

    return result;
}
