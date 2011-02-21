#ifndef IOUSERINTERFACE_QT_H
#define IOUSERINTERFACE_QT_H

#include "ui_IOForm.h"
#include "IOUserInterface.h"
#include "ImageView.h"
#include "matrixview.h"
#include "TableIOEOWidget.h"


#include <QPoint>
#include <QGridLayout>
#include <QPushButton>
#include <QStandardItemModel>

class IOUserInterface_Qt : public QMainWindow, public Ui::IOMainWindow, public IOUserInterface
{
	Q_OBJECT

public:
	IOUserInterface_Qt(IOManager* manager, QWidget* parent = 0, Qt::WindowFlags fl = Qt::Window);
	~IOUserInterface_Qt();

public slots:
	virtual void informState();
	virtual void receiveMark(QPoint p);
	virtual void makeRepaint();
	virtual void activeSetMode();
	virtual void activePanMode();
	virtual void activeZoomMode();
	virtual void fitView();
	virtual bool calculateIO();
	virtual bool viewReport();
	virtual void testActivateIO();
	virtual void acceptIO();

protected:

	QWidget *windowReport;
	ImageView *myImageView;
	QGridLayout *imageLayout;
	QStandardItemModel *points;
	int selectedPoint;
	int calculationMode;
	IOManager *manager;

protected slots:
	virtual void languageChange();

private:
	void init();
	bool measureMark(int id, int col, int lin);

public:
	// Other Methods
	//
	bool exec();

};

#endif // IOUSERINTERFACE_QT_H
