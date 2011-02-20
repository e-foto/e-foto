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

#ifndef PROJECTHEADERVIEW_H
#define PROJECTHEADERVIEW_H

#include <QAbstractItemView>
#include <QStandardItemModel>
#include <QDataWidgetMapper>
#include <QString>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QString>
#include <iostream>
#include <vector>
#include <string>
#include <QDialog>
#include "AbstractEModel.h"
#include "EDom.h"

using namespace std;

class ProjectHeaderModel : public AbstractEModel 
{
public:
	ProjectHeaderModel(void);
	void fillvalues(vector <string>);
	string getvalues();
};

class ProjectHeaderMapper; 

class ProjectHeaderLayout : public QGridLayout
{
    Q_OBJECT
public:
    ProjectHeaderLayout(QWidget *);
    ~ProjectHeaderLayout();

friend class ProjectHeaderMapper; 

private:
    QTextEdit   *DescriptionTextEdit; 
      
    QLineEdit 	*NameLineEdit, *FilePathLineEdit, *FileNameLineEdit, 
    			*OwnerLineEdit, *AimsLineEdit, *ContextLineEdit;

	QDateTimeEdit *CreationDateTimeEdit, *ModificationDateTimeEdit;

	QLabel 		*NameLabel, *DescriptionLabel, *FilePathLabel,
				*FileNameLabel, *CreationLabel, *ModificationLabel, 
				*OwnerLabel, *AimsLabel, *ContextLabel;
				
	QGridLayout *mainGridLayout;
	
	QVBoxLayout *mainVBoxLayout;
	
	QHBoxLayout *mainHBoxLayout;
};

class ProjectHeaderMapper : public QDataWidgetMapper
{
	  Q_OBJECT
public:
      void makeMapper(ProjectHeaderLayout* createMapper, ProjectHeaderModel *getMapperModel);
public slots:
      void changeText();
};       
    
class ProjectHeaderView : public QWidget
{
      Q_OBJECT
      
public:
    ProjectHeaderView(QWidget *parent = 0,  ProjectHeaderModel* projectheadermodel=0);
    ~ProjectHeaderView();
    
private:
    ProjectHeaderLayout * projectheaderlayout;
    ProjectHeaderMapper projectheadermapper;
    QWidget 	*MainWidget;
};

#endif
