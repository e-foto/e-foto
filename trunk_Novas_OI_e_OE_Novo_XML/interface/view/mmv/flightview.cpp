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

#include "flightview.h"

FlightModel::FlightModel(void)
{
	for(int i=0; i<=7 ; i++)
	{
     QStandardItem *item = new QStandardItem();   
     setItem(0, i, item);
    }	
}

void FlightModel::fillvalues(vector<string> StrVector)
{
	string xmlFlightValues;
	xmlFlightValues = StrVector[0];
	EDomElement ede(xmlFlightValues);
	string edefrac;
	int index = (xmlFlightValues.find("</mml:mn>")) + 9;
	edefrac = xmlFlightValues.substr(index);
	EDomElement edomfrac(edefrac);

	item(0, 0)->setText(QString::fromUtf8(ede.elementByTagName("flight").attribute("key").c_str()));
	item(0, 1)->setText(QString::fromUtf8(ede.elementByTagName("description").toString().c_str()));
	item(0, 2)->setText(QString::fromUtf8(ede.elementByTagName("execution").toString().c_str()));
	item(0, 3)->setText(QString::fromUtf8(ede.elementByTagName("producerName").toString().c_str()));
	item(0, 4)->setText(QString::fromUtf8(edomfrac.elementByTagName("mml:mn").toString().c_str()));
	item(0, 5)->setText(QString::fromUtf8(ede.elementByTagName("flightHeight").toString().c_str()));
	item(0, 6)->setText(QString::fromUtf8(ede.elementByTagName("longitudinal").toString().c_str()));
	item(0, 7)->setText(QString::fromUtf8(ede.elementByTagName("transversal").toString().c_str()));
	strsensorkey = ede.elementByTagName("flight").attribute("sensor_key").c_str();

};

void FlightModel::fillvalues(string values)
{
	if (values.length() > 0)
	{
		string xmlFlightValues = values;
		EDomElement ede(xmlFlightValues);
		string edefrac;
		int index = (xmlFlightValues.find("</mml:mn>")) + 9;
		edefrac = xmlFlightValues.substr(index);
		EDomElement edomfrac(edefrac);

		item(0, 0)->setText(QString::fromUtf8(ede.elementByTagName("flightId").toString().c_str()));
		item(0, 1)->setText(QString::fromUtf8(ede.elementByTagName("description").toString().c_str()));
		item(0, 2)->setText(QString::fromUtf8(ede.elementByTagName("execution").toString().c_str()));
		item(0, 3)->setText(QString::fromUtf8(ede.elementByTagName("producerName").toString().c_str()));
		item(0, 4)->setText(QString::fromUtf8(edomfrac.elementByTagName("mml:mn").toString().c_str()));
		item(0, 5)->setText(QString::fromUtf8(ede.elementByTagName("flightHeight").toString().c_str()));
		item(0, 6)->setText(QString::fromUtf8(ede.elementByTagName("longitudinal").toString().c_str()));
		item(0, 7)->setText(QString::fromUtf8(ede.elementByTagName("transversal").toString().c_str()));
		strsensorkey = ede.elementByTagName("flight").attribute("sensor_key").c_str();
		strkey = ede.elementByTagName("flight").attribute("key").c_str();
	}
	else
	{
		item(0, 0)->setText("1");
		item(0, 1)->setText("1");
	}
};

string FlightModel::getvalues()
{
    string xmlflightsstring;
	stringstream auxStream;
	QString fracnum, fracden, frac;
	frac = (item(0,4)->text());
	int div = frac.indexOf("/");
	fracnum = frac.left(div);
	fracden = frac.mid(div+1);

	auxStream << "<flight key=\"" << strkey << "\" sensor_key=\"" << strsensorkey << "\">\n";
	auxStream << "\t\t\t<flightId>" << item(0,0)->text().toUtf8().data() << "</flightId>\n";
	auxStream << "\t\t\t<description>" << item(0,1)->text().toUtf8().data() << "</description>\n";
	auxStream << "\t\t\t<execution>" << item(0,2)->text().toUtf8().data() << "</execution>\n";
	auxStream << "\t\t\t<producerName>" << item(0,3)->text().toUtf8().data() << "</producerName>\n";
	auxStream << "\t\t\t<nominalScale>\n";
	auxStream << "\t\t\t\t<mml:mfrac>\n";
	auxStream << "\t\t\t\t\t<mml:mn>" << 1 /*fracnum.toUtf8().data()*/ << "</mml:mn>\n";
	auxStream << "\t\t\t\t\t<mml:mn>" << fracden.toUtf8().data() << "</mml:mn>\n";
	auxStream << "\t\t\t\t</mml:mfrac>\n";
	auxStream << "\t\t\t</nominalScale>\n";
	auxStream << "\t\t\t<flightHeight uom=\"#m\">" << item(0,5)->text().toUtf8().data() << "</flightHeight>\n";
	auxStream << "\t\t\t<overlap>\n";
	auxStream << "\t\t\t\t<longitudinal uom=\"#%\" >" << item(0,6)->text().toUtf8().data() << "</longitudinal>\n";
	auxStream << "\t\t\t\t<transversal uom=\"#%\" >" << item(0,7)->text().toUtf8().data() << "</transversal>\n";
	auxStream << "\t\t\t</overlap>\n";
	auxStream << "\t\t</flight>";
	xmlflightsstring=auxStream.str();
    return xmlflightsstring;
};

