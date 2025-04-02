#include "RasterResource.h"

#include <cmath>
#include <QMessageBox>
#include <QDebug>

RasterResource::RasterResource(QString filepath, bool withSmoothIn,
                               bool withSmoothOut):_useSmoothIn(withSmoothIn),
                                                   _useSmoothOut(withSmoothOut),
                                                   _dataset(nullptr),
                                                   _nViewBands(0)
{
    // Tenta carregar a imagem e registra se o recurso é válido
    _isValid = load(filepath);
}

RasterResource::~RasterResource()
{
    if (_dataset)
        GDALClose(_dataset);
    if (_nViewBands)
        delete[] _viewBands;
}

void RasterResource::emitLoadError()
{
    QMessageBox* msgBox = new QMessageBox();
    msgBox->setText("Error: The image loading process.");
    msgBox->exec();
}

void RasterResource::useSmoothIn(bool useSmooth)
{
    _useSmoothIn = useSmooth;
}

void RasterResource::transformImage(double H[9])
{
    /* Revisar
    if (_isValid) {
        QImage newImage(*_pyramid[0]);
        QTransform h(H[0],H[3],H[6],H[1],H[4],H[7],H[2],H[5],H[8]);
        //qDebug("\n%f %f %f\n%f %f %f\n%f %f %f",H[0],H[3],H[6],H[1],H[4],H[7],H[2],H[5],H[8]);
        load(newImage.transformed(h));
    }
    */
}
bool RasterResource::load(QImage image)
{
    /*

     Revisar
    // Impede carga de imagem nula
    if (image.isNull())
    {
        emitLoadError();
        return false;
    }

    // Remove imagem pré-existente
    if (_isValid)
    {
        for(int l = 0; l < _levels; l++)
            delete(_pyramid[l]);
        free(_pyramid);
    }
    _isValid = true;

    // Calcular o numero de niveis da piramide
    _imageDim = image.size();
    _levels =  log(_imageDim.width() < _imageDim.height() ? _imageDim.width() : _imageDim.height() ) / log(2) -4;
    if (_levels < 0) _levels = 0;

    // Alocando espaço para a piramide
    _pyramid = (QImage**) calloc(_levels, sizeof(QImage*));

    // Atribui a imagem original ao primeiro nível
    _pyramid[0] = new QImage(image.convertToFormat(QImage::Format_ARGB32));
    if (_pyramid[0]->width() == 0 || _pyramid[0]->height() == 0)
    {
        _isValid = false;
        delete(_pyramid[0]);
        free(_pyramid);
        emitLoadError();
        return false;
    }

    // Construindo imagens
    for(int l = 1; l < _levels; l++)
    {
        // Cada imagem do novo nível é igual ao nível anterior reduzida pela metade
        int w = _pyramid[l-1]->width()/2;
        int h = _pyramid[l-1]->height()/2;
        _pyramid[l] = new QImage(_pyramid[l-1]->scaled(w,h,Qt::IgnoreAspectRatio, _useSmoothOut ? Qt::SmoothTransformation : Qt::FastTransformation));

        if (_pyramid[l]->width() == 0 || _pyramid[l]->height() == 0)
        {
            _isValid = false;
            for (int k = l; l >= 0; l--)
                delete(_pyramid[k]);
            free(_pyramid);
            emitLoadError();
            return false;
        }
    }
    return _isValid;
*/
}

