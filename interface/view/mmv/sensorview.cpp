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
#include "sensorview.h"
SensorModel::SensorModel()
{
	for(int i=0; i<=55 ; i++)
	{
     QStandardItem *item = new QStandardItem(); 

     setItem(0, i, item);
	}
};

void SensorModel::resetFDMarkSigma(int value)
{
	if (value==Qt::Unchecked)
		for (int i=26; i<=47;i+=3)
			item(0,i)->setText("Not Available");
}


void SensorModel::fillvalues(vector<string> StrVector)
{
	string xmlSensorValues = StrVector[0];
	EDomElement ede(xmlSensorValues);
	QString strcoordprincipal, strsigmaprincipal, stringcsieta, qkeyString;
	string keyString;
	QVariant keyVariant;
	strsensor = ede.elementByTagName("sensor").attribute("key");

	setCombos(ede.elementByTagName("geometry").toString(),ede.elementByTagName("platform").toString(),ede.elementByTagName("detector").toString());
	item(0, 54)->setText(QString::fromUtf8(ede.elementByTagName("description").toString().c_str()));

	item(0, 3)->setText(QString::fromUtf8(ede.elementByTagName("number").toString().c_str()));
	item(0, 4)->setText(QString::fromUtf8(ede.elementByTagName("dispatch").toString().c_str()));
	item(0, 5)->setText(QString::fromUtf8(ede.elementByTagName("expiration").toString().c_str()));

	EDomElement edeFocalDistance(ede.elementByTagName("focalDistance"));
	item(0, 6)->setText(QString::fromUtf8(edeFocalDistance.elementByTagName("value").toString().c_str()));
	item(0, 7)->setText(QString::fromUtf8(edeFocalDistance.elementByTagName("sigma").toString().c_str()));

	EDomElement edek0(ede.elementByTagName("k0"));
	item(0, 8)->setText(QString::fromUtf8(edek0.elementByTagName("value").toString().c_str()));
	item(0, 9)->setText(QString::fromUtf8(edek0.elementByTagName("sigma").toString().c_str()));

	EDomElement edek1(ede.elementByTagName("k1"));
	item(0, 10)->setText(QString::fromUtf8(edek1.elementByTagName("value").toString().c_str()));
	item(0, 11)->setText(QString::fromUtf8(edek1.elementByTagName("sigma").toString().c_str()));

	EDomElement edek2(ede.elementByTagName("k2"));
	item(0, 12)->setText(QString::fromUtf8(edek2.elementByTagName("value").toString().c_str()));
	item(0, 13)->setText(QString::fromUtf8(edek2.elementByTagName("sigma").toString().c_str()));

	EDomElement edek3(ede.elementByTagName("k3"));
	item(0, 14)->setText(QString::fromUtf8(edek3.elementByTagName("value").toString().c_str()));
	item(0, 15)->setText(QString::fromUtf8(edek3.elementByTagName("sigma").toString().c_str()));

	EDomElement edep1(ede.elementByTagName("P1"));
	item(0, 16)->setText(QString::fromUtf8(edep1.elementByTagName("value").toString().c_str()));
	item(0, 17)->setText(QString::fromUtf8(edep1.elementByTagName("sigma").toString().c_str()));

	EDomElement edep2(ede.elementByTagName("P2"));
	item(0, 18)->setText(QString::fromUtf8(edep2.elementByTagName("value").toString().c_str()));
	item(0, 19)->setText(QString::fromUtf8(edep2.elementByTagName("sigma").toString().c_str()));

	EDomElement edeprincipalpoint(ede.elementByTagName("principalPointCoordinates"));
	strcoordprincipal = QString::fromUtf8(edeprincipalpoint.elementByTagName("gml:pos").toString().c_str());
	item(0,20)->setText(strcoordprincipal.section(' ',0,0));
	item(0,21)->setText(strcoordprincipal.section(' ',1,1));
	strsigmaprincipal = QString::fromUtf8(edeprincipalpoint.elementByTagName("sigma").toString().c_str());
	item(0,22)->setText(strsigmaprincipal.section(' ',0,0));
	item(0,23)->setText(strsigmaprincipal.section(' ',1,1));

	EDomElement edefiductial(ede.elementByTagName("fiductialMarks"));
	int keyitem = 24;
	for(int keyfiductial=1; keyfiductial<=8; keyfiductial++)
	 {
		keyVariant = keyfiductial;
		qkeyString = keyVariant.toString();
		keyString = qkeyString.toUtf8().data();
		EDomElement edefiductialkey(edefiductial.elementByTagAtt("fiductialMark","key",keyString));
		stringcsieta = QString::fromUtf8(edefiductialkey.elementByTagName("gml:pos").toString().c_str());
		item(0,(keyitem))->setText(stringcsieta.section(' ',0,0));
		item(0,(keyitem  + 1))->setText(stringcsieta.section(' ',1,1));
		item(0,(keyitem + 2))->setText(QString::fromUtf8(edefiductialkey.elementByTagName("sigma").toString().c_str()));
		keyitem = keyitem + 3;
	 }

	 EDomElement edeXa(ede.elementByTagName("Xa"));
	 item(0, 48)->setText(QString::fromUtf8(edeXa.elementByTagName("a0").toString().c_str()));
	 item(0, 49)->setText(QString::fromUtf8(edeXa.elementByTagName("a1").toString().c_str()));
	 item(0, 50)->setText(QString::fromUtf8(edeXa.elementByTagName("a2").toString().c_str()));
	 item(0, 51)->setText(QString::fromUtf8(edeXa.elementByTagName("b0").toString().c_str()));
	 item(0, 52)->setText(QString::fromUtf8(edeXa.elementByTagName("b1").toString().c_str()));
	 item(0, 53)->setText(QString::fromUtf8(edeXa.elementByTagName("b2").toString().c_str()));
	deque<EDomElement> dequeFiductial = edefiductial.children();

	item(0,55)->setText("0");
	if (dequeFiductial.size()==8)
		item(0,55)->setText("1");
};

