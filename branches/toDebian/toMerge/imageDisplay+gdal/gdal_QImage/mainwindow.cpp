#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QPixmap>
#include <cmath>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    img = nullptr;
    _dataset = nullptr;
}

MainWindow::~MainWindow()
{
    if (img)
        delete img;
    if (_dataset)
        GDALClose(_dataset);
    delete ui;
}


void MainWindow::on_load_clicked()
{
    QString filename = QFileDialog::getOpenFileName(0, "Open Image", ".", "*.bmp *.png *.tif *.ppm *.jpg");
    // Raster resource só é válido se for dado um caminho para imagem não vazio
    if (filename == "")
        return;

    if(ui->useGDAL->isChecked()) {
        GDALDataset* newDataset;

        // Localizar e instanciar todos os drivers da gdal
        GDALAllRegister();

        // Converter caminho para o gdal
        QByteArray _filepath = filename.toLocal8Bit();

        // Abrir a imagem como um dataset gdal
        newDataset = static_cast<GDALDataset *>( GDALOpen(_filepath, GA_ReadOnly) );

        // Testar se é válida
        if (newDataset == nullptr)
        {
                // Fechar o gdal dataset
                GDALClose(newDataset);
                return;
        }

        // Fechar dataset anterior se existir
        if (_dataset)
            GDALClose(_dataset);

        // Atribuir novo dataset
        _dataset = newDataset;

    } else {
        QImage* newImage = new QImage(filename);
        if ( ! newImage->isNull() ){
            if (img)
                delete img;
            img = newImage;
        }
    }
}

void MainWindow::on_show_clicked()
{
    if(ui->useGDAL->isChecked()) {
        auto sw = _dataset->GetRasterXSize();
        auto sh = _dataset->GetRasterYSize();
        auto tw = std::min(sw, ui->width->value());
        auto th = std::min(sh, ui->height->value());

        /* Imagem em tons de cinza
        uchar* data = static_cast< uchar* >( CPLMalloc(sizeof(uchar) * tw * th) );
        GDALRasterBand* gdalBand = _dataset->GetRasterBand(1);
        gdalBand->RasterIO( GF_Read, 0, 0, sw, sh,
                            data, tw, th, GDT_Byte,
                            0, 0 );
        QImage* srcImage = new QImage((const uchar *) data,
                                      tw, th, tw*1, QImage::Format_Grayscale8);
        */

        //* Imagem colorida BIP (INTERLEAVE=PIXEL) => https://lists.osgeo.org/pipermail/gdal-dev/2011-July/029372.html
        uint* data = static_cast< uint* >( CPLMalloc(sizeof(uint) * tw * th) );
        int nViewBands = 3;
        int viewBands[] = {1,2,3};
        _dataset->RasterIO( GF_Read, 0, 0, sw, sh,
                          data, tw, th, GDT_Byte,
                          nViewBands, viewBands,
                          4, tw*4, 1 );
        QImage* srcImage = new QImage((const uchar *) data,
                                      tw, th, tw*4, QImage::Format_RGBX8888);
        //*/

        /* Resolver  BIL e BSQ */
        /* Resolver imagem indexada 8 bits */
        /* Resolver MDS imagem float */

        ui->view->setPixmap(QPixmap::fromImage(*srcImage));

        CPLFree(data);
    } else {
        if (img)
            ui->view->setPixmap(QPixmap::fromImage(img->scaled(ui->width->value(),ui->height->value())));
    }
}

/*

    int gdalWidth;
    int gdalHeight;
    int BytesPerPixel;
    GDALDataType eType;
    GDALDataset *theDataset, *newDataset;
    GDALRasterBand *gdalBand;
    double noData;
    double Transform[6];    //geotransform matrix

    _useSmoothIn = withSmoothIn;
    _useSmoothOut = withSmoothOut;

    // Raster resource só é válido se for dado um caminho para imagem não vazio
    _isValid = false;
    if (filepath == "")
        return;

    // Código antigo
    //QImage* srcImage = new QImage();
    //_isValid = srcImage->load(filepath); //returns true or false
    //_levels = 0;


    // Localizar e instanciar todos os drivers da gdal
    GDALAllRegister();
    // Converter caminho para o gdal
    QByteArray _filepath = filepath.toLocal8Bit();

    // Abrir a imagem como um dataset gdal
    theDataset = static_cast<GDALDataset *>( GDALOpen(_filepath, GA_ReadOnly) ); // Este objeto substituirá QImage** _pyramid; no .h


    // Testar se é válida
    _isValid = (theDataset != nullptr);

    //if (!_isValid || srcImage->width() == 0 || srcImage->height() == 0)
    if (!_isValid)
    {
            //Não fechar gdal com del usar gdal close
            GDALClose(theDataset);
            emitLoadError();
            return;
    }

    gdalHeight = theDataset->GetRasterYSize();
    gdalWidth = theDataset->GetRasterXSize();
    //noData = theDataset->GetRasterBand(1)->GetNoDataValue(); //usar depois
    //theDataset->GetGeoTransform(Transform); //usar depois


    //transformar para um arquivo temporario // Em edição!!!
          //banda
    gdalBand = theDataset->GetRasterBand(1);
    int* data = static_cast< int* >( CPLMalloc(sizeof(int) * gdalWidth * gdalHeight) );
    gdalBand->RasterIO( GF_Read, 0, 0, gdalWidth, gdalHeight,
                      data, gdalWidth, 1, GDT_UInt32,
                      0, 0 );
    eType = gdalBand -> GetRasterDataType();
    BytesPerPixel = GDALGetDataTypeSize(eType)/8;
    //QImage::QImage(const uchar *data, int width, int height, QImage::Format format, QImageCleanupFunction cleanupFunction = nullptr, void *cleanupInfo = nullptr)
    QImage* srcImage = new QImage((const uchar *) data,
                           gdalHeight,
                           gdalWidth,
                           BytesPerPixel,
                          QImage::Format_ARGB32);

 */
