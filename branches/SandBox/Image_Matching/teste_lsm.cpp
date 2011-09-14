#include <stdio.h>
#include <qimage.h>
#include "Matrix.h"
#include "LeastSquaresMatching.h"


Matrix& loadImage(char *filename)
{
	int levels=256;

	QImage img;
	img.load(filename);

	Matrix *I = new Matrix(img.height(), img.width());

	int pixel;
	for (int i=1; i<=img.height(); i++)
	{
		for (int j=1; j<=img.width(); j++)
		{
			pixel = img.pixel(j-1,i-1);
//			pixel = ((pixel >> 16) & 0xFF)*0.2989 + ((pixel >> 8) & 0xFF)*0.5870 + (pixel & 0xFF)*0.1140;
			pixel = (pixel & 0xFF);
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
	for (int i=1; i<=img.height(); i++)
	{
		for (int j=1; j<=img.width(); j++)
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

	printf("Loading image left ...\n");
	Matrix img1 = loadImage("/home/marts/EFOTO/Dev/data_and_images/1997_016_300dpi.bmp");
	printf("Loading image right ...\n");
	Matrix img2 = loadImage("/home/marts/EFOTO/Dev/data_and_images/1997_017_300dpi_c.bmp");

	printf("Performing LSM matching ...\n");
	LeastSquaresMatching lsm;
	printf("Calculating gradients ...\n");
//	lsm.setGradientFilter(LeastSquaresMatching::Gradient);
//	lsm.imgGradient(&img2);
	printf("Searching ...\n");
	lsm.setTemplate(10);

//	int i = lsm.searchHomologous(&img1, &img2, 1800, 1063, 768, 1042);
//	int i = lsm.searchHomologous(&img1, &img2, 1802, 1329, 817, 1309);
//	int i = lsm.searchHomologous(&img1, &img2, 2156, 1340, 1173, 1316);
	int i = lsm.searchHomologous(&img1, &img2, 1853, 1326, 867, 1308);
	printf("Sucess: %d\n",i);
	printf("Homologous points at: %f x %f\n",lsm.getBestX(),lsm.getBestY());
	printf("Quality: %f\n",lsm.getBestP());

	// Next points
	double next_x, next_y;
	double step_x = 3, step_y = 3;
	next_x = lsm.getBestX();
	next_y = lsm.getBestY();
	lsm.getNextPosition(&next_x, &next_y, step_x, 0);
	printf("Next position: %f,%f\n",next_x,next_y);
	next_x = lsm.getBestX();
	next_y = lsm.getBestY();
	lsm.getNextPosition(&next_x, &next_y, -step_x, 0);
	printf("Next position: %f,%f\n",next_x,next_y);
	next_x = lsm.getBestX();
	next_y = lsm.getBestY();
	lsm.getNextPosition(&next_x, &next_y, 0, step_y);
	printf("Next position: %f,%f\n",next_x,next_y);
	next_x = lsm.getBestX();
	next_y = lsm.getBestY();
	lsm.getNextPosition(&next_x, &next_y, 0, -step_y);
	printf("Next position: %f,%f\n",next_x,next_y);	

//	saveImage("saida.bmp",&G);

/*
	// Test with matrices
	Matrix img1(5,5);
	img1.set(1,1, 6); img1.set(1,2,55); img1.set(1,3, 7); img1.set(1,4,24); img1.set(1,5,86);
	img1.set(2,1,64); img1.set(2,2,74); img1.set(2,3,45); img1.set(2,4,18); img1.set(2,5,49);
	img1.set(3,1,97); img1.set(3,2,20); img1.set(3,3, 5); img1.set(3,4,49); img1.set(3,5,90);
	img1.set(4,1,60); img1.set(4,2,11); img1.set(4,3,96); img1.set(4,4,20); img1.set(4,5,28);
	img1.set(5,1,14); img1.set(5,2,67); img1.set(5,3,15); img1.set(5,4,83); img1.set(5,5,96);
	lsm.setGradientFilter(LeastSquaresMatching::Prewitt);
	Matrix Hx, Hy, G;
	lsm.prewitt(&Hx, &Hy);
	G = lsm.imfilter(&img1,&Hx);
	G.show();
*/

	return 1;
}