void SensorModel::fillvalues(string values)
{
	if (values.length() > 0)
	{
		string xmlSensorValues = values;
		EDomElement ede(xmlSensorValues);
		QString strcoordprincipal, strsigmaprincipal, stringcsieta, qkeyString;
		string keyString;
		QVariant keyVariant;
		strsensor = ede.elementByTagName("sensor").attribute("key");

		setCombos(ede.elementByTagName("geometry").toString(),ede.elementByTagName("platform").toString(),ede.elementByTagName("detector").toString());
		item(0, 54)->setText(QString::fromUtf8(ede.elementByTagName("description").toString().c_str()));

		item(0, 3)->setText(QString::fromUtf8(ede.elementByTagName("number").toString().c_str()));
		item(0, 4)->setText(QString::fromUtf8(ede.elementByTagName("dispatch").toString().c_str()));
		item(0, 5)->setText(QString::fromUtf8(ede.elementByTagName("expiration").toString().c_str()));

		EDomElement edeFocalDistance(ede.elementByTagName("focalDistance"));
		item(0, 6)->setText(QString::fromUtf8(edeFocalDistance.elementByTagName("value").toString().c_str()));
		item(0, 7)->setText(QString::fromUtf8(edeFocalDistance.elementByTagName("sigma").toString().c_str()));

		EDomElement edek0(ede.elementByTagName("k0"));
		item(0, 8)->setText(QString::fromUtf8(edek0.elementByTagName("value").toString().c_str()));
		item(0, 9)->setText(QString::fromUtf8(edek0.elementByTagName("sigma").toString().c_str()));

		EDomElement edek1(ede.elementByTagName("k1"));
		item(0, 10)->setText(QString::fromUtf8(edek1.elementByTagName("value").toString().c_str()));
		item(0, 11)->setText(QString::fromUtf8(edek1.elementByTagName("sigma").toString().c_str()));

		EDomElement edek2(ede.elementByTagName("k2"));
		item(0, 12)->setText(QString::fromUtf8(edek2.elementByTagName("value").toString().c_str()));
		item(0, 13)->setText(QString::fromUtf8(edek2.elementByTagName("sigma").toString().c_str()));

		EDomElement edek3(ede.elementByTagName("k3"));
		item(0, 14)->setText(QString::fromUtf8(edek3.elementByTagName("value").toString().c_str()));
		item(0, 15)->setText(QString::fromUtf8(edek3.elementByTagName("sigma").toString().c_str()));

		EDomElement edep1(ede.elementByTagName("P1"));
		item(0, 16)->setText(QString::fromUtf8(edep1.elementByTagName("value").toString().c_str()));
		item(0, 17)->setText(QString::fromUtf8(edep1.elementByTagName("sigma").toString().c_str()));

		EDomElement edep2(ede.elementByTagName("P2"));
		item(0, 18)->setText(QString::fromUtf8(edep2.elementByTagName("value").toString().c_str()));
		item(0, 19)->setText(QString::fromUtf8(edep2.elementByTagName("sigma").toString().c_str()));

		EDomElement edeprincipalpoint(ede.elementByTagName("principalPointCoordinates"));
		strcoordprincipal = QString::fromUtf8(edeprincipalpoint.elementByTagName("gml:pos").toString().c_str());
		item(0,20)->setText(strcoordprincipal.section(' ',0,0));
		item(0,21)->setText(strcoordprincipal.section(' ',1,1));
		strsigmaprincipal = QString::fromUtf8(edeprincipalpoint.elementByTagName("sigma").toString().c_str());
		item(0,22)->setText(strsigmaprincipal.section(' ',0,0));
		item(0,23)->setText(strsigmaprincipal.section(' ',1,1));

		EDomElement edefiductial(ede.elementByTagName("fiductialMarks"));
		int keyitem = 24;
		for(int keyfiductial=1; keyfiductial<=8; keyfiductial++)
		{
			keyVariant = keyfiductial;
			qkeyString = keyVariant.toString();
			keyString = qkeyString.toUtf8().data();
			EDomElement edefiductialkey(edefiductial.elementByTagAtt("fiductialMark","key",keyString));
			stringcsieta = QString::fromUtf8(edefiductialkey.elementByTagName("gml:pos").toString().c_str());
			item(0,(keyitem))->setText(stringcsieta.section(' ',0,0));
			item(0,(keyitem  + 1))->setText(stringcsieta.section(' ',1,1));
			item(0,(keyitem + 2))->setText(QString::fromUtf8(edefiductialkey.elementByTagName("sigma").toString().c_str()));
			keyitem = keyitem + 3;
		}

		EDomElement edeXa(ede.elementByTagName("Xa"));
		item(0, 48)->setText(QString::fromUtf8(edeXa.elementByTagName("a0").toString().c_str()));
		item(0, 49)->setText(QString::fromUtf8(edeXa.elementByTagName("a1").toString().c_str()));
		item(0, 50)->setText(QString::fromUtf8(edeXa.elementByTagName("a2").toString().c_str()));
		item(0, 51)->setText(QString::fromUtf8(edeXa.elementByTagName("b0").toString().c_str()));
		item(0, 52)->setText(QString::fromUtf8(edeXa.elementByTagName("b1").toString().c_str()));
		item(0, 53)->setText(QString::fromUtf8(edeXa.elementByTagName("b2").toString().c_str()));
		deque<EDomElement> dequeFiductial = edefiductial.children();

		item(0,55)->setText("0");
		if (dequeFiductial.size()==8)
			item(0,55)->setText("1");
	}
	else
	{
		strsensor = "1";
		item(0, 0)->setText("0");
		item(0, 1)->setText("0");
		item(0, 2)->setText("0");
	}
};

void SensorModel::setReadOnly(bool state)
{
	// nao implementado para mviews do Meffe
}

void SensorModel::setCombos(string geom, string plat, string detc)
{
	if (geom == "frame")
		item(0,0)->setText("0");
	if (geom == "push broom")
		item(0,0)->setText("1");
	if (geom == "whisk broom")
		item(0,0)->setText("2");

	if (plat == "aerial")
		item(0,1)->setText("0");
	if (plat ==  "spacial")
		item(0,1)->setText("1");
	if (plat == "terrestrial")
		item(0,1)->setText("2");

	if (detc == "film")
		item(0,2)->setText("0");
	if (detc == "ccd")
		item(0,2)->setText("1");
}

void SensorModel::mountstring()
{
    
    stringstream auxStream;
    QString geometryoutstr, platformoutstr, detectoroutstr;
    
    switch (item(0,0)->text().toAscii().toInt())//item(0, 0)->text().toUtf8().data()
    {
	case 0:
		geometryoutstr = "frame";
		break;
	case 1:
		geometryoutstr = "push broom";
		break;
	case 2:
		geometryoutstr = "whisk broom";
		break;
	
	};
	
    switch (item(0,1)->text().toAscii().toInt())
    {
	case 0:
		platformoutstr = "aerial";
		break;
	case 1:
		platformoutstr = "spacial";
		break;
	case 2:
		platformoutstr = "terrestrial";
		break;
	};

    switch (item(0,2)->text().toAscii().toInt())
    {
	case 0:
		detectoroutstr = "film";
		break;
	case 1:
		detectoroutstr = "ccd";
		break;
	};
	auxStream << "<sensor key=\"" << strsensor << "\">";
	auxStream << "<type>";
	auxStream << "<geometry>" << geometryoutstr.toUtf8().data() << "</geometry>";
	auxStream << "<platform>" << platformoutstr.toUtf8().data() << "</platform>";
	auxStream << "<detector>" << detectoroutstr.toUtf8().data() << "</detector>";
	auxStream << "<energySource>natural</energySource>";
	auxStream << "<spectralRanges uom=\"#um\">";
	auxStream << "<spectralRange band=\"1\"><inferiorLimit>0.4</inferiorLimit><superiorLimit>0.7</superiorLimit>";
	auxStream << "</spectralRange></spectralRanges></type>";
	auxStream << "<description>" << item(0,54)->text().toUtf8().data() << "</description>";
	auxStream << "<calibrationCertificate>";
	auxStream << "<number>" << item(0,3)->text().toUtf8().data() << "</number>";
	auxStream << "<dispatch>" << item(0,4)->text().toUtf8().data() << "</dispatch>";
    auxStream << "<expiration>" << item(0,5)->text().toUtf8().data() << "</expiration>";
	auxStream << "</calibrationCertificate>";
	auxStream << "<focalDistance uom=\"#mm\">";
	auxStream << "<value>" << item(0,6)->text().toUtf8().data() << "</value>";
	auxStream << "<sigma>" << item(0,7)->text().toUtf8().data() << "</sigma></focalDistance>";
	auxStream << "<distortionCoefficients><radialSimetric>";
	auxStream << "<k0><value>" << item(0,8)->text().toUtf8().data() << "</value>";
	auxStream << "<sigma>" << item(0,9)->text().toUtf8().data() << "</sigma></k0>";
	auxStream << "<k1><value>" << item(0,10)->text().toUtf8().data() << "</value>";
	auxStream << "<sigma>" << item(0,11)->text().toUtf8().data() << "</sigma></k1>";
	auxStream << "<k2><value>" << item(0,12)->text().toUtf8().data() << "</value>";
	auxStream << "<sigma>" << item(0,13)->text().toUtf8().data() << "</sigma></k2>";
	auxStream << "<k3><value>" << item(0,14)->text().toUtf8().data() << "</value>";
	auxStream << "<sigma>" << item(0,15)->text().toUtf8().data() << "</sigma></k3></radialSimetric>";
    auxStream << "<descentered><P1>";
	auxStream << "<value>" << item(0,16)->text().toUtf8().data() << "</value>";
	auxStream << "<sigma>" << item(0,17)->text().toUtf8().data() << "</sigma></P1>";
	auxStream << "<P2><value>" << item(0,18)->text().toUtf8().data() << "</value>";
	auxStream << "<sigma>" << item(0,19)->text().toUtf8().data() << "</sigma></P2></descentered>";
	auxStream << "</distortionCoefficients><principalPointCoordinates uom=\"#mm\">";				
	auxStream << "<gml:pos>" << item(0,20)->text().toUtf8().data();
	auxStream << " " << item(0,21)->text().toUtf8().data() << "</gml:pos>";
	auxStream << "<sigma>" << item(0,22)->text().toUtf8().data();
	auxStream << " " << item(0,23)->text().toUtf8().data() << "</sigma></principalPointCoordinates>";
	
	if ((geometryoutstr == "frame") && (detectoroutstr == "film"))
	 {
	 	auxStream << "<fiductialMarks uom=\"#mm\">";
	 	for(int auxfiductial=1; auxfiductial<=(item(0,55)->text().toAscii().toInt()==0?4:8); auxfiductial++)
	 	{
	 		int itemfiductial = (((auxfiductial -1)*3) + 24);
	 		auxStream << "<fiductialMark key=\"" << auxfiductial << "\">";
	 		auxStream << "<gml:pos>" << item(0,itemfiductial)->text().toUtf8().data() << " ";
	 		auxStream << item(0,(itemfiductial+1))->text().toUtf8().data() << "</gml:pos>";
	 		//if (layoutReturnCombo->sigmafiductialCheckBox->isChecked())
	 		// {
	 		 	auxStream << "<sigma>" << item(0,(itemfiductial+2))->text().toUtf8().data() << "</sigma>";
	 		// }
	 		// else
	 		//  {
	 		 // 	auxStream << "<sigma>Not Available</sigma>";
	 		//  }
	 		auxStream <<  "</fiductialMark>";
	 	}
	 	 auxStream <<  "</fiductialMarks>";
	  }
	
	if ((geometryoutstr == "frame") && (detectoroutstr == "ccd"))
	 {
	 	auxStream << "<Xa><a0>" << item(0,48)->text().toUtf8().data() << "</a0>";
	 	auxStream << "<a1>" << item(0,49)->text().toUtf8().data() << "</a1>";
	 	auxStream << "<a2>" << item(0,50)->text().toUtf8().data() << "</a2>";
	 	auxStream << "<b0>" << item(0,51)->text().toUtf8().data() << "</b0>";
	 	auxStream << "<b1>" << item(0,52)->text().toUtf8().data() << "</b1>";
	 	auxStream << "<b2>" << item(0,53)->text().toUtf8().data() << "</b2></Xa>";
	 }	
	auxStream << "</sensor>";
	xmlSensorstring = auxStream.str();
    return;
};

