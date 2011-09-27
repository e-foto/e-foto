#ifndef FLIGHTDIRECTIONFORM_H
#define FLIGHTDIRECTIONFORM_H

#include "ui_FlightDirectionForm.h"

class FlightDirectionForm : public QWidget, public Ui::FlightDirectionForm
{
	Q_OBJECT
public:
	FlightDirectionForm(QWidget *parent = 0);

public slots:
	void acceptValues();
	void setSpinValue(int value);
	void setDialValue(int value);

signals:
	void valuesFlightDirectionForm(QString,double);

};

#endif // FLIGHTDIRECTIONFORM_H
