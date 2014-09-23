#ifndef IOUSERINTERFACE_QT_H
#define IOUSERINTERFACE_QT_H
/*Copyright 2002-2014 e-foto team (UERJ)
  This file is part of e-foto.

    e-foto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    e-foto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with e-foto.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "ui_IOForm.h"
#include "IOUserInterface.h"

#include <QGridLayout>
#include <QStandardItemModel>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class SingleViewer;
class Marker;

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
        virtual void OIdone();

protected:
	static IOUserInterface_Qt* ioInst;
	IOUserInterface_Qt(IOManager* manager, QWidget* parent = 0, Qt::WindowFlags fl = Qt::Window);
	~IOUserInterface_Qt();
	QWidget *windowReport;
	//ImageView *oldImageView;
	Marker *mark;
	SingleViewer *imageView;
	QGridLayout *imageLayout;
	QStandardItemModel *points;
	int selectedPoint;
	int calculationMode;
	IOManager *manager;
	void closeEvent(QCloseEvent *e);

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
