#include <stdio.h>
#include <qimage.h>
#include "Matrix.h"
#include "NormalizedCrossCorrelation.h"


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
/*
	// Test with windows - Coordinates in matricial form (range from 1 - N, i,j)
	NormalizedCrossCorrelation ncc;
	ncc.setTemplate(5);
	Matrix A(5,5);
	A.set(1,1, 6); A.set(1,2,55); A.set(1,3, 7); A.set(1,4,24); A.set(1,5,86);
	A.set(2,1,64); A.set(2,2,74); A.set(2,3,45); A.set(2,4,18); A.set(2,5,49);
	A.set(3,1,97); A.set(3,2,20); A.set(3,3, 5); A.set(3,4,49); A.set(3,5,90);
	A.set(4,1,60); A.set(4,2,11); A.set(4,3,96); A.set(4,4,20); A.set(4,5,28);
	A.set(5,1,14); A.set(5,2,67); A.set(5,3,15); A.set(5,4,83); A.set(5,5,96);
	
	Matrix B(5,5);
	B.set(1,1, 6); B.set(1,2,55); B.set(1,3, 7); B.set(1,4,24); B.set(1,5,86);
	B.set(2,1,64); B.set(2,2,74); B.set(2,3,45); B.set(2,4,18); B.set(2,5,49);
	B.set(3,1,97); B.set(3,2,20); B.set(3,3,25); B.set(3,4,49); B.set(3,5,90);
	B.set(4,1,60); B.set(4,2,11); B.set(4,3,96); B.set(4,4,20); B.set(4,5,28);
	B.set(5,1,14); B.set(5,2,67); B.set(5,3,15); B.set(5,4,83); B.set(5,5,96);

	printf("Correlation between A and B is: %f\n",ncc.directCorrelation(&A,&B,3,3,3,3));
*/
/*
	// Test with windows - Coordinates in matricial form (range from 1 - N, i,j)
	NormalizedCrossCorrelation ncc;
	ncc.setTemplate(4);
	Matrix A(4,4);
	A.set(1,1, 6); A.set(1,2,55); A.set(1,3, 7); A.set(1,4,24);
	A.set(2,1,64); A.set(2,2,74); A.set(2,3,45); A.set(2,4,18);
	A.set(3,1,97); A.set(3,2,20); A.set(3,3, 5); A.set(3,4,49);
	A.set(4,1,60); A.set(4,2,11); A.set(4,3,96); A.set(4,4,20);
	
	Matrix B(4,4);
	B.set(1,1, 6); B.set(1,2,55); B.set(1,3, 7); B.set(1,4,24);
	B.set(2,1,64); B.set(2,2,74); B.set(2,3,45); B.set(2,4,18);
	B.set(3,1,97); B.set(3,2,20); B.set(3,3,25); B.set(3,4,49);
	B.set(4,1,60); B.set(4,2,11); B.set(4,3,96); B.set(4,4,20);

	printf("Correlation between A and B is: %f\n",ncc.directCorrelation(&A,&B,2,2,2,2));
*/

	// Test with images
	printf("Loading image left ...\n");
	Matrix img1 = loadImage("/home/marts/EFOTO/Dev/data_and_images/1997_016_300dpi.bmp");

	printf("Loading image right ...\n");
	Matrix img2 = loadImage("/home/marts/EFOTO/Dev/data_and_images/1997_017_300dpi.bmp");

	printf("Performing matching ...\n");
	NormalizedCrossCorrelation ncc;
	ncc.setTemplate(7);
	ncc.setSearchWindow(15);

	ncc.setTemplateCenter(1797, 1064);
	ncc.setSearchWindowCenter(767, 1045);
	ncc.searchHomologous(&img1,&img2);
	printf("Best point found in: %d, %d\n",ncc.getBestX(),ncc.getBestY());
	printf("Correlation: %f\n",ncc.getBestP());
	img2.set(ncc.getBestY(),ncc.getBestX(),0.0);

	ncc.setTemplateCenter(2155, 1340);
	ncc.setSearchWindowCenter(1177, 1315);
	ncc.searchHomologous(&img1,&img2);
	printf("Best point found in: %d, %d\n",ncc.getBestX(),ncc.getBestY());
	printf("Correlation: %f\n",ncc.getBestP());
	img2.set(ncc.getBestY(),ncc.getBestX(),0.0);

	ncc.setTemplateCenter(1803, 1329);
	ncc.setSearchWindowCenter(812, 1310);
	ncc.searchHomologous(&img1,&img2);
	printf("Best point found in: %d, %d\n",ncc.getBestX(),ncc.getBestY());
	printf("Correlation: %f\n",ncc.getBestP());
	img2.set(ncc.getBestY(),ncc.getBestX(),0.0);

	ncc.setTemplateCenter(1854, 1327);
	ncc.setSearchWindowCenter(864, 1307);
	ncc.searchHomologous(&img1,&img2);
	printf("Best point found in: %d, %d\n",ncc.getBestX(),ncc.getBestY());
	printf("Correlation: %f\n",ncc.getBestP());
	img2.set(ncc.getBestY(),ncc.getBestX(),0.0);

	printf("Saving image ...\n");
	saveImage("result.bmp",&img2);

/*
1      1800.00    1063.00     766.00    1042.00
2      2155.00    1340.00    1177.00    1315.00
3      1803.00    1329.00     812.00    1310.00
4      1854.00    1327.00     864.00    1307.00
*/

/*
	// Teste 1
	Matrix A(3,3);
	A.set(1,1,1); A.set(1,2,2); A.set(1,3,3);
	A.set(2,1,4); A.set(2,2,5); A.set(2,3,6);
	A.set(3,1,7); A.set(3,2,8); A.set(3,3,9);
	printf("Média: %f\n",ncc.average(&A,2,3,1,2));
	printf("Desvio: %f\n",ncc.stddev(&A,2,3,1,2));

	Matrix B(3,3);
	B.set(1,1,1); B.set(1,2,2); B.set(1,3,3);
	B.set(2,1,4); B.set(2,2,5); B.set(2,3,6);
	B.set(3,1,7); B.set(3,2,8); B.set(3,3,9);
	printf("Covariancia: %f\n",ncc.covXY(&A,&B));
	printf("Correlação: %f\n",ncc.correlation(&A,&B));
*/

/*
	// Teste 2
	Matrix A(3,3);
	A.set(1,1,1); A.set(1,2,2); A.set(1,3,3);
	A.set(2,1,4); A.set(2,2,5); A.set(2,3,6);
	A.set(3,1,7); A.set(3,2,8); A.set(3,3,9);

	Matrix B(3,3);
	B.set(1,1,5); B.set(1,2,1); B.set(1,3,2);
	B.set(2,1,8); B.set(2,2,4); B.set(2,3,5);
	B.set(3,1,0); B.set(3,2,4); B.set(3,3,5);
	printf("Correlação: %f\n",ncc.correlation(&A,&B,1,2,1,2,1,2,2,3));
*/

	return 1;
}

