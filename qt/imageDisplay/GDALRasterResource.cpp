#include <cpl_conv.h>
#include <gdal_priv.h>
#include <gdalwarper.h>

#include <QFileInfo>
#include <QMessageBox>
#include <QTemporaryFile>
#include <cmath>

#include "RasterResource.h"

RasterResource::RasterResource(QString filepath, bool withSmoothIn,
                               bool withSmoothOut, bool metadataOnly) {
  GDALAllRegister();  // Registrar todos os drivers GDAL
  _pyramid = nullptr;
  _levels = 0;
  _isValid = false;
  _useSmoothIn = withSmoothIn;
  _useSmoothOut = withSmoothOut;
  if (!filepath
           .isEmpty()) {  // && std::filesystem::exists(filepath.toStdString()))
                          // {
    load(filepath, metadataOnly);
  }
}

RasterResource::~RasterResource() {
  if (!_isValid || !_pyramid) return;

  for (int l = 0; l < _levels; l++) {
    if (_pyramid[l]) {
      GDALClose(_pyramid[l]);
    }
  }
  free(_pyramid);
}

void RasterResource::emitLoadError(const QString& filepath) {
  QString message = "Error: The image loading process failed.";
  if (!filepath.isEmpty()) {
    if (!std::filesystem::exists(filepath.toStdString())) {
      message += " File not found!";
    } else {
      message += " Unable to read this file!";
    }
    // TODO: Alterar essa implementação para garantir que use proj->getSavedIn()
    QFileInfo fileInfo(filepath);
    QString absolutePath = fileInfo.absoluteFilePath();

    message += "\nFile: " + absolutePath;
  }
  QMessageBox::critical(nullptr, "Load Error", message);
}
void RasterResource::useSmoothIn(bool useSmooth) { _useSmoothIn = useSmooth; }

void RasterResource::transformImage(double H[9]) {
  if (_isValid) {
    // TODO: Revisar isso para que não dependa da transformação em QImage
    QImage newImage(gdalDatasetToQImage(_pyramid[0]));
    QTransform h(H[0], H[3], H[6], H[1], H[4], H[7], H[2], H[5], H[8]);
    load(newImage.transformed(h));
  }
}

bool RasterResource::load(QImage image) {
  // Salvar QImage em buffer temporário e carregar com GDAL
  QTemporaryFile tempFile;
  if (!tempFile.open()) {
    emitLoadError("");
    return false;
  }
  if (!image.save(&tempFile, "PNG")) {
    emitLoadError("");
    return false;
  }
  tempFile.close();
  return load(tempFile.fileName());
}

bool RasterResource::load(QString filepath, bool metadataOnly) {
  // Limpar dados existentes
  if (_pyramid) {
    for (int l = 0; l < _levels; l++) {
      if (_pyramid[l]) {
        GDALClose(_pyramid[l]);
      }
    }
    free(_pyramid);
    _pyramid = nullptr;
  }

  _isValid = false;

  // Abrir dataset GDAL
  GDALDataset* srcDataset =
      (GDALDataset*)GDALOpen(filepath.toUtf8().constData(), GA_ReadOnly);
  if (!srcDataset) {
    emitLoadError(filepath);
    return false;
  }

  // Obter dimensões
  _imageDim = QSize(srcDataset->GetRasterXSize(), srcDataset->GetRasterYSize());
  if (_imageDim.width() == 0 || _imageDim.height() == 0) {
    GDALClose(srcDataset);
    emitLoadError(filepath);
    return false;
  }

  // Calcular número de níveis da pirâmide
  _levels = log(_imageDim.width() < _imageDim.height() ? _imageDim.width()
                                                       : _imageDim.height()) /
                log(2) -
            4;
  if (_levels < 1) _levels = 1;

  // Evita o tempo de carregamento da pirâmide em casos de carregamento
  // exclusivo para leitura de metadados
  if (metadataOnly) return true;

  // Alocar array para pirâmide
  _pyramid = (GDALDataset**)calloc(_levels, sizeof(GDALDataset*));
  if (!_pyramid) {
    GDALClose(srcDataset);
    emitLoadError(filepath);
    return false;
  }

  // Primeiro nível é o dataset original
  _pyramid[0] = srcDataset;
  _isValid = true;

  // Construir níveis da pirâmide
  for (int l = 1; l < _levels; l++) {
    _pyramid[l] = createPyramidLevel(_pyramid[l - 1], l);
    if (!_pyramid[l]) {
      _isValid = false;
      break;
    }
  }

  if (!_isValid) {
    for (int l = 0; l < _levels; l++) {
      if (_pyramid[l]) {
        GDALClose(_pyramid[l]);
      }
    }
    free(_pyramid);
    _pyramid = nullptr;
    emitLoadError(filepath);
    return false;
  }

  return true;
}

