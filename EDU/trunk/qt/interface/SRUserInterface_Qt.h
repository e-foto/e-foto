#ifndef SRUSERINTERFACE_QT_H
#define SRUSERINTERFACE_QT_H

#include "ui_SRForm.h"
#include "SRUserInterface.h"
#include "ImageViewers.h"
#include "LoadingScreen.h"
#include "FlightDirectionForm.h"

#include <QPoint>
#include <QGridLayout>
#include <QInputDialog>
#include <QStandardItemModel>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {


class SRUserInterface_Qt : public QMainWindow, public Ui::SRMainWindow, public SRUserInterface
{
	Q_OBJECT

protected slots:
	virtual void languageChange();

public slots:
	virtual void informState();
        virtual void receivePoint(QPointF p);
        virtual void setFlightDirection(QString,double);
        virtual void updateDisplayedPoints();
        virtual void updateSelection(QStandardItem* item);
        virtual void updateAll();
	virtual void makeRepaint();
	virtual void activeSetMode();
	virtual void activeUnsetMode();
	virtual void activePanMode();
	virtual void activeZoomMode();
	virtual void fitView();
	virtual bool calculateSR();
	virtual bool viewReport();
	virtual void testActivateSR();
	virtual void setFlight();
	virtual void acceptSR();
        virtual void EOdone();

private:
	void init();
	bool measurePoint(int id, double col, double lin);

protected:
	static SRUserInterface_Qt* srInst;
	SRUserInterface_Qt(SRManager* manager, QWidget* parent = 0, Qt::WindowFlags fl = Qt::Window);
	~SRUserInterface_Qt();
	QWidget *windowReport;
	//ImageView *oldImageView;
	Marker *markOn;
	Marker *markOff;
	SingleViewer *imageView;
	QGridLayout *imageLayout;
	QStandardItemModel *points;
	bool flightAvailable;
	FlightDirectionForm *flightDirectionForm;
	void closeEvent(QCloseEvent *e);

public:
	static SRUserInterface_Qt* instance(SRManager* manager);
	bool exec();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // SRUSERINTERFACE_QT_H
