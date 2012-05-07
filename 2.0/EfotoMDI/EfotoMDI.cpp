#include "EfotoMDI.h"

#include <QList>
#include <QMdiSubWindow>
#include <QScrollArea>
#include <QVBoxLayout>

EfotoMDI::EfotoMDI(QWidget *parent) :
    QWidget(parent)
{
	setupUi(this);

	connect(headerButton,SIGNAL(clicked()),this,SLOT(openHeaderForm()));
	connect(terrainButton,SIGNAL(clicked()),this,SLOT(openTerrainForm()));
	connect(flightButton,SIGNAL(clicked()),this,SLOT(openFlightForm()));
	connect(sensorButton,SIGNAL(clicked()),this,SLOT(openSensorForm()));

}

void EfotoMDI::openHeaderForm()
{
	//qDebug("foi chamado header");

	if (isOpenHeaderForm())
	{
		//eFotoMdiArea->subWindowList();
	}
	else
	{
		header  = new HeaderForm();
		eFotoMdiArea->addSubWindow(header);
		header->setWindowTitle("HEADER");
		header->show();
		connect(header,SIGNAL(destroyed()),this,SLOT(closeHeaderForm()));
	}
}

void EfotoMDI::openFlightForm()
{
	QList<QMdiSubWindow*> lista = eFotoMdiArea->subWindowList();
	for(int i=0;i<lista.size();i++)
	{
		qDebug()<<lista.at(i)->widget()->windowTitle();
		//if(lista.at(i)->widget()->;
	}
	//qDebug("Foi chamado flight");
	if (isOpenFlightForm())
	{

	}
	else
	{
		flight  = new FlightForm();
		eFotoMdiArea->addSubWindow(flight);
		flight->setWindowTitle("FLIGHT");
		flight->show();
		connect(flight,SIGNAL(destroyed()),this,SLOT(closeFlightForm()));
	}
}

void EfotoMDI::openTerrainForm()
{
	//qDebug("foi chamado terrain");

	if(isOpenTerrainForm())
	{
	}
	else
	{
		terrain = new TerrainForm();
		eFotoMdiArea->addSubWindow(terrain);
		terrain->setWindowTitle("TERRAIN");
		terrain->show();
		connect(terrain,SIGNAL(destroyed()),this,SLOT(closeTerrainForm()));
	}
}

void EfotoMDI::openSensorForm()
{
	//qDebug("foi chamado sensor");
	if(isOpenSensorForm())
	{
	}
	else
	{
		sensor = new SensorForm();
		sensor->setWindowTitle("SENSOR");

		QScrollArea *area = new QScrollArea();
		area->setWidget(sensor);
		eFotoMdiArea->addSubWindow(area);
		area->show();
		connect(area,SIGNAL(destroyed()),this,SLOT(closeSensorForm()));
	}
}

bool EfotoMDI::isOpenHeaderForm()
{
	if (header!=NULL)
		return true;
	else
		return false;
}

bool EfotoMDI::isOpenTerrainForm()
{
	if (terrain!=NULL)
		return true;
	else
		return false;
}

bool EfotoMDI::isOpenFlightForm()
{
	if (flight!=NULL)
		return true;
	else
		return false;
}

bool EfotoMDI::isOpenSensorForm()
{
	if (sensor!=NULL)
		return true;
	else
		return false;
}

void EfotoMDI::closeHeaderForm()
{
	header = NULL;
}

void EfotoMDI::closeTerrainForm()
{
	terrain = NULL;
}

void EfotoMDI::closeSensorForm()
{
	sensor = NULL;
}

void EfotoMDI::closeFlightForm()
{
	flight = NULL;
}