string SensorModel::getvalues()
{
	mountstring();
   	return xmlSensorstring;
};

void SensorModel::findTextCombo(SensorLayout* layoutFindCombo)
{ // pode ser que seja preciso mudar algo aqui
   	int i;
   	
    QVariant index;
    for(i=0; i<=54 ; i++)
	{
     switch (i)
     {
       case 0:
       {
        index = layoutFindCombo->geometryComboBox->findText(item(0, i)->text().toLower());
        item(0,i)->setText(index.toString());
        break;
       }
       case 1:
       {
        index = layoutFindCombo->platformComboBox->findText(item(0, i)->text().toLower());
        item(0,i)->setText(index.toString());
        break;
       }
       case 2:
       {
        index = layoutFindCombo->detectorComboBox->findText(item(0, i)->text().toLower());
        item(0,i)->setText(index.toString());
        break;
       }
     }
     }
};

void SensorModel::showModel()
{
	//for(int i=0; i<=55 ; i++)
	{
	   //cout << item(0,i)->text().toUtf8().data() << " :   ";
	}
};

SensorLayout::SensorLayout(QWidget* Owner)
{
    
    descriptionLabel = new QLabel(tr("Description:"),Owner);
    descriptionTextEdit = new QTextEdit;//new QLineEdit;
    descriptionTextEdit->setFixedHeight(40);
    descriptionTextEdit->setFixedWidth(420);
    descriptionHLayout = new QHBoxLayout;
    descriptionHLayout->addWidget(descriptionLabel);
    descriptionHLayout->addWidget(descriptionTextEdit);
    descriptionHLayout->addStretch();
    
    geometryLabel = new QLabel(tr("Geometry:"),Owner);
    geometryComboBox = new QComboBox(Owner);
    geometryComboBox->addItem(tr("frame"));
	//geometryComboBox->addItem(tr("push broom"));
	//geometryComboBox->addItem(tr("whisk broom"));
    
       
    geometryHLayout = new QHBoxLayout;
    geometryHLayout->addWidget(geometryLabel);
    geometryHLayout->addWidget(geometryComboBox);
    geometryHLayout->addStretch();
    
    platformLabel = new QLabel(tr("Platform:"),Owner);
    platformComboBox = new QComboBox(Owner);
    platformComboBox->addItem(tr("aerial"));
	//platformComboBox->addItem(tr("spacial"));
	//platformComboBox->addItem(tr("terrestrial"));

    
       
    platformHLayout = new QHBoxLayout;
    platformHLayout->addWidget(platformLabel);
    platformHLayout->addWidget(platformComboBox);
    platformHLayout->addStretch();
    
    detectorLabel = new QLabel(tr("Detector:"),Owner);
    detectorComboBox = new QComboBox(Owner);
    detectorComboBox->addItem(tr("film"));
    detectorComboBox->addItem(tr("ccd"));
    
        
    detectorHLayout = new QHBoxLayout;
    detectorHLayout->addWidget(detectorLabel);
    detectorHLayout->addWidget(detectorComboBox);
    detectorHLayout->addStretch();
    
    typeVLayout = new QVBoxLayout;
    typeVLayout->addLayout(geometryHLayout);
    typeVLayout->addLayout(platformHLayout);
    typeVLayout->addLayout(detectorHLayout);
    
    typeGroupBox = new QGroupBox(tr("Type"),Owner);
    typeGroupBox->setLayout(typeVLayout);
    typeGroupBox->setMaximumWidth(500);
    
    numberCalibrationLabel = new QLabel(tr("Number:"),Owner);
    numberCalibrationLineEdit = new QLineEdit(Owner);
    numberHLayout = new QHBoxLayout;
    numberHLayout->addWidget(numberCalibrationLabel);
    numberHLayout->addWidget(numberCalibrationLineEdit);
    numberHLayout->addStretch();
    
    dispatchCalibrationLabel = new QLabel(tr("Dispatch:"),Owner);
    dispatchCalibrationDateTimeEdit = new QDateTimeEdit(Owner);
    dispatchHLayout = new QHBoxLayout;
    dispatchHLayout->addWidget(dispatchCalibrationLabel);
    dispatchHLayout->addWidget(dispatchCalibrationDateTimeEdit);
    dispatchHLayout->addStretch();
    
    expirationCalibrationLabel = new QLabel(tr("Expiration:"),Owner);
    expirationCalibrationDateTimeEdit = new QDateTimeEdit(Owner);
    expirationHLayout = new QHBoxLayout;
    expirationHLayout->addWidget(expirationCalibrationLabel);
    expirationHLayout->addWidget(expirationCalibrationDateTimeEdit);
    expirationHLayout->addStretch();
    
    calibrationVLayout = new QVBoxLayout;
    calibrationVLayout->addLayout(numberHLayout);
    calibrationVLayout->addLayout(dispatchHLayout);    
    calibrationVLayout->addLayout(expirationHLayout);
    
    calibrationGroupBox = new QGroupBox(tr("Camera Calibration Certificate: "),Owner);
    calibrationGroupBox->setLayout(calibrationVLayout);
    calibrationGroupBox->setMaximumWidth(500);
    
    focaldistanceLabel = new QLabel(tr("Focal Distance:"),Owner);
    valuefocalLineEdit = new QLineEdit(Owner);
    valuefocalLineEdit->setValidator(new QDoubleValidator(valuefocalLineEdit));
    sigmafocalLabel = new QLabel(tr("sigma:"),Owner);
    sigmafocalLineEdit = new QLineEdit(Owner);
    QLabel *mmLabel = new QLabel("mm",Owner);
    QLabel *mmsigmaLabel = new QLabel("mm",Owner);
    focaldistanceHLayout = new QHBoxLayout;
    focaldistanceHLayout->addWidget(focaldistanceLabel);
    focaldistanceHLayout->addWidget(valuefocalLineEdit);
    focaldistanceHLayout->addWidget(mmLabel);
    focaldistanceHLayout->addSpacing(30);
    focaldistanceHLayout->addWidget(sigmafocalLabel);
    focaldistanceHLayout->addWidget(sigmafocalLineEdit);
    focaldistanceHLayout->addWidget(mmsigmaLabel);
    
    k0Label = new QLabel(tr("k0"),Owner);
    k0LineEdit = new QLineEdit(Owner);
    k0LineEdit->setValidator(new QDoubleValidator(k0LineEdit));    
    sigmak0Label = new QLabel(tr("sigma"),Owner);
    sigmak0LineEdit = new QLineEdit(Owner);
    k1Label = new QLabel(tr("k1"),Owner);
    k1LineEdit = new QLineEdit(Owner);
    k1LineEdit->setValidator(new QDoubleValidator(k1LineEdit));    
    sigmak1Label = new QLabel(tr("sigma"),Owner);
    sigmak1LineEdit = new QLineEdit(Owner);
    k2Label = new QLabel(tr("k2"),Owner);
    k2LineEdit = new QLineEdit(Owner);
    k2LineEdit->setValidator(new QDoubleValidator(k2LineEdit));    
    sigmak2Label = new QLabel(tr("sigma"),Owner);
    sigmak2LineEdit = new QLineEdit(Owner);
    k3Label = new QLabel(tr("k3"),Owner);
    k3LineEdit = new QLineEdit(Owner);
    k3LineEdit->setValidator(new QDoubleValidator(k3LineEdit));    
    sigmak3Label = new QLabel(tr("sigma"),Owner);
    sigmak3LineEdit = new QLineEdit(Owner);
    radialGridLayout = new QGridLayout;
    radialGridLayout->addWidget(k0Label,0,0);    
    radialGridLayout->addWidget(k0LineEdit,0,1);
    radialGridLayout->addWidget(sigmak0Label,0,2);    
    radialGridLayout->addWidget(sigmak0LineEdit,0,3);
    radialGridLayout->addWidget(k1Label,1,0);    
    radialGridLayout->addWidget(k1LineEdit,1,1);
    radialGridLayout->addWidget(sigmak1Label,1,2);    
    radialGridLayout->addWidget(sigmak1LineEdit,1,3);
    radialGridLayout->addWidget(k2Label,2,0);    
    radialGridLayout->addWidget(k2LineEdit,2,1);
    radialGridLayout->addWidget(sigmak2Label,2,2);    
    radialGridLayout->addWidget(sigmak2LineEdit,2,3);
    radialGridLayout->addWidget(k3Label,3,0);    
    radialGridLayout->addWidget(k3LineEdit,3,1);
    radialGridLayout->addWidget(sigmak3Label,3,2);    
    radialGridLayout->addWidget(sigmak3LineEdit,3,3);
    radialsimetricGroupBox = new QGroupBox(tr("Radial Simetric"),Owner);               
    radialsimetricGroupBox->setLayout(radialGridLayout);
    
    
    p1Label = new QLabel(tr("p1"),Owner);
    p1LineEdit = new QLineEdit(Owner);
    p1LineEdit->setValidator(new QDoubleValidator(p1LineEdit));    
    sigmap1Label = new QLabel(tr("sigma"),Owner);
    sigmap1LineEdit = new QLineEdit(Owner);
    p2Label = new QLabel(tr("p2"),Owner);
    p2LineEdit = new QLineEdit(Owner);
    p2LineEdit->setValidator(new QDoubleValidator(p2LineEdit));    
    sigmap2Label = new QLabel(tr("sigma"),Owner);
    sigmap2LineEdit = new QLineEdit(Owner);
    descenteredGridLayout = new QGridLayout;    
    descenteredGridLayout->addWidget(p1Label,0,0);    
    descenteredGridLayout->addWidget(p1LineEdit,0,1);
    descenteredGridLayout->addWidget(sigmap1Label,0,2);    
    descenteredGridLayout->addWidget(sigmap1LineEdit,0,3);
    descenteredGridLayout->addWidget(p2Label,1,0);    
    descenteredGridLayout->addWidget(p2LineEdit,1,1);
    descenteredGridLayout->addWidget(sigmap2Label,1,2);    
    descenteredGridLayout->addWidget(sigmap2LineEdit,1,3);
    descenteredGroupBox = new QGroupBox(tr("Descentered"),Owner);               
    descenteredGroupBox->setLayout(descenteredGridLayout);
    
    distortioncoefGroupBox = new  QGroupBox(tr("Distortion Coefficients"));
    distortioncoefVLayout = new QVBoxLayout;
    distortioncoefVLayout->addWidget(radialsimetricGroupBox);
    distortioncoefVLayout->addWidget(descenteredGroupBox);    
    distortioncoefGroupBox-> setLayout(distortioncoefVLayout);    

    opticalVLayout = new QVBoxLayout;
    opticalVLayout->addLayout(focaldistanceHLayout);
 //   distortionLabel = new QLabel(tr("<b>Distortion Coefficients:</b>"),Owner);
    opticalVLayout->addWidget(distortioncoefGroupBox);
 //   opticalVLayout->addWidget(radialsimetricGroupBox);
//    opticalVLayout->addWidget(descenteredGroupBox);
    opticalGroupBox = new QGroupBox(tr("Optical Parameters"),Owner);
    opticalGroupBox->setLayout(opticalVLayout);
    opticalGroupBox->setMaximumWidth(500);
    
    QLabel *csiLabel = new QLabel(QChar(0x03BE));
    QLabel *etaLabel = new QLabel(QChar(0x03B7));
    coord01principalpointLineEdit = new QLineEdit(Owner);
    coord01principalpointLineEdit->setValidator(new QDoubleValidator(coord01principalpointLineEdit));    
    coord02principalpointLineEdit = new QLineEdit(Owner);
    coord02principalpointLineEdit->setValidator(new QDoubleValidator(coord02principalpointLineEdit));    
    sigmaprincipalpointLabel = new QLabel(tr("Sigma:"),Owner);
    QLabel *csisigmaLabel = new QLabel(QChar(0x03BE));
    QLabel *etasigmaLabel = new QLabel(QChar(0x03B7));    
    sigmacoord01principalpointLineEdit = new QLineEdit(Owner);
    sigmacoord02principalpointLineEdit = new QLineEdit(Owner);    
    principalpointHLayout = new QHBoxLayout;
    principalpointHLayout->addWidget(csiLabel);
    principalpointHLayout->addWidget(coord01principalpointLineEdit);
    principalpointHLayout->addWidget(etaLabel);
    principalpointHLayout->addWidget(coord02principalpointLineEdit);
    principalpointHLayout->addSpacing(20);    
    principalpointHLayout->addWidget(sigmaprincipalpointLabel);
    principalpointHLayout->addWidget(csisigmaLabel);
    principalpointHLayout->addWidget(sigmacoord01principalpointLineEdit);
    principalpointHLayout->addWidget(etasigmaLabel);    
    principalpointHLayout->addWidget(sigmacoord02principalpointLineEdit);
    principalpointGroupBox = new QGroupBox(tr("Principal Points Coordinates"),Owner);
    principalpointGroupBox->setLayout(principalpointHLayout);
    principalpointGroupBox->setMaximumWidth(500);
    
    nrfiductialLabel = new QLabel(tr("Number fiductial marks"),Owner);
    nrfiductialComboBox = new QComboBox(Owner);
    nrfiductialComboBox->addItem("4");
    nrfiductialComboBox->addItem("8");
    sigmafiductialCheckBox = new QCheckBox(tr("Sigma avaiable"),Owner);
    sigmafiductialCheckBox->setCheckState(Qt::Checked);
    fiductialoptionsHLayout = new QHBoxLayout;
    fiductialoptionsHLayout->addWidget(nrfiductialLabel);
    fiductialoptionsHLayout->addWidget(nrfiductialComboBox);
    fiductialoptionsHLayout->addStretch();
    fiductialoptionsHLayout->addWidget(sigmafiductialCheckBox);
    QLabel *fiductial01csiLabel = new QLabel(QChar(0x03BE));
    fiductial01csiLineEdit = new QLineEdit(Owner);
    fiductial01csiLineEdit->setValidator(new QDoubleValidator(fiductial01csiLineEdit));
    QLabel *fiductial01etaLabel = new QLabel(QChar(0x03B7));
    fiductial01etaLineEdit = new QLineEdit(Owner);
    fiductial01etaLineEdit->setValidator(new QDoubleValidator(fiductial01etaLineEdit));    
    fiductial01sigmaLabel = new QLabel(tr("sigma:"),Owner);
    fiductial01sigmaLineEdit = new QLineEdit(Owner);
    fiductial01HLayout = new QHBoxLayout;
    fiductial01HLayout->addWidget(fiductial01csiLabel);
    fiductial01HLayout->addWidget(fiductial01csiLineEdit);
    fiductial01HLayout->addWidget(fiductial01etaLabel);
    fiductial01HLayout->addWidget(fiductial01etaLineEdit);
    fiductial01HLayout->addStretch();
    fiductial01HLayout->addWidget(fiductial01sigmaLabel);
    fiductial01HLayout->addWidget(fiductial01sigmaLineEdit);
    
    QLabel *fiductial02csiLabel = new QLabel(QChar(0x03BE));
    fiductial02csiLineEdit = new QLineEdit(Owner);
    fiductial02csiLineEdit->setValidator(new QDoubleValidator(fiductial02csiLineEdit));    
    QLabel *fiductial02etaLabel = new QLabel(QChar(0x03B7));
    fiductial02etaLineEdit = new QLineEdit(Owner);
    fiductial02etaLineEdit->setValidator(new QDoubleValidator(fiductial02etaLineEdit));    
    fiductial02sigmaLabel = new QLabel(tr("sigma:"),Owner);
    fiductial02sigmaLineEdit = new QLineEdit(Owner);
    fiductial02HLayout = new QHBoxLayout;
    fiductial02HLayout->addWidget(fiductial02csiLabel);
    fiductial02HLayout->addWidget(fiductial02csiLineEdit);
    fiductial02HLayout->addWidget(fiductial02etaLabel);
    fiductial02HLayout->addWidget(fiductial02etaLineEdit);
    fiductial02HLayout->addStretch();
    fiductial02HLayout->addWidget(fiductial02sigmaLabel);
    fiductial02HLayout->addWidget(fiductial02sigmaLineEdit);

    QLabel *fiductial03csiLabel = new QLabel(QChar(0x03BE));
    fiductial03csiLineEdit = new QLineEdit(Owner);
    fiductial03csiLineEdit->setValidator(new QDoubleValidator(fiductial03csiLineEdit));    
    QLabel *fiductial03etaLabel = new QLabel(QChar(0x03B7));
    fiductial03etaLineEdit = new QLineEdit(Owner);
    fiductial03etaLineEdit->setValidator(new QDoubleValidator(fiductial03etaLineEdit));    
    fiductial03sigmaLabel = new QLabel(tr("sigma:"),Owner);
    fiductial03sigmaLineEdit = new QLineEdit(Owner);
    fiductial03HLayout = new QHBoxLayout;
    fiductial03HLayout->addWidget(fiductial03csiLabel);
    fiductial03HLayout->addWidget(fiductial03csiLineEdit);
    fiductial03HLayout->addWidget(fiductial03etaLabel);
    fiductial03HLayout->addWidget(fiductial03etaLineEdit);
    fiductial03HLayout->addStretch();
    fiductial03HLayout->addWidget(fiductial03sigmaLabel);
    fiductial03HLayout->addWidget(fiductial03sigmaLineEdit);
    
    QLabel *fiductial04csiLabel = new QLabel(QChar(0x03BE));
    fiductial04csiLineEdit = new QLineEdit(Owner);
    fiductial04csiLineEdit->setValidator(new QDoubleValidator(fiductial04csiLineEdit));    
    QLabel *fiductial04etaLabel = new QLabel(QChar(0x03B7));
    fiductial04etaLineEdit = new QLineEdit(Owner);
    fiductial04etaLineEdit->setValidator(new QDoubleValidator(fiductial04etaLineEdit));    
    fiductial04sigmaLabel = new QLabel(tr("sigma:"),Owner);
    fiductial04sigmaLineEdit = new QLineEdit(Owner);
    fiductial04HLayout = new QHBoxLayout;
    fiductial04HLayout->addWidget(fiductial04csiLabel);
    fiductial04HLayout->addWidget(fiductial04csiLineEdit);
    fiductial04HLayout->addWidget(fiductial04etaLabel);
    fiductial04HLayout->addWidget(fiductial04etaLineEdit);
    fiductial04HLayout->addStretch();
    fiductial04HLayout->addWidget(fiductial04sigmaLabel);
    fiductial04HLayout->addWidget(fiductial04sigmaLineEdit);
    
    fourfiductialVLayout = new QVBoxLayout;
	fourfiductialVLayout->addLayout(fiductial01HLayout);
	fourfiductialVLayout->addLayout(fiductial02HLayout);
	fourfiductialVLayout->addLayout(fiductial03HLayout);
	fourfiductialVLayout->addLayout(fiductial04HLayout);	
	fourfiductialGroupBox = new QGroupBox;
	fourfiductialGroupBox->setLayout(fourfiductialVLayout);
	fourfiductialGroupBox->setStyleSheet("QGroupBox {border: 0px}");

    QLabel *fiductial05csiLabel = new QLabel(QChar(0x03BE));
    fiductial05csiLineEdit = new QLineEdit(Owner);
    fiductial05csiLineEdit->setValidator(new QDoubleValidator(fiductial05csiLineEdit));    
    QLabel *fiductial05etaLabel = new QLabel(QChar(0x03B7));
    fiductial05etaLineEdit = new QLineEdit(Owner);
    fiductial05etaLineEdit->setValidator(new QDoubleValidator(fiductial05etaLineEdit));    
    fiductial05sigmaLabel = new QLabel(tr("sigma:"),Owner);
    fiductial05sigmaLineEdit = new QLineEdit(Owner);
    fiductial05HLayout = new QHBoxLayout;
    fiductial05HLayout->addWidget(fiductial05csiLabel);
    fiductial05HLayout->addWidget(fiductial05csiLineEdit);
    fiductial05HLayout->addWidget(fiductial05etaLabel);
    fiductial05HLayout->addWidget(fiductial05etaLineEdit);
    fiductial05HLayout->addStretch();
    fiductial05HLayout->addWidget(fiductial05sigmaLabel);
    fiductial05HLayout->addWidget(fiductial05sigmaLineEdit);
    
    QLabel *fiductial06csiLabel = new QLabel(QChar(0x03BE));
    fiductial06csiLineEdit = new QLineEdit(Owner);
    fiductial06csiLineEdit->setValidator(new QDoubleValidator(fiductial06csiLineEdit));    
    QLabel *fiductial06etaLabel = new QLabel(QChar(0x03B7));
    fiductial06etaLineEdit = new QLineEdit(Owner);
    fiductial06etaLineEdit->setValidator(new QDoubleValidator(fiductial06etaLineEdit));    
    fiductial06sigmaLabel = new QLabel(tr("sigma:"),Owner);
    fiductial06sigmaLineEdit = new QLineEdit(Owner);
    fiductial06HLayout = new QHBoxLayout;
    fiductial06HLayout->addWidget(fiductial06csiLabel);
    fiductial06HLayout->addWidget(fiductial06csiLineEdit);
    fiductial06HLayout->addWidget(fiductial06etaLabel);
    fiductial06HLayout->addWidget(fiductial06etaLineEdit);
    fiductial06HLayout->addStretch();
    fiductial06HLayout->addWidget(fiductial06sigmaLabel);
    fiductial06HLayout->addWidget(fiductial06sigmaLineEdit);

    QLabel *fiductial07csiLabel = new QLabel(QChar(0x03BE));
    fiductial07csiLineEdit = new QLineEdit(Owner);
    fiductial07csiLineEdit->setValidator(new QDoubleValidator(fiductial07csiLineEdit));    
    QLabel *fiductial07etaLabel = new QLabel(QChar(0x03B7));
    fiductial07etaLineEdit = new QLineEdit(Owner);
    fiductial07etaLineEdit->setValidator(new QDoubleValidator(fiductial07etaLineEdit));    
    fiductial07sigmaLabel = new QLabel(tr("sigma:"),Owner);
    fiductial07sigmaLineEdit = new QLineEdit(Owner);
    fiductial07HLayout = new QHBoxLayout;
    fiductial07HLayout->addWidget(fiductial07csiLabel);
    fiductial07HLayout->addWidget(fiductial07csiLineEdit);
    fiductial07HLayout->addWidget(fiductial07etaLabel);
    fiductial07HLayout->addWidget(fiductial07etaLineEdit);
    fiductial07HLayout->addStretch();
    fiductial07HLayout->addWidget(fiductial07sigmaLabel);
    fiductial07HLayout->addWidget(fiductial07sigmaLineEdit);
    
    QLabel *fiductial08csiLabel = new QLabel(QChar(0x03BE));
    fiductial08csiLineEdit = new QLineEdit(Owner);
    fiductial08csiLineEdit->setValidator(new QDoubleValidator(fiductial08csiLineEdit));    
    QLabel *fiductial08etaLabel = new QLabel(QChar(0x03B7));
    fiductial08etaLineEdit = new QLineEdit(Owner);
    fiductial08etaLineEdit->setValidator(new QDoubleValidator(fiductial08etaLineEdit));    
    fiductial08sigmaLabel = new QLabel(tr("sigma:"),Owner);
    fiductial08sigmaLineEdit = new QLineEdit(Owner);
    fiductial08HLayout = new QHBoxLayout;
    fiductial08HLayout->addWidget(fiductial08csiLabel);
    fiductial08HLayout->addWidget(fiductial08csiLineEdit);
    fiductial08HLayout->addWidget(fiductial08etaLabel);
    fiductial08HLayout->addWidget(fiductial08etaLineEdit);
    fiductial08HLayout->addStretch();
    fiductial08HLayout->addWidget(fiductial08sigmaLabel);
    fiductial08HLayout->addWidget(fiductial08sigmaLineEdit);
    
    eigthfiductialVLayout = new QVBoxLayout;
	eigthfiductialVLayout->addLayout(fiductial05HLayout);
	eigthfiductialVLayout->addLayout(fiductial06HLayout);
	eigthfiductialVLayout->addLayout(fiductial07HLayout);
	eigthfiductialVLayout->addLayout(fiductial08HLayout);	
	eigthfiductialGroupBox = new QGroupBox;
	eigthfiductialGroupBox->setLayout(eigthfiductialVLayout);
	eigthfiductialGroupBox->setStyleSheet("QGroupBox {border: 0px}");	
	
	fiductialVLayout = new QVBoxLayout;
	fiductialVLayout->addLayout(fiductialoptionsHLayout);
	fiductialVLayout->addWidget(fourfiductialGroupBox);
	fiductialVLayout->addWidget(eigthfiductialGroupBox);
	
	fiductialGroupBox = new QGroupBox(tr("Fiductial Marks"),Owner);
	fiductialGroupBox->setLayout(fiductialVLayout);
	fiductialGroupBox->setMaximumWidth(500);
	
	a0Label = new QLabel(tr("a0"),Owner);
	a0LineEdit = new QLineEdit(Owner);
    a0LineEdit->setValidator(new QDoubleValidator(a0LineEdit));	
	a1Label = new QLabel(tr("a1"),Owner);
	a1LineEdit = new QLineEdit(Owner);
    a1LineEdit->setValidator(new QDoubleValidator(a1LineEdit));		
	a2Label = new QLabel(tr("a2"),Owner);
	a2LineEdit = new QLineEdit(Owner);
    a2LineEdit->setValidator(new QDoubleValidator(a2LineEdit));		
	b0Label = new QLabel(tr("b0"),Owner);
	b0LineEdit = new QLineEdit(Owner);
    b0LineEdit->setValidator(new QDoubleValidator(b0LineEdit));		
	b1Label = new QLabel(tr("b1"),Owner);
	b1LineEdit = new QLineEdit(Owner);
    b1LineEdit->setValidator(new QDoubleValidator(b1LineEdit));		
	b2Label = new QLabel(tr("b2"),Owner);
	b2LineEdit = new QLineEdit(Owner);
    b2LineEdit->setValidator(new QDoubleValidator(b2LineEdit));		
	xaGridLayout = new QGridLayout;
	xaGridLayout->addWidget(a0Label,0,0);
	xaGridLayout->addWidget(a0LineEdit,0,1);
	xaGridLayout->addWidget(a1Label,1,0);
	xaGridLayout->addWidget(a1LineEdit,1,1);
	xaGridLayout->addWidget(a2Label,2,0);
	xaGridLayout->addWidget(a2LineEdit,2,1);
	xaGridLayout->addWidget(b0Label,3,0);
	xaGridLayout->addWidget(b0LineEdit,3,1);
	xaGridLayout->addWidget(b1Label,4,0);
	xaGridLayout->addWidget(b1LineEdit,4,1);
	xaGridLayout->addWidget(b2Label,5,0);
	xaGridLayout->addWidget(b2LineEdit,5,1);
	xaHLayout = new QHBoxLayout;
	xaHLayout->addStretch();
	xaHLayout->addLayout(xaGridLayout);
	xaHLayout->addStretch();			
	xaGroupBox = new QGroupBox(tr("Xa"),Owner);
	xaGroupBox->setLayout(xaHLayout);
	xaGroupBox->setMaximumWidth(500);			

	mainLayout = new QVBoxLayout;
	mainLayout->addLayout(descriptionHLayout);
	mainLayout->addWidget(typeGroupBox);
	mainLayout->addWidget(calibrationGroupBox);
	mainLayout->addWidget(opticalGroupBox);
	mainLayout->addWidget(principalpointGroupBox);
	mainLayout->addWidget(fiductialGroupBox);
	mainLayout->addWidget(xaGroupBox);		
		
	addLayout(mainLayout);

	connect(sigmafiductialCheckBox,SIGNAL(stateChanged(int)),this,SLOT(showsigma(int)));	
	connect(geometryComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(showfiductialXa()));
	connect(detectorComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(showfiductialXa()));
	connect(nrfiductialComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(shownrfiductial()));	
	

// This version only works with frame, aerial, film/ccd sensors
	geometryComboBox->setEnabled(0);platformComboBox->setEnabled(0);detectorComboBox->setEnabled(0);
};

