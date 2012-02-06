#include "DemFeatures.h"
#include "MatchingPoints.h"
#include <qimage.h>

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
//			pixel = round(I->get(i,j)*double(levels-1));
//			pixel = (pixel << 16) + (pixel << 8) + pixel;

			if (fabs(I->get(i,j) - 0.0) < 0.000000000001)
				pixel = 0xFF;
			else
				pixel = 0xFFFFFF;

			img.setPixel(j-1, i-1, pixel);
		}
	}

	img.save(filename,"BMP");

	return 1;
}

int main(void)
{
/*
	//
	// Teste 1
	//
	DemFeatures df;
	MatchingPointsList mpl;

//	printf("Feature type: %s\n",df.getFeatureTypeName(2).c_str());
//	df.loadFeatures((char *)"/home/marts/EFOTO/Develop/data_and_images/UERJ_FEATURES.txt",1,false);
	df.loadFeatures((char *)"/home/marcelo/projects/e-foto/Old E-FOTO/Database/FEICOES_UERJ_OK_4.txt",1,false);
//	df.getFeature(8).description = "Rua do bairro do Maracanã"; // Ok, read only
//	df.showFeatures(true);

//	printf("Point id: %d\n",df.addNewPoint(8, 680890.0, 7465159.0, 20.0)); // Ok
//	df.updatePoint(8, 4, 680890.0, 7465159.0, 20.0); // Ok
//	df.setName(8, "Teste"); // Ok
	df.setDescription(8, "Rua do bairro do Maracanã");
//	df.deleteFeature(7); // Ok
//	df.deletePoint(8,7); // Ok
//	df.addNewPoint(8,8,10.0,20.0,30.0); // Ok
//	df.copyFeature(6); // Ok
//	df.addNewPoint(9,2,1.0,2.0,3.0); // Ok, work independently
	df.copyFeature(7,1.0);
	df.showFeatures(true);
//	df.showClasses(); // Ok
//	printf("Nearest feature: %d\n",df.getNearestFeature(681102.3714, 7465262.5574, 21.76));
//	printf("Nearest feature: %d\n",df.getNearestFeature(680828.2934, 7465382.5400, 13.69));

	df.addFeaturesToPairList(&mpl,true);
	mpl.listMp();
*/
/*
	//
	// Teste 2
	//
	DemFeatures df;

	df.loadFeatures((char *)"/home/marcelo/projects/e-foto/Old E-FOTO/Database/FEICOES_UERJ_OK_4.txt",1,false);
//	df.showFeatures(true);
	double Xi, Yi, Xf, Yf;
	df.calculateBoundingBox(1, Xi, Yi, Xf, Yf);
	printf("Bounding box: %f, %f, %f, %f\n", Xi, Yi, Xf, Yf);

//	Matrix mapPol = df.mapPolygon(1,1.0,1.0);
//	mapPol.show();

	Matrix overMap = df.createPolygonMap(680332.43766, 7464125.88182, 682283.11950, 7465958.62632, 1.0, 1.0);

	saveImage("overmap.bmp", &overMap);
*/

	//
	// Teste 3
	//

	// UERJ 16 e 17 images
	DemFeatures dfs;
//	dfs.setImagePairSize(2895, 2838, 2895, 2838);
	dfs.setImagePairSize(2000, 2000, 2000, 2000);
	dfs.loadFeatures((char *)"/home/marcelo/DevEFOTO/e-foto/eppsHomePage/FEICOES_UERJ.txt",1,false); // Change to current machine path

	// Check if must render, for each image
	int no_feats = dfs.getNumFeatures();
	DemFeature df;
	bool renderLeft, renderRight;

	for (int i=1; i<= no_feats; i++)
	{
		df = dfs.getFeature(i);

		// Not rendered
		if (df.is_on_screen == 0)
			continue;

		// Is rended somewhere
		renderLeft = (df.is_on_screen & 1) == 1;
		renderRight = (df.is_on_screen & 2) == 2;
		printf("Feature #%d:\n",i);

		for (int k=0; k<df.points.size(); k++)
		{
			// Render left image
			if (renderLeft)
				printf("Left: %f, %f ", df.points.at(k).left_x, df.points.at(k).left_y);

			// Render right image
			if (renderRight)
				printf("Right: %f, %f ", df.points.at(k).right_x, df.points.at(k).right_y);

			printf("\n");
		}
	}

	return 1;
}