bool RasterResource::save(QString filepath, QString format) {
  if (!_isValid || !_pyramid || !_pyramid[0]) return false;

  // TODO: Repensar se não é mais seguro salvar a imagem usando o próprio GDAL
  QImage image = gdalDatasetToQImage(_pyramid[0]);
  return image.save(filepath, format.toUtf8().data());
}

bool RasterResource::isValid() { return _isValid; }

int RasterResource::width() { return _imageDim.width(); }

int RasterResource::height() { return _imageDim.height(); }

QSize RasterResource::size() { return _imageDim; }

int RasterResource::levels() { return _levels; }

QPointF RasterResource::center() {
  return QPointF(width() / 2.0, height() / 2.0);
}

QImage RasterResource::getImageCut(QSize targetSize, QRectF imageCut) {
  QImage result;
  if (!_isValid || !_pyramid || !_pyramid[0]) return result;

  // Inicialização de variáveis para o recorte
  QRect rectToCut = imageCut.toRect();
  GDALDataset* dataset = _pyramid[0];
  int l = 0;

  // Calcular escala
  double wscale = targetSize.width() / imageCut.width();
  double hscale = targetSize.height() / imageCut.height();
  double scale = (wscale > hscale) ? wscale : hscale;

  if (scale > 1.0) {
    // Caso de ampliação
    rectToCut.setLeft(rectToCut.left() - 1);
    rectToCut.setTop(rectToCut.top() - 1);
    rectToCut.setWidth(rectToCut.width() + 2);
    rectToCut.setHeight(rectToCut.height() + 2);

    QSize newTargetSize = rectToCut.size() * scale;
    QRect finalCut(
        ((imageCut.topLeft() - QPointF(rectToCut.topLeft())) * scale).toPoint(),
        targetSize);

    // Converter recorte para QImage e redimensionar
    QImage tempImage = gdalDatasetToQImage(dataset, rectToCut);
    result = tempImage
                 .scaled(newTargetSize, Qt::KeepAspectRatioByExpanding,
                         _useSmoothIn ? Qt::SmoothTransformation
                                      : Qt::FastTransformation)
                 .copy(finalCut);
  } else if (scale > 0.5) {
    // Caso de redução moderada
    result = gdalDatasetToQImage(dataset, rectToCut)
                 .scaled(targetSize, Qt::KeepAspectRatioByExpanding,
                         _useSmoothOut ? Qt::SmoothTransformation
                                       : Qt::FastTransformation);
  } else {
    // Caso de redução significativa - usar pirâmide
    while (scale <= 0.5 && l < _levels - 1) {
      scale *= 2;
      l++;
      QPointF center(
          QPointF(imageCut.center().x() / 2.0, imageCut.center().y() / 2.0));
      imageCut.setSize(imageCut.size() / 2.0);
      imageCut.moveCenter(center);
    }

    dataset = _pyramid[l];
    rectToCut = imageCut.toRect();
    result = gdalDatasetToQImage(dataset, rectToCut)
                 .scaled(targetSize, Qt::KeepAspectRatioByExpanding,
                         _useSmoothOut ? Qt::SmoothTransformation
                                       : Qt::FastTransformation);
  }

  return result;
}

