/*Copyright 2002-2014 e-foto team (UERJ)
  This file is part of e-foto.

    e-foto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    e-foto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with e-foto.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Orthorectification.h"

#include <iostream>
#include <fstream>
#include <gdal_priv.h>
#include <ogr_spatialref.h>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/*
 * The orthoimage has values from 0-1
 **/
Orthorectification::Orthorectification(double _Xi,
                                       double _Yi,
                                       double _Xf,
                                       double _Yf,
                                       double _res_x,
                                       double _res_y):
    color_depth(8),
    no_bands(1),
    coord_system(0),
    spheroid(0),
    datum(WGS84)
{
    createNewGrid(_Xi, _Yi, _Xf, _Yf, _res_x, _res_y);
}

void Orthorectification::createNewGrid(double _Xi, double _Yi, double _Xf, double _Yf, double _res_x, double _res_y)
{
    // Update data
    Xi = _Xi;
    Yi = _Yi;
    Xf = _Xf;
    Yf = _Yf;
    res_x = _res_x;
    res_y = _res_y;

    // Calculate DEM size
    ortho_width = int(1.0 + floor((Xf - Xi) / res_x));
    ortho_height = int(1.0 + floor((Yf - Yi) / res_y));

    // Create DEM
    orthoimage.resize(ortho_height, ortho_width);
}

// Always remember: values from 0-1
double Orthorectification::getOrthoimagePixel(int col, int row) const
{
    if (col < 1 || row < 1 || col > ortho_width || row > ortho_height)
        return -1.0;

    return orthoimage.get(row, col);
}

void Orthorectification::setOrthoimagePixel(double X, double Y, double val)
{
    int col, lin;

    col = int(1.0 + (X-Xi)/res_x);
    lin = int(1.0 + (Y-Yi)/res_y);

    setOrthoimagePixel(col, lin, val);
}

// Always remember: values from 0-1
void Orthorectification::setOrthoimagePixel(int col, int row, double val)
{
    if (col < 1 || row < 1 || col > ortho_width || row > ortho_height)
        return;

    if (val < 0.0) val = 0.0;
    if (val > 1.0) val = 1.0;

    orthoimage.set(row, col, val);
}

void Orthorectification::getXYAt(int col, int row, double &X, double &Y) const
{
    X = res_x * (double(col) - 1.0) + Xi;
    Y = res_y * (double(row) - 1.0) + Yi;
}

void Orthorectification::getXYAt(double col, double row, double &X, double &Y)
{
        X = res_x * (double(col) - 1.0) + Xi;
        Y = res_y * (double(row) - 1.0) + Yi;
}

void Orthorectification::getColRowAt(double X, double Y, double &col, double &row)
{
        col = 1.0 + (X - Xi) / res_x;
        row = 1.0 + (Y - Yi) / res_y;
}

void Orthorectification::getColRowAt(double X, double Y, int &col, int &row) const
{
        col = int(1.0 + (X - Xi) / res_x);
        row = int(1.0 + (Y - Yi) / res_y);
}

/************************
 *                      *
 * Read/Write functions *
 *                      *
 ************************/


void Orthorectification::saveOrtho(char * filename/*, int mode*/)
{
    // Only one option
    saveOrthoEfoto(filename);
}

void Orthorectification::loadOrtho(char * filename/*, int mode*/)
{
    // Only one option
    loadOrthoEfoto(filename);
}

void Orthorectification::saveOrthoEfoto(char * filename) const
{
    FILE *fp;

    fp = fopen(filename,"wb");

    // Write header
    double header[13];
    unsigned int header_size_bytes = 13 * 8;
    header[0] = Xi;
    header[1] = Yi;
    header[2] = Xf;
    header[3] = Yf;
    header[4] = res_x;
    header[5] = res_y;
    header[6] = double(ortho_width);
    header[7] = double(ortho_height);
    header[8] = double(color_depth);
    header[9] = double(no_bands);
    header[10] = double(coord_system);
    header[11] = double(spheroid);
    header[12] = double(datum);

    fwrite(&header, 1, header_size_bytes, fp);

    // Write DEM
    int p=0;
    unsigned int dem_size = ortho_width*ortho_height;
    unsigned int dem_size_bytes = dem_size * 8;
    double *data = new double[dem_size];

    for (int i=1; i<=ortho_height; i++)
    {
        for (int j=1; j<=ortho_width; j++)
        {
            data[p] = orthoimage.get(i,j);
            p++;
        }
    }

    fwrite(data, 1, dem_size_bytes, fp);

    fclose(fp);

    delete[] data;
}

