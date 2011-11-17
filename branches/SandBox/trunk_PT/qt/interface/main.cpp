#include <iostream>

using namespace std;

#include <QApplication>
#include <QTranslator>
#include "LoadingScreen.h"
#include "EFotoManager.h"

#include "Matrix.h"
#include "ConvertionsSystems.h"
#include "GeoSystem.h"
#include <sys/time.h>
#include <QDebug>

#include <math.h>
//#include <qdebug.h>

#define TESTE4



int main( int argc, char ** argv )
{

    QApplication efoto(argc, argv);
	efoto.setStyle("plastique");
	efoto.quitOnLastWindowClosed();
/*
    //Code for translations
    QString locale = QLocale::system().name();
    QTranslator translator;
    translator.load(QString("FormProject_") + locale);
    efoto.installTranslator(&translator);

    efoto.connect(&efoto, SIGNAL(lastWindowClosed()), &efoto, SLOT(quit()));
*/
	//LoadingScreen::instance().show();
/*
	Matrix mat(3,3);
	for (int i=1;i<4;i++)
	{
		for (int j=1;j<4;j++)
		mat.set(i,j,rand()*255);
	}
	Matrix iden;
	iden.identity(3);
	(mat&iden).triang().reduction().show('g',8,"teste");
*/

    GeoSystem sys(WGS84);

	double phi = -80*M_PI/180;
	double lambda = 180*M_PI/180;
    double h1 = 3.56;

#ifdef TESTE4
	double E=665449.291;
	double N=7465313.753;
	double H=25.662;
    int hemi=-1;
    int zona=23;
	double Ei=E;
	double Ni=N;
	double Hi=H;
	int hemii=hemi;
    int zonai=zona;
#endif
//	qDebug("inicio\nphi: %.18f\tlambda: %.18f\taltitude : %.18f",phi,lambda,h1);
	for (int i=0;i<100;i++)
	{
#ifdef TESTE1

        Matrix result1=ConvertionsSystems::GeoelipToGeocentricCartesian( phi,lambda,h1,sys );
        result1.show('f',18,"ida");
        double X=result1.get(1,1);
        double Y=result1.get(1,2);
        double Z=result1.get(1,3);

        Matrix result2=ConvertionsSystems::GeocentricCartesianToGeoelip(X,Y,Z,sys);
        result2.show('f',18,"volta");

        phi=result2.get(1,1);
        lambda = result2.get(1,2);
        h1=result2.get(1,3);

#endif
#ifdef TESTE2
		Matrix result1=ConvertionsSystems::geoToUtm(phi,lambda,sys);
		result1.show('f',10,"ida");
        double E=result1.get(1,1);
        double N=result1.get(1,2);
        int zona=result1.getInt(1,3);
        int hemi=result1.get(1,4);

        Matrix result2=ConvertionsSystems::utmToGeo(E,N,zona,hemi,sys);
		result2.show('f',10,"volta");

        qDebug("Diferenças: %.10f \t%.10f",phi-result2.get(1,1),lambda - result2.get(1,2));

        phi=result2.get(1,1);
        lambda = result2.get(1,2);


#endif
#ifdef TESTE3
		qDebug("ida:\nE = %.5f\tN = %.5f\tZona = %d\tHemi = %d",E,N,zona,hemi);
		Matrix result1=ConvertionsSystems::utmToGeo(E,N,zona,hemi,sys);
//        result1.show('f',10,"ida");
        double phi=result1.get(1,1);
        double lambda=result1.get(1,2);

        Matrix result2=ConvertionsSystems::geoToUtm(phi,lambda,sys);
        //result2.show('f',10,"volta");

//		qDebug("%d-diferença: %.10f\t%.10f\t%d\t%d",i,E-result2.get(1,1),N - result2.get(1,2),zona-result2.getInt(1,3),hemi-result2.getInt(1,4));

        E=result2.get(1,1);
        N=result2.get(1,2);
        zona=result2.getInt(1,3);
        hemi=result2.getInt(1,4);

		qDebug("Volta:\nE = %.5f\tN = %.5f\tZona = %d\tHemi = %d\n\n",E,N,zona,hemi);
#endif

#ifdef TESTE4
		qDebug("ida:\nE = %.5f\tN = %.5f\tZona = %d\tHemi = %d",E,N,zona,hemi);

		Matrix result1=ConvertionsSystems::utmToGeocentrica(E,N,H,zona,hemi,sys);
//        result1.show('f',10,"ida");
		double X=result1.get(1,1);
		double Y=result1.get(1,2);
		double Z=result1.get(1,3);

		Matrix result2=ConvertionsSystems::GeocentricaToUtm(X,Y,Z,sys);
		//result2.show('f',10,"volta");

		//qDebug("%d-diferença: %.10f\t%.10f\t%.10f\t%d\t%d",i,E-result2.get(1,1),N - result2.get(1,2),H-result2.get(1,3),zona-result2.getInt(1,4),hemi-result2.getInt(1,5));

		E=result2.get(1,1);
		N=result2.get(1,2);
		H=result2.get(1,3);
		zona=result2.getInt(1,4);
		hemi=result2.getInt(1,5);

		qDebug("Volta:\nE = %.5f\tN = %.5f\tH = %.5f\tZona = %d\tHemi = %d\n\n",E,N,H,zona,hemi);
#endif

    }

	qDebug("Diferenças Final: %.10f\t%.10f\t%.10f\t%d\t%d",E-Ei,N-Ni,H-Hi,zona-zonai,hemi-hemii);


    qApp->processEvents();
    EFotoManager manager;
    manager.setInterfaceType("Qt");
    if (argc==2){
        ++(argv);
        manager.exec(*argv);
    }
    else
    {
        manager.exec();
    }
	return efoto.exec();

}