QColor RasterResource::getColor(QPoint at) {
  if (!_isValid || !_pyramid || !_pyramid[0] || at.x() < 0 || at.y() < 0 ||
      at.x() >= width() || at.y() >= height())
    return QColor();

  GDALDataset* dataset = _pyramid[0];
  int bandCount = dataset->GetRasterCount();
  uint8_t pixel[4] = {0, 0, 0, 255};  // R, G, B, A

  if (bandCount >= 3) {
    // RGB
    dataset->GetRasterBand(1)->RasterIO(GF_Read, at.x(), at.y(), 1, 1,
                                        &pixel[0], 1, 1, GDT_Byte, 0, 0);
    dataset->GetRasterBand(2)->RasterIO(GF_Read, at.x(), at.y(), 1, 1,
                                        &pixel[1], 1, 1, GDT_Byte, 0, 0);
    dataset->GetRasterBand(3)->RasterIO(GF_Read, at.x(), at.y(), 1, 1,
                                        &pixel[2], 1, 1, GDT_Byte, 0, 0);

    if (bandCount >= 4) {
      dataset->GetRasterBand(4)->RasterIO(GF_Read, at.x(), at.y(), 1, 1,
                                          &pixel[3], 1, 1, GDT_Byte, 0, 0);
    }
  } else if (bandCount == 1) {
    // Tons de cinza
    dataset->GetRasterBand(1)->RasterIO(GF_Read, at.x(), at.y(), 1, 1,
                                        &pixel[0], 1, 1, GDT_Byte, 0, 0);
    pixel[1] = pixel[2] = pixel[0];  // Copiar para G e B
  }

  return QColor(pixel[0], pixel[1], pixel[2], pixel[3]);
}

unsigned int RasterResource::getGrayColor(QPoint at, bool linear) {
  QColor result = getColor(at);
  if (linear)
    return static_cast<unsigned int>(
        0.299 * result.red() + 0.587 * result.green() + 0.114 * result.blue());
  return static_cast<unsigned int>(
      (result.red() + result.green() + result.blue()) / 3.0);
}

QImage RasterResource::gdalDatasetToQImage(GDALDataset* dataset,
                                           const QRect& rect) const {
  if (!dataset) return QImage();

  // Obter dimensões da imagem original
  int imgWidth = dataset->GetRasterXSize();
  int imgHeight = dataset->GetRasterYSize();

  // Determinar área efetiva para cópia (intersecção entre o rect solicitado e a
  // imagem)
  QRect validRect = rect.intersected(QRect(0, 0, imgWidth, imgHeight));

  // Criar QImage do tamanho solicitado, inicializada com zeros (transparente)
  QImage image(rect.width(), rect.height(), QImage::Format_ARGB32);
  image.fill(Qt::transparent);

  // Se não houver área válida para copiar, retornar a imagem vazia
  if (validRect.isEmpty()) {
    return image;
  }

  // Calcular deslocamento na imagem de destino
  int dstX = validRect.x() - rect.x();
  int dstY = validRect.y() - rect.y();

  // Verificar número de bandas
  int bandCount = dataset->GetRasterCount();

  // Criar buffer para os dados da imagem
  if (bandCount >= 3) {
    // RGB ou RGBA - usar buffer ARGB
    std::vector<uint8_t> buffer(validRect.width() * validRect.height() * 4, 0);

    // Ler bandas R, G, B
    CPLErr errR = dataset->GetRasterBand(1)->RasterIO(
        GF_Read, validRect.x(), validRect.y(), validRect.width(),
        validRect.height(), buffer.data() + 2, validRect.width(),
        validRect.height(), GDT_Byte, 4, validRect.width() * 4);
    CPLErr errG = dataset->GetRasterBand(2)->RasterIO(
        GF_Read, validRect.x(), validRect.y(), validRect.width(),
        validRect.height(), buffer.data() + 1, validRect.width(),
        validRect.height(), GDT_Byte, 4, validRect.width() * 4);
    CPLErr errB = dataset->GetRasterBand(3)->RasterIO(
        GF_Read, validRect.x(), validRect.y(), validRect.width(),
        validRect.height(), buffer.data(), validRect.width(),
        validRect.height(), GDT_Byte, 4, validRect.width() * 4);

    // Se tiver banda alpha
    if (bandCount >= 4) {
      CPLErr errA = dataset->GetRasterBand(4)->RasterIO(
          GF_Read, validRect.x(), validRect.y(), validRect.width(),
          validRect.height(), buffer.data() + 3, validRect.width(),
          validRect.height(), GDT_Byte, 4, validRect.width() * 4);
    } else {
      // Preencher alpha com 255 (opaco) para os pixels válidos
      for (int i = 3; i < validRect.width() * validRect.height() * 4; i += 4) {
        buffer[i] = 255;
      }
    }

    // Copiar dados válidos para a QImage na posição correta
    for (int y = 0; y < validRect.height(); y++) {
      const uint8_t* srcLine = buffer.data() + y * validRect.width() * 4;
      uint8_t* dstLine = image.bits() + ((dstY + y) * rect.width() + dstX) * 4;
      memcpy(dstLine, srcLine, validRect.width() * 4);
    }
  } else if (bandCount == 1) {
    // Tons de cinza
    std::vector<uint8_t> buffer(validRect.width() * validRect.height(), 0);
    CPLErr err = dataset->GetRasterBand(1)->RasterIO(
        GF_Read, validRect.x(), validRect.y(), validRect.width(),
        validRect.height(), buffer.data(), validRect.width(),
        validRect.height(), GDT_Byte, 0, 0);

    // Converter para RGB e copiar para a posição correta
    for (int y = 0; y < validRect.height(); y++) {
      for (int x = 0; x < validRect.width(); x++) {
        uint8_t gray = buffer[y * validRect.width() + x];
        image.setPixel(dstX + x, dstY + y, qRgba(gray, gray, gray, 255));
      }
    }
  }

  return image;
}

