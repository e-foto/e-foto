#ifndef CONVERSIONSLAYOUT_H
#define CONVERSIONSLAYOUT_H

#include "ui_ConvertionsForm.h"
#include "ConvertionsSystems.h"
#include <QWidget>

class ConversionsLayout: public QWidget, public Ui::convertionsLayout
{
	Q_OBJECT

public:
	ConversionsLayout(QWidget *parent=0);


public slots:
	void calculateUtmToGeo();
	void calculateGeoToUtm();


};

#endif // CONVERSIONSLAYOUT_H
