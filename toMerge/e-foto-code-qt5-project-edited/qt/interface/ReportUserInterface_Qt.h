#ifndef REPORTUSERINTERFACE_QT_H
#define REPORTUSERINTERFACE_QT_H
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

#include "ui_ReportForm.h"
#include "ReportUserInterface.h"

class QTreeWidgetItem;

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class ReportUserInterface_Qt : public QMainWindow, public Ui::ReportMainWindow, public ReportUserInterface
{
	Q_OBJECT

public:

public slots:

	void selectAllChildren(QTreeWidgetItem* father);
	void unselectFatherByKid(QTreeWidgetItem* kid);
    bool saveEPR();

protected:
	static ReportUserInterface_Qt* ReportInst;
    explicit ReportUserInterface_Qt(ReportManager* manager, QWidget* parent = 0, Qt::WindowFlags fl = Qt::Window);
	~ReportUserInterface_Qt();
	ReportManager *manager;        
	void closeEvent(QCloseEvent *e);
	void newTree();        
        QList<QTreeWidgetItem*> treeItems;

protected slots:
	virtual void languageChange();

private:
        void init();

public:
	static ReportUserInterface_Qt* instance(ReportManager* manager);

        // Other Methods
	//
        bool exec();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // REPORTUSERINTERFACE_QT_H
