#ifndef SPUSERINTERFACE_QT_H
#define SPUSERINTERFACE_QT_H

#include "ui_Stereoplotter.h"
#include "SPUserInterface.h"
#include "ImageViewers.h"
#include "LoadingScreen.h"
#include "Matrix.h"
#include "TreeFeatures.h"
#include <QPoint>
#include <QGridLayout>
#include <QPushButton>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QMessageBox>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class SPUserInterface_Qt : public QMainWindow, public Ui::StereoPlotterForm, public SPUserInterface
{
	Q_OBJECT

public:

public slots:

protected:
	static SPUserInterface_Qt* spInst;
	SPUserInterface_Qt(SPManager* manager, QWidget* parent = 0, Qt::WindowFlags fl = Qt::Window);
	~SPUserInterface_Qt();
	SPManager *manager;
	SeparatedStereoViewer* viewer;
	SeparatedStereoToolsBar* tools;
	void closeEvent(QCloseEvent *e);
	int checkBoundingBox();
	void updateData();
	void updateTable();

protected slots:
	virtual void languageChange();
	void updateClass(int feat_type);
	void onLoadButton();
	void onSaveButton();
	void onAddButton();
	void onRemoveButton();
	void onRemoveAllButton();
	void changePair(int leftKey, int rightKey);

private:
	void init();
	//void close();
	bool allow_close;

public:
	static SPUserInterface_Qt* instance(SPManager* manager);
	// Other Methods
	//
	bool exec();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // DEMUSERINTERFACE_QT_H