bool RasterResource::load(QString filename)
{
    GDALDataset* newDataset;
    // Raster resource só é válido se for dado um caminho para imagem não vazio
    if (filename == ""){
        return false;
       };


    // Localizar e instanciar todos os drivers da gdal
    GDALAllRegister();

    // Converter caminho para o gdal
    QByteArray filepath = filename.toLocal8Bit();

    // Abrir a imagem como um dataset gdal
    newDataset = static_cast<GDALDataset *>( GDALOpen(filepath, GA_ReadOnly) );

    // Testar se é válida
    if (newDataset == nullptr)
    {
            // Fechar o gdal dataset
            GDALClose(newDataset);
            return false;
    }

    // Fechar dataset anterior se existir
    if (_dataset)
        GDALClose(_dataset);

    // Atribuir novo dataset
    _dataset = newDataset;

    // Recuperar dimensões
    _imageDim = QSize(_dataset->GetRasterXSize(),_dataset->GetRasterYSize());

    // Recuperar numero de bandas e determinar tipo de imagem
    // Esta bloco do código encontra-se em aperfeiçoamento em 2018-12-19.
    _nBands = _dataset->GetRasterCount();
    if (_nViewBands)
        delete[] _viewBands;
    _nViewBands = 0;
    // A menos que tenhamos uma imagem de DEM (float), pan (16 bits por canal) ou indexada o tipo básico de retorno de getImageCut é:
    _qtFormat = QImage::Format_RGBA8888;
    if (_nBands == 1) {
        GDALRasterBand* band = _dataset->GetRasterBand(1);
        GDALColorTable* cTable = band->GetColorTable();
        if (cTable == nullptr) {
            ///qDebug() << "We have a grayscale image here!";
            // Checar o tipo de pixel da imagem pode ser aprofundado neste caso,
            // pois podemos ter um DEM ou pancromática 16 bits em mãos.
            _nViewBands = 3;
            _viewBands = new int[3]{1,1,1};
        } else {
            ///qDebug() << "We have a paletted band here!";
            // cTable pode ser traduzido para um QVector<QRgb>, e podemos usar setColorTable na QImage ao entregar recortes.
            // O número de cores em cTable pode ser consultado com GetColorEntryCount()
            // Uma cor em cTable pode ser consultada com GetColorEntry() ou GetColorEntryAsRGB()
            // Um GDALColorEntry tem atributos c1, c2, c3 e c4 para r, g, b e a.
            // O tipo de uma QImage neste caso é QImage::Format_Indexed8
            auto nColors = cTable->GetColorEntryCount();
            _cTable.resize(nColors);
            qDebug() << "Total de cores = " << nColors;
            for (auto i = 0; i < nColors; i++) {
                auto color = cTable->GetColorEntry(i);
                _cTable[i] = qRgba(color->c1,color->c2,color->c3,color->c4);

            }
            _nViewBands = 1;
            _viewBands = new int[1]{1};
            _qtFormat = QImage::Format_Indexed8;
        }
    } else {
        // Checar o tipo da imagem pode ser aprofundado neste caso, pois podemos ter uma orthoimagem em mãos.
        // A nomeação de bandas por canal pode ser melhorada pelo uso de uma checagem sobre qual banda representa qual canal
        _nViewBands = 3;
        _viewBands = new int[3];
        _viewBands[0] = 1; // red
        _viewBands[1] = 2; // green
        _viewBands[2] = 3; // blue
    }

    // Verificando a transformação afim associada ao Raster
    //double transform[6];
    //_dataset->GetGeoTransform(transform);
    //qDebug() << "Xp = " << transform[0] << " + P*" << transform[1] << " + L*" << transform[2];
    //qDebug() << "Yp = " << transform[3] << " + P*" << transform[4] << " + L*" << transform[5];

    return _isValid = true;
}

bool RasterResource::save(QString filepath, QString format)
{
    /* Repensar os métodos de salvar imagens para contemplar imagens, ortoimagens e DSMs
    if (_pyramid && _pyramid[0]->save(filepath,format.toLocal8Bit().constData()))
        return true;
    return false;
    */
}

bool RasterResource::isValid()
{
    return _isValid;
}

int RasterResource::width()
{
    return _imageDim.width();
}

int RasterResource::height()
{
    return _imageDim.height();
}

QSize RasterResource::size()
{
    return _imageDim;
}

/* Repensar toda a questão de montagem e consulta da piramide de imagem
int RasterResource::levels()
{
    return _levels;
}
*/

QPointF RasterResource::center()
{
    return QPointF(width()/2.0,height()/2.0);
}

