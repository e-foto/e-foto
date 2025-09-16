/**************************************************************************/
/*Copyright 2002-2025 e-foto team (UERJ)
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
#include "ReportUserInterface_Qt.h"

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QPixmap>
#include <QString>
#include <QVariant>
#include <QtWidgets>

#include "ETableWidget.h"
#include "LoadingScreen.h"
#include "ReportManager.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

ReportUserInterface_Qt* ReportUserInterface_Qt::ReportInst = NULL;

ReportUserInterface_Qt* ReportUserInterface_Qt::instance(
    ReportManager* manager) {
  if (ReportInst == NULL) {
    delete ReportInst;
    ReportInst = NULL;
  }
  if (ReportInst == NULL) {
    ReportInst = new ReportUserInterface_Qt(manager);
  }
  return ReportInst;
}

ReportUserInterface_Qt::ReportUserInterface_Qt(ReportManager* manager,
                                               QWidget* parent,
                                               Qt::WindowFlags fl)
    : QMainWindow(parent, fl) {
  setupUi(this);

  this->manager = manager;

  qApp->processEvents();
  init();
}

ReportUserInterface_Qt::~ReportUserInterface_Qt() {
  // no need to delete child widgets, Qt does it all for us
}

void ReportUserInterface_Qt::languageChange() { retranslateUi(this); }

void ReportUserInterface_Qt::init() {
  newTree();
  connect(generateButton, SIGNAL(clicked()), this, SLOT(saveEPR()));
}

void ReportUserInterface_Qt::closeEvent(QCloseEvent* e) {
  LoadingScreen::instance().show();
  qApp->processEvents();
  manager->returnProject();
  QMainWindow::closeEvent(e);
}

void ReportUserInterface_Qt::newTree() {
  QTreeWidget* treeWidget = new QTreeWidget(this);

  connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this,
          SLOT(selectAllChildren(QTreeWidgetItem*)));

  connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this,
          SLOT(unselectFatherByKid(QTreeWidgetItem*)));

  treeWidget->clear();
  treeItems.clear();

  QTreeWidgetItem* item = new QTreeWidgetItem(treeWidget);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Select All");
  item->setExpanded(true);
  treeItems.push_back(item);
  QTreeWidgetItem* fatherTree = item;

  item = new QTreeWidgetItem(fatherTree);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Header");
  treeItems.push_back(item);

  item = new QTreeWidgetItem(fatherTree);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Terrain");
  treeItems.push_back(item);

  item = new QTreeWidgetItem(fatherTree);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Sensors");
  treeItems.push_back(item);

  item = new QTreeWidgetItem(fatherTree);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Flights");
  treeItems.push_back(item);

  item = new QTreeWidgetItem(fatherTree);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Images");
  treeItems.push_back(item);
  QTreeWidgetItem* n1RootItem = item;

  item = new QTreeWidgetItem(n1RootItem);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Metadata");
  treeItems.push_back(item);

  item = new QTreeWidgetItem(n1RootItem);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Orientation parameters");
  treeItems.push_back(item);

  item = new QTreeWidgetItem(fatherTree);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Points");
  treeItems.push_back(item);
  n1RootItem = item;

  item = new QTreeWidgetItem(n1RootItem);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Coordinates in Object Space Frame");
  treeItems.push_back(item);

  item = new QTreeWidgetItem(n1RootItem);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Coordinates in Image Space Frame");
  treeItems.push_back(item);

  item = new QTreeWidgetItem(fatherTree);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Affine Transformation");
  treeItems.push_back(item);
  n1RootItem = item;

  item = new QTreeWidgetItem(n1RootItem);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Initialization data");
  treeItems.push_back(item);

  item = new QTreeWidgetItem(n1RootItem);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Quality data");
  treeItems.push_back(item);

  item = new QTreeWidgetItem(fatherTree);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Spatial Resection");
  treeItems.push_back(item);
  n1RootItem = item;

  item = new QTreeWidgetItem(n1RootItem);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Initialization data");
  treeItems.push_back(item);

  item = new QTreeWidgetItem(n1RootItem);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Quality data");
  treeItems.push_back(item);

  item = new QTreeWidgetItem(n1RootItem);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Values at each iteration");
  treeItems.push_back(item);

  item = new QTreeWidgetItem(fatherTree);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Photogrammetric Block");
  treeItems.push_back(item);
  n1RootItem = item;

  item = new QTreeWidgetItem(n1RootItem);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Initialization data");
  treeItems.push_back(item);

  item = new QTreeWidgetItem(n1RootItem);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Values at each iteration");
  treeItems.push_back(item);

  item = new QTreeWidgetItem(fatherTree);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Interior Orientation");
  treeItems.push_back(item);
  n1RootItem = item;

  item = new QTreeWidgetItem(n1RootItem);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Xa");
  treeItems.push_back(item);

  item = new QTreeWidgetItem(n1RootItem);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Matrix V");
  treeItems.push_back(item);

  item = new QTreeWidgetItem(n1RootItem);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "SigmaXa");
  treeItems.push_back(item);

  item = new QTreeWidgetItem(fatherTree);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Exterior Orientation");
  treeItems.push_back(item);
  n1RootItem = item;

  item = new QTreeWidgetItem(n1RootItem);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "EO Type");
  treeItems.push_back(item);

  item = new QTreeWidgetItem(n1RootItem);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Number of Iterations for Convergence");
  treeItems.push_back(item);

  item = new QTreeWidgetItem(n1RootItem);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Xa");
  treeItems.push_back(item);

  item = new QTreeWidgetItem(n1RootItem);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Matrix V");
  treeItems.push_back(item);

  item = new QTreeWidgetItem(n1RootItem);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "SigmaXa");
  treeItems.push_back(item);

  item = new QTreeWidgetItem(fatherTree);
  item->setCheckState(0, Qt::Checked);
  item->setText(0, "Stereo Pairs");

  manager->checkTree(treeItems);

  treeWidget->setCurrentItem(treeItems.at(0));
  treeWidget->header()->hide();

  treeArea->setLayout(new QVBoxLayout());
  treeArea->layout()->addWidget(treeWidget);
}

bool ReportUserInterface_Qt::exec() {
  this->show();
  LoadingScreen::instance().close();
  qApp->processEvents();
  return true;
}

void ReportUserInterface_Qt::selectAllChildren(QTreeWidgetItem* father) {
  int numChild = father->childCount();

  if (father->checkState(0) == Qt::Checked) {
    for (int i = 0; i < numChild; i++) {
      QTreeWidgetItem* aux = father->child(i);
      if (!aux->isDisabled()) aux->setCheckState(0, Qt::Checked);
      if (aux->childCount() != 0) {
        selectAllChildren(aux);
      }
    }
  } else if (father->checkState(0) == Qt::Unchecked) {
    for (int i = 0; i < numChild; i++) {
      QTreeWidgetItem* aux = father->child(i);
      aux->setCheckState(0, Qt::Unchecked);
      if (aux->childCount() != 0) {
        selectAllChildren(aux);
      }
    }
  }
}

void ReportUserInterface_Qt::unselectFatherByKid(QTreeWidgetItem* kid) {
  QTreeWidgetItem* father = kid->parent();

  if (father == NULL) return;

  int numChild = father->childCount();
  int count_uncheck = 0;
  int count_check = 0;

  for (int i = 0; i < numChild; i++) {
    if (!father->child(i)->isDisabled()) {
      if (father->child(i)->checkState(0) == Qt::Unchecked) {
        ++count_uncheck;
      } else if (father->child(i)->checkState(0) == Qt::Checked) {
        ++count_check;
      } else if (father->child(i)->checkState(0) == Qt::PartiallyChecked) {
        father->setCheckState(0, Qt::PartiallyChecked);
      }
    }
  }

  if (count_uncheck > 0) {
    if (count_uncheck == numChild) {
      father->setCheckState(0, Qt::Unchecked);
    } else {
      father->setCheckState(0, Qt::PartiallyChecked);
    }
  }

  if (count_check > 0) {
    if (count_check == numChild) {
      father->setCheckState(0, Qt::Checked);
    }
  }
  unselectFatherByKid(father);
}

bool ReportUserInterface_Qt::wait(QProcess& p, QString msg = tr("Error")) {
  p.waitForFinished();
  if (p.exitStatus()) {
    QMessageBox::critical(this, msg, p.errorString());
    return false;
  }
  QString status = QString::fromLocal8Bit(p.readAllStandardError());
  if (!status.isEmpty()) {
    QMessageBox::critical(this, msg, status);
    return false;
  }
  return true;
}

bool ReportUserInterface_Qt::saveEPR() {
  QFileDialog salvar(this, tr("Save File As"), ".", tr("*.xml;;*.txt;;*.html"));
  salvar.setAcceptMode(QFileDialog::AcceptSave);

#ifdef Q_OS_WIN64
  QSettings binSettings("uerj", "efoto");
  QString binPath = binSettings.value("binPath").toString().replace("/", "\\");
#endif

  if (salvar.exec()) {
    QString chosenName = salvar.selectedFiles()[0];
    QString chosenExtension = salvar.selectedNameFilter().remove('*');
    QString tmp, path, filename, xml;
    int idExt = XMLTYPE;
    if (chosenName.isEmpty()) {
      return false;
    } else {
      QFileInfo output(chosenName);
      path = output.path();
      filename = output.fileName();
      if (!filename.toLower().endsWith(chosenExtension)) {
        filename.append(chosenExtension);
      }
      tmp = "tmp_" + filename;

      if (chosenExtension.toStdString() == ".txt")
        idExt = TXTTYPE;
      else if (chosenExtension.toStdString() == ".html")
        idExt = HTMTYPE;
      else
        tmp = filename;
      xml = path + "/" + tmp;
    }

    if (!manager->makeFile(xml.toUtf8().data(), idExt, treeItems)) {
      QMessageBox::critical(this, tr("Error"), tr("Unable to write report!"));
      return false;
    }

    bool doneXslt = false;
    if (idExt == XMLTYPE) {
      QMessageBox::information(this, tr("Process done"),
                               tr("Successfully saved report!"));
      return true;
    } else {
      doneXslt = manager->makeXslt(idExt, path.toUtf8().data());
    }

    if (doneXslt) {
      QProcess pro;
      pro.setWorkingDirectory(path);
      QStringList test_args, main_args, remove_args;
      main_args << "-o" << filename << "epr.xsl" << tmp;
#ifdef unix  // LINUX
      QString test_cmd = "sh", main_cmd = "xsltproc", remove_cmd = "rm";
      test_args << "-c" << main_cmd;
#endif
#ifdef Q_OS_WIN64  // WINDOWS
      QString test_cmd = "cmd", main_cmd = binPath + "\\" + "xsltproc.exe",
              remove_cmd = "cmd";
      test_args << "/C" << main_cmd;
      remove_args << "/C" << "del";
#endif
      remove_args << "epr.xsl" << tmp;

      pro.start(test_cmd, test_args);
      if (!wait(pro, tr("Checking xslt processor health"))) return false;

      pro.start(main_cmd, main_args);
      if (!wait(pro, tr("Processing the report transformation"))) return false;

      pro.start(remove_cmd, remove_args);
      if (!wait(pro, tr("Removing temporary files"))) return false;

      pro.kill();

      QMessageBox::information(this, tr("Process done"),
                               tr("Successfully saved report!"));

      return true;
    } else {
      QMessageBox::critical(
          this, tr("Error"),
          tr("A configuration file for the report could not created!"));
      return false;
    }
  }
  return false;
}

}  // namespace efoto
}  // namespace eng
}  // namespace uerj
}  // namespace br
