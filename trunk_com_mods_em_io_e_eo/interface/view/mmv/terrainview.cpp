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

#include "terrainview.h"

TerrainModel::TerrainModel(void)
{
	for(int i=0; i<=14 ; i++)
	{
     QStandardItem *item = new QStandardItem();   
     setItem(0, i, item);
    }	
}

void TerrainModel::fillvalues(vector<string> StrVector)
{
	string xmlTerrainValues;
    
    xmlTerrainValues=StrVector[0];

    EDomElement ede(xmlTerrainValues); 
	EDomElement edeLat(ede.elementByTagName("Lat"));
	EDomElement edeLong(ede.elementByTagName("Long")); 
	item(0, 0)->setText((QString::fromUtf8(ede.elementByTagName("minAltitude").attribute("uom").c_str())).remove(0,1));
	item(0, 1)->setText(QString::fromUtf8(ede.elementByTagName("minAltitude").toString().c_str()));
	item(0, 2)->setText(QString::fromUtf8(ede.elementByTagName("meanAltitude").toString().c_str()));
	item(0, 3)->setText(QString::fromUtf8(ede.elementByTagName("maxAltitude").toString().c_str()));
	item(0, 4)->setText(QString::fromUtf8(ede.elementByTagName("GRS").toString().c_str()));
	item(0, 5)->setText(QString::fromUtf8(ede.elementByTagName("CPS").toString().c_str()));
	item(0, 6)->setText(QString::fromUtf8(ede.elementByTagName("utmFuse").toString().c_str()));
	item(0, 7)->setText(QString::fromUtf8(edeLat.elementByTagName("degrees").toString().c_str()));
	item(0, 8)->setText(QString::fromUtf8(edeLat.elementByTagName("minutes").toString().c_str()));
	item(0, 9)->setText(QString::fromUtf8(edeLat.elementByTagName("seconds").toString().c_str()));
	item(0, 10)->setText(QString::fromUtf8(ede.elementByTagName("Lat").attribute("direction").c_str()));
	item(0, 11)->setText(QString::fromUtf8(edeLong.elementByTagName("degrees").toString().c_str()));
	item(0, 12)->setText(QString::fromUtf8(edeLong.elementByTagName("minutes").toString().c_str()));
	item(0, 13)->setText(QString::fromUtf8(edeLong.elementByTagName("seconds").toString().c_str()));
	item(0, 14)->setText(QString::fromUtf8(ede.elementByTagName("Long").attribute("direction").c_str()));
};

string TerrainModel::getvalues()
{
    string xmlTerrainString;
    stringstream auxStream;
    auxStream << "<terrain>";
    auxStream << "<meanAltitude uom=\"#" << item(0,0)->text().toUtf8().data() << "\">" << item(0,2)->text().toUtf8().data() << "</meanAltitude>";
    auxStream << "<minAltitude uom=\"#" << item(0,0)->text().toUtf8().data();
    auxStream << "\">"<<item(0,1)->text().toUtf8().data()<<"</minAltitude>";
	auxStream << "<maxAltitude uom=\"#" << item(0,0)->text().toUtf8().data();
    auxStream << "\">"<<item(0,3)->text().toUtf8().data()<<"</maxAltitude>";
    auxStream << "<GRS>"<<item(0,4)->text().toUtf8().data()<<"</GRS>";
    auxStream << "<CPS>"<<item(0,5)->text().toUtf8().data()<<"</CPS>";
    auxStream << "<workAreaCenterCoordinates><Lat direction=\"" << item(0,10)->text().toUtf8().data() << "\">";
    auxStream << "<degrees>" <<item(0,7)->text().toUtf8().data()<<"</degrees>";
    auxStream << "<minutes>"<<item(0,8)->text().toUtf8().data()<<"</minutes>";
    auxStream << "<seconds>"<<item(0,9)->text().toUtf8().data()<<"</seconds></Lat>";
    auxStream << "<Long direction=\"" << item(0,14)->text().toUtf8().data() << "\">";
    auxStream << "<degrees>" <<item(0,11)->text().toUtf8().data()<<"</degrees>";
    auxStream << "<minutes>"<<item(0,12)->text().toUtf8().data()<<"</minutes>";
    auxStream << "<seconds>"<<item(0,13)->text().toUtf8().data()<<"</seconds></Long>";
    auxStream << "<utmFuse>"<<item(0,6)->text().toUtf8().data()<<"</utmFuse>";
    auxStream << "</workAreaCenterCoordinates>";
    auxStream << "</terrain>";
    xmlTerrainString=auxStream.str();
    return xmlTerrainString;
};

