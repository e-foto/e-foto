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

#ifndef FLIGHTSVIEW_H
#define FLIGHTSVIEW_H

#include <iostream>
#include <string>
#include <vector>

#include <QStandardItemModel>
#include <QDataWidgetMapper>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDateTimeEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QString>
#include <QDialog>

#include "AbstractEModel.h"
#include "EDom.h"

using namespace std;

class FlightModel : public AbstractEModel //public QStandardItemModel 
{
public:
	FlightModel(void);
	void fillvalues(vector <string> StrVector);

	void fillvalues(string values);
	string getvalues();
	void showModel(void);

private:
	string strsensorkey;
	string strkey;
};

class FlightMapper;

class FlightLayout : public QGridLayout //QGridLayoutQDialog
{
    Q_OBJECT

public:
    FlightLayout(QWidget *);
    ~FlightLayout();

	friend class FlightMapper;    

//private:
    QLineEdit 	*FlightNameLineEdit, *ProducerNameLineEdit, *FlightHeightLineEdit, 
    			*NominalScaleLineEdit, *Flight_IDLineEdit;

	QDateEdit *ExecutionDateTimeEdit;

	QLabel 		*ProducerNameLabel, *FlightHeightLabel, *NominalScaleLabel,
				*ExecutionDateLabel, *LongLabel, *TransvLabel, *FlightNameLabel,
				*Flight_IDLabel;
	
	QSpinBox *LongSpinBox, *TransvSpinBox;
				
	QGroupBox   *OverlapGroupBox;
	
	QHBoxLayout *LongHorizLayout, *TransvHorizLayout, *ProducerHorizLayout,
                *ExecutiondateHorizLayout, *NominalScaleHorizLayout,
                *FlightHeightHorizLayout,*LocalFlightLayout,*OverlapLayout,
                *Flight_IDLayout;
                
    QVBoxLayout  *FlightVLayout;           
};

class FlightMapper : public QDataWidgetMapper
{
	  Q_OBJECT
public:
      void makeMapper(FlightLayout* LayoutMapper, FlightModel *getMapperModel);
public slots:
      void changeText();
};       
    
class FlightView : public QWidget
{
      Q_OBJECT
      
public:
    FlightView(QWidget *parent = 0,  FlightModel* flightsmodel=0);
    ~FlightView();

	void fillvalues(string values);
	string getvalues();
	void setReadOnly(bool state);
	bool isForm(string formName);
    
private:
	FlightLayout * flightslayout;
	FlightMapper flightsmapper;
	FlightModel* flightsmodel;
};

#endif
