#include "Orthorectification.h"
#include <QRgb>
#include <terralib/kernel/TeProjection.h>
#include <terralib/kernel/TeRaster.h>
#include <terralib/kernel/TeDatum.h>
#include <terralib/kernel/TeInitRasterDecoders.h>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/*
 * The orthoimage has values from 0-1
 **/
Orthorectification::Orthorectification(double _Xi, double _Yi, double _Xf, double _Yf, double _res_x, double _res_y)
{
	// Default values
	color_depth = 8;
	no_bands = 1;
	coord_system = 0;
	spheroid = 0;
	datum = 0;
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
	if (val > 1.0) val - 1.0;

	orthoimage.set(row, col, val);
}

void Orthorectification::getXYAt(int col, int row, double &X, double &Y)
{
	X = res_x * (double(col) - 1.0) + Xi;
	Y = res_y * (double(row) - 1.0) + Yi;
}


/************************
 *                      *
 * Read/Write functions *
 *                      *
 ************************/

// This is an internal function used to debug loading functions
void Orthorectification::printData()
{
	printf("Orthoimage header:\n");
	printf(" Xi: %f\n Yi: %f\n Xf: %f\n Yf: %f\n",Xi,Yi,Xf,Yf);
	printf(" Resolution X: %f\n Resolution Y: %f\n",res_x,res_y);
	printf(" GRID width: %d\n GRID height: %d\n",ortho_width,ortho_height);
	printf(" Image color depth: %d\ Number of bands: %d",color_depth,no_bands);
	printf(" Coordinate system: %d\n Spheroid: %d\n Datum: %d\n",coord_system, spheroid, datum);
	printf("Sample Ortho data:\n");

	int w,h;
	(ortho_width < 10) ? w = ortho_width : w = 10;
	(ortho_height < 10) ? h = ortho_height : h = 10;

	for (unsigned int i=1; i<=h; i++)
	{
		for (unsigned int j=1; j<=w; j++)
			printf("%.2f  ",orthoimage.get(i,j));
		printf("\n");
	}
}

void Orthorectification::saveOrtho(char * filename, int mode)
{
	// Only one option
	saveOrthoEfoto(filename);
}

void Orthorectification::loadOrtho(char * filename, int mode)
{
	// Only one option
	loadOrthoEfoto(filename);
}

void Orthorectification::saveOrthoGeoTiff(char * filename, int mode)
{
    saveOrthoGeoTiffEfoto(filename);
}

void Orthorectification::saveOrthoGeoTiffEfoto(char * filename)
{
    
    TeRasterParams params;
    params.setDummy(0);
    params.boundingBoxLinesColumns(Xi, Yi, Xf, Yf,ortho_height,ortho_width);
    //params.boundingBoxLinesColumns(Xi, Yi, Xf, Yf, ortho_width, ortho_height);//original
    //Falta fazer um método para converter coord_system para um TeProjection
    TeDatum teDatum= TeDatumFactory::make("WGS84");
     std::cout << "--------Entrou no OrthoManager::orthoRectification: "<< Xi << " , "<< Yi << " , " << Xf << " , " << Yf << " , " << ortho_width << " , " << ortho_height << " , " << no_bands << " , " << utmFuse << " , " << filename << std::endl;
    /*
    if (datum==0){
        teDatum = TeDatumFactory::make("SAD69");
    }else if (datum==1){
        teDatum = TeDatumFactory::make("WGS84");
    }else if (datum==2){
        teDatum = TeDatumFactory::make("SIRGAS2000");
    }else{
        teDatum = TeDatumFactory::make("SAD69");
    }
    */
    //TeDatum teDatum = TeDatumFactory::make(datum);//nomeDoDatum: SAD69, WGS84 ou SIRGAS2000

    TeProjectionParams projParams;
    projParams.name = "UTM";
    /*
    if(coord_system==0){
        projParams.name = "UTM";
    }
    */
    //projParams.name = coord_system;//sistema de coordenadas "UTM" - pro enquanto trabalha-se apenas com UTM, mas no futuro serão acrescentados outros sistemas de coordenadas, e portanto, será necessário implementar outras formas de criar uma projeção TerraLib

    projParams.datum = teDatum;
    projParams.hemisphere = TeSOUTH_HEM;//enum: TeSOUTH_HEM ou TeNORTH_HEM
    projParams.lon0 = -utmFuse * TeCDR;//zona é o fuso UTM
    TeProjection* proj = TeProjectionFactory::make(projParams);
    //if(proj == 0) { ABORTAR} //fazer o teste se a projection foi criada com sucesso
    if (proj != 0)
        std::cout << "Criou o proj" << std::endl;
    //criar o atributo utmFuse
    params.projection(proj);
    params.setDataType(TeUNSIGNEDCHAR);
    //params.nBands(no_bands);
    params.nBands(1);
    params.mode_ = 'c';
    params.decoderIdentifier_ = "TIF";
    //params.decoderIdentifier_ = "MEM";
    params.fileName_=filename;
    //params.fileName_="/home/rodrigo/arqGeoTiff.tif";

    TeRaster* rasterMem = new TeRaster(params);
    TeInitRasterDecoders();

    std::cout << "-=00000000000000))))))))--Entrou no OrthoManager::orthoRectification: Criou o rasterMem" << std::endl;
    //orthoimage.show('f',100,"Orto-imagem");


    if(rasterMem->init() == true)
    {
         std::cout << "------------------Entrou no OrthoManager::orthoRectification: Entrou em rasterMem->init()" << std::endl;
        //for(int i = 0; i < ortho_width; i++)//original
        for(int i = 0; i < ortho_height; i++)
        {
            //for(int j = 0; j < ortho_height; j++)//original
            for(int j = 0; j < ortho_width; j++)
            {
                //QRgb cell = orthoimage.get(j ,i);

                //int rVal = qRed(cell);
                //int gVal = qGreen(cell);
                //int bVal = qBlue(cell);
                //rasterMem->setElement(j, i, rVal, gVal, bVal);
                //rasterMem->setElement(j, i, qGray(cell),0);

                //int gVal = qGray(cell);
                //rasterMem->setElement(j, i, gVal);

                //rasterMem->setElement(j, i, (int)(orthoimage.get(j ,i)*255));\original
                rasterMem->setElement(j, ortho_height-i-1, (int)(orthoimage.get(i+1 ,j+1)*255));


                std::cout << "--------\t" << i << " , \t" << j << " , \tDouble:" << (int)(orthoimage.get(i ,j)*255) << " , \t\tUnidade:" << orthoimage.getUnit() << std::endl;
                 //std::cout << "--------: "<< Xi << " , "<< Yi << " , " << Xf << " , " << Yf << " , " << ortho_width << " , " << ortho_height << " , " << no_bands << " , " << utmFuse << " , " << filename << std::endl;
            }
        }
    }else
        std::cout << "=-=-=-=Erro: " << params.errorMessage_ << std::endl;

    delete(rasterMem);
    //FILE *fp;
    //fp = fopen(filename,"wb");
    //fwrite(&header, 1, header_size_bytes, fp);
    //fclose(fp);
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
	datum = int(header[12]);

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

	//	printData();
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

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
