#include <qimage.h>
#include <stdio.h>
#include "MatchingPoints.h"
#include "MatchingPointsGrid.h"
#include "DemGrid.h"

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

int main(void)
{
	MatchingPointsList myList;
//	if(!myList.load((char *)"../Analysis/uerj_3d_points.txt",MatchingPointsList::loadidXYZ, false, 16, 17))
	if (!myList.load((char *)"/home/marts/Documentos/e-foto/eppsHomePage/Seeds.txt",MatchingPointsList::loadFull, false, 1, 2))
	{
		printf("Points file not found.\n");
		return 0;
	}

	DemGrid dg(680466.54795, 7464269.15739, 681360.02541, 7465947.83362, 1.0, 1.0);
	dg.setPointList(&myList);
//	dg.interpolateNearestPoint();
//	dg.interpolateTrendSurface(3);
//	dg.interpolateMovingAverage(0.5, 5.0, 0);
	dg.interpolateMovingAverage(0.5, 1000.0, 0);
//	dg.interpolateMovingSurface(0.5, 5.0, 0, 3);
//	dg.cutGrid(0.0,100.0); // User set min and max height
	dg.cutGrid(1.0,1.0,true); // Set min and max height from 3D point list
//	dg.loadDem("../Analysis/uerj_DEM_NP.dsm",DemGrid::efotoDEM);
//	dg.loadDem("../Analysis/uerj_DEM_NP.txt",DemGrid::ascii);
//	dg.saveDem("uerj_DEM_NP.dsm",DemGrid::efotoDEM);
//	dg.saveDem("uerj_DEM_NP.txt",DemGrid::ascii);
//	dg.saveDem("uerj_DEM_TS.dsm",DemGrid::efotoDEM);
//	dg.saveDem("uerj_DEM_MA.dsm",DemGrid::efotoDEM);
//	dg.saveDem("uerj_DEM_MS.dsm",DemGrid::efotoDEM);

	Matrix *img;
	img = dg.getDemImage();
	printf("Saving DEM image ...\n");
	saveImage("dem_uerj.bmp",img);

	return 1;
}