void SensorLayout::showsigma(int isAvaiable)
{
	if (isAvaiable == 0)
	 {   
		fiductial01sigmaLineEdit->setText(tr("Not Available"));
    	fiductial01sigmaLineEdit->setReadOnly(1);
    	fiductial02sigmaLineEdit->setText(tr("Not Available"));
    	fiductial02sigmaLineEdit->setReadOnly(1);    	
    	fiductial03sigmaLineEdit->setText(tr("Not Available"));
    	fiductial03sigmaLineEdit->setReadOnly(1);    	
    	fiductial04sigmaLineEdit->setText(tr("Not Available"));
    	fiductial04sigmaLineEdit->setReadOnly(1);    	
    	fiductial05sigmaLineEdit->setText(tr("Not Available"));
    	fiductial05sigmaLineEdit->setReadOnly(1);    	
    	fiductial06sigmaLineEdit->setText(tr("Not Available"));
    	fiductial06sigmaLineEdit->setReadOnly(1);    	
    	fiductial07sigmaLineEdit->setText(tr("Not Available"));
    	fiductial07sigmaLineEdit->setReadOnly(1);    	
    	fiductial08sigmaLineEdit->setText(tr("Not Available"));
    	fiductial08sigmaLineEdit->setReadOnly(1);  
 	 }
	else
	 { 
	    fiductial01sigmaLineEdit->setReadOnly(0);
	    fiductial01sigmaLineEdit->setValidator(new QDoubleValidator(fiductial01sigmaLineEdit));
	    fiductial01sigmaLineEdit->clear();
	    fiductial02sigmaLineEdit->setReadOnly(0);
	    fiductial02sigmaLineEdit->setValidator(new QDoubleValidator(fiductial01sigmaLineEdit));
	    fiductial02sigmaLineEdit->clear();	    
	    fiductial03sigmaLineEdit->setReadOnly(0);
	    fiductial03sigmaLineEdit->setValidator(new QDoubleValidator(fiductial01sigmaLineEdit));
	    fiductial03sigmaLineEdit->clear();	    
	    fiductial04sigmaLineEdit->setReadOnly(0);
	    fiductial04sigmaLineEdit->setValidator(new QDoubleValidator(fiductial01sigmaLineEdit));
	    fiductial04sigmaLineEdit->clear();
        fiductial05sigmaLineEdit->setReadOnly(0);
        fiductial05sigmaLineEdit->setValidator(new QDoubleValidator(fiductial01sigmaLineEdit));
	    fiductial05sigmaLineEdit->clear();
	    fiductial06sigmaLineEdit->setReadOnly(0);
	    fiductial06sigmaLineEdit->setValidator(new QDoubleValidator(fiductial01sigmaLineEdit));
	    fiductial06sigmaLineEdit->clear();	    
	    fiductial07sigmaLineEdit->setReadOnly(0);
	    fiductial07sigmaLineEdit->setValidator(new QDoubleValidator(fiductial01sigmaLineEdit));
	    fiductial07sigmaLineEdit->clear();	    
	    fiductial08sigmaLineEdit->setReadOnly(0);
	    fiductial08sigmaLineEdit->setValidator(new QDoubleValidator(fiductial01sigmaLineEdit));
	    fiductial08sigmaLineEdit->clear();	    
     }
};    	
	 	

