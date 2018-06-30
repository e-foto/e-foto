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

#include "ImageViewers.h"
#include <QDesktopWidget>
#include "StereoDisplay.h"
#include "SingleTools.h"
#include "DemFeatures.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

SingleViewer::SingleViewer(QWidget* parent) : QMainWindow(parent) {
    setupUi(this);
    sd_ = new SingleDisplay(this);
    setCentralWidget(sd_);
    tool_ = new SingleToolsBar(sd_, this);
    addToolBar(Qt::TopToolBarArea, tool_);
    statusBar()->addWidget(tool_->getInfo());
    addDockWidget(Qt::RightDockWidgetArea, tool_->getNearview());
    addDockWidget(Qt::RightDockWidgetArea, tool_->getOverview());
    QRect desktop = QDesktopWidget().screenGeometry();
    move(-pos().x() - width() / 2 + desktop.width() / 2,
         -pos().y() - height() / 2 + desktop.height() / 2);
}

void SingleViewer::closeEvent(QCloseEvent*) {
    if (tool_->showNearview->isChecked()) {
        tool_->showNearview->trigger();
    }

    if (tool_->showOverview->isChecked()) {
        tool_->showOverview->trigger();
    }

    delete tool_;
    delete sd_;
}

void SingleViewer::loadImage(QString filename) {
    SingleScene* ss = dynamic_cast<SingleScene*>(sd_->getCurrentScene());
    ss->loadImage(filename);
    tool_->setFitView->trigger();
}

void SingleViewer::loadImage(QImage* image) {
    SingleScene* ss = dynamic_cast<SingleScene*>(sd_->getCurrentScene());
    ss->loadImage(*image);
    tool_->setFitView->trigger();
}

void SingleViewer::loadImage(Matrix* image, bool isGrayscale) {
    if (image == nullptr) {
        return;
    }

    SingleScene* ss = dynamic_cast<SingleScene*>(sd_->getCurrentScene());
    ss->loadImage(image, isGrayscale);
    tool_->setFitView->trigger();
}

void SingleViewer::blockOpen() {
    tool_->setOpenVisible(false);
}

void SingleViewer::blockSave() {
    tool_->setSaveVisible(false);
}

void SingleViewer::blockMark() {
    tool_->setMarkVisible(false);
    tool_->mark.setToOnlyEmitClickedMode();
    tool_->near_.setNearCursor(QCursor(Qt::ArrowCursor));
}

void SingleViewer::setImageMode() {
    tool_->setImageMode();
}

void SingleViewer::setOrtoImageMode(double xi, double dx, double yi,
                                    double dy) {
    tool_->setOrtoImageMode(xi, dx, yi, dy);
}

void SingleViewer::setElevationImageMode(double xi, double dx, double yi,
        double dy, double zi, double dz) {
    tool_->setElevationImageMode(xi, dx, yi, dy, zi, dz);
}

MarkTool* SingleViewer::getMarker() {
    return &tool_->mark;
}


SeparatedStereoViewer::SeparatedStereoViewer(QWidget* parent) : QMainWindow(
        parent) {
    QWidget* centerWidget = new QWidget(this);
    QMainWindow* leftPart = new QMainWindow(this);
    QMainWindow* rightPart = new QMainWindow(this);
    leftDisplay_ = new SingleDisplay(this);
    rightDisplay_ = new SingleDisplay(this);
    tool = new SeparatedStereoToolsBar(leftDisplay_, rightDisplay_, this);
    addToolBar(Qt::LeftToolBarArea, tool);
    leftPart->setCentralWidget(leftDisplay_);
    leftPart->statusBar()->addWidget(tool->getLeftInfo());
    leftPart->addDockWidget(Qt::TopDockWidgetArea, tool->getLeftOverview());
    leftPart->addDockWidget(Qt::TopDockWidgetArea, tool->getLeftNearview());
    rightPart->setCentralWidget(rightDisplay_);
    rightPart->statusBar()->addWidget(tool->getRightInfo());
    rightPart->addDockWidget(Qt::TopDockWidgetArea, tool->getRightNearview());
    rightPart->addDockWidget(Qt::TopDockWidgetArea, tool->getRightOverview());
    QHBoxLayout* layout = new QHBoxLayout(0);
    layout->addWidget(leftPart);
    layout->addWidget(rightPart);
    centerWidget->setLayout(layout);
    setCentralWidget(centerWidget);
    QRect desktop = QDesktopWidget().screenGeometry();
    move(-pos().x() - width() / 2 + desktop.width() / 2,
         -pos().y() - height() / 2 + desktop.height() / 2);
}

