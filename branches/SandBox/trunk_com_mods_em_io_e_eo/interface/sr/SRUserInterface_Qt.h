#ifndef SRUSERINTERFACE_QT_H
#define SRUSERINTERFACE_QT_H

#include "ui_SRForm.h"
#include "SRUserInterface.h"
#include "ImageView.h"
#include "matrixview.h"
#include "TableIOEOWidget.h"

#include <QPoint>
#include <QGridLayout>
#include <QInputDialog>
#include <QStandardItemModel>
#include <QTableWidget>

class SRUserInterface_Qt : public QMainWindow, public Ui::SRMainWindow, public SRUserInterface
{
    Q_OBJECT

    protected slots:
    virtual void languageChange();

public slots:
	virtual void informState();
    virtual void receivePoint(QPoint p);
    virtual void receiveFlightDirection(QPoint p);
    virtual void actualizeSelection(QStandardItem* item);
    virtual void makeRepaint();
    virtual void activeSetMode();
    virtual void activeUnsetMode();
	virtual void activePanMode();
	virtual void activeZoomMode();
	virtual void activeInsertPoint();
    virtual void fitView();
	virtual void insertPoint();
    virtual bool calculateSR();
    virtual bool viewReport();
    virtual void testActivateSR();
    virtual void setFlight();
    virtual void acceptSR();

private:
    void init();
	void loadPointsTable();
    bool measurePoint(int id, int col, int lin);

protected:
	QWidget *windowReport;
    ImageView *myImageView;
    QGridLayout *imageLayout;
    QStandardItemModel *points;
	bool flightAvailable;
	QWidget* pointsOccurrenceList;
	QTableWidget* table2PointsOccurrence;
	deque<string> imagePoints, allPoints;

public:
    SRUserInterface_Qt(SRManager* manager, QWidget* parent = 0, Qt::WindowFlags fl = Qt::Window);
    ~SRUserInterface_Qt();
    bool exec();
};

#endif // SRUSERINTERFACE_QT_H