void SensorLayout::showfiductialXa()
{
	if ((geometryComboBox->currentText() == "frame") && (detectorComboBox->currentText() == "film"))
	 {
       fiductialGroupBox->show();
       shownrfiductial();
       a0LineEdit->clear();
       a1LineEdit->clear();
       a2LineEdit->clear();
       b0LineEdit->clear();
       b1LineEdit->clear();
       b2LineEdit->clear();
       xaGroupBox->hide();
	 }
	else
	 {
	   fiductialGroupBox->hide();
	   if ((geometryComboBox->currentText() == "frame") && (detectorComboBox->currentText() == "ccd"))
	 	{
	   		xaGroupBox->show();
	 	}
    	else
	 	 {		
       		xaGroupBox->hide();
       		a0LineEdit->clear();
       		a1LineEdit->clear();
       		a2LineEdit->clear();
       		b0LineEdit->clear();
       		b1LineEdit->clear();
       		b2LineEdit->clear();
	 	 }
    	fiductial01csiLineEdit->clear();
    	fiductial01etaLineEdit->clear();
    	fiductial01sigmaLineEdit->clear();
    	fiductial02csiLineEdit->clear();
    	fiductial02etaLineEdit->clear();
    	fiductial02sigmaLineEdit->clear(); 
    	fiductial03csiLineEdit->clear();
    	fiductial03etaLineEdit->clear();
    	fiductial03sigmaLineEdit->clear();
    	fiductial04csiLineEdit->clear();
    	fiductial04etaLineEdit->clear();
    	fiductial04sigmaLineEdit->clear();
    	fiductial05csiLineEdit->clear();
    	fiductial05etaLineEdit->clear();
    	fiductial05sigmaLineEdit->clear();
    	fiductial06csiLineEdit->clear();
    	fiductial06etaLineEdit->clear();
    	fiductial06sigmaLineEdit->clear();
    	fiductial07csiLineEdit->clear();
    	fiductial07etaLineEdit->clear();
    	fiductial07sigmaLineEdit->clear();
    	fiductial08csiLineEdit->clear();
    	fiductial08etaLineEdit->clear();
    	fiductial08sigmaLineEdit->clear();	 	 
	 }
	 
        //cout << "saida showfiductialXa" << endl;
    return;
};