void FlightModel::showModel()
{
	for(int i=0; i<=7 ; i++)
	{
       cout << item(0,i)->text().toUtf8().data() << endl; 
    }
};

FlightLayout::FlightLayout(QWidget* FlightOwner):QGridLayout (FlightOwner)
{
    Flight_IDLayout = new QHBoxLayout;
    
    Flight_IDLabel= new QLabel(tr("Flight ID:"), FlightOwner);
	Flight_IDLineEdit =new QLineEdit(FlightOwner);
	Flight_IDLineEdit->setFixedWidth(30);
	
	Flight_IDLayout = new QHBoxLayout;
	Flight_IDLayout->addWidget(Flight_IDLabel);
	Flight_IDLayout->addWidget(Flight_IDLineEdit);
	Flight_IDLayout->addStretch();
    
	FlightNameLabel= new QLabel(tr("Local Flight:"), FlightOwner);
	FlightNameLineEdit =new QLineEdit(FlightOwner);
	FlightNameLineEdit->setMinimumWidth(300);
	LocalFlightLayout = new QHBoxLayout;
	LocalFlightLayout->addWidget(FlightNameLabel);
	LocalFlightLayout->addWidget(FlightNameLineEdit);
	LocalFlightLayout->addStretch();
	
	ExecutionDateLabel = new QLabel(tr("Execution:"),FlightOwner);
	ExecutionDateTimeEdit =new QDateEdit(FlightOwner); 
//	ExecutionDateTimeEdit->setReadOnly(true);
	ExecutiondateHorizLayout = new QHBoxLayout;
	ExecutiondateHorizLayout->addWidget(ExecutionDateLabel);
	ExecutiondateHorizLayout->addWidget(ExecutionDateTimeEdit,0,Qt::AlignLeft);
	ExecutiondateHorizLayout->addStretch();
	
	ProducerNameLabel= new QLabel(tr("Producer Name:"),FlightOwner);
	ProducerNameLineEdit =new QLineEdit(FlightOwner);
	ProducerHorizLayout = new QHBoxLayout;
	ProducerHorizLayout->addWidget(ProducerNameLabel);
	ProducerHorizLayout->addWidget(ProducerNameLineEdit);
	ProducerHorizLayout->addStretch();
	
	NominalScaleLabel= new QLabel(tr("Nominal Scale:   1:"),FlightOwner);
	NominalScaleLineEdit =new QLineEdit(FlightOwner);
	NominalScaleLineEdit->setValidator(new QIntValidator(NominalScaleLineEdit));
	NominalScaleLineEdit->setMaximumWidth(80);
	NominalScaleHorizLayout = new QHBoxLayout;
	NominalScaleHorizLayout->addWidget(NominalScaleLabel);
	NominalScaleHorizLayout->addWidget(NominalScaleLineEdit);
	NominalScaleHorizLayout->addStretch();
	
	FlightHeightLabel= new QLabel(tr("FlightHeight:"),FlightOwner);
	FlightHeightLineEdit =new QLineEdit(FlightOwner);
	FlightHeightLineEdit->setValidator(new QDoubleValidator(FlightHeightLineEdit));
	FlightHeightLineEdit->setMaximumWidth(80);	
	QLabel *FlightHeightUnitLabel= new QLabel(tr("m"),FlightOwner);
	FlightHeightHorizLayout = new QHBoxLayout;
	FlightHeightHorizLayout->addWidget(FlightHeightLabel);
	FlightHeightHorizLayout->addWidget(FlightHeightLineEdit);
	FlightHeightHorizLayout->addWidget(FlightHeightUnitLabel);
	FlightHeightHorizLayout->addStretch();	

	LongLabel= new QLabel(tr("Longitudinal:"),FlightOwner);
	LongSpinBox =new QSpinBox(FlightOwner);
	LongSpinBox->setRange(1,100);
	LongSpinBox->setMaximumWidth(60);
	QLabel *longpercentualLabel = new QLabel(tr("\%"));
	LongHorizLayout = new QHBoxLayout;
	LongHorizLayout->addWidget(LongLabel,0,Qt::AlignRight);
	LongHorizLayout->addWidget(LongSpinBox,0,Qt::AlignLeft);
	LongHorizLayout->addWidget(longpercentualLabel,0,Qt::AlignLeft);	
	
	TransvLabel= new QLabel(tr("Transversal:"),FlightOwner);
	TransvSpinBox =new QSpinBox(FlightOwner);
	TransvSpinBox->setRange(1,100);
	TransvSpinBox->setMaximumWidth(60);
	QLabel *transvpercentualLabel = new QLabel(tr("%"));
	TransvHorizLayout = new QHBoxLayout;
	TransvHorizLayout->addWidget(TransvLabel,0,Qt::AlignRight);
	TransvHorizLayout->addWidget(TransvSpinBox,0,Qt::AlignLeft);
	TransvHorizLayout->addWidget(transvpercentualLabel,0,Qt::AlignLeft);
    
    OverlapLayout = new QHBoxLayout;
    OverlapLayout->addLayout(LongHorizLayout);
    OverlapLayout->addStretch();	
    OverlapLayout->addLayout(TransvHorizLayout);
    	
    OverlapGroupBox = new QGroupBox(tr("OverLap"));
    OverlapGroupBox->setLayout(OverlapLayout);
    OverlapGroupBox->setFixedSize(400,60);
    
	FlightVLayout = new QVBoxLayout;
	FlightVLayout->addLayout(Flight_IDLayout);
	FlightVLayout->addLayout(LocalFlightLayout);
    FlightVLayout->addLayout(ExecutiondateHorizLayout);
    FlightVLayout->addLayout(ProducerHorizLayout);
    FlightVLayout->addLayout(NominalScaleHorizLayout);
    FlightVLayout->addLayout(FlightHeightHorizLayout);
    FlightVLayout->addWidget(OverlapGroupBox);
    FlightVLayout->addStretch();
    
    addLayout(FlightVLayout,0,0,0);
    

};

