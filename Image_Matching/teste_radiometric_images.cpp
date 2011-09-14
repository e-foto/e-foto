#include <stdio.h>
#include <qimage.h>
#include "Matrix.h"
#include "RadiometricTransformation.h"


Matrix& loadImage(char *filename)
{
	int levels=256;

	QImage img;
	img.load(filename);

	Matrix *I = new Matrix(img.height(), img.width());

	int pixel;
	for (unsigned int i=1; i<=img.height(); i++)
	{
		for (unsigned int j=1; j<=img.width(); j++)
		{
			pixel = img.pixel(j-1,i-1);
			pixel = ((pixel >> 16) & 0xFF)*0.2989 + ((pixel >> 8) & 0xFF)*0.5870 + (pixel & 0xFF)*0.1140;
			I->set(i, j, pixel/double(levels-1));
		}
	}

	return *I;
}

int saveImage(char *filename, Matrix *I)
{
	int levels = 256;

	QImage img(I->getCols(), I->getRows(), 32);

	int pixel;
	for (unsigned int i=1; i<=img.height(); i++)
	{
		for (unsigned int j=1; j<=img.width(); j++)
		{
			pixel = round(I->get(i,j)*double(levels-1));
			pixel = (pixel << 16) + (pixel << 8) + pixel;
			img.setPixel(j-1, i-1, pixel);
		}
	}

	img.save(filename,"BMP");

	return 1;
}


int main(void)
{
	printf("Loading image ...\n");
//	Matrix img1 = loadImage("/home/marts/EFOTO/Dev/data_and_images/1997_016_300dpi.bmp");
//	Matrix img2 = loadImage("/home/marts/EFOTO/Dev/data_and_images/1997_017_300dpi.bmp");
//	Matrix img1 = loadImage("../1997_016_300dpi.bmp");
//	Matrix img2 = loadImage("../1997_017_300dpi.bmp");

	Matrix img1 = loadImage("../Histogram Matching/forest.bmp");
	Matrix img2 = loadImage("../Histogram Matching/cristo.bmp");

	printf("Appling radiometric transformation ...\n");
	RadiometricTransformation rt;
/*
	// Normalization
	rt.imadjust(&img1);
	rt.histeq(&img1);
	printf("Saving ...\n");
	saveImage("teste.bmp",&img1);
*/
	// Histogram matching
	rt.histmatching(&img1,&img2);
	printf("Saving ...\n");
	saveImage("../Histogram Matching/new_cristo.bmp",&img2);

	return 1;
}
