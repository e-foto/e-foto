#include <stdio.h>
#include <qimage.h>
#include "Matrix.h"
#include "Interpolation.h"

Matrix& loadImage(char *filename)
{
	QImage img;
	img.load(filename);

	Matrix *I = new Matrix(img.height(), img.width());

	double pixel;
	for (int i=1; i<=img.height(); i++)
	{
		for (int j=1; j<=img.width(); j++)
		{
//			pixel = double(img.pixel(j-1,i-1)&0xFFFFFF)/0xFFFFFF; // Color
			pixel = double((img.pixel(j-1,i-1)&0xFF0000) >> 16)/255.0; // Gray
			I->set(i, j, pixel);
		}
	}

	return *I;
}

int saveImage(char *filename, Matrix *I)
{
	QImage img(I->getCols(), I->getRows(), 32);

	int pixel;
	for (int i=1; i<=img.height(); i++)
	{
		for (int j=1; j<=img.width(); j++)
		{
//			pixel = int(I->get(i,j)*0xFFFFFF); // Color
			pixel = int(I->get(i,j)*255); // Gray
			pixel = (pixel << 16) + (pixel << 8) + pixel; // Gray
			img.setPixel(j-1, i-1, pixel);
		}
	}

	img.save(filename,"BMP");

	return 1;
}


int main(void)
{

	printf("Loading image ...\n");
	Matrix img = loadImage("../brasil.jpg");
	Matrix newimg(img.getRows(), img.getCols());

	Interpolation interp;
        interp.setMode(0);

	printf("Processing image ...\n");
	double pixel, x, y;
	for (int i=1; i<= img.getRows(); i++)
	{
		for (int j=1; j<= img.getCols(); j++)
		{
			x = double(j) + 0.3;
			y = double(i) + 0.7;

//			pixel = img.get(i,j);
			pixel = interp.interpolate(&img, x, y, 3);

			newimg.set(i,j,pixel);
		}
	}

	printf("Saving image ...\n");
	saveImage("../brasil2.bmp",&newimg);

	return 1;
}