FlightLayout::~FlightLayout()
{
    delete Flight_IDLabel;
	delete Flight_IDLineEdit;
	delete FlightNameLabel;
	delete FlightNameLineEdit;
	delete ExecutionDateLabel;
	delete ExecutionDateTimeEdit;
	delete ProducerNameLabel;
	delete ProducerNameLineEdit;
	delete NominalScaleLabel;
	delete NominalScaleLineEdit;
	delete FlightHeightLabel;
	delete FlightHeightLineEdit;
	delete LongLabel;
	delete LongSpinBox;
	delete TransvLabel;
	delete TransvSpinBox;
	

}

void FlightMapper::changeText()
{
	 	FlightModel *MyFlightModel;
	 	MyFlightModel = (FlightModel*) this->model();
	 	MyFlightModel->getvalues();
};

void FlightMapper::makeMapper(FlightLayout* LayoutMapper, FlightModel *getMapperModel)
{
	setModel(getMapperModel);
	addMapping(LayoutMapper->Flight_IDLineEdit,0);
	addMapping(LayoutMapper->FlightNameLineEdit,1);
	addMapping(LayoutMapper->ExecutionDateTimeEdit,2);
	addMapping(LayoutMapper->ProducerNameLineEdit,3);
	addMapping(LayoutMapper->NominalScaleLineEdit,4);
	addMapping(LayoutMapper->FlightHeightLineEdit,5);
	addMapping(LayoutMapper->LongSpinBox,6);
	addMapping(LayoutMapper->TransvSpinBox,7);
    toFirst();
    connect(getMapperModel,SIGNAL(itemChanged(QStandardItem *)),this,SLOT(changeText()));
};

FlightView::FlightView(QWidget *parent, FlightModel* flightsmodel) : QWidget(parent)
{
       flightslayout = new FlightLayout(this);
	   if (!flightsmodel)
		   flightsmodel = new FlightModel();
	   this->flightsmodel = flightsmodel;
       flightsmapper.makeMapper(flightslayout,flightsmodel);
};

FlightView::~FlightView()
{
       delete flightslayout;
};

void FlightView::fillvalues(string values)
{
	flightsmodel->fillvalues(values);
};

void FlightView::setReadOnly(bool enabled)
{
	flightslayout->Flight_IDLineEdit->setReadOnly(enabled);
    flightslayout->FlightNameLineEdit->setReadOnly(enabled);
    flightslayout->ExecutionDateTimeEdit->setReadOnly(enabled);
    flightslayout->ProducerNameLineEdit->setReadOnly(enabled);
    flightslayout->NominalScaleLineEdit->setReadOnly(enabled);
    flightslayout->FlightHeightLineEdit->setReadOnly(enabled);
    flightslayout->LongSpinBox->setReadOnly(enabled);
    flightslayout->TransvSpinBox->setReadOnly(enabled);
}

string FlightView::getvalues()
{
	return flightsmodel->getvalues();
};
