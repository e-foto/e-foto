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

#ifndef POINTSVIEW_H
#define POINTSVIEW_H

#include <vector>
#include <iostream>
#include <string>
#include <stdlib.h> 
#include <deque>

#include <QStandardItemModel>
#include <QString>
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QVariant>
#include <QPushButton>
#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QComboBox>
#include <QLabel>
#include <QGroupBox>
#include <QSortFilterProxyModel>
#include <QAbstractItemModel>
#include <QDialog>
#include <QLineEdit>
#include <QDataWidgetMapper>
#include <QProxyModel>
#include <QtGui>

#include "AbstractEModel.h"
#include "EDom.h"

using namespace std;

class TypeComboBoxDelegate : public QItemDelegate
 {
     Q_OBJECT

 public:
     TypeComboBoxDelegate(QObject *parent = 0);

     QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;

     void setEditorData(QWidget *editor, const QModelIndex &index) const;
     void setModelData(QWidget *editor, QAbstractItemModel *pointmodel,
                       const QModelIndex &index) const;

     void updateEditorGeometry(QWidget *editor,
         const QStyleOptionViewItem &option, const QModelIndex &index) const;
 };

class PointsModel : public AbstractEModel  
{
    Q_OBJECT
private:
    int selectedRow;
    int  numberpoints;

public:
    PointsModel(void);
    void fillvalues(vector <string> StrVector);
    QStandardItemModel *startDataPoint;
    string getvalues();
    void showModel(void);
    string inttostring(int intconvert);
    string getNextkey();
    void fillPoint(string PointTag, int row);
    int getrow(string gcp_id);

public slots:	
    void removepoint(string strremovepoint);
    void addpoint(void);
    void selectRow(const QModelIndex &index);
    void deleteSelectedRow(void);
};

class PointsLayout : public QVBoxLayout
{
    Q_OBJECT
public:
    PointsLayout(QWidget *);
    ~PointsLayout();
    
    friend class PointsView;

    QTableView *PointsTable;
    QVBoxLayout *tableLayout;
    QHBoxLayout  *buttonHLayout;
};

class PointsView : public QWidget
{
    Q_OBJECT

public:
    PointsView(QWidget *parent = 0,  QAbstractItemModel* pointmodel=0);
    ~PointsView();
    QTableView* getPointsTable(void)
    {
        return pointslayout->PointsTable;
    }
    // este método é uma gambiarra!!!
    void hideColumns(void)
    {
        pointslayout->PointsTable->hideColumn(0);
        pointslayout->PointsTable->hideColumn(8);
    }

    private:
    PointsLayout * pointslayout;
    TypeComboBoxDelegate typedelegate;
};

class SinglePointModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    void setSourceModel( QAbstractItemModel* paramSourceModel);

public slots:
    void selectRow(QString PointKey);
    void selectRow(const QModelIndex & index);
};

class SinglePointMapper;
class SinglePointView;

class SinglePointLayout : public QHBoxLayout
{
    Q_OBJECT
public:
    SinglePointLayout(QWidget *);
    ~SinglePointLayout();

    friend class SinglePointMapper;
    friend class SinglePointView;

private:
    QLabel *gcpIDLabel, *typeLabel, *descriptionLabel, *spatialCoord01Label,
    *spatialCoord02Label, *spatialCoord03Label,  *sigmaLabel;
    QTextEdit *descriptionTextEdit;
    QLineEdit *gcpIDLineEdit, *spatialCoord01LineEdit,
    *spatialCoord02LineEdit, *spatialCoord03LineEdit,
    *sigmaLineEdit;

    QGroupBox *spatialCoordGroupBox;

    QComboBox *typeComboBox;

    QHBoxLayout *gcpIDLayout,*typeLayout, *descriptionLayout,
    *spatialCoordHLayout, *sigmaLayout;

    QVBoxLayout *spatialCoordVLayout, *verticalGeneralLayout;

};

class SinglePointMapper : public QDataWidgetMapper
{
    Q_OBJECT
public:
    void makeMapper(SinglePointLayout* getlayoutMapper,  QStandardItemModel *getMapperModel);
};

class SinglePointView : public QWidget
{
    Q_OBJECT

public:
    SinglePointView(QWidget *parent = 0,   QAbstractItemModel* singlepointmodel=0);
    ~SinglePointView();
    void setToUTM(void);
    void setReadOnly(bool enabled);

private:
    QVariant indexGridsType;
    SinglePointLayout *singlepointlayout;
    SinglePointMapper singlepointmapper;

public slots:
    void remap(void);
    void clean(void);
};

#endif
