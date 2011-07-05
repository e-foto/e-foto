#include "ConvertionLayout.h"
#include "Matrix.h"

ConvertionsLayout::ConvertionsLayout(QWidget* parent)
{
    setupUi(this);
	setWindowTitle("Conversões");
	QApplication::setStyle("cleanlooks");
	//QApplication::setStyle("windows");

/** Entre Datums
  */
    oldSystem = new GeoSystem(WGS84);
    newSystem = new GeoSystem(SAD69);
    oldSystemComboBox->setCurrentIndex(3);
    newSystemComboBox->setCurrentIndex(1);
	//methodComboBox->setCurrentIndex(1);
    oldLatDmsEdit->getDmsValue()->setDms(30,20,52.161194,true);
    oldLatDmsEdit->updateValue();
    oldLongDmsEdit->getDmsValue()->setDms(60,45,22.108690,true);
    oldLongDmsEdit->updateValue();
    oldAltDoubleSpin->setValue(611.827);
    connect(convertButton, SIGNAL(clicked()), this, SLOT(convertDatums()));
    connect(oldTypeComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(changeOldEntryMode(int)));
    connect(newTypeComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(changeNewEntryMode(int)));
    connect(oldSystemComboBox,SIGNAL(activated(int)),this,SLOT(systemChanged(int)));
    connect(newSystemComboBox,SIGNAL(activated(int)),this,SLOT(systemChanged(int)));

  /** Entre Sistemas*/

    //Geo to UTM

    geoUtmSystem = new GeoSystem(WGS84);

    latGeotoUtmDmsEdit->getDmsValue()->setDms(23,0,0,true);
	latGeotoUtmDmsEdit->updateValue();
    longGeotoUtmDmsEdit->getDmsValue()->setDms(48,0,0,true);
	longGeotoUtmDmsEdit->updateValue();

    //hemiNSGeoUTMComboBox->setCurrentIndex(1);
	geoUTMOldSystemComboBox->setCurrentIndex(3);

	connect(geoUTMOldSystemComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(systemChangedGeoUtm(int)));
    connect(geoUTMOldSystemComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(convertGeoUtmSystems()));
    //connect(hemiNSGeoUTMComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(convertGeoUtmSystems()));
    //connect(hemiWEGeoUtmComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(convertGeoUtmSystems()));

    connect(latGeotoUtmDmsEdit,SIGNAL(textChanged(QString)),this,SLOT(convertGeoUtmSystems()));
    connect(longGeotoUtmDmsEdit,SIGNAL(textChanged(QString)),this,SLOT(convertGeoUtmSystems()));
    //connect(mCGeoToUtmDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(convertGeoUtmSystems()));


    // UTM to Geo

    hemiNSUtmGeoComboBox->setCurrentIndex(1);
    utmGeoOldSystemComboBox->setCurrentIndex(3);

    connect(utmGeoOldSystemComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(systemChangedGeoUtm(int)));
    connect(utmGeoOldSystemComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(convertUtmGeoSystems()));
    connect(hemiNSUtmGeoComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(convertUtmGeoSystems()));
    //connect(hemiWEUtmGeoComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(convertUtmGeoSystems()));

    connect(EUtmToGeoDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(convertUtmGeoSystems()));
    connect(NUtmToGeoDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(convertUtmGeoSystems()));
    connect(zonaUtmGeoSpinBox,SIGNAL(valueChanged(int)),this,SLOT(convertUtmGeoSystems()));

    //connect(latUtmToGeoDmsEdit,SIGNAL(textChanged(QString)),this,SLOT(convertGeoUtmSystems()));
    //connect(longUtmToGeoDmsEdit,SIGNAL(textChanged(QString)),this,SLOT(convertGeoUtmSystems()));
}

void ConvertionsLayout::convertDatums()
{

    double oldLat=oldLatDmsEdit->getDmsValue()->dmsToRadiano();
    double oldLong=oldLongDmsEdit->getDmsValue()->dmsToRadiano();

    double *converted;
	//if(methodComboBox->currentIndex()==0)
	  //  converted=ConvertionsSystems::convertSystems3Parameters(*oldSystem,*newSystem,oldLat,oldLong,oldAltDoubleSpin->value());
	//else if(methodComboBox->currentIndex()==1)
        converted=ConvertionsSystems::convertSystemsSimplifiedMolodensky(*oldSystem,*newSystem,oldLat,oldLong,oldAltDoubleSpin->value());
	//else if(methodComboBox->currentIndex()==2)
	  //  converted=ConvertionsSystems::convertSystemsFullMolodensky(*oldSystem,*newSystem,oldLat,oldLong,oldAltDoubleSpin->value());

    newLatDmsEdit->getDmsValue()->radianoToDms(converted[0]);
    newLongDmsEdit->getDmsValue()->radianoToDms(converted[1]);
    newAltDoubleSpin->setValue(converted[2]);

    newLatDmsEdit->updateValue();
    newLongDmsEdit->updateValue();

    newLatDmsEdit->setDmsEditMode(newLatDmsEdit->getDmsEditMode());
    newLongDmsEdit->setDmsEditMode(newLongDmsEdit->getDmsEditMode());

    newLatDmsEdit->updateValue();
    newLongDmsEdit->updateValue();

}

