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

#ifndef DIGITALIMAGES_H
#define DIGITALIMAGES_H

#include <QStandardItemModel>
#include <QDataWidgetMapper>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <iostream>
#include <QPixmap>
#include <QGroupBox>
#include <QVariant>
#include <QtGui>
#include <vector>
#include <string>
#include <QFont>

#include "EDom.h"
#include "AbstractEModel.h"

using namespace std;

class DigitalImageLayout;

class DigitalImageModel : public AbstractEModel
{
public:
	DigitalImageModel(void);
        void fillvalues(vector <string> xmlDigitalImageValues);
        string getvalues(void);
	void showModel(void);
        void fillComboValues(vector<string> StrVector);
        vector <string> getSensorTexts(void) {return SensorComboText;}
        vector <string> getFlightTexts(void) {return FlightComboText;}
private:
    string xmldigitalimagestring;
    vector <string> SensorComboText, FlightComboText;
    vector <int> SensorComboCode, FlightComboCode;
};

class DigitalImageView;
class DigitalImageMapper;

class DigitalImageLayout : public QHBoxLayout
{
    Q_OBJECT
public:
    DigitalImageLayout(QWidget *);
    ~DigitalImageLayout();

    friend class DigitalImageView;
    friend class DigitalImageModel;
    friend class DigitalImageMapper;

private:
    QLabel *ImageIdLabel, *ImageLabel, /* *DigitalImageLabel,*/ *FlightidLabel,*WidthLabel,
           *HeightLabel,*ResolutionLabel,*ResolutionUnitLabel, *SensorLabel,
           *WidthpixLabel, *HeightpixLabel, *Coord1Label, *Coord2Label, *Coord3Label, *Coord1UnitLabel, *Coord2UnitLabel, *Coord3UnitLabel;
    QLineEdit *Coord1LineEdit, *Coord2LineEdit, *Coord3LineEdit, *ImageNameLineEdit,*FilenameLineEdit,*PathImageLineEdit, *ImageIdLineEdit, *WidthLineEdit, *HeightLineEdit;
    QPushButton *BrowseButton;
    QComboBox *FlightidComboBox, *SensorComboBox;
    QSpinBox *ResolutionSpinBox;

    QGroupBox *CoordGroupBox;
      
    QHBoxLayout *ImageIdHorizLayout, *ImageHorizLayout,*idHorizLayout, *WidthHorizLayout,
                *HeightHorizLayout, *ResolutionHorizLayout, *SensorHorizLayout , *ImageNameHLayout;

    QHBoxLayout * CoordinatesLayout, *ImageSizeLayout, *ImageFileHBoxLayout, *ImagePathHBoxLayout, *ImageNameHBoxLayout;

    QVBoxLayout *ImageFileVBoxLayout, *MainLayout;

    QGroupBox *CoordinatesGroupBox, *ImageSizeGroupBox, *ImageFileGroupBox;

                
    QVBoxLayout *ImageDataLayout;
    void ImageData();                
    
};

class DigitalImageMapper : public QDataWidgetMapper
{
	  Q_OBJECT
public:
      void makeMapper(DigitalImageLayout* layoutMapper, DigitalImageModel *getMapperModel);
};       
    
class DigitalImageView : public QWidget
{
      Q_OBJECT
      
public:
    DigitalImageView(QWidget *parent = 0, DigitalImageModel* projectdigitalimagemodel=0);
    ~DigitalImageView();

    void setToUTM(void);

private:
    DigitalImageLayout * digitalimagelayout;
    DigitalImageMapper digitalimagemapper;
    void flightIdComboFillvalues(vector<string> dataFlightId);
    void SensorComboFillvalues(vector<string> dataSensor);
    
private slots:
    void changeText();
    void browse();
};

#endif
