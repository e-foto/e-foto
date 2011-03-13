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

#ifndef SENSORVIEW_H
#define SENSORVIEW_H

#include <deque>
#include <iostream>
#include <vector>
#include <string>

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
#include <QCheckBox>
#include <QString>
#include <QGroupBox>
#include <QValidator>
#include <QRegExp>
#include <QScrollArea>
#include <QDateTimeEdit>

#include "AbstractEModel.h"
#include "EDom.h"

using namespace std;

class SensorLayout;

class SensorView;

class SensorModel : public AbstractEModel 
{
	Q_OBJECT
	
public:
	SensorModel();
	void fillvalues(vector <string> StrVector /*string xmlSensorValues*/);
	void mountstring();

	string getvalues();
	void fillvalues(string values);
	void setReadOnly(bool state);

	void showModel(void);
	void findTextCombo(SensorLayout* layoutFindCombo);
	string strsensor;

public slots:
     void resetFDMarkSigma(int value);	
private:
    string xmlSensorstring;
	void setCombos(string geom, string plat, string detc);
};

class  SensorMapper;

class SensorLayout : public QVBoxLayout
{
    Q_OBJECT
    
public:
    SensorLayout(QWidget *);
    ~SensorLayout();

//friend class  SensorMapper;
//friend class SensorModel;
//friend class SensorView;

//private:
      QLabel *geometryLabel, *platformLabel, *detectorLabel, *descriptionLabel,
             *numberCalibrationLabel, *dispatchCalibrationLabel, *expirationCalibrationLabel,
             *focaldistanceLabel, *sigmafocalLabel, *k0Label, *sigmak0Label,
             *k1Label, *sigmak1Label, *k2Label, *sigmak2Label, *k3Label,
             *sigmak3Label, *p1Label, *sigmap1Label, *p2Label, *sigmap2Label,
             *sigmaprincipalpointLabel, *nrfiductialLabel,
             *fiductial01sigmaLabel, *fiductial02sigmaLabel,
             *fiductial03sigmaLabel, *fiductial04sigmaLabel,
             *fiductial05sigmaLabel, *fiductial06sigmaLabel,
             *fiductial07sigmaLabel, *fiductial08sigmaLabel,
             *a0Label, *a1Label, *a2Label, *b0Label, *b1Label, *b2Label;
      
      QLineEdit *numberCalibrationLineEdit, *valuefocalLineEdit, *sigmafocalLineEdit,
                *k0LineEdit, *sigmak0LineEdit, *k1LineEdit, *sigmak1LineEdit, *k2LineEdit,
                *sigmak2LineEdit, *k3LineEdit,*sigmak3LineEdit, *p1LineEdit,
                *sigmap1LineEdit, *p2LineEdit, *sigmap2LineEdit, *coord01principalpointLineEdit,
                *coord02principalpointLineEdit, *sigmacoord01principalpointLineEdit,
                *sigmacoord02principalpointLineEdit,
                *fiductial01csiLineEdit, *fiductial01etaLineEdit,
                *fiductial02csiLineEdit, *fiductial02etaLineEdit,
                *fiductial03csiLineEdit, *fiductial03etaLineEdit,
                *fiductial04csiLineEdit, *fiductial04etaLineEdit,
                *fiductial05csiLineEdit, *fiductial05etaLineEdit,
                *fiductial06csiLineEdit, *fiductial06etaLineEdit,
                *fiductial07csiLineEdit, *fiductial07etaLineEdit,
                *fiductial08csiLineEdit, *fiductial08etaLineEdit,
                *fiductial01sigmaLineEdit, *fiductial02sigmaLineEdit,
                *fiductial03sigmaLineEdit, *fiductial04sigmaLineEdit,
                *fiductial05sigmaLineEdit, *fiductial06sigmaLineEdit,
                *fiductial07sigmaLineEdit, *fiductial08sigmaLineEdit,
                *a0LineEdit, *a1LineEdit, *a2LineEdit,
                *b0LineEdit, *b1LineEdit, *b2LineEdit;//,
      QTextEdit          *descriptionTextEdit;
                
      QDateTimeEdit *dispatchCalibrationDateTimeEdit, *expirationCalibrationDateTimeEdit;          
               
      QGroupBox *typeGroupBox, *opticalGroupBox, *radialsimetricGroupBox,
                *descenteredGroupBox, *fiductialGroupBox, *calibrationGroupBox,
                *principalpointGroupBox, *fourfiductialGroupBox,
                *eigthfiductialGroupBox, *xaGroupBox, *distortioncoefGroupBox;
      
      QComboBox *geometryComboBox, *platformComboBox, *detectorComboBox,
                *nrfiductialComboBox;
      
      QCheckBox *sigmafiductialCheckBox;
      
      QHBoxLayout *geometryHLayout, *platformHLayout, *detectorHLayout, *descriptionHLayout,
                  *numberHLayout, *dispatchHLayout, *expirationHLayout, *focaldistanceHLayout,
                  *principalpointHLayout, *fiductialoptionsHLayout, *fiductial01HLayout,
                  *fiductial02HLayout, *fiductial03HLayout, *fiductial04HLayout,
                  *fiductial05HLayout, *fiductial06HLayout, *fiductial07HLayout,
                  *fiductial08HLayout, *xaHLayout;
      
      QVBoxLayout *typeVLayout, *calibrationVLayout, *opticalVLayout, *sensorVLayout,
      			  *fourfiductialVLayout, *eigthfiductialVLayout, *fiductialVLayout,
      			  *mainLayout, *distortioncoefVLayout, *mainsensorVLayout;
      
      QGridLayout *radialGridLayout, *descenteredGridLayout, *principalGridLayout,
      			  *xaGridLayout;
      			  
public slots:
      void showfiductialXa();
      void showsigma(int);
      void shownrfiductial();
};

class SensorMapper : public QDataWidgetMapper
{
	  Q_OBJECT
public:
      void makeMapper(SensorLayout* getlayoutMapper, SensorModel *getMapperModel);
};       
    
class SensorView : public QWidget//QScrollArea
{
      Q_OBJECT
      
public:
    SensorView(QWidget *parent = 0,  SensorModel* sensormodel=0);
    ~SensorView();

	void fillvalues(string values);
	string getvalues();
	void setReadOnly(bool state);
	bool isForm(string formName);
    
//friend class SensorModel;

//private:
	SensorLayout *sensorlayout;
	SensorMapper sensormapper;
	SensorModel* sensorsmodel;
    int auxnumberpoints;
    
private slots:
    void changeText();
};

#endif
