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

#include <QtGui>
#include "ProgressWindow_Qt.h"

ProgressWindow::ProgressWindow(QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);

	// Center window
	QDesktopWidget *desktop = QApplication::desktop();
	int Cx,Cy;
	QRect rect = geometry();
	Cx = (desktop->width() - rect.width())/2;
	Cy = (desktop->height() - rect.height())/2;
	move(Cx,Cy);

	allow_close = true;
}

void ProgressWindow::setProgress(int prog)
{
	ui.progressBar->setValue(prog);
}

void ProgressWindow::setDescription(QString desc)
{
	ui.label->setText(desc);
}

void ProgressWindow::closeEvent(QCloseEvent *e)
{
	if (!allow_close)
	{
		e->ignore();
		return;
	}
}
