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
#ifndef BLOCKVIEW_H
#define BLOCKVIEW_H

#include <iostream>
#include <string>
#include <vector>

#include <QStandardItemModel>
#include <QItemDelegate>
#include <QString>
#include <QTableView>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QVariant>
#include <QLineEdit>
#include <QPainter>
#include <QStyleOptionViewItem>
#include "AbstractEModel.h"
#include "EDom.h"

using namespace std;

//mudar o nome desta classe

class ImageBlockDelegate : public QItemDelegate
 {
     Q_OBJECT

 public:
     ImageBlockDelegate(QObject *parent = 0);

     void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

};

class ConnectionBlock2Image;

class MatricialBlockModel : public AbstractEModel//QStandardItemModel
{
Q_OBJECT
private:
    int selectedRow;
    int SelectedCol, SelectedRow, BlockKey;
    string SensorsString, FlightsString;

    friend class ConnectionBlock2Image;
public:
        MatricialBlockModel(void);
        MatricialBlockModel(int inicialId,int blockkey,int rows, int colsint);
        void setSensorsXml(string Str) { SensorsString = Str; }
        void setFlightsXml(string Str) { FlightsString = Str; }
        void fillvalues(vector<string> StrVector);
        string getvalues();
        void showModel(void);

    public slots:
        void selectImage(const QModelIndex &index);
        string getSelected(void);
        void setSelected(string ImageXMLString);
};

class MatricialBlockView: public QTableView
{
public:
    MatricialBlockView(QWidget* parent, MatricialBlockModel* pFiductialMarksModel);
private:
    ImageBlockDelegate ImageIdDelegate;
};

class DigitalImageModel;
class DigitalImageView;

class ConnectionBlock2Image: public QObject
{
    MatricialBlockModel* myMatricialBlockModel;
    MatricialBlockView* myMatricialBlockView;
    DigitalImageModel* myDigitalImageModel;
    DigitalImageView* myDigitalImageView;
public:
    ConnectionBlock2Image(MatricialBlockModel* pMatricialBlockModel, MatricialBlockView* pMatricialBlockView,
                          DigitalImageModel* pDigitalImageModel, DigitalImageView* pDigitalImageView);

Q_OBJECT
public slots:
    void FromBlock2Image(const QModelIndex &index);
    void FromImage2Block( QStandardItem* );
};
#endif
