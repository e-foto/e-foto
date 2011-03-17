#include "ConversionsLayout.h"
#include "Matrix.h"

ConversionsLayout::ConversionsLayout(QWidget* parent)
{
	setupUi(this);

	latUtmToGeoDmsEdit->getDmsValue()->setSecondsPrecision(2);
	longUtmToGeoDmsEdit->getDmsValue()->setSecondsPrecision(2);

	EUtmToGeoDoubleSpinBox->setValue(404853.4314);
	NUtmToGeoDoubleSpinBox->setValue(3374982.8875);
	mcUtmToGeoDoubleSpin->setValue(81);

	//	connect(latUtmtoGeoDmsEdit,SIGNAL(editingFinished()),this,SLOT(calculateUtmToGeo()));
	//connect(longUtmtoGeoDmsEdit,SIGNAL(editingFinished()),this,SLOT(calculateUtmToGeo()));
	connect(calculateUtmButton,SIGNAL(clicked()),this,SLOT(calculateGeoToUtm()));
	connect(calculateGeoButton,SIGNAL(clicked()),this,SLOT(calculateUtmToGeo()));

}

void ConversionsLayout::calculateGeoToUtm()
{
	//qDebug("metodo chamado");
	ConvertionsSystems conv;
	Matrix converted=conv.geoElipToUtm(latGeotoUtmDmsEdit->getRadianValue(),longGeotoUtmDmsEdit->getRadianValue(),0.0,'n');

	EGeoToUtmDoubleSpinBox->setValue(converted.get(1,1));
	NGeoToUtmDoubleSpinBox->setValue(converted.get(1,2));
	mcGeoToUtmDoubleSpin->setValue(converted.get(1,4));
}

void ConversionsLayout::calculateUtmToGeo()
{
	ConvertionsSystems conv;
	Matrix converted=conv.uTmToGeo(EUtmToGeoDoubleSpinBox->value(),NUtmToGeoDoubleSpinBox->value(),0.0,mcUtmToGeoDoubleSpin->value(),'n');

	Dms temp;
	temp.radianoToDms(converted.get(1,1));
	Dms temp2;
	temp2.radianoToDms(converted.get(1,2));

	latUtmToGeoDmsEdit->setNewDms(&temp);
	longUtmToGeoDmsEdit->setNewDms(&temp2);

	latUtmToGeoDmsEdit->setText(latUtmToGeoDmsEdit->getDmsValue()->toString(2));
	longUtmToGeoDmsEdit->setText(longUtmToGeoDmsEdit->getDmsValue()->toString(2));

}