GDALDataset* RasterResource::createPyramidLevel(GDALDataset* srcDataset,
                                                int level) const {
  if (!srcDataset) return nullptr;

  int srcWidth = srcDataset->GetRasterXSize();
  int srcHeight = srcDataset->GetRasterYSize();
  int dstWidth = srcWidth / 2;
  int dstHeight = srcHeight / 2;

  if (dstWidth <= 0 || dstHeight <= 0) return nullptr;

  // Criar dataset de destino
  GDALDriver* driver = GetGDALDriverManager()->GetDriverByName("MEM");
  GDALDataset* dstDataset = driver->Create(
      "", dstWidth, dstHeight, srcDataset->GetRasterCount(), GDT_Byte, nullptr);

  if (!dstDataset) return nullptr;

  // Configurar transformação geográfica (se aplicável)
  double geoTransform[6];
  if (srcDataset->GetGeoTransform(geoTransform) == CE_None) {
    geoTransform[1] *= 2.0;  // Ajustar escala X
    geoTransform[5] *= 2.0;  // Ajustar escala Y
    dstDataset->SetGeoTransform(geoTransform);
  }

  // Copiar metadados
  dstDataset->SetMetadata(srcDataset->GetMetadata());

  // Reduzir cada banda
  for (int i = 1; i <= srcDataset->GetRasterCount(); i++) {
    GDALRasterBand* srcBand = srcDataset->GetRasterBand(i);
    GDALRasterBand* dstBand = dstDataset->GetRasterBand(i);

    // Configurar banda de destino
    dstBand->SetColorInterpretation(srcBand->GetColorInterpretation());

    // Alocar buffer para dados da banda
    std::vector<uint8_t> srcBuffer(srcWidth * srcHeight);
    std::vector<uint8_t> dstBuffer(dstWidth * dstHeight);

    // Ler dados da banda de origem
    CPLErr err =
        srcBand->RasterIO(GF_Read, 0, 0, srcWidth, srcHeight, srcBuffer.data(),
                          srcWidth, srcHeight, GDT_Byte, 0, 0);

    if (err != CE_None) continue;

    // Reduzir imagem (média 2x2)
    for (int y = 0; y < dstHeight; y++) {
      for (int x = 0; x < dstWidth; x++) {
        int srcX = x * 2;
        int srcY = y * 2;

        // Calcular média dos 4 pixels
        int sum = 0;
        int count = 0;

        for (int dy = 0; dy < 2 && srcY + dy < srcHeight; dy++) {
          for (int dx = 0; dx < 2 && srcX + dx < srcWidth; dx++) {
            sum += srcBuffer[(srcY + dy) * srcWidth + (srcX + dx)];
            count++;
          }
        }

        dstBuffer[y * dstWidth + x] = count > 0 ? sum / count : 0;
      }
    }

    // Escrever dados na banda de destino
    dstBand->RasterIO(GF_Write, 0, 0, dstWidth, dstHeight, dstBuffer.data(),
                      dstWidth, dstHeight, GDT_Byte, 0, 0);
  }

  return dstDataset;
}
