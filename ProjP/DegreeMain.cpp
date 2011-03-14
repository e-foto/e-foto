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

    Dms *testeLambda= new Dms(80,0,30.5); // testar esses valores 60.0 59.0
    Dms *testePhi= new Dms(30,30,12.13);

    ConvertionsSystems trans;
	Matrix xyz(1,4);
	Matrix plh(1,3);

    plh.set(1,1,testePhi->dmsToRadiano());
    plh.set(1,2,testeLambda->dmsToRadiano());
    plh.set(1,3,0.0);
	for (int i=0;i<2000;i++)//N=2214295,199  E=186073,7205
    {
        qDebug("phi, lambda, h to E N H");
        xyz = trans.geoElipToUtm(plh.get(1,1),plh.get(1,2),plh.get(1,3),'n');//,'e');
        qDebug()<< "Valores esperados:\n E= 404853.431417, N= 3374982.887511, H= 0.0, MC= 81";

        qDebug("E N H to phi, lambda, h");
        plh = trans.uTmToGeo(xyz.get(1,1),xyz.get(1,2),xyz.get(1,3),xyz.get(1,4),'n');

	   qDebug()<< "Valores esperados:\n phi= 30°30'12.12922\", lambda= 80°0'30.49991\" , H= 0.0, MC= 81";

    }
 /*  
	//box->setFixedSize(300,50);
	//box->show();*/

	return 0;//aplicat.exec();
}