void TerrainModel::showModel()
{
	for(int i=0; i<=14 ; i++)
	{
       cout << item(0,i)->text().toUtf8().data() << endl; 
    }
};

TerrainLayout::TerrainLayout(QWidget* Owner)
{
    UnitComboBox = new QComboBox(Owner);
    UnitLabel = new QLabel(tr("unit"),Owner);
    AltitudeUnitLayout = new QHBoxLayout;
    AltitudeUnitLayout->addWidget(UnitLabel);
    AltitudeUnitLayout->addWidget(UnitComboBox);
    AltitudeUnitLayout->addStretch();
    
    MinLabel = new QLabel(tr("Min:"),Owner);
    MinLineEdit = new QLineEdit(Owner);
    MinLineEdit->setValidator(new QDoubleValidator(MinLineEdit));    
    MeanLabel = new QLabel(tr("   Mean:"),Owner);
    MeanLineEdit = new QLineEdit(Owner);
    MeanLineEdit->setValidator(new QDoubleValidator(MeanLineEdit));    
    MaxLabel = new QLabel(tr("   Max:"),Owner);
    MaxLineEdit = new QLineEdit(Owner);
    MaxLineEdit->setValidator(new QDoubleValidator(MaxLineEdit));        
    
    AltitudedataLayout = new QHBoxLayout;
    AltitudedataLayout->addWidget(MinLabel);
    AltitudedataLayout->addWidget(MinLineEdit);
    AltitudedataLayout->addWidget(MeanLabel);
    AltitudedataLayout->addWidget(MeanLineEdit);
    AltitudedataLayout->addWidget(MaxLabel);
    AltitudedataLayout->addWidget(MaxLineEdit);
    
    AltitudeVBoxLayout = new QVBoxLayout;
    AltitudeVBoxLayout->addLayout(AltitudeUnitLayout);
    AltitudeVBoxLayout->addLayout(AltitudedataLayout);
    
    AltitudeGroupBox = new QGroupBox(tr("Altitude"),Owner);
    AltitudeGroupBox->setLayout(AltitudeVBoxLayout);
    AltitudeGroupBox->setFixedSize(450,100);
    
    
    GRSLabel = new QLabel(tr("GRS:"),Owner);
    GRSLabel->setAlignment(Qt::AlignRight|Qt::AlignCenter);    
    GRSComboBox = new QComboBox;
    GRSLayout = new QHBoxLayout;
    GRSLayout->addWidget(GRSLabel);
    GRSLayout->addWidget(GRSComboBox);
    GRSLayout->addStretch();
     
    CPSLabel = new QLabel(tr("CPS:"),Owner);
    CPSLabel->setAlignment(Qt::AlignRight|Qt::AlignCenter);    
    CPSComboBox = new QComboBox;
    CPSLayout = new QHBoxLayout;
    CPSLayout->addWidget(CPSLabel);
    CPSLayout->addWidget(CPSComboBox);
    CPSLayout->addStretch();    
    
    UtmLabel = new QLabel(tr("UTM Fuse:"),Owner);
    UtmLabel->setAlignment(Qt::AlignRight|Qt::AlignCenter);    
    UtmSpinBox = new QSpinBox(Owner);
    UtmSpinBox->setRange(1,60);
    UtmLayout = new QHBoxLayout;
    UtmLayout->addWidget(UtmLabel);
    UtmLayout->addWidget(UtmSpinBox);
    UtmLayout->addStretch();    
     
    WorkAreaGroup = new QGroupBox(tr("Work Area Center Coordinates"),Owner);

    LatitudeLabel = new QLabel(tr("Latitude:"),Owner);
    LatdegreeSpinBox = new QSpinBox;
    LatdegreeSpinBox->setAlignment(Qt::AlignRight);
    LatdegreeSpinBox->setRange(0,90);
    
    DegreeLabel = new QLabel(tr("degrees"),Owner);
    LatminuteSpinBox = new QSpinBox;
    LatminuteSpinBox->setAlignment(Qt::AlignRight);
    LatminuteSpinBox->setRange(0,59);
          
    MinuteLabel = new QLabel(tr("min"),Owner);
    LatsecondDoubleSpinBox = new QDoubleSpinBox;
    LatsecondDoubleSpinBox->setAlignment(Qt::AlignRight);
    LatsecondDoubleSpinBox->setDecimals(2);
    LatsecondDoubleSpinBox->setRange(0.00,59.99);
    LatsecondDoubleSpinBox->setValue(0.00);

    SecondLabel = new QLabel(tr("sec"),Owner);
    LatComboBox = new QComboBox(Owner);
    LatComboBox->addItem("S");
    LatComboBox->addItem("N");
    
    LongitudeLabel = new QLabel(tr("Longitude:"),Owner);
    LongdegreeSpinBox = new QSpinBox;
    LongdegreeSpinBox->setAlignment(Qt::AlignRight);
    LongdegreeSpinBox->setRange(0,90);
  
    DegreeLongLabel = new QLabel(tr("degrees"),Owner);    
    LongminuteSpinBox = new QSpinBox;
    LongminuteSpinBox->setAlignment(Qt::AlignRight);
    LongminuteSpinBox->setRange(0,59);

    MinuteLongLabel = new QLabel(tr("min"),Owner);    
    LongsecondDoubleSpinBox = new QDoubleSpinBox;
    LongsecondDoubleSpinBox->setAlignment(Qt::AlignRight);
    LongsecondDoubleSpinBox->setDecimals(2);
    LongsecondDoubleSpinBox->setRange(0.00,59.99);

    SecondLongLabel = new QLabel(tr("sec"),Owner);    
    LongComboBox = new QComboBox(Owner);
    LongComboBox->addItem("W");
    LongComboBox->addItem("E");
    
    WorkAreaLayout = new QGridLayout;
    WorkAreaLayout->addWidget(LatitudeLabel,0,0);
    WorkAreaLayout->addWidget(LatdegreeSpinBox,0,1);
    WorkAreaLayout->addWidget(DegreeLabel,0,2);
    WorkAreaLayout->addWidget(LatminuteSpinBox,0,3);
    WorkAreaLayout->addWidget(MinuteLabel,0,4);
    WorkAreaLayout->addWidget(LatsecondDoubleSpinBox,0,5);
    WorkAreaLayout->addWidget(SecondLabel,0,6);
    WorkAreaLayout->addWidget(LatComboBox,0,7);
    WorkAreaLayout->addWidget(LongitudeLabel,1,0);
    WorkAreaLayout->addWidget(LongdegreeSpinBox,1,1);
    WorkAreaLayout->addWidget(DegreeLongLabel,1,2);
    WorkAreaLayout->addWidget(LongminuteSpinBox,1,3);
    WorkAreaLayout->addWidget(MinuteLongLabel,1,4);
    WorkAreaLayout->addWidget(LongsecondDoubleSpinBox,1,5);
    WorkAreaLayout->addWidget(SecondLongLabel,1,6);
    WorkAreaLayout->addWidget(LongComboBox,1,7);
    WorkAreaGroup->setLayout(WorkAreaLayout);
    WorkAreaGroup->setFixedSize(450,150);
    
    
    addWidget(AltitudeGroupBox);
    addSpacing(15);
    addLayout(GRSLayout);
    addLayout(CPSLayout);
    addLayout(UtmLayout);
    addSpacing(15);
    addWidget(WorkAreaGroup);
    addStretch();
};

