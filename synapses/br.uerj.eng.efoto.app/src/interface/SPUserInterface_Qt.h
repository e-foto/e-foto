#ifndef SPUSERINTERFACE_QT_H
#define SPUSERINTERFACE_QT_H

#include "ui_Stereoplotter.h"
#include "SPUserInterface.h"
#include "LoadingScreen.h"
#include "Matrix.h"
#include "TreeFeatures.h"
#include <QPoint>
#include <QGridLayout>
#include <QPushButton>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QMessageBox>

#ifdef INTEGRATED_EFOTO
#include "ImageViewers.h"
#endif //INTEGRATED_EFOTO REVER!

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

#ifdef INTEGRATED_EFOTO
	SeparatedStereoViewer* viewerSeparated;
	SeparatedStereoToolsBar* toolsSeparated;
	StereoViewer* viewer;
	StereoToolsBar* tools;
#endif //INTEGRATED_EFOTO REVER!

    TreeFeatures *tree;
    void closeEvent(QCloseEvent *e);
	void updateData();
	void updateTable();

public slots:
	virtual void languageChange();
	void updateClass(int feat_type);
	void onLoadButton();
	void onSaveButton();
	void onAddButton();
	void onRemoveButton();
	void onRemoveAllButton();
	void onAddPtButton();
	void onEditPtButton();
	void onSelPtButton();
	void onRemovePtButton();
	void onFeatureListClicked(QModelIndex index);
	void onChangePair(int pos);
	void changePair(int leftKey, int rightKey);
	void onFeatureSelected();
	void onCloseFeature();
	void stereoClicked(QPointF, QPointF);
    void stereoMoved(QPointF, QPointF);
    void adjustFit(int width, int height);

#ifdef INTEGRATED_EFOTO
    void centerImages(ObjectSpaceCoordinate coord, double zoom);
#endif //INTEGRATED_EFOTO REVER!

private:
	void init();
	//void close();
	bool allow_close;
	int measure_mode; // 0 - None, 1- Add, 2- Edit, 3- Select

public:
	static SPUserInterface_Qt* instance(SPManager* manager);
	// Other Methods
	//
	bool exec();
	void addImagePair(char *);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // DEMUSERINTERFACE_QT_H