void SensorLayout::shownrfiductial()
{
	if ((nrfiductialComboBox->currentText() == "4"))
      {
        eigthfiductialGroupBox->hide();
	  }
	else
	  {
	  	eigthfiductialGroupBox->show();
	  }	  
};

SensorLayout::~SensorLayout()
{
    delete geometryLabel;
    delete platformLabel;
    delete detectorLabel;
    delete descriptionLabel;
    delete numberCalibrationLabel;
    delete dispatchCalibrationLabel;
    delete expirationCalibrationLabel;
    delete focaldistanceLabel;
    delete sigmafocalLabel;
    delete k0Label;
    delete sigmak0Label;
    delete k1Label; 
    delete sigmak1Label;
    delete k2Label;
    delete sigmak2Label;
    delete k3Label;
    delete sigmak3Label;
    delete p1Label;
    delete sigmap1Label;
    delete p2Label;
    delete sigmap2Label;
    delete sigmaprincipalpointLabel;
	delete nrfiductialLabel;
    delete fiductial01sigmaLabel;
    delete fiductial02sigmaLabel;
    delete fiductial03sigmaLabel;
    delete fiductial04sigmaLabel;
    delete fiductial05sigmaLabel;
    delete fiductial06sigmaLabel;
    delete fiductial07sigmaLabel;
    delete fiductial08sigmaLabel;
    delete a0Label;
    delete a1Label;
    delete a2Label;
    delete b0Label;
    delete b1Label;
    delete b2Label;
    delete numberCalibrationLineEdit;
    delete dispatchCalibrationDateTimeEdit;
    delete expirationCalibrationDateTimeEdit;
    delete valuefocalLineEdit;
    delete sigmafocalLineEdit;
    delete k0LineEdit;
    delete sigmak0LineEdit;
    delete k1LineEdit;
    delete sigmak1LineEdit;
    delete k2LineEdit;
    delete sigmak2LineEdit;
    delete k3LineEdit;
    delete sigmak3LineEdit;
    delete p1LineEdit;
    delete sigmap1LineEdit;
    delete p2LineEdit;
    delete sigmap2LineEdit;
    delete coord01principalpointLineEdit;
    delete coord02principalpointLineEdit;
    delete sigmacoord01principalpointLineEdit;
    delete sigmacoord02principalpointLineEdit;
    delete fiductial01csiLineEdit;
    delete fiductial01etaLineEdit;
    delete fiductial02csiLineEdit;
    delete fiductial02etaLineEdit;
    delete fiductial03csiLineEdit;
    delete fiductial03etaLineEdit;
    delete fiductial04csiLineEdit;
    delete fiductial04etaLineEdit;
    delete fiductial05csiLineEdit;
    delete fiductial05etaLineEdit;
    delete fiductial06csiLineEdit;
    delete fiductial06etaLineEdit;
    delete fiductial07csiLineEdit;
    delete fiductial07etaLineEdit;
    delete fiductial08csiLineEdit;
    delete fiductial08etaLineEdit;
    delete fiductial01sigmaLineEdit;
    delete fiductial02sigmaLineEdit;
    delete fiductial03sigmaLineEdit;
    delete fiductial04sigmaLineEdit;
    delete fiductial05sigmaLineEdit;
    delete fiductial06sigmaLineEdit;
    delete fiductial07sigmaLineEdit;
    delete fiductial08sigmaLineEdit;
    delete a0LineEdit;
    delete a1LineEdit;
    delete a2LineEdit;
 	delete b0LineEdit;
 	delete b1LineEdit;
 	delete b2LineEdit;
        delete descriptionTextEdit;
    delete geometryComboBox;
    delete platformComboBox;
    delete detectorComboBox;
    delete nrfiductialComboBox;
    delete sigmafiductialCheckBox;
    delete typeGroupBox;
    delete calibrationGroupBox;
    delete radialsimetricGroupBox;
    delete descenteredGroupBox;
    delete distortioncoefGroupBox;
    delete principalpointGroupBox;
    delete opticalGroupBox;
 	delete fourfiductialGroupBox;
    delete eigthfiductialGroupBox;
    delete xaGroupBox;
 	delete fiductialGroupBox;    

}

