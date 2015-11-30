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

//#include <math.h>
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
    datum(SAD69)
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

void Orthorectification::changeGridResolution(double _res_x, double _res_y)
{
    createNewGrid(Xi, Yi, Xf, Yf, _res_x, _res_y);
}

double Orthorectification::getOrthoimagePixel(double X, double Y)
{
    int col, lin;

    col = int(1.0 + (X-Xi)/res_x);
    lin = int(1.0 + (Y-Yi)/res_y);

    return getOrthoimagePixel(col, lin);
}

// Always remember: values from 0-1
double Orthorectification::getOrthoimagePixel(int col, int row)
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

void Orthorectification::getXYAt(int col, int row, double &X, double &Y)
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

void Orthorectification::getColRowAt(double X, double Y, int &col, int &row)
{
        col = int(1.0 + (X - Xi) / res_x);
        row = int(1.0 + (Y - Yi) / res_y);
}

/************************
 *                      *
 * Read/Write functions *
 *                      *
 ************************/

// This is an internal function used to debug loading functions
void Orthorectification::printData()
{
    std::cout << "Orthoimage header:\n";
    std::cout << " Xi: " << Xi << "\t" << Yi << "\t" << Xf << "\t" << Yf << std::endl;
    std::cout << " Resolution X: " << res_x << std::endl << "Resolution Y: " << res_y << std::endl;
    std::cout << " GRID width: " << ortho_width << std::endl << "GRID height: " << ortho_height <<std::endl;
    std::cout << " Image color depth: "<< color_depth <<" Number of bands: " << no_bands;
    std::cout << " Coordinate system: " << coord_system << std::endl << "Spheroid: " << spheroid << std::endl << " Datum: " << datum << std::endl;
    std::cout << "Sample Ortho data:\n";

    unsigned int w,h;
    (ortho_width < 10) ? w = ortho_width : w = 10;
    (ortho_height < 10) ? h = ortho_height : h = 10;

    for (unsigned int i=1; i<=h; i++)
    {
        for (unsigned int j=1; j<=w; j++)
            std::cout << orthoimage.get(i,j);
        std::cout << std::endl;
    }
}

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

void Orthorectification::saveOrthoEfoto(char * filename)
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

    delete data;
}

void Orthorectification::loadOrthoEfoto(char * filename)
{
    FILE *fp;

    fp = fopen(filename,"rb");

    // Read header
    double header[13];
    unsigned int header_size_bytes = 13 * 8;
    fread(&header, 1, header_size_bytes, fp);

    Xi = header[0];
    Yi = header[1];
    Xf = header[2];
    Yf = header[3];
    res_x = header[4];
    res_x = header[5];
    ortho_width = int(header[6]);
    ortho_height = int(header[7]);
    color_depth = int(header[8]);
    no_bands = int(header[9]);
    coord_system = int(header[10]);
    spheroid = int(header[11]);
    datum = DatumType(header[12]);

    // Read DEM
    orthoimage.resize(ortho_height, ortho_width);
    unsigned int file_size = ortho_width*ortho_height;
    unsigned int file_size_bytes = file_size*8;
    double *data = new double[file_size];
    int p=0;
    fread(data, 1, file_size_bytes, fp);

    for (int i=1; i<=ortho_height; i++)
    {
        for (int j=1; j<=ortho_width; j++)
        {
            orthoimage.set(i, j ,data[p]);
            p++;
        }
    }

    fclose(fp);

    delete data;
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

void Orthorectification::saveOrthoGeoTiffEfoto(char * filename)
{
    GDALAllRegister();

    const char *pszFormat = "GTiff";
    GDALDriver *poDriver;

    poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);

    if( poDriver == NULL )
    {
        exit( 1 );
        //CPLError();
    }

    GDALDataset *poDstDS;
    char **papszOptions = NULL;
    papszOptions = CSLSetNameValue( papszOptions, "TILED", "YES" );
    papszOptions = CSLSetNameValue( papszOptions, "COMPRESS", "PACKBITS" );

    poDstDS = poDriver->Create( filename, ortho_width, ortho_height, no_bands, GDT_Byte,
                                papszOptions );

    if( poDstDS == NULL )
    {
        exit( 1 );
        //CPLError();
        //CPLGetLastErrorMsg();
    }

    double adfGeoTransform[6] = { Xi, res_x, 0, Yi, 0, res_y };
    OGRSpatialReference oSRS;
    char *pszSRS_WKT = NULL;
    GDALRasterBand **poBand = new GDALRasterBand*[no_bands] ;

#ifdef WIN32
    GByte* abyRaster = new GByte[no_bands][ortho_width*ortho_height];
#elif unix
    GByte abyRaster[no_bands][ortho_width*ortho_height];
#endif

    poDstDS->SetGeoTransform( adfGeoTransform );

    //bool sph = (spheroid==1)  ? TRUE : FALSE;
    oSRS.SetUTM( utmFuse, spheroid);//TRUE: hemisfério norte -- FALSE: hemisfério sul

    //well known name accepted by SetWellKnownGeogCS(), such as NAD27, NAD83, WGS84 or WGS72.
    //OGRErr OGRSpatialReference::SetFromUserInput 	( 	const char *  	pszDefinition 	 )  -->>Verificar documentação
    switch (datum) {
    case SAD69:
        //oSRS.SetWellKnownGeogCS( "SAD69" );
        oSRS.importFromEPSG(4618);
        break;
    case WGS84:
        oSRS.SetWellKnownGeogCS( "WGS84" );
        break;
    case SIRGAS2000:
        //oSRS.SetWellKnownGeogCS( "SIRGAS2000" );
        //Scope: Geodetic survey.
        oSRS.importFromEPSG(4989);
        break;
    default:
        oSRS.SetWellKnownGeogCS( "WGS84" );
        break;
    }


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

        for(int i = 0; i < ortho_height; i++)
        {
            for(int j = 0; j < ortho_width; j++)
            {
                tmp=(orthoimage.get(i+1 ,j+1))*0xFFFFFF;
                tmpint = ((int)tmp)&0xFFFFFF;
                abyRaster[2][(ortho_height-i-1)*ortho_width+j]= tmpint&0xFF; //red
                tmpint >>=8;
                abyRaster[1][(ortho_height-i-1)*ortho_width+j]= tmpint&0xFF; //green
                tmpint >>=8;
                abyRaster[0][(ortho_height-i-1)*ortho_width+j]= tmpint&0xFF; //blue
            }
        }

        //Acertar RasterIO(...)
    for(int k=0 ; k < no_bands ; k++)
    {
        //poBand[i]->RasterIO(GF_Write, x, y, blockWidth, blockHeight,abyRaster, blockWidth, blockHeight,(GDALDataType)this->datatpye, 0, 0);
        poBand[k]->RasterIO(GF_Write, 0, 0, ortho_width, ortho_height,abyRaster[k], ortho_width, ortho_height,GDT_Byte, 0, 0);
    }

    if(poDstDS!=NULL)
        GDALClose(poDstDS);

#ifdef WIN32
    delete abyRaster;
#endif
}


void Orthorectification::saveOrthoGeoTiff(char * filename, int mode)
{
    saveOrthoGeoTiffEfoto(filename);
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
