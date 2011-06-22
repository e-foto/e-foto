#include <QtGui>

#include <QApplication>
#include "Dms.h"
#include "ConvertionsSystems.h"
#include "ConvertionLayout.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	GeoSystem oldSys(WGS84);
	GeoSystem newSys(SIRGAS2000);

/*
	qDebug("%.16f\t\t%.16f\t\t%.16f",Dms::degreeDecimalToRadiano(-0.1177),Dms::degreeDecimalToRadiano(0.1341),Dms::degreeDecimalToRadiano(0.0698));
	qDebug("%.16f\t\t%.16f\t\t%.16f",Dms::degreeDecimalToRadiano(-0.1236),Dms::degreeDecimalToRadiano(0.1343),Dms::degreeDecimalToRadiano(0.1911));
	qDebug("%.16f\t\t%.16f\t\t%.16f",Dms::degreeDecimalToRadiano(-0.1125),Dms::degreeDecimalToRadiano(0.1311),Dms::degreeDecimalToRadiano(0.4386));
/*	qDebug("%.16f\t\t%.16f\t\t%.16f",Dms::degreeDecimalToRadiano(-0.1418),Dms::degreeDecimalToRadiano(-0.1502),Dms::degreeDecimalToRadiano(-179.0521));
	qDebug("%.16f\t\t%.16f\t\t%.16f",Dms::degreeDecimalToRadiano(-0.1151),Dms::degreeDecimalToRadiano(-0.1401),Dms::degreeDecimalToRadiano(-179.2036));
	qDebug("%.16f\t\t%.16f\t\t%.16f",Dms::degreeDecimalToRadiano(-0.1299),Dms::degreeDecimalToRadiano(-0.1484),Dms::degreeDecimalToRadiano(-179.2775));
*/
	QApplication aplicat(argc,argv);

	ConvertionsLayout *widget=new ConvertionsLayout();
	widget->show();
/*
	Dms *geoPhi= new Dms(-22,54,46.5649);
	Dms *geoLambda= new Dms(-43,23,26.4143);


	double inrad=geoLambda->dmsToRadiano();
	double geoPhiRad= geoPhi->dmsToDegreeDecimal();
	double geoLamRad= geoLambda->dmsToDegreeDecimal();
    //qDebug("%.10f %.10f",geoPhiRad,geoLamRad);

    //Matrix result=ConvertionsSystems::GeoelipToGeocentricCartesian(geoPhiRad,geoLamRad,600,oldSys);
	//ConvertionsSystems::GeocentricCartesianToGeoelip();//result.get(1,1),result.get(1,2),result.get(1,3),oldSys);
    //ConvertionsSystems::GeocentricCartesianToGeoelip(2745642.82,1250203.99,800.28,oldSys);


	//qDebug("geophi %s \ngeolambda %s",geoPhi->radianoToDms(3*3.14159265358/2)->toString(11).toStdString().c_str(),geoLambda->toString(11).toStdString().c_str());
	//M_PI=3.1415926535897931

	/*

    qDebug("oldsys: %s",oldSys.toString().c_str());

    //qDebug("phi, lambda, h to XYZ");
	//Matrix geotocart1(ConvertionsSystems::GeoelipToGeocentricCartesian(geoPhi->dmsToRadiano(),geoLambda->dmsToRadiano(),1852,oldSys));
    //qDebug("XYZ to phi, lambda , h");
	//ConvertionsSystems::GeocentricCartesianToGeoelip(geotocart1.get(1,1),geotocart1.get(1,2),geotocart1.get(1,3),oldSys);


    qDebug("newSys: %s",newSys.toString().c_str());
	//Dms *testeUtmN= new Dms(48,0,0);
//	Dms *testeUtmE= new Dms(14,0,0);
    //qDebug("phi, lambda, h to XYZ\n");
   // Matrix geotocart2(ConvertionsSystems::GeoelipToGeocentricCartesian(testeUtmN->dmsToRadiano(),testeUtmE->dmsToRadiano(),0,newSys));
    //qDebug("XYZ to phi, lambda , h\n");
	//ConvertionsSystems::GeocentricCartesianToGeoelip(geotocart2.get(1,1),geotocart2.get(1,2),geotocart2.get(1,3),newSys);

    qDebug("So conversão");
	double *result=ConvertionsSystems::convertSystems(oldSys,newSys,geoPhi->dmsToRadiano(),geoLambda->dmsToRadiano(),600);
    Dms *phid=new Dms();
    Dms *lambd=new Dms();
	//qDebug("phi %f lambda %f",result[0],result[1]);
    qDebug("result:\nphi=%s lambda=%s altura=%f", phid->radianoToDms(result[0])->toString().toStdString().c_str(), lambd->radianoToDms(result[1])->toString().toStdString().c_str(), result[2]);

/*
    Dms *testePhi= new Dms(16,23,32.01651);
    Dms *testeLambda= new Dms(54,51,22.1918);
    Dms *testeLambda0= new Dms(57,0,0);
  */
/*
    Dms *testeLambda= new Dms(16,0,0.0); // testar esses valores 60.0 59.0
    Dms *testePhi= new Dms(11,0,0.0);

    ConvertionsSystems trans;
	Matrix xyz(1,4);
	Matrix plh(1,3);

    plh.set(1,1,testePhi->dmsToRadiano());
    plh.set(1,2,testeLambda->dmsToRadiano());
    plh.set(1,3,0.0);

    //for (int i=0;i<10;i++)//N=2214295,199  E=186073,7205
	{
		qDebug("phi, lambda, h to E N H");
		xyz = trans.geoElipToUtm(plh.get(1,1),plh.get(1,2),plh.get(1,3),'n');//,'e');
		//qDebug()<< "Valores esperados:\n E= 404853.431417, N= 3374982.887511, H= 0.0, MC= 81";

		qDebug("E N H to phi, lambda, h");
        plh = trans.utmToGeo(xyz.get(1,1),xyz.get(1,2),xyz.get(1,3),xyz.get(1,4),'n');
		//qDebug()<< "Valores esperados:\n phi= 30°30'12.12922\", lambda= 80°0'30.49991\" , H= 0.0, MC= 81";

    }

	/*
	//box->setFixedSize(300,50);
	//box->show();*/
	return aplicat.exec();
	//return 0;
}