QImage RasterResource::getImageCut(QSize targetSize, QRectF imageCut)
{
    QImage result;
    if (!_isValid)
        return result;

    // Inicialização de variáveis para o recorte na imagem do topo da pilha
    QRect rectToCut = imageCut.toRect();
    QSize newTargetSize = targetSize;

    // Na prática, qualquer corte deveria possuir a mesma escala em width e height
    // mas isso não ocorre para o recorte de Overview. Por isso recuperamos a escala assim:
    double wscale = targetSize.width() / imageCut.width();
    double hscale = targetSize.height() / imageCut.height();
    double scale = (wscale > hscale) ? wscale : hscale;

    //qDebug() << "wscale: "<< wscale;
    //qDebug() << "hscale: "<< hscale;
    //qDebug() << "scale: "<< scale;

    // Argumentos de reamostragem para o recorte com a gdal
    GDALRasterIOExtraArg extraArg;
    INIT_RASTERIO_EXTRA_ARG(extraArg);

    // Seleciona a imagem correta a recortar, faz o recorte e ajustes para perfeito encaixe do centro do recorte na imagem com a imagem resultante
    if (scale > 1.0)
    {
        // Este caso precisa ser tratado com atenção.
        // Precisamos manter o aspécto da imagem, o zoom correto e o ponto central do recorte corretamente alinhado

        // Primeiro evitamos problemas com o espaço recortado aumentando ligeiramente a área de recorte
        //Andando com a borda superior esquerda uma vez para esquerda e para cima
        rectToCut.setLeft(rectToCut.left()-1);
        rectToCut.setTop(rectToCut.top()-1);
        //Andando com a borda inferior direita uma vez para baixo e para direita, é necessário compensar o paço anterior
        rectToCut.setWidth(rectToCut.width()+2);
        rectToCut.setHeight(rectToCut.height()+2);

        // Criamos um newTargetSize para a nova escala com o aspécto do primeiro recorte
        newTargetSize = rectToCut.size() * scale;

        // Definimos o algoritmo de reamostragem
        if (_useSmoothIn) {
            extraArg.eResampleAlg = GRIORA_Lanczos;
        } else {
            extraArg.eResampleAlg = GRIORA_NearestNeighbour;
        }
    }
    else
    {
        extraArg.eResampleAlg = GRIORA_Bilinear;
    }
    // Em bandas indexadas não há como usar outros algoritmos de resampling
    if (_qtFormat == QImage::Format_Indexed8)
        extraArg.eResampleAlg = GRIORA_NearestNeighbour;

    // Getting source values to cut the image with gdal library
    auto sl = rectToCut.left(); // source left
    auto st = rectToCut.top(); // source top
    auto sw = rectToCut.width(); // source width
    auto sh = rectToCut.height(); // source height

    // Overflow from image border on cut
    auto rborder = sl + sw - _imageDim.width(); rborder = rborder > 0 ? rborder : 0;
    auto bborder = st + sh - _imageDim.height(); bborder = bborder > 0 ? bborder : 0;
    auto lborder = sl < 0 ? (-sl) : 0;
    auto tborder = st < 0 ? (-st) : 0;

    // source correction
    sl += lborder;
    st += tborder;
    sw -= rborder + lborder;
    sh -= bborder + tborder;

    //qDebug() << "sh: "<< sh;
    //qDebug() << "sw: "<< sw;
    //qDebug() << "st: "<< st;
    //qDebug() << "sl: "<< sl;

    // target size of the widget to paint
    auto tw = newTargetSize.width();  // target width
    auto th = newTargetSize.height(); // target height

    // target size update
    tw -= scale*(lborder + rborder);
    th -= scale*(tborder + bborder);

    //qDebug() << "tw: "<< tw;
    //qDebug() << "th: "<< th;

    // Imagem colorida BIP (INTERLEAVE=PIXEL) => https://lists.osgeo.org/pipermail/gdal-dev/2011-July/029372.html
    QImage* tmp = new QImage(QSize(tw,th), _qtFormat);
    if (tmp->isNull())
        return result;
    tmp->fill(Qt::white);
    _dataset->RasterIO( GF_Read, sl, st, sw, sh,
                      tmp->bits(), tw, th, GDT_Byte,
                      _nViewBands, _viewBands,
                      tmp->depth()/8, tmp->bytesPerLine(), 1,
                      &extraArg);

    if (_qtFormat == QImage::Format_Indexed8) {
        tmp->setColorTable(_cTable);
        *tmp = tmp->convertToFormat(QImage::Format_RGBA8888);
    }

    /* Questões conhecidas a serem revisadas */
    /* Resolver imagem indexada 8 bits (estamos quase lá) */
    /* Resolver MDS imagem float */
    /* Resolver pan imagem 16 bits por canal */
    /* Talvez seja necessário resolver entrelaçamentos do tipo BIL e BSQ */

    // Gera cópia para retorno no tamanho da tela com bordas transparentes se necessário
    QPoint topLeft(-scale*lborder,-scale*tborder);
    QRect toTargetSize(topLeft,newTargetSize);
    result = tmp->copy(toTargetSize);

    // Nos casos de ampliação
    if (scale > 1) {
        // Ajustamos o recorte ao tamanho alvo para garantir a correta posição de centro do recorte (em sub pixels)
        result = result.copy(QRect(((imageCut.topLeft() - QPointF(rectToCut.topLeft())) * scale).toPoint(), targetSize));
    }

    // Libera buffer e destroi imagem temporária
    delete tmp;

    return result;
}

QColor RasterResource::getColor(QPoint at)
{
    /* Revisar
    if (_isValid && at.x() >= 0 && at.y() >= 0 && at.x() < width() -1 && at.y() < height() - 1)
        return QColor(_pyramid[0]->pixel(at));
    else
        return QColor();
    */
}

unsigned int RasterResource::getGrayColor(QPointF at, bool linear)
{
    /* Revisar
    unsigned int result = 0;
    if (_isValid && at.x() >= 0 && at.y() >= 0 && at.x() < width() -1 && at.y() < height() - 1)
    {
        if (linear)
        {
            // adicionar a parte linear aqui depois.
        }
        else result = qGray(_pyramid[0]->pixel((int)floor(at.x()),(int)floor(at.y())));
    }
    return result;
    */
}
