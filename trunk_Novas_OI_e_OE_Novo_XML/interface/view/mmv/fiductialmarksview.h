/***************************************************************************
 *   E-Foto Project: Digital Photogrammetric Workstation                   *
 *   Rio de Janeiro State University                                       *
 *   Program of Post-Graduation in Geomatics                               *
 *                                                                         *
 *   Copyright (C) 2010 by E-Foto Project                                  *
 *                                                                         *
 *   Developers:           Guilherme Lucio Abelha Mota                     *
 *                         Marcos Vinícius Meffe                           *
 *                                                                         *
 *   contact:  guimota at ime.uerj.br                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef INTERIORORIENTAIONVIEW_H
#define INTERIORORIENTAIONVIEW_H

#include <iostream>
#include <string>
#include <vector>

#include <QStandardItemModel>
#include <QString>
#include <QTableView>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QVariant>

#include "AbstractEModel.h"
#include "EDom.h"

using namespace std;

class FiductialMarksModel : public AbstractEModel//QStandardItemModel
{
Q_OBJECT
private:
    int selectedRow;
public:
        FiductialMarksModel(vector<string> StrVector);
	int NumberRows;
        QString NewCol, NewRow;
        void fillvalues(vector<string> StrVector);
	string getvalues();
	void showModel(void);
    public slots:
        void selectRow(const QModelIndex &index);
        void measure(QString Col, QString Row);
        void measure(void);
        void setNewCol(QString Col);
        void setNewRow(QString Row);
};

class FiductialMarksView: public QTableView
{
public:
    FiductialMarksView(QWidget* parent, FiductialMarksModel* pFiductialMarksModel);
};

#endif