void TerrainView::UnitComboFillvalues(vector<string> dataUnit)
{
	int unitsize = dataUnit.size();
	for(int v=0; v < unitsize; v++)
	{
		terrainlayout->UnitComboBox->addItem(QString::fromUtf8(dataUnit[v].c_str()));
    }
};

void TerrainView::CPSComboFillvalues(vector<string> dataCPS)
{
	int cpssize = dataCPS.size();
	for(int v=0; v < cpssize; v++)
	{
		terrainlayout->CPSComboBox->addItem(QString::fromUtf8(dataCPS[v].c_str()));
    }
};

void TerrainView::GRSComboFillvalues(vector<string> dataGRS)
{
	int grssize = dataGRS.size();
	for(int v=0; v < grssize; v++)
	{
		terrainlayout->GRSComboBox->addItem(QString::fromUtf8(dataGRS[v].c_str()));
    }
};              

TerrainLayout::~TerrainLayout()
{
    delete UnitComboBox;
    delete UnitLabel;
    delete MinLabel;
    delete MinLineEdit;
    delete MeanLabel;
    delete MeanLineEdit;
    delete MaxLabel;
    delete MaxLineEdit;
    delete AltitudeGroupBox;
    delete GRSLabel;
    delete GRSComboBox;
    delete CPSLabel;
    delete CPSComboBox;
    delete UtmLabel;
    delete UtmSpinBox;
    delete LatitudeLabel;
    delete LatdegreeSpinBox;
    delete DegreeLabel;
    delete LatminuteSpinBox;
    delete MinuteLabel;
    delete LatsecondDoubleSpinBox;
    delete SecondLabel;
    delete LatComboBox;
    delete LongitudeLabel;
    delete LongdegreeSpinBox;
    delete DegreeLongLabel;
    delete LongminuteSpinBox;
    delete MinuteLongLabel;
    delete LongsecondDoubleSpinBox;
    delete SecondLongLabel;
    delete LongComboBox;
    delete WorkAreaGroup;

}

