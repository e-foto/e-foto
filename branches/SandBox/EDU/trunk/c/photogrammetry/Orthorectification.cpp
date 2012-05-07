#include "Orthorectification.h"

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
