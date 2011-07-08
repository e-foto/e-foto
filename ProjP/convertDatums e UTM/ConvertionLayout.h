#ifndef CONVERTIONSLAYOUT_H
#define CONVERTIONSLAYOUT_H

#include <QWidget>
#include "ui_ConvertionsForm.h"
#include "ConvertionsSystems.h"
#include "DmsEdit.h"
#include "GenericSystemDialog.h"

class ConvertionsLayout: public QWidget, public Ui::convertionLayout
{
	Q_OBJECT

protected:
    GeoSystem *oldSystem;
    GeoSystem *newSystem;
	GeoSystem *geoUtmSystem;

public:
    ConvertionsLayout(QWidget *parent=0);

public slots:
    void convertDatums();
    void changeOldEntryMode(int index);
    void changeNewEntryMode(int index);
protected slots:
    void systemChanged(int index);
    void callDialog(int);
    void acceptedOldSystem(double a, double f);
    void acceptedNewSystem(double a, double f);

	/** Entre Sistemas*/
    //void systemChangedGeoUtm(int index);
    void convertGeoUtmSystems();
    void convertUtmGeoSystems();
//    void rejected();

};

#endif // CONVERTIONSLAYOUT_H
