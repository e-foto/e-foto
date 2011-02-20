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

#ifndef TERRAINVIEW_H
#define TERRAINVIEW_H

#include <QStandardItemModel>
#include <QDataWidgetMapper>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QString>
#include <iostream>
#include <vector>
#include <QGroupBox>
#include <string>
#include "AbstractEModel.h"
#include "EDom.h"

using namespace std;

class TerrainLayout;

class TerrainModel : public AbstractEModel
{
public:
	TerrainModel(void);

	void fillvalues(vector <string>);
	string getvalues();
	void showModel(void);
	void findTextCombo(TerrainLayout* layoutFindCombo);
	void returnTextCombo(TerrainLayout* layoutReturnCombo);
};

class TerrainMapper;
class TerrainView;

class TerrainLayout : public QVBoxLayout
{
    Q_OBJECT
public:
    TerrainLayout(QWidget *);
    ~TerrainLayout();
    
    friend class TerrainMapper;
	friend class TerrainView;
    friend class TerrainModel;		
    
private:
    QLabel *UnitLabel, *MinLabel, *MeanLabel, *MaxLabel,
           *GRSLabel, *CPSLabel, *UtmLabel, *LatitudeLabel,
           *DegreeLabel, *MinuteLabel, *SecondLabel, *LongitudeLabel,
           *DegreeLongLabel, *MinuteLongLabel, *SecondLongLabel;
      
    QLineEdit *MinLineEdit, *MeanLineEdit, *MaxLineEdit;
               
    QComboBox *UnitComboBox, *GRSComboBox, *CPSComboBox, *LatComboBox, *LongComboBox;
      
    QSpinBox *UtmSpinBox,
             *LatdegreeSpinBox, *LatminuteSpinBox,
             *LongdegreeSpinBox, *LongminuteSpinBox,;
                
    QDoubleSpinBox  *LatsecondDoubleSpinBox, *LongsecondDoubleSpinBox;        
      
    QGroupBox  *WorkAreaGroup, *AltitudeGroupBox;
      
    QHBoxLayout *AltitudeUnitLayout, *AltitudedataLayout, 
                  *GRSLayout, *CPSLayout, *UtmLayout;
      
    QVBoxLayout *AltitudeVBoxLayout;
      
    QGridLayout *WorkAreaLayout;
};

class TerrainMapper : public QDataWidgetMapper
{
	  Q_OBJECT
public:
      void makeMapper(TerrainLayout* layoutMapper, TerrainModel *getMapperModel);
};       
    
class TerrainView : public QWidget
{
      Q_OBJECT
      
public:
    TerrainView(QWidget *parent = 0,  TerrainModel* projectterrainmodel=0,vector<string> pDataUnit=vector<string>(0), vector<string> pDataCPS=vector<string>(0), vector<string> pDataGRS=vector<string>(0));
    ~TerrainView();
    void UnitComboFillvalues(vector<string> dataUnit);
    void CPSComboFillvalues(vector<string> dataCPS);
    void GRSComboFillvalues(vector<string> dataGRS);
    
private:
    TerrainLayout * terrainlayout;
    TerrainMapper terrainmapper;
    
private slots:
    void changeText();    
};

#endif
