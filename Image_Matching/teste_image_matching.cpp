#include <stdio.h>
#include <qimage.h>
#include "Matrix.h"
#include "ImageMatching.h"
#include "MatchingPoints.h"


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
//			pixel = ((pixel >> 16) & 0xFF)*0.2989 + ((pixel >> 8) & 0xFF)*0.5870 + (pixel & 0xFF)*0.1140;
			pixel = pixel & 0xFF;
			I->set(i, j, pixel/double(levels-1));
		}
	}

	return *I;
}

int saveImage(char *filename, Matrix *I)
{
	int levels = 256;

	QImage img(I->getCols(), I->getRows(), 32); // Qt3
//	QImage img(I->getCols(), I->getRows(), QImage::Format_RGB32); // Qt4

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
	Matrix img1 = loadImage("/home/marts/EFOTO/Develop/data_and_images/1997_016_300dpi.bmp");
	Matrix img2 = loadImage("/home/marts/EFOTO/Develop/data_and_images/1997_017_300dpi.bmp");
//	Matrix img1 = loadImage("../1997_016_300dpi.bmp");
//	Matrix img2 = loadImage("../1997_017_300dpi.bmp");

	MatchingPointsList rep, pairs;

	rep.add(16, 17, 1796.0, 1064.0, 764.0, 1044.0, 0.0);
	rep.add(16, 17, 1801.0, 1328.0, 814.0, 1310.0, 0.0);
	rep.add(16, 17, 2155.0, 1339.0, 1173.0, 1317.0, 0.0);
	rep.add(16, 17, 1852.0, 1325.0, 865.0, 1306.0, 0.0);
	rep.add(16, 17, 1305.0, 452.0, 296.0, 426.0, 0.0);
	rep.add(16, 17, 2083.0, 2482.0, 1096.0, 2455.0, 0.0);

//	rep.load((char *)"maracana_pairs_refined.pix", MatchingPointsList::loadPairs, false, 16, 17);
//	rep.listMp();

	ImageMatching im;
	im.setImagesIds(16,17);
	im.setCorrelationThreshold(0.65);
	printf("Performing radiometric correction ...\n");
	im.setPerformRadiometric(true);
//	im.setMatchingMethod(ImageMatching::NCC);
//	im.setMatchingMethod(ImageMatching::LSM);
//	im.setRadiometricMode(ImageMatching::Equalization);
	im.setRadiometricMode(ImageMatching::HistMatching);
//	im.setMatchingLimits(1800,1900,1300,1400);
	im.getNCC()->setTemplate(10);
	im.getNCC()->setSearchWindow(15);
	im.getLSM()->setTemplate(10);
	im.getLSM()->setMaxIterations(10);

	printf("Start searching ...\n");
	im.performImageMatching(&img1, &img2, &rep, &pairs);
	printf("Coverage: %f %%\n",im.getCoverage());

//	pairs.save((char *)"lista_final.pix", MatchingPointsList::saveFull);
	pairs.save((char *)"lista_final.pix", MatchingPointsList::savePairsHann);
/*
	// Write on image
	int x,y;
	MatchingPoints mp;
	for (int i=1; i<=pairs.size(); i++)
	{
		mp = *pairs.get(i);
		x = mp.left_x;
		y = mp.left_y;
		img1.set(y,x,0.0);
		x = mp.right_x;
		y = mp.right_y;
		img2.set(y,x,0.0);
	}

	saveImage((char *)"saidaL.bmp", &img1);
	saveImage((char *)"saidaR.bmp", &img2);
*/
	return 1;
}