void Orthorectification::loadOrthoEfoto(char * filename)
{
    FILE *fp;

    fp = fopen(filename,"rb");

    // Read header
    double header[13];
    unsigned int header_size_bytes = 13 * 8;
    unsigned int result = fread(&header, 1, header_size_bytes, fp);
    if (result != header_size_bytes) {
		throw std::runtime_error( "Invalid header" );
	}
	
    Xi = header[0];
    Yi = header[1];
    Xf = header[2];
    Yf = header[3];
    res_x = header[4];
    res_y = header[5];
    ortho_width = int(header[6]);
    ortho_height = int(header[7]);
    color_depth = int(header[8]);
    no_bands = int(header[9]);
    coord_system = int(header[10]);
    spheroid = int(header[11]);
    datum = DatumType(int(header[12]));

    // Read DEM
    orthoimage.resize(ortho_height, ortho_width);
    unsigned int file_size = ortho_width*ortho_height;
    unsigned int file_size_bytes = file_size*8;
    std::unique_ptr<double[]> data (new double[file_size]);
    int p=0;
    result = fread(data.get(), 1, file_size_bytes, fp);
    if (result != header_size_bytes) {
		throw std::runtime_error( "Invalid DEM" );
	}

    for (int i=1; i<=ortho_height; i++)
    {
        for (int j=1; j<=ortho_width; j++)
        {
            orthoimage.set(i, j ,data[p]);
            p++;
        }
    }

    fclose(fp);
}

/*
 * Return ortho-image
 **/

Matrix * Orthorectification::getOrthoImage()
{
    Matrix *img = new Matrix(ortho_height, ortho_width);

    // Invert image
    for (int i=1; i<= ortho_height; i++)
    {
        for (int j=1; j<= ortho_width; j++)
            img->set(i, j, orthoimage.get(ortho_height-i+1, j));
    }

    return img;
}

/**
 * Wrapping your function call with ignore_result makes it more clear to
 * readers, compilers and linters that you are, in fact, ignoring the
 * function's return value on purpose.
 * https://stackoverflow.com/a/30099727
 */
static inline void ignore_result(long long int unused_result) {
    (void) unused_result;
}


void Orthorectification::saveOrthoGeoTiffEfoto(char * filename) const
{
    GDALAllRegister();

    const char *pszFormat = "GTiff";
    GDALDriver *poDriver;

    poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);

    if( poDriver == nullptr )
    {
        exit( 1 );
        //CPLError();
    }

    GDALDataset *poDstDS;
    char **papszOptions = nullptr;
    papszOptions = CSLSetNameValue( papszOptions, "TILED", "YES" );
    //papszOptions = CSLSetNameValue( papszOptions, "COMPRESS", "PACKBITS" );

    poDstDS = poDriver->Create( filename, ortho_width, ortho_height, no_bands, GDT_Byte,
                                papszOptions );

    if( poDstDS == nullptr )
    {
        exit( 1 );
        //CPLError();
        //CPLGetLastErrorMsg();
    }

    // O sinal eixo Y precisa ser invertido quando gravamos o Geotiff
    double adfGeoTransform[6] = { Xi, res_x, 0, Yf, 0, -res_y };
    char *pszSRS_WKT = nullptr;
    GDALRasterBand **poBand = new GDALRasterBand*[no_bands] ;

    std::unique_ptr<GByte[]> abyRasterRed (new GByte[ortho_width*ortho_height]);
    std::unique_ptr<GByte[]> abyRasterGreen (new GByte[ortho_width*ortho_height]);
    std::unique_ptr<GByte[]> abyRasterBlue (new GByte[ortho_width*ortho_height]);

    poDstDS->SetGeoTransform( adfGeoTransform );

    // Definindo o sistema de referência espacial
    // Caution!
    // At this time the above order is important in order to create a valid definition, but in the future the object will automatically reorder the internal representation as needed to remain valid.
    OGRSpatialReference oSRS;
    char hemisphere = spheroid?'N':'S'; //The OGR convention
    char proj[50];
    switch (datum) {

    case WGS84:
        sprintf( proj, "WGS84 / UTM zone %d%c.", utmFuse, hemisphere);
        oSRS.SetProjCS( (const char*) proj );
        oSRS.SetWellKnownGeogCS( "WGS84" );
        break;
    case SIRGAS2000:
        sprintf( proj, "SIRGAS 2000 / UTM zone %d%c.", utmFuse, hemisphere);
        oSRS.SetProjCS( (const char*) proj );
        oSRS.SetWellKnownGeogCS( "GRS70" );
        break;
    case SAD69:
        sprintf( proj, "SAD69 / UTM zone %d%c.", utmFuse, hemisphere);
        oSRS.SetProjCS( (const char*) proj );
        oSRS.SetWellKnownGeogCS( "GRS67" );
        break;
    default:
        sprintf( proj, "CORREGO ALEGRE 1961 / UTM zone %d%c.", utmFuse, hemisphere);
        oSRS.SetProjCS( (const char*) proj );
        oSRS.SetWellKnownGeogCS( "INTL" );
        break;
    }

    oSRS.SetUTM( utmFuse, spheroid);

    oSRS.exportToWkt( &pszSRS_WKT );
    poDstDS->SetProjection( pszSRS_WKT );
    CPLFree( pszSRS_WKT );
    double tmp =0.0;
    int tmpint =0;

    for(int i=0 ; i < no_bands ; i++)
    {
        poBand[i] = poDstDS->GetRasterBand(i+1);
    }


