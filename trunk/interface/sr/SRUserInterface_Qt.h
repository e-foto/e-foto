#ifndef SRUSERINTERFACE_QT_H
#define SRUSERINTERFACE_QT_H

#include "ui_SRForm.h"
#include "SRUserInterface.h"
#include "ImageView.h"
#include "matrixview.h"

#include <QPoint>
#include <QGridLayout>
#include <QInputDialog>
#include <QStandardItemModel>

class SRUserInterface_Qt : public QMainWindow, public Ui::SRMainWindow, public SRUserInterface
{
    Q_OBJECT

    protected slots:
    virtual void languageChange();

public slots:
    virtual void receivePoint(QPoint p);
    virtual void receiveFlightDirection(QPoint p);
    virtual void actualizeSelection(QStandardItem* item);
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

private:
    void init();
    bool measurePoint(int id, int col, int lin);

protected:
    ImageView *myImageView;
    QGridLayout *imageLayout;
    QStandardItemModel *points;
    bool flightAvailable;

public:
    SRUserInterface_Qt(SRManager* manager, QWidget* parent = 0, Qt::WindowFlags fl = Qt::Window);
    ~SRUserInterface_Qt();
    bool exec();
};

#endif // SRUSERINTERFACE_QT_H
