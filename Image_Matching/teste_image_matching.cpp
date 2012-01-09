#include <stdio.h>
#include <qimage.h>
#include "Matrix.h"
#include "ImageMatching.h"
#include "MatchingPoints.h"


Matrix& loadImage(char *filename, double sample=1.0)
{
	int levels=256;

	QImage img;
	img.load(filename);

	int step = int(1.0/sample);
	int width = int(img.width()*sample);
	int height = int(img.height()*sample);
	int pixel;

	Matrix *I = new Matrix(height, width);

	for (unsigned int i=1; i<=height; i++)
	{
		for (unsigned int j=1; j<=width; j++)
		{
			pixel = img.pixel((j-1)*step,(i-1)*step);
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

void resamplePoints(MatchingPointsList *list, double resample)
{
	MatchingPoints *mp;
	for (int i=1; i<=list->size(); i++)
	{
		mp = list->get(i);
		mp->left_x = mp->left_x * resample;
		mp->left_y = mp->left_y * resample;
		mp->right_x = mp->right_x * resample;
		mp->right_y = mp->right_y * resample;
	}
}

int main(void)
{
	printf("Loading images ...\n");
//	Matrix img1 = loadImage("/home/marts/EFOTO/Develop/data_and_images/138_7623.bmp",0.25);
//	Matrix img2 = loadImage("/home/marts/EFOTO/Develop/data_and_images/138_7624.bmp",0.25);
	Matrix img1 = loadImage("/home/marts/EFOTO/Develop/data_and_images/1997_016_300dpi.bmp",0.25);
	Matrix img2 = loadImage("/home/marts/EFOTO/Develop/data_and_images/1997_017_300dpi.bmp",0.25);
//	Matrix img1 = loadImage("../1997_016_300dpi.bmp");
//	Matrix img2 = loadImage("../1997_017_300dpi.bmp");

	MatchingPointsList rep, pairs;
/*
	// Seeds for Itaipava 23-24 - Image coordinates
	rep.add(23, 24, 6057.0, 5311.0, 2310.0, 5120.0, 0.0);
	rep.add(23, 24, 6979.0, 4470.0, 6013.0, 4291.0, 0.0);
	rep.add(23, 24, 10931.0, 7103.0, 7224.0, 6886.0, 0.0);
	rep.add(23, 24, 8530.0, 1963.0, 4618.0, 1778.0, 0.0);
	resamplePoints(&rep,0.25);
*/
/*
	// Seeds for Maracana 16-17
	rep.add(16, 17, 1796.0, 1064.0, 764.0, 1044.0, 0.0);
	rep.add(16, 17, 1801.0, 1328.0, 814.0, 1310.0, 0.0);
	rep.add(16, 17, 2155.0, 1339.0, 1173.0, 1317.0, 0.0);
	rep.add(16, 17, 1852.0, 1325.0, 865.0, 1306.0, 0.0);
	rep.add(16, 17, 1305.0, 452.0, 296.0, 426.0, 0.0);
	rep.add(16, 17, 2083.0, 2482.0, 1096.0, 2455.0, 0.0);
*/
//	rep.load((char *)"maracana_pairs_refined.pix", MatchingPointsList::loadPairs, false, 16, 17);
	rep.load((char *)"/home/marts/Documentos/e-foto/eppsHomePage/Seeds_ok.txt", MatchingPointsList::loadFull, false, 16, 17);
	rep.remove(14);
	rep.remove(13);
//	rep.listMp();
	resamplePoints(&rep,0.25);

	ImageMatching im;
//	im.setImagesIds(23,24); // Itaipava
//	im.setImagesIds(16,17); // Maracana
	im.setImagesIds(1,2); // Maracana from file
	im.setCorrelationThreshold(0.60);
	printf("Performing radiometric correction ...\n");
	im.setPerformRadiometric(true);
//	im.setMatchingMethod(ImageMatching::NCC);
//	im.setMatchingMethod(ImageMatching::LSM); // Default
//	im.setRadiometricMode(ImageMatching::Equalization);
	im.setRadiometricMode(ImageMatching::HistMatching);
//	im.setMatchingLimits(1800,1900,1300,1400);
	im.setMinStd(0.10); // Use 0.10 for Itaipava, 0.15 for Maracanã (default value)
	im.getNCC()->setTemplate(10);
	im.getNCC()->setSearchWindow(15);
	im.getLSM()->setTemplate(10);
	im.getLSM()->setMaxIterations(10);

	printf("Start searching ...\n");
	im.performImageMatching(&img1, &img2, &rep, &pairs);
	printf("Coverage: %f %%\n",im.getCoverage());
//	resamplePoints(&pairs,4.0);

//	pairs.save((char *)"lista_final.pix", MatchingPointsList::saveFull);
//	pairs.save((char *)"lista_final.pix", MatchingPointsList::savePairsHann);

	// Write on image
	int x,y;
	MatchingPoints mp;
	for (int i=1; i<=pairs.size(); i++)
//	for (int i=1; i<=rep.size(); i++)
	{
		mp = *pairs.get(i);
//		mp = *rep.get(i);
		x = mp.left_x;
		y = mp.left_y;
		img1.set(y,x,0.0);
		x = mp.right_x;
		y = mp.right_y;
		img2.set(y,x,0.0);
	}

	saveImage((char *)"saidaL.bmp", &img1);
	saveImage((char *)"saidaR.bmp", &img2);

	return 1;
}