void SeparatedStereoViewer::closeEvent(QCloseEvent*) {
    if (tool->showNearview->isChecked()) {
        tool->showNearview->trigger();
    }

    if (tool->showOverview->isChecked()) {
        tool->showOverview->trigger();
    }

    delete tool;
    delete leftDisplay_;
    delete rightDisplay_;
}

void SeparatedStereoViewer::loadLeftImage(QString filename) {
    SingleScene* ss = dynamic_cast<SingleScene*>(leftDisplay_->getCurrentScene());
    ss->loadImage(filename);
}

void SeparatedStereoViewer::loadRightImage(QString filename) {
    SingleScene* ss = dynamic_cast<SingleScene*>(rightDisplay_->getCurrentScene());
    ss->loadImage(filename);
}

void SeparatedStereoViewer::loadLeftImage(QImage* image) {
    SingleScene* ss = dynamic_cast<SingleScene*>(leftDisplay_->getCurrentScene());
    ss->loadImage(*image);
}

void SeparatedStereoViewer::loadRightImage(QImage* image) {
    SingleScene* ss = dynamic_cast<SingleScene*>(rightDisplay_->getCurrentScene());
    ss->loadImage(*image);
}

void SeparatedStereoViewer::loadLeftImage(Matrix* image, bool isGrayscale) {
    SingleScene* ss = dynamic_cast<SingleScene*>(leftDisplay_->getCurrentScene());
    ss->loadImage(image, isGrayscale);
}

void SeparatedStereoViewer::loadRightImage(Matrix* image, bool isGrayscale) {
    SingleScene* ss = dynamic_cast<SingleScene*>(rightDisplay_->getCurrentScene());
    ss->loadImage(image, isGrayscale);
}

void SeparatedStereoViewer::setFeatures(DemFeatures* df) {
    leftDisplay_->getCurrentScene()->geometry()->setFeatures(df, 0);
    rightDisplay_->getCurrentScene()->geometry()->setFeatures(df, 1);
}

void SeparatedStereoViewer::blockOpen() {
    tool->setOpenVisible(false);
}

void SeparatedStereoViewer::blockSave() {
    tool->setSaveVisible(false);
}

void SeparatedStereoViewer::blockMark() {
    tool->setMarkVisible(false);
}

MarkTool& SeparatedStereoViewer::getLeftMarker() {
    return (tool->leftMark);
}

MarkTool& SeparatedStereoViewer::getRightMarker() {
    return (tool->rightMark);
}

StereoViewer::StereoViewer(QWidget* parent) : QMainWindow(parent) {
    stereoDisplay_ = new StereoDisplay(this);
    stereoTool_ = new StereoToolsBar(stereoDisplay_, this);
    addToolBar(Qt::RightToolBarArea, stereoTool_);
    statusBar()->addWidget(stereoTool_->getStereoInfo());
    statusBar()->addWidget(stereoTool_->getLeftInfo());
    statusBar()->addWidget(stereoTool_->getRightInfo());
    setCentralWidget(stereoDisplay_);
    QDockWidget* overDock = stereoTool_->getOverviews();
    addDockWidget(Qt::BottomDockWidgetArea, overDock, Qt::Horizontal);
    QDockWidget* nearDock = stereoTool_->getNearviews();
    addDockWidget(Qt::BottomDockWidgetArea, nearDock, Qt::Horizontal);
}

MarkStereoTool& StereoViewer::getMarker() {
    return stereoTool_->_mark;
}



void StereoViewer::closeEvent(QCloseEvent*) {
}

void StereoViewer::loadLeftImage(QString filename) {
    stereoDisplay_->loadLeftImage(filename);
}

void StereoViewer::loadRightImage(QString filename) {
    stereoDisplay_->loadRightImage(filename);
}

void StereoViewer::loadLeftImage(QImage* image) {
    stereoDisplay_->loadLeftImage(image);
}

void StereoViewer::loadRightImage(QImage* image) {
    stereoDisplay_->loadRightImage(image);
}

void StereoViewer::loadLeftImage(Matrix* image, bool isGrayscale) {
    stereoDisplay_->loadLeftImage(image, isGrayscale);
}

void StereoViewer::loadRightImage(Matrix* image, bool isGrayscale) {
    stereoDisplay_->loadRightImage(image, isGrayscale);
}

void StereoViewer::setFeatures(DemFeatures* df) {
    stereoDisplay_->getCurrentScene()->getLeftScene()->geometry()->setFeatures(df,
            0);
    stereoDisplay_->getCurrentScene()->getRightScene()->geometry()->setFeatures(df,
            1);
}

void StereoViewer::blockOpen() {
    stereoTool_->setOpenVisible(false);
}

void StereoViewer::blockSave() {
    stereoTool_->setSaveVisible(false);
}

void StereoViewer::blockMark() {
    stereoTool_->setMarkVisible(false);
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
