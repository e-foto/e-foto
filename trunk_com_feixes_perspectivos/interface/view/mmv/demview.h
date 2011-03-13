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

#ifndef DEMVIEW_H
#define DEMVIEW_H

#include <iostream>
#include <vector>
#include <string>

#include <QDataWidgetMapper>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QString>
#include <QGroupBox>
#include <QFileDialog>
#include <QDir>

#include "AbstractEModel.h"
#include "EDom.h"

using namespace std;

class DemModel : public AbstractEModel 
{
public:
	DemModel(void);
	
	string strkey, strstereopairkey;
	
	void fillvalues(vector <string>); //string xmlDemValues, string strunit);
	string getvalues();
	void showModel(void);
};

class DemMapper;
class DemView;

class DemLayout : public QVBoxLayout
{
    Q_OBJECT
public:
    DemLayout(QWidget *);
    ~DemLayout();
    
friend class DemMapper;
friend class DemView;

private:    
      QLabel *filePathLabel, *fileNameLabel, *upperspatialCoordLabel, *bottomspatialCoordLabel,
             *horizontalLabel, *verticalLabel,  *viewunit, *firstunitupper, *secondunitupper,
             *firstunitbottom, *secondunitbottom;
      
      QLineEdit *filePathLineEdit, *fileNameLineEdit, *upperspatialCoordLineEditX,
                *upperspatialCoordLineEditY, *upperspatialCoordLineEditZ,
                *bottomspatialCoordLineEditX, *bottomspatialCoordLineEditY,
                *bottomspatialCoordLineEditZ, *horizontalLineEdit, *verticalLineEdit;
                
      QPushButton *importPushButton, *createPushButton;               
               
      QGroupBox *upperLeftGroupBox, *bottomRightGroupBox, *divisionsGroupBox, *gridsGroupBox;
      
      QComboBox *gridsComboBox;
      
      QHBoxLayout *filePathLayout,*fileNameLayout, *divisionsLayout,
                  *upperSpatialCoordLayout, *bottomSpatialCoordLayout;
      
      QVBoxLayout *upperLeftVLayout, *bottomRightVLayout, *gridsLayout,*mainLayout;
      
      QString StrType;
      
public slots:
       void showDivision(int);
};

class DemMapper : public QDataWidgetMapper
{
	  Q_OBJECT
public:
      void makeMapper(DemLayout* getlayoutMapper, DemModel *getMapperModel);
};       
    
class DemView : public QWidget
{
      Q_OBJECT
      
public:
    DemView(QWidget *parent = 0,  DemModel* demmodel=0);
    ~DemView();
    
private:
    QVariant indexGridsType;
    DemLayout *demlayout;
    DemMapper demmapper;
    
private slots:
    void changeText();
    void import();
    void create();            
};

#endif
