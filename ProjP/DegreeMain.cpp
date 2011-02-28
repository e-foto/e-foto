#include <QtGui>

#include <QApplication>
#include "Dms.h"
#include "ConvertionsSystems.h"

int main(int argc, char **argv)
{
    QApplication aplicat(argc,argv);

	//DegreeLineEdit *box= new DegreeLineEdit();

/*
	Dms *testeUtmGeoPhi= new Dms(47,48,15);
	Dms *testeUtmGeoLambda= new Dms(13,42,02);
	qDebug("phi, lambda, h to XYZ");
	Matrix geotocart1( TransformationsCoordinates::GeoelipToGeocentricCartesian(testeUtmGeoPhi->dmsToRadiano(),testeUtmGeoLambda->dmsToRadiano(),1852));
	qDebug("XYZ to phi, lambda , h");
	TransformationsCoordinates::GeocentricCartesianToGeoelip(geotocart1.get(1,1),geotocart1.get(1,2),geotocart1.get(1,3));

	Dms *testeUtmN= new Dms(48,0,0);
	Dms *testeUtmE= new Dms(14,0,0);
	qDebug("phi, lambda, h to XYZ");
	Matrix geotocart2(TransformationsCoordinates::GeoelipToGeocentricCartesian(testeUtmN->dmsToRadiano(),testeUtmE->dmsToRadiano(),0));
	qDebug("XYZ to phi, lambda , h");
	TransformationsCoordinates::GeocentricCartesianToGeoelip(geotocart2.get(1,1),geotocart2.get(1,2),geotocart2.get(1,3));

	*/
/*
    Dms *testePhi= new Dms(16,23,32.01651);
    Dms *testeLambda= new Dms(54,51,22.1918);
    Dms *testeLambda0= new Dms(57,0,0);
  */

    Dms *testePhi= new Dms(34,15,34.742);
    Dms *testeLambda= new Dms(96,2,43.158);
    //Dms *testeLambda0= new Dms(57,0,0);

    ConvertionsSystems trans;
    Matrix xyz(1,3);
    Matrix plh(1,3);

    plh.set(1,1,testePhi->dmsToRadiano());
    plh.set(1,2,testeLambda->dmsToRadiano());
    //plh.set(1,3,testeLambda0->dmsToRadiano());
   // for (int i=0;i<200;i++)
    {
        qDebug("phi, lambda, h to XYZ");
        xyz = trans.geoElipToUtm(testePhi->dmsToRadiano(),testeLambda->dmsToRadiano(),0.0,'n');//,'e');
        qDebug("\n\nXYZ to phi, lambda, h");
        plh = trans.uTmToGeo(xyz.get(1,1),xyz.get(1,2),xyz.get(1,3),xyz.get(1,4),'n');
    }
 /*   qDebug("Manual");
    trans.uTmToGeo(76025.312,5205649.348,0.0,testeLambda0->dmsToRadiano());
    qDebug("");

	//box->setFixedSize(300,50);
	//box->show();*/

	return 0;//aplicat.exec();
}