void ConvertionsLayout::changeNewEntryMode(int index)
{
	newLatDmsEdit->setDmsEditMode(DmsEditMode(index));
	newLongDmsEdit->setDmsEditMode(DmsEditMode(index));
}

void ConvertionsLayout::changeOldEntryMode(int index)
{
	oldLatDmsEdit->setDmsEditMode(DmsEditMode(index));
	oldLongDmsEdit->setDmsEditMode(DmsEditMode(index));
}

void ConvertionsLayout::systemChanged(int index)
{
	/*
    if (index ==4)
    {
        if (sender()->objectName()==oldSystemComboBox->objectName())
            callDialog(0);
        else
            callDialog(1);
    }
	else*/
    {
        if (sender()->objectName()==oldSystemComboBox->objectName())
            oldSystem->setSystem(GeoSystemEnum(index));
        else
            newSystem->setSystem(GeoSystemEnum(index));
    }

//    qDebug("%s\n%s",oldSystem->toString().c_str(),newSystem->toString().c_str());
}

void ConvertionsLayout::callDialog(int index)
{
    GenericSystemDialog *dialog = new GenericSystemDialog(this);
    dialog->setModal(true);
    dialog->show();
    dialog->setWindowTitle("Entre com os valores do sistema");

    if (index==0)
    {
        dialog->aDoubleSpin->setValue(oldSystem->getA());
        dialog->fDoubleSpin->setValue(oldSystem->getF());
        connect(dialog,SIGNAL(ok(double,double)),this,SLOT(acceptedOldSystem(double,double)));
    }
    else
    {
        dialog->aDoubleSpin->setValue(newSystem->getA());
        dialog->fDoubleSpin->setValue(newSystem->getF());
        connect(dialog,SIGNAL(ok(double,double)),this,SLOT(acceptedNewSystem(double,double)));
    }
}

void ConvertionsLayout::acceptedOldSystem(double a, double f)
{
    qDebug("old system\ta: %.6f f= %.6f",a,f);
    oldSystem->setSystem(a,f,"MyOldSystem");
}

void ConvertionsLayout::acceptedNewSystem(double a, double f)
{
    qDebug("new system\ta: %.6f f= %.6f",a,f);
    newSystem->setSystem(a,f,"MyNewSystem");
}


/** Entre Sistemas
  */

void ConvertionsLayout::systemChangedGeoUtm(int index)
{
	geoUtmSystem->setSystem(GeoSystemEnum(index));
}

void ConvertionsLayout::convertGeoUtmSystems()
{
    char hemNS,hemWE;

    /*if(hemiNSGeoUTMComboBox->currentIndex()==0)
        hemNS='n';
	else
        hemNS='s';
*/
    /*if(hemiWEGeoUtmComboBox->currentIndex()==0)
        hemWE='w';
    else
        hemWE='e';
*/
    Matrix utm=ConvertionsSystems::geoToUtmFran(latGeotoUtmDmsEdit->getRadianValue(),longGeotoUtmDmsEdit->getRadianValue(),0.0,*geoUtmSystem,hemNS,hemWE);

	EGeoToUtmDoubleSpinBox->setValue(utm.get(1,1));
	NGeoToUtmDoubleSpinBox->setValue(utm.get(1,2));
	mCGeoToUtmDoubleSpinBox->setValue(utm.get(1,4));
        zonaGeoUtmSpinBox->setValue(utm.getInt(1,5));

    EUtmToGeoDoubleSpinBox->setValue(utm.get(1,1));
    NUtmToGeoDoubleSpinBox->setValue(utm.get(1,2));
    mCUtmToGeoDoubleSpinBox->setValue(utm.get(1,4));
    zonaUtmGeoSpinBox->setValue(utm.getInt(1,5));

}

void ConvertionsLayout::convertUtmGeoSystems()
{
    char hemNS,hemWE;

    if(hemiNSUtmGeoComboBox->currentIndex()==0)
        hemNS='n';
    else
        hemNS='s';

    /*if(hemiWEUtmGeoComboBox->currentIndex()==0)
        hemWE='w';
    else
        hemWE='e';
*/
    Matrix geo=ConvertionsSystems::utmToGeoFran(EUtmToGeoDoubleSpinBox->value(),NUtmToGeoDoubleSpinBox->value(),0.0,zonaUtmGeoSpinBox->value(),*geoUtmSystem,hemNS,hemWE);

    longUtmToGeoDmsEdit->getDmsValue()->radianoToDms(geo.get(1,1));
    longUtmToGeoDmsEdit->updateValue();
    latUtmToGeoDmsEdit->getDmsValue()->radianoToDms(geo.get(1,2));
    latUtmToGeoDmsEdit->updateValue();
    mCUtmToGeoDoubleSpinBox->setValue(geo.get(1,4));
    //zonaUtmGeoSpinBox->setValue(geo.getInt(1,4));

}














