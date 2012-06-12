#ifndef IOUSERINTERFACE_QT_H
#define IOUSERINTERFACE_QT_H

#include "ui_IOForm.h"
#include "IOUserInterface.h"
#include "LoadingScreen.h"

#include <QPoint>
#include <QGridLayout>
#include <QPushButton>
#include <QStandardItemModel>

#ifdef INTEGRATED_EFOTO
#include "ImageViewers.h"
#endif //INTEGRATED_EFOTO

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class IOUserInterface_Qt : public QMainWindow, public Ui::IOMainWindow, public IOUserInterface
{
	Q_OBJECT

public:

public slots:
	virtual void informState();
	virtual void receiveMark(QPointF p);
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
	static IOUserInterface_Qt* ioInst;
	IOUserInterface_Qt(IOManager* manager, QWidget* parent = 0, Qt::WindowFlags fl = Qt::Window);
	~IOUserInterface_Qt();
    QWidget *windowReport;
	QGridLayout *imageLayout;
	QStandardItemModel *points;
	int selectedPoint;
	int calculationMode;
	IOManager *manager;
	void closeEvent(QCloseEvent *e);

#ifdef INTEGRATED_EFOTO
    Marker *mark;
    SingleViewer *imageView;
#endif //INTEGRATED_EFOTO

protected slots:
	virtual void languageChange();

private:
	void init();
	bool measureMark(int id, double col, double lin);

public:
	static IOUserInterface_Qt* instance(IOManager* manager);
	// Other Methods
	//
	bool exec();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // IOUSERINTERFACE_QT_H