void SensorView::changeText()
{
	 	SensorModel *MySensorModel;
 	 	MySensorModel = (SensorModel*) sensormapper.model();	
		MySensorModel->mountstring();
};

void SensorMapper::makeMapper(SensorLayout* getlayoutMapper, SensorModel *getMapperModel)
{
	getMapperModel->findTextCombo(getlayoutMapper);
	setModel(getMapperModel);	
    addMapping(getlayoutMapper->geometryComboBox,0,"currentIndex");
    addMapping(getlayoutMapper->platformComboBox,1,"currentIndex"); 
    addMapping(getlayoutMapper->detectorComboBox,2,"currentIndex");
    addMapping(getlayoutMapper->numberCalibrationLineEdit,3);  
    addMapping(getlayoutMapper->dispatchCalibrationDateTimeEdit,4);
    addMapping(getlayoutMapper->expirationCalibrationDateTimeEdit,5);   
    addMapping(getlayoutMapper->valuefocalLineEdit,6);  
    addMapping(getlayoutMapper->sigmafocalLineEdit,7);
    addMapping(getlayoutMapper->k0LineEdit,8);
    addMapping(getlayoutMapper->sigmak0LineEdit,9);
    addMapping(getlayoutMapper->k1LineEdit,10);
    addMapping(getlayoutMapper->sigmak1LineEdit,11);
    addMapping(getlayoutMapper->k2LineEdit,12);
    addMapping(getlayoutMapper->sigmak2LineEdit,13);
    addMapping(getlayoutMapper->k3LineEdit,14);
    addMapping(getlayoutMapper->sigmak3LineEdit,15);
    addMapping(getlayoutMapper->p1LineEdit,16);
    addMapping(getlayoutMapper->sigmap1LineEdit,17);
    addMapping(getlayoutMapper->p2LineEdit,18);
    addMapping(getlayoutMapper->sigmap2LineEdit,19);
    addMapping(getlayoutMapper->coord01principalpointLineEdit,20);
    addMapping(getlayoutMapper->coord02principalpointLineEdit,21);
    addMapping(getlayoutMapper->sigmacoord01principalpointLineEdit,22);
    addMapping(getlayoutMapper->sigmacoord02principalpointLineEdit,23);
    addMapping(getlayoutMapper->fiductial01csiLineEdit,24);
    addMapping(getlayoutMapper->fiductial01etaLineEdit,25);
    addMapping(getlayoutMapper->fiductial01sigmaLineEdit,26);
    addMapping(getlayoutMapper->fiductial02csiLineEdit,27);
    addMapping(getlayoutMapper->fiductial02etaLineEdit,28);
    addMapping(getlayoutMapper->fiductial02sigmaLineEdit,29); 
    addMapping(getlayoutMapper->fiductial03csiLineEdit,30);
    addMapping(getlayoutMapper->fiductial03etaLineEdit,31);
    addMapping(getlayoutMapper->fiductial03sigmaLineEdit,32);
    addMapping(getlayoutMapper->fiductial04csiLineEdit,33);
    addMapping(getlayoutMapper->fiductial04etaLineEdit,34);
    addMapping(getlayoutMapper->fiductial04sigmaLineEdit,35);
    addMapping(getlayoutMapper->fiductial05csiLineEdit,36);
    addMapping(getlayoutMapper->fiductial05etaLineEdit,37);
    addMapping(getlayoutMapper->fiductial05sigmaLineEdit,38);
    addMapping(getlayoutMapper->fiductial06csiLineEdit,39);
    addMapping(getlayoutMapper->fiductial06etaLineEdit,40);
    addMapping(getlayoutMapper->fiductial06sigmaLineEdit,41);
    addMapping(getlayoutMapper->fiductial07csiLineEdit,42);
    addMapping(getlayoutMapper->fiductial07etaLineEdit,43);
    addMapping(getlayoutMapper->fiductial07sigmaLineEdit,44);
    addMapping(getlayoutMapper->fiductial08csiLineEdit,45);
    addMapping(getlayoutMapper->fiductial08etaLineEdit,46);
    addMapping(getlayoutMapper->fiductial08sigmaLineEdit,47);
    addMapping(getlayoutMapper->a0LineEdit,48);
    addMapping(getlayoutMapper->a1LineEdit,49);
    addMapping(getlayoutMapper->a2LineEdit,50);
    addMapping(getlayoutMapper->b0LineEdit,51);
    addMapping(getlayoutMapper->b1LineEdit,52);
    addMapping(getlayoutMapper->b2LineEdit,53);
    addMapping(getlayoutMapper->descriptionTextEdit,54);
    //addMapping(                                       
    addMapping(getlayoutMapper->nrfiductialComboBox,55,"currentIndex");
                
    toFirst();
    if((QString::compare(getlayoutMapper->fiductial01sigmaLineEdit->text(),"Not Available",Qt::CaseInsensitive)) != 0)
     {
     	getlayoutMapper->sigmafiductialCheckBox->setChecked(1);
    	getlayoutMapper->showsigma(1);
     }
     else
     {
     	getlayoutMapper->sigmafiductialCheckBox->setChecked(0);
    	getlayoutMapper->showsigma(0);     	
     }
     	
    getlayoutMapper->showfiductialXa();
	connect(getlayoutMapper->sigmafiductialCheckBox, SIGNAL(stateChanged ( int )),getMapperModel,SLOT(resetFDMarkSigma(int)));
};

