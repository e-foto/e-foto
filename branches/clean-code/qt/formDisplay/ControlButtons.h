#ifndef CONTROLBUTTONS_H
#define CONTROLBUTTONS_H
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

#include <QWidget>

class QPushButton;
class QHBoxLayout;

class ControlButtons : public QWidget
{
	Q_OBJECT

public:

	QPushButton* editButton;
	QPushButton* deleteButton;
	QPushButton* newButton;
	QPushButton* saveButton;
	QPushButton* cancelButton;
	QHBoxLayout* mainLayout;

	ControlButtons();

public slots:
    virtual void toSingleMode();
	virtual void toMultiMode();
	virtual void toEditMode();
	virtual void toInvisibleMode();
	virtual void disconnectAll();
	virtual void singleConnect();
	virtual void multiConnect();
};

#endif // CONTROLBUTTONS_H
