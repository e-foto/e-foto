#ifndef SPUSERINTERFACE_QT_H
#define SPUSERINTERFACE_QT_H
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

#include "ui_Stereoplotter.h"
#include "SPUserInterface.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class SeparatedStereoViewer;
class SeparatedStereoToolsBar;
class StereoViewer;
class StereoToolsBar;
class TreeFeatures;

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
    SeparatedStereoViewer* viewerSeparated;
    SeparatedStereoToolsBar* toolsSeparated;
    StereoViewer* viewer;
    StereoToolsBar* tools;
    TreeFeatures *tree;
    void closeEvent(QCloseEvent *e);
    void updateData();
    void updateTable();

public slots:
    virtual void languageChange();
    void updateClass(int feat_type);
    void onLoadButton();
    void onSaveButton();
    void onExportButton();
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
    void refreshFeatureSelectedData();
    void onCloseFeature();
    void stereoClicked(QPointF, QPointF);
    void stereoMoved(QPointF, QPointF);
    void centerImages(ObjectSpaceCoordinate coord, double zoom);
    void adjustFit(int width, int height);
    void setReverseLensGlasses(int option);
    void setColorMaskLeft(int option);
    void setColorMaskRight(int option);
    void onStereoModeChanged(int option);

private:
    void init();
    //void close();
    bool allow_close;
    int measure_mode; // 0 - None, 1- Add, 2- Edit, 3- Select
    // Anagliph colors: Red, Green, Blue, Cyan, Magenta, Yellow
    bool color_map[6][3];
    QString lastDir;

public:
    static SPUserInterface_Qt* instance(SPManager* manager);
    // Other Methods
    //
    bool exec();
    void addImagePair(char *);
    void showErrorMessage(QString msg);

};

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    AddDialog(SPManager* spmanager, QWidget *parent = 0);

private:
    SPManager *manager;
    QLabel *nameLabel;
    QLabel *typeLabel;
    QLabel *classLabel;
    QLineEdit *nameEdit;
    QComboBox *typeCombo;
    QComboBox *classCombo;
    QPushButton *okButton;
    QPushButton *cancelButton;

public slots:
    void updateClassCombo(int);
    void checkAcceptance(QString);
    void accept();
    void reject();

signals:
    void parametersAccepted(QString fname, int ftype, int fclass);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // DEMUSERINTERFACE_QT_H