/*
 * A chamada de RasterIO espera os seguintes argumentos.
 *
 * CPLErr GDALRasterBand::RasterIO( GDALRWFlag eRWFlag,
 *                                int nXOff, int nYOff, int nXSize, int nYSize,
 *                                void * pData, int nBufXSize, int nBufYSize,
 *                                GDALDataType eBufType,
 *                                int nPixelSpace,
 *                                int nLineSpace )
 *
 * Note que a mesma chamada de RasterIO() poderá ler, ou gravar dependendo do valor de
 * eRWFlag (GF_Read ou GF_Write). Os argumentos nXOff, nYOff, nXSize, nYSize descreve a
 * janela de dados da raster para ler (ou para gravar). Não necessita ser coincidente com
 * os limites da image embora o acesso pode ser mais eficiente se for.
 *
 * O pData é o buffer de memória para os dados que serão lidos ou gravados. O verdadeiro tipo
 * de dado é aquele passado por eBufType, tal como GDT_Float32, ou GDT_Byte. A chamada de
 * RasterIO() cuidará de converter entre o tipo de dados do buffer e o tipo de dados da banda.
 * Anotar que ao converter dados do ponto flutuando para o inteiro RasterIO arredonda para baixo,
 * e ao converter de para fora dos limites de valores válidos para o tipo de saída, será
 * escolhido o mais próximo valor possível. Isto implica, por exemplo, que os dados 16bit
 * lidos em um buffer de GDT_Byte converterão todos os valores maiores de 255 para 255,
 * os dados não estão escalados!
 *
 * Os valores nBufXSize e nBufYSize descrevem o tamanho do buffer. Ao carregar dados na resolução
 * completa os valores seria o mesmo que o tamanho da janela. Entretanto, para carregar uma
 * vista de solução reduzida (overview) os valores podiam ser ajustado para menos do que a
 * janela no arquivo. Neste caso o RasterIO() utilizará overview para fazer mais eficientemente
 * o IO se as overview forem apropriadas.
 *
 * O nPixelSpace, e o nLineSpace são normalmente zero indicando que os valores default devem
 * ser usados. Entretanto, podem ser usados controlar o acesso à dados da memória, permitindo
 * a leitura em um buffer que contem dados intercalados (interleave) pixel por exemplo.
 */

#define X_Y(i,j)(j +(ortho_height-1-i)*ortho_width) // Mapeamento com inversão do eixo Y

    for(int i = 0; i < ortho_height; i++)
    {
        for(int j = 0; j < ortho_width; j++)
        {
            tmp=(orthoimage.get(i+1 ,j+1))*0xFFFFFF;
            tmpint = ((int)tmp)&0xFFFFFF;
            abyRasterRed[X_Y(i,j)]= tmpint&0xFF; //red
            tmpint >>=8;
            abyRasterGreen[X_Y(i,j)]= tmpint&0xFF; //green
            tmpint >>=8;
            abyRasterBlue[X_Y(i,j)]= tmpint&0xFF; //blue
        }
    }

        //Acertar RasterIO(...)
    ignore_result(poBand[0]->RasterIO(GF_Write, 0, 0, ortho_width, ortho_height,abyRasterBlue.get(), ortho_width, ortho_height,GDT_Byte, 0, 0));
    ignore_result(poBand[1]->RasterIO(GF_Write, 0, 0, ortho_width, ortho_height,abyRasterGreen.get(), ortho_width, ortho_height,GDT_Byte, 0, 0));
    ignore_result(poBand[2]->RasterIO(GF_Write, 0, 0, ortho_width, ortho_height,abyRasterRed.get(), ortho_width, ortho_height,GDT_Byte, 0, 0));

    if(poDstDS!=NULL)
        GDALClose(poDstDS);
}


void Orthorectification::saveOrthoGeoTiff(char * filename)
{
    saveOrthoGeoTiffEfoto(filename);
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
