#include <QtGui>

#include <QApplication>
#include "Dms.h"
#include "ConvertionsSystems.h"

int main(int argc, char **argv)
{
    QApplication aplicat(argc,argv);

	//DegreeLineEdit *box= new DegreeLineEdit();

/*
	Dms *teste1myType= new Dms(40,50,30.20);
	Dms *teste2myType= new Dms(100,30,50.50);
	Dms *teste3myType= new Dms(100,30,50.50,true);
*/
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

    Dms *testePhi= new Dms(47,0,0);
    Dms *testeLambda= new Dms(16,0,0);
    Dms *testeLambda0= new Dms(15,0,0);
    ConvertionsSystems trans;
    Matrix xyz(1,3);
    Matrix plh(1,3);

    plh.set(1,1,testePhi->dmsToRadiano());
    plh.set(1,2,testeLambda->dmsToRadiano());
    plh.set(1,3,testeLambda0->dmsToRadiano());
   // for (int i=0;i<200;i++)
    {
        qDebug("phi, lambda, h to XYZ");
        xyz = trans.geoElipToUtm(plh.get(1,1),plh.get(1,2),testeLambda0->dmsToRadiano(),0.0);
        qDebug("XYZ to phi, lambda, h");
        plh = trans.uTmToGeo(xyz.get(1,1),xyz.get(1,2),xyz.get(1,3),testeLambda0->dmsToRadiano());
    }
    qDebug("Manual");
    trans.uTmToGeo(76025.312,5205649.348,0.0,testeLambda0->dmsToRadiano());
    qDebug("");
/*
    qDebug()<<(new Dms())->radianoToDms(0.000001)->toString();

	qDebug()<<(new Dms(0,0,0.1))->dmsToRadiano();

	qDebug()<<(new Dms())->radianoToDms(4.84814e-7)->toString();
	/*
    qDebug("%s comparando com %s :%d",teste1myType->toString().toStdString().c_str(),teste3myType->toString().toStdString().c_str(),teste1myType->compareDegMinSecs(teste3myType));
    qDebug("%s comparando com %s :%d",teste3myType->toString().toStdString().c_str(),teste1myType->toString().toStdString().c_str(),teste3myType->compareDegMinSecs(teste1myType));
	//teste1myType->compareDegMinSecs(new Dms(40,50,30.21))

	qDebug("%s comparando com %s :%d",teste1myType->toString().toStdString().c_str(),(new Dms(40,50,30.21))->toString().toStdString().c_str(),teste1myType->compareDegMinSecs(new Dms(40,50,30.21)));
	qDebug("%s comparando com %s :%d",teste1myType->toString().toStdString().c_str(),(new Dms(40,50,30.19))->toString().toStdString().c_str(),teste1myType->compareDegMinSecs(new Dms(40,50,20.19)));
	qDebug("%s comparando com %s :%d\n",teste1myType->toString().toStdString().c_str(),(new Dms(40,52,20.21))->toString().toStdString().c_str(),teste1myType->compareDegMinSecs(new Dms(40,52,20.21)));

	qDebug("%s comparando com %s :%d",teste1myType->toString().toStdString().c_str(),(new Dms(42,52,30.21))->toString().toStdString().c_str(),teste1myType->compareDegMinSecs(new Dms(42,52,30.21)));
	qDebug("%s comparando com %s :%d\n",teste1myType->toString().toStdString().c_str(),(new Dms(42,50,10.21))->toString().toStdString().c_str(),teste1myType->compareDegMinSecs(new Dms(42,50,10.21)));

	qDebug("%s comparando com %s :%d",teste1myType->toString().toStdString().c_str(),(new Dms(38,50,30.21))->toString().toStdString().c_str(),teste1myType->compareDegMinSecs(new Dms(38,50,30.21)));
	qDebug("%s comparando com %s :%d",teste1myType->toString().toStdString().c_str(),(new Dms(38,52,0.21))->toString().toStdString().c_str(),teste1myType->compareDegMinSecs(new Dms(38,52,0.21)));
	qDebug("%s comparando com %s :%d\n\n",teste1myType->toString().toStdString().c_str(),(new Dms(40,50,30.20))->toString().toStdString().c_str(),teste1myType->compareDegMinSecs(new Dms(40,50,30.20)));
*/
/*
	Dms *somaTest1Test2= Dms::addDegMinSecs(teste1myType,teste2myType);
    qDebug("soma de teste1 e teste2:\t%s ",somaTest1Test2->toString().toStdString().c_str());

	Dms *subTest1Test2= Dms::addDegMinSecs(teste1myType,teste3myType); //-59º40'20,30" = -(teste1-teste3)
    qDebug("subtracao de teste1 e teste3:\t%s ",subTest1Test2->toString().toStdString().c_str());

	Dms *mulTest1= Dms::mulDegMinSecs(2,teste1myType);
    qDebug("multiplicacao de teste1 *2:\t%s ",mulTest1->toString().toStdString().c_str());

	Dms *mulTest2= Dms::mulDegMinSecs(3,teste1myType);
    qDebug("multiplicacao de teste1 *3:\t%s ",mulTest2->toString().toStdString().c_str());

/*
    qDebug("\n\nteste 1\n");
	double teste1dec = teste1myType->dmsToDegreeDecimal(); //myType to degree decimal
	double teste1rad = teste1myType->dmsToRadiano(); // myType to radiano
    qDebug("%s to degree decimal: %f",teste1myType->toString().toStdString().c_str(),teste1dec);
    qDebug("%s to radiano: %f rad\n",teste1myType->toString().toStdString().c_str(),teste1rad);

	qDebug("%f rad to xxºxx'xx: %s",teste1rad,teste1myType->radianoToDms(teste1rad)->toString().toStdString().c_str() );
	qDebug("%f rad to degree decimal: %f\n",teste1rad, Dms::radianoToDegreeDecimal(teste1rad));

	qDebug("%f degree to xxºxx'xx: %s",teste1dec,teste1myType->degreeDecimalToDms(teste1dec)->toString().toStdString().c_str() );
	qDebug("%f degree to degree decimal: %f",teste1dec, Dms::degreeDecimalToRadiano(teste1dec) );

    qDebug("\n\nteste 2\n");
	double teste2dec = teste2myType->dmsToDegreeDecimal(); //myType to degree decimal
	double teste2rad =teste2myType->dmsToRadiano(); // myType to radiano

    qDebug("%s to degree decimal: %f",teste2myType->toString().toStdString().c_str(),teste2dec);
    qDebug("%s to radiano: %f rad\n",teste2myType->toString().toStdString().c_str(),teste2rad);

	qDebug("%f rad to xxºxx'xx: %s",teste2rad,teste2myType->radianoToDms(teste2rad)->toString().toStdString().c_str() );
	qDebug("%f rad to degree decimal: %f\n",teste2rad, Dms::radianoToDegreeDecimal(teste2rad) );

	qDebug("%f degree to xxºxx'xx: %s",teste2dec,teste2myType->degreeDecimalToDms(teste2dec)->toString().toStdString().c_str() );
	qDebug("%f degree to degree decimal: %f",teste2dec, Dms::degreeDecimalToRadiano(teste2dec) );

    qDebug("\n\nteste 3\n");
	double teste3dec = teste3myType->dmsToDegreeDecimal(); //myType to degree decimal
	double teste3rad =teste3myType->dmsToRadiano(); // myType to radiano

    qDebug("%s to degree decimal: %f",teste3myType->toString().toStdString().c_str(),teste3dec);
    qDebug("%s to radiano: %f rad\n",teste3myType->toString().toStdString().c_str(),teste3rad);

	qDebug("%f rad to xxºxx'xx: %s",teste3rad,teste3myType->radianoToDms(teste3rad)->toString().toStdString().c_str() );
	qDebug("%f rad to degree decimal: %f\n",teste3rad, Dms::radianoToDegreeDecimal(teste3rad) );

	qDebug("%f degree to xxºxx'xx: %s",teste3dec,teste3myType->degreeDecimalToDms(teste3dec)->toString().toStdString().c_str() );
	qDebug("%f degree to degree decimal: %f",teste3dec, Dms::degreeDecimalToRadiano(teste3dec) );


    qDebug("\n\nteste 4\n");
	double teste4dec = teste4myType->dmsToDegreeDecimal(); //myType to degree decimal
	double teste4rad =teste4myType->dmsToRadiano(); // myType to radiano

    qDebug("%s to degree decimal: %f",teste4myType->toString().toStdString().c_str(),teste4dec);
    qDebug("%s to radiano: %f rad\n",teste4myType->toString().toStdString().c_str(),teste4rad);

	qDebug("%f rad to xxºxx'xx: %s",teste4rad,teste4myType->radianoToDms(teste4rad)->toString().toStdString().c_str() );
	qDebug("%f rad to degree decimal: %f\n",teste4rad, Dms::radianoToDegreeDecimal(teste4rad) );

	qDebug("%f degree to xxºxx'xx: %s",teste4dec,teste4myType->degreeDecimalToDms(teste4dec)->toString().toStdString().c_str() );
	qDebug("%f degree to degree decimal: %f",teste4dec, Dms::degreeDecimalToRadiano(teste4dec) );
*/

	//box->setFixedSize(300,50);
	//box->show();

	return 0;//aplicat.exec();
}