SensorView::SensorView(QWidget *parent, SensorModel* sensormodel) : QWidget(parent)//QScrollArea(parent)//QWidget(parent)//
{
       sensorlayout = new SensorLayout(this/*MainWidget*/);

	   this->setLayout(sensorlayout);

	   if (!sensormodel)
		   sensormodel = new SensorModel();
		sensorsmodel = sensormodel;
	   sensormapper.makeMapper(sensorlayout,sensormodel);
	   sensormodel->mountstring(/*sensorlayout*/);
};

SensorView::~SensorView()
{
       changeText(); 
       delete sensorlayout;
};

void SensorView::setReadOnly(bool enable)
{
    sensorlayout->numberCalibrationLineEdit->setReadOnly(enable);
    sensorlayout->dispatchCalibrationDateTimeEdit->setReadOnly(enable);
    sensorlayout->expirationCalibrationDateTimeEdit->setReadOnly(enable);

    sensorlayout-> valuefocalLineEdit->setReadOnly(enable);
    sensorlayout-> sigmafocalLineEdit->setReadOnly(enable);
    sensorlayout-> k0LineEdit->setReadOnly(enable);
    sensorlayout-> sigmak0LineEdit->setReadOnly(enable);
    sensorlayout-> k1LineEdit->setReadOnly(enable);
    sensorlayout-> sigmak1LineEdit->setReadOnly(enable);
    sensorlayout-> k2LineEdit->setReadOnly(enable);
    sensorlayout-> sigmak2LineEdit->setReadOnly(enable);
    sensorlayout-> k3LineEdit->setReadOnly(enable);
    sensorlayout-> sigmak3LineEdit->setReadOnly(enable);
    sensorlayout-> p1LineEdit->setReadOnly(enable);
    sensorlayout-> sigmap1LineEdit->setReadOnly(enable);
    sensorlayout-> p2LineEdit->setReadOnly(enable);
    sensorlayout-> sigmap2LineEdit->setReadOnly(enable);
    sensorlayout-> coord01principalpointLineEdit->setReadOnly(enable);
    sensorlayout-> coord02principalpointLineEdit->setReadOnly(enable);
    sensorlayout-> sigmacoord01principalpointLineEdit->setReadOnly(enable);
    sensorlayout-> sigmacoord02principalpointLineEdit->setReadOnly(enable);
    sensorlayout-> fiductial01csiLineEdit->setReadOnly(enable);
    sensorlayout-> fiductial01etaLineEdit->setReadOnly(enable);
    sensorlayout-> fiductial02csiLineEdit->setReadOnly(enable);
    sensorlayout-> fiductial02etaLineEdit->setReadOnly(enable);
    sensorlayout-> fiductial03csiLineEdit->setReadOnly(enable);
    sensorlayout-> fiductial03etaLineEdit->setReadOnly(enable);
    sensorlayout-> fiductial04csiLineEdit->setReadOnly(enable);
    sensorlayout-> fiductial04etaLineEdit->setReadOnly(enable);
    sensorlayout-> fiductial05csiLineEdit->setReadOnly(enable);
    sensorlayout-> fiductial05etaLineEdit->setReadOnly(enable);
    sensorlayout-> fiductial06csiLineEdit->setReadOnly(enable);
    sensorlayout-> fiductial06etaLineEdit->setReadOnly(enable);
    sensorlayout-> fiductial07csiLineEdit->setReadOnly(enable);
    sensorlayout-> fiductial07etaLineEdit->setReadOnly(enable);
    sensorlayout-> fiductial08csiLineEdit->setReadOnly(enable);
    sensorlayout-> fiductial08etaLineEdit->setReadOnly(enable);
    sensorlayout-> fiductial01sigmaLineEdit->setReadOnly(enable);
    sensorlayout-> fiductial02sigmaLineEdit->setReadOnly(enable);
    sensorlayout-> fiductial03sigmaLineEdit->setReadOnly(enable);
    sensorlayout-> fiductial04sigmaLineEdit->setReadOnly(enable);
    sensorlayout ->fiductial05sigmaLineEdit->setReadOnly(enable);
    sensorlayout ->fiductial06sigmaLineEdit->setReadOnly(enable);
    sensorlayout->fiductial07sigmaLineEdit->setReadOnly(enable);
    sensorlayout->fiductial08sigmaLineEdit->setReadOnly(enable);
    sensorlayout->a0LineEdit->setReadOnly(enable);
    sensorlayout->a1LineEdit->setReadOnly(enable);
    sensorlayout->a2LineEdit->setReadOnly(enable);
    sensorlayout->b0LineEdit->setReadOnly(enable);
    sensorlayout->b1LineEdit->setReadOnly(enable);
    sensorlayout->b2LineEdit->setReadOnly(enable);
    sensorlayout->descriptionTextEdit->setReadOnly(enable);
	sensorlayout ->geometryComboBox->setEnabled(!enable);
	sensorlayout-> platformComboBox->setEnabled(!enable);
	sensorlayout ->detectorComboBox->setEnabled(!enable);
    sensorlayout-> nrfiductialComboBox->setDisabled(enable);
    sensorlayout-> sigmafiductialCheckBox->setDisabled(enable);
    }

void SensorView::fillvalues(string values)
{
	sensorsmodel->fillvalues(values);
};

string SensorView::getvalues()
{
	return sensorsmodel->getvalues();
};

bool SensorView::isForm(string formName)
{
	return !formName.compare("SensorView");
}
