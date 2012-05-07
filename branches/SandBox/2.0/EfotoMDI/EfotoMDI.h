#ifndef EFOTOMDI_H
#define EFOTOMDI_H

#include <QWidget>
#include "ui_EfotoMDIForm.h"

#include "HeaderForm.h"
#include "TerrainForm.h"
#include "FlightForm.h"
#include "SensorForm.h"

class EfotoMDI : public QWidget , public Ui::efotoMdiForm
{
Q_OBJECT
public:
	EfotoMDI(QWidget *parent = 0);

public slots:
	void openHeaderForm();
	void openTerrainForm();
	void openFlightForm();
	void openSensorForm();

	bool isOpenHeaderForm();
	bool isOpenTerrainForm();
	bool isOpenFlightForm();
	bool isOpenSensorForm();

	void closeHeaderForm();
	void closeFlightForm();
	void closeSensorForm();
	void closeTerrainForm();


private:

	HeaderForm  *header;
	TerrainForm *terrain;
	FlightForm  *flight;
	SensorForm  *sensor;


};

#endif // EFOTOMDI_H
