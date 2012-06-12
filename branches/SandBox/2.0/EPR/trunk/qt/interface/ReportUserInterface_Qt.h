#ifndef REPORTUSERINTERFACE_QT_H
#define REPORTUSERINTERFACE_QT_H

#include "ui_ReportForm.h"
#include "ReportUserInterface.h"
//#include "ImageView.h"
#include "ImageViewers.h"
#include "LoadingScreen.h"
#include "EDom.h"

#include <QPoint>
#include <QGridLayout>
#include <QPushButton>
#include <QStandardItemModel>


#include <QtGui>

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
	ReportUserInterface_Qt(ReportManager* manager, QWidget* parent = 0, Qt::WindowFlags fl = Qt::Window);
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