void TerrainView::changeText()
{
	 	TerrainModel *MyTerrainModel;
 	 	MyTerrainModel = (TerrainModel*) terrainmapper.model();
	 	MyTerrainModel->returnTextCombo(terrainlayout); 
	 	MyTerrainModel->getvalues();
};

void TerrainModel::findTextCombo(TerrainLayout* layoutFindCombo)
{
   	int i;
   	
    QVariant index;
    for(i=0; i<=14 ; i++)
	{
     switch (i)
     {
       case 0:
       {
        index = layoutFindCombo->UnitComboBox->findText(item(0, i)->text().toLower());
        item(0,i)->setText(index.toString());
        break;
       }
       case 4:
       {
        index = layoutFindCombo->GRSComboBox->findText(item(0, i)->text().toUpper());
        item(0,i)->setText(index.toString());
        break;
       }
       case 5:
       {
        index = layoutFindCombo->CPSComboBox->findText(item(0, i)->text().toUpper());
        item(0,i)->setText(index.toString());
        break;
       }
       case 10:
       {
        index = layoutFindCombo->LatComboBox->findText(item(0, i)->text().toUpper());
        item(0,i)->setText(index.toString());
        break;
       }
       case 14:
       {
        index = layoutFindCombo->LongComboBox->findText(item(0, i)->text().toUpper());
        item(0,i)->setText(index.toString());
        break;
       }              
     }
     }
  };
     
void TerrainModel::returnTextCombo(TerrainLayout* layoutReturnCombo)
{
   	int i;
    for(i=0; i<=14 ; i++)
	{
     switch (i)
     {
       case 0:
       {
        item(0,i)->setText(layoutReturnCombo->UnitComboBox->currentText());
        break;
       }
       case 4:
       {
         item(0,i)->setText(layoutReturnCombo->GRSComboBox->currentText());
         break;
       }
       case 5:
       {
        item(0,i)->setText(layoutReturnCombo->CPSComboBox->currentText());
        break;
       }
       case 10:
       {
        item(0,i)->setText(layoutReturnCombo->LatComboBox->currentText());
        break;
       }
       case 14:
       {
        item(0,i)->setText(layoutReturnCombo->LongComboBox->currentText());
        break;
       }             
     }
     }
};
void TerrainMapper::makeMapper(TerrainLayout* layoutMapper, TerrainModel *getMapperModel)
{
	getMapperModel->findTextCombo(layoutMapper);
	setModel(getMapperModel);
    addMapping(layoutMapper->UnitComboBox,0,"currentIndex");
    addMapping(layoutMapper->MinLineEdit,1); 
    addMapping(layoutMapper->MeanLineEdit,2);
    addMapping(layoutMapper->MaxLineEdit,3);   
    addMapping(layoutMapper->GRSComboBox,4,"currentIndex");  
    addMapping(layoutMapper->CPSComboBox,5,"currentIndex");  
    addMapping(layoutMapper->UtmSpinBox,6);  
    addMapping(layoutMapper->LatdegreeSpinBox,7);  
    addMapping(layoutMapper->LatminuteSpinBox,8);  
    addMapping(layoutMapper->LatsecondDoubleSpinBox,9);  
    addMapping(layoutMapper->LatComboBox,10,"currentIndex");  
    addMapping(layoutMapper->LongdegreeSpinBox,11);  
    addMapping(layoutMapper->LongminuteSpinBox,12);
    addMapping(layoutMapper->LongsecondDoubleSpinBox,13);
    addMapping(layoutMapper->LongComboBox,14,"currentIndex");
    toFirst();
   // getMapperModel->showModel();
    
    
};

TerrainView::TerrainView( QWidget *parent, TerrainModel* terrainmodel , vector<string> pDataUnit, vector<string> pDataCPS, vector<string> pDataGRS) : QWidget(parent)
{
       terrainlayout = new TerrainLayout(this/*MainWidget*/);
       this->UnitComboFillvalues(pDataUnit);
       this->CPSComboFillvalues(pDataCPS);
       this->GRSComboFillvalues(pDataGRS);
	   this->setLayout(terrainlayout);

	   terrainmapper.makeMapper(terrainlayout,terrainmodel);
	   terrainmodel->returnTextCombo(terrainlayout);
	   setMinimumSize(400,350);
       connect(terrainmodel,SIGNAL(itemChanged(QStandardItem *)),this,SLOT(changeText()));
};

TerrainView::~TerrainView()
{
       changeText();
       delete terrainlayout;
};
