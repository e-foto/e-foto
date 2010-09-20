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

#include "demview.h"


DemModel::DemModel(void)
{
	for(int i=0; i<=11 ; i++)
	{
     QStandardItem *item = new QStandardItem();   
     setItem(0, i, item);
    }	
}

void DemModel::fillvalues(vector <string> StrVector)
{
	string xmlDemValues;
	string strunit;
    
    xmlDemValues=StrVector[0];
    strunit =StrVector[1];
	EDomElement ede(xmlDemValues); 
	EDomElement edeUpperLeft(ede.elementByTagName("upperLeft"));
	EDomElement edeBottomRight(ede.elementByTagName("bottomRight"));
	QString StrspatialCoord, StrleftImageCoord, StrrightImageCoord;
         
	item(0, 0)->setText(QString::fromUtf8(ede.elementByTagName("filePath").toString().c_str()));
	item(0,0)->setEditable(0);
	item(0,1)->setText(QString::fromUtf8(ede.elementByTagName("fileName").toString().c_str()));
	item(0,1)->setEditable(0);
	item(0,2)->setText(QString::fromUtf8(ede.elementByTagName("grid").attribute("type").c_str()));	
	
	EDomElement spatialCoordValues(edeUpperLeft.elementByTagName("spatialCoordinates"));
	StrspatialCoord = QString::fromUtf8(spatialCoordValues.elementByTagName("gml:pos").toString().c_str());
	item(0,3)->setText(StrspatialCoord.section(' ',0,0));
	item(0,4)->setText(StrspatialCoord.section(' ',1,1));
	item(0,5)->setText(StrspatialCoord.section(' ',2,2));

    EDomElement botoomSpatialCoord(edeBottomRight.elementByTagName("spatialCoordinates"));	
	StrspatialCoord = QString::fromUtf8(botoomSpatialCoord.elementByTagName("gml:pos").toString().c_str());
	item(0,6)->setText(StrspatialCoord.section(' ',0,0));
	item(0,7)->setText(StrspatialCoord.section(' ',1,1));
	item(0,8)->setText(StrspatialCoord.section(' ',2,2));

	item(0,9)->setText(QString::fromUtf8(ede.elementByTagName("horizontal").toString().c_str()));
	item(0,10)->setText(QString::fromUtf8(ede.elementByTagName("vertical").toString().c_str()));
	item(0,11)->setText(QString::fromUtf8(strunit.c_str()));
	strkey = ede.elementByTagName("grid").attribute("key").c_str();
	strstereopairkey = ede.elementByTagName("grid").attribute("stereo_pair_key").c_str();
};

string DemModel::getvalues()
{
    string xmlDemString;
    stringstream auxStream;
    auxStream << "<dem>";
	auxStream << "<filePath>" << item(0,0)->text().toUtf8().data() << "</filePath>";
	auxStream << "<fileName>" << item(0,1)->text().toUtf8().data() << "</fileName>";
	auxStream << "<grid key=\"" << strkey << "\" ";
	auxStream << "type=\""<< item(0,2)->text().toUtf8().data();
	auxStream << "\" stereo_pair_key=\"" << strstereopairkey << "\">";
	auxStream << "<upperLeft><spatialCoordinates uom=\"#m\"><gml:pos>";
	auxStream << item(0,3)->text().toUtf8().data() << " " << item(0,4)->text().toUtf8().data() << " ";
    auxStream << item(0,5)->text().toUtf8().data() << "</gml:pos></spatialCoordinates></upperLeft>";
	auxStream << "<bottomRight><spatialCoordinates uom=\"#m\"><gml:pos>";
	auxStream << item(0,6)->text().toUtf8().data() << " " << item(0,7)->text().toUtf8().data() << " ";
    auxStream << item(0,8)->text().toUtf8().data() << "</gml:pos></spatialCoordinates></bottomRight>";
    string strtype = item(0,2)->text().toUtf8().data();
   	auxStream << "<divisions>";
    if (strtype == "regular")
    {
       auxStream << "<horizontal>" << item(0,9)->text().toUtf8().data() << "</horizontal>";
	   auxStream << "<vertical>" << item(0,10)->text().toUtf8().data() << "</vertical>";
	}
	auxStream << "</divisions></grid></dem>";
    xmlDemString=auxStream.str();
    return xmlDemString;
};

void DemModel::showModel()
{
	for(int i=0; i<=11 ; i++)
	{
       cout << item(0,i)->text().toUtf8().data() << endl; 
    }
};

DemLayout::DemLayout(QWidget* Owner)
{
    
    filePathLabel = new QLabel(tr("FilePath"),Owner);
    filePathLineEdit = new QLineEdit(Owner);
    filePathLineEdit->setReadOnly(1);
    
    fileNameLabel = new QLabel(tr("FileName"),Owner);
    fileNameLineEdit = new QLineEdit;
    fileNameLineEdit->setReadOnly(1);
    
    importPushButton = new QPushButton(tr("import..."),Owner);
    createPushButton = new QPushButton(tr("create..."),Owner);    
    
    gridsGroupBox = new QGroupBox(tr("Grids"),Owner);
    
    gridsComboBox = new QComboBox;
    gridsComboBox->addItem(tr("regular"));
    gridsComboBox->addItem(tr("irregular"));
    gridsComboBox->setFixedWidth(400);
    
    upperLeftGroupBox = new QGroupBox(tr("Upper Left"),Owner);
    upperspatialCoordLabel = new QLabel(tr("SpatialCoordinates(m)"),Owner);
    upperspatialCoordLineEditX = new QLineEdit(Owner);
    upperspatialCoordLineEditX ->setValidator(new QDoubleValidator(upperspatialCoordLineEditX));
    upperspatialCoordLineEditY = new QLineEdit(Owner);
    upperspatialCoordLineEditY ->setValidator(new QDoubleValidator(upperspatialCoordLineEditY));    
    upperspatialCoordLineEditZ = new QLineEdit;

    bottomRightGroupBox = new QGroupBox(tr("Bottom Right"),Owner);
    bottomspatialCoordLabel = new QLabel(tr("SpatialCoordinates(m)"),Owner);
    bottomspatialCoordLineEditX = new QLineEdit(Owner);
    bottomspatialCoordLineEditX ->setValidator(new QDoubleValidator(bottomspatialCoordLineEditX));    
    bottomspatialCoordLineEditY = new QLineEdit(Owner);
    bottomspatialCoordLineEditY ->setValidator(new QDoubleValidator(bottomspatialCoordLineEditY));    
    bottomspatialCoordLineEditZ = new QLineEdit;

    divisionsGroupBox = new QGroupBox(tr("Divisions"),Owner);
    horizontalLabel = new QLabel(tr("horizontal"),Owner);
    horizontalLineEdit = new QLineEdit(Owner);
    horizontalLineEdit ->setValidator(new QIntValidator(horizontalLineEdit));    
    verticalLabel = new QLabel(tr("vertical"),Owner);
    verticalLineEdit = new QLineEdit(Owner);
    verticalLineEdit ->setValidator(new QIntValidator(verticalLineEdit));
            
    
    filePathLayout = new QHBoxLayout;
    filePathLayout->addWidget(filePathLabel);
    filePathLayout->addWidget(filePathLineEdit);
    filePathLayout->addWidget(importPushButton);
    filePathLayout->addWidget(createPushButton);    
    fileNameLayout = new QHBoxLayout;
    fileNameLayout->addWidget(fileNameLabel);
    fileNameLayout->addWidget(fileNameLineEdit);
    
    viewunit =  new QLabel(" ");
    firstunitupper = new QLabel(" X:",Owner);
    firstunitbottom = new QLabel(" X:",Owner);
    secondunitupper = new QLabel(" Y:",Owner);
    secondunitbottom = new QLabel(" Y:",Owner);
    
    upperSpatialCoordLayout = new QHBoxLayout;
    upperSpatialCoordLayout->addWidget(upperspatialCoordLabel);
    upperSpatialCoordLayout->addWidget(firstunitupper);
    upperSpatialCoordLayout->addWidget(upperspatialCoordLineEditX);
    upperSpatialCoordLayout->addWidget(secondunitupper);    
    upperSpatialCoordLayout->addWidget(upperspatialCoordLineEditY);
    upperLeftGroupBox->setLayout(upperSpatialCoordLayout);
    upperLeftGroupBox->setMinimumHeight(65);
    
    bottomSpatialCoordLayout = new QHBoxLayout;
    bottomSpatialCoordLayout->addWidget(bottomspatialCoordLabel);
    bottomSpatialCoordLayout->addWidget(firstunitbottom);
    bottomSpatialCoordLayout->addWidget(bottomspatialCoordLineEditX);
    bottomSpatialCoordLayout->addWidget(secondunitbottom);    
    bottomSpatialCoordLayout->addWidget(bottomspatialCoordLineEditY);
    bottomRightGroupBox->setLayout(bottomSpatialCoordLayout);
    bottomRightGroupBox->setMinimumHeight(65);   
    
    divisionsLayout = new QHBoxLayout;
    divisionsLayout->addWidget(horizontalLabel);
	divisionsLayout->addWidget(horizontalLineEdit);
	divisionsLayout->addSpacing(30);
	divisionsLayout->addWidget(verticalLabel);
	divisionsLayout->addWidget(verticalLineEdit);
	divisionsGroupBox->setLayout(divisionsLayout);
	divisionsGroupBox->setMinimumHeight(65);
	
	gridsLayout = new QVBoxLayout;
	gridsLayout->addWidget(gridsComboBox);
	gridsLayout->addWidget(upperLeftGroupBox);
	gridsLayout->addWidget(bottomRightGroupBox);
	gridsLayout->addWidget(divisionsGroupBox);
	gridsGroupBox->setLayout(gridsLayout);
	gridsGroupBox->setMaximumSize(500,400);
	
	mainLayout = new QVBoxLayout;
	mainLayout->addLayout(filePathLayout);
	mainLayout->addLayout(fileNameLayout);
	mainLayout->addWidget(gridsGroupBox);
	
	addLayout(mainLayout);
	addStretch();
	
	connect(gridsComboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(showDivision(int)));
};

void DemView::import()
{	
	 QDir *strRelativePath = new QDir(QDir::current());
     QFileDialog importDialog(this,tr("import file"),strRelativePath->path(),tr("All Files(*.*)"));
	 importDialog.setFileMode(QFileDialog::ExistingFile);
     importDialog.setLabelText(QFileDialog::Accept,tr("import..."));
     importDialog.exec();
     QStringList strListPath = importDialog.selectedFiles();
     
	 QString strPath = strListPath.at(0);
     demlayout->fileNameLineEdit->setText(strPath.section('/',-1));
     demlayout->filePathLineEdit->setText("./" + strRelativePath->relativeFilePath(strPath).section('/',0,-2));  
     demmapper.submit();
};

 void DemView::create()
{	
	 QDir *strRelativePath = new QDir(QDir::current());
     QFileDialog createDialog(this,tr("Create file"),strRelativePath->path(),tr("All Files"));
	 createDialog.setFileMode(QFileDialog::AnyFile);
     createDialog.setLabelText(QFileDialog::Accept,tr("create..."));
     createDialog.exec();
     QStringList strListPath = createDialog.selectedFiles();
     
	 QString strPath = strListPath.at(0);
     demlayout->fileNameLineEdit->setText(strPath.section('/',-1));
     demlayout->filePathLineEdit->setText("./" + strRelativePath->relativeFilePath(strPath).section('/',0,-2));  
     demmapper.submit();
};

void DemLayout::showDivision(int index)
{
	if (index == 0)
	 {
        divisionsGroupBox->show();
     } 
     else
     { 
        divisionsGroupBox->hide();
     }
     return;
};                   

DemLayout::~DemLayout()
{
    delete filePathLabel;
    delete filePathLineEdit;
    delete fileNameLabel;
    delete fileNameLineEdit;
    delete upperspatialCoordLabel;
    delete upperspatialCoordLineEditX;
    delete upperspatialCoordLineEditY;
    delete viewunit;
    delete firstunitupper;
    delete secondunitupper;
    delete firstunitbottom;
    delete secondunitbottom;    
    delete upperLeftGroupBox;
    delete bottomspatialCoordLabel;
    delete bottomspatialCoordLineEditX;
    delete bottomspatialCoordLineEditY;
    delete gridsComboBox;
    delete bottomRightGroupBox;
    delete horizontalLabel;
    delete horizontalLineEdit;
    delete verticalLabel;
    delete verticalLineEdit;    
    delete divisionsGroupBox;    
    delete gridsGroupBox;    
};

void DemView::changeText()
{
		DemModel *MyDemModel;
 		MyDemModel = (DemModel*) demmapper.model();
 		MyDemModel->item(0,2)->setText(demlayout->gridsComboBox->currentText());
		MyDemModel->getvalues();
};

void DemMapper::makeMapper(DemLayout* getlayoutMapper, DemModel *getMapperModel)
{
	setModel(getMapperModel);
    addMapping(getlayoutMapper->filePathLineEdit,0);
    addMapping(getlayoutMapper->fileNameLineEdit,1);
    addMapping(getlayoutMapper->gridsComboBox,2,"currentindex"); 
    addMapping(getlayoutMapper->upperspatialCoordLineEditX,3);
    addMapping(getlayoutMapper->upperspatialCoordLineEditY,4);   
    addMapping(getlayoutMapper->upperspatialCoordLineEditZ,5);  
    addMapping(getlayoutMapper->bottomspatialCoordLineEditX,6);
    addMapping(getlayoutMapper->bottomspatialCoordLineEditY,7);   
    addMapping(getlayoutMapper->bottomspatialCoordLineEditZ,8);  
    addMapping(getlayoutMapper->horizontalLineEdit,9);
    addMapping(getlayoutMapper->verticalLineEdit,10);
    addMapping(getlayoutMapper->viewunit,11);
    toFirst();
    if ((getlayoutMapper->viewunit->text()).contains("UTM", Qt::CaseInsensitive))
       {
        getlayoutMapper->firstunitupper->setText(" E:");
        getlayoutMapper->firstunitbottom->setText(" E:");
        getlayoutMapper->secondunitupper->setText(" N:");
        getlayoutMapper->secondunitbottom->setText(" N:");
       }

    getMapperModel->showModel();
};



DemView::DemView( QWidget *parent, DemModel* demmodel) : QWidget(parent)
{
       demlayout = new DemLayout(this/*MainWidget*/);
	   this->setLayout(demlayout);
	   indexGridsType = demlayout->gridsComboBox->findText(demmodel->item(0,2)->text());
	   demlayout->gridsComboBox->setCurrentIndex(indexGridsType.toInt());
//       demmodel->item(0,2)->setText(indexGridsType.toString());	   
	   demmapper.makeMapper(demlayout,demmodel);
       connect(demmodel,SIGNAL(itemChanged(QStandardItem *)),this,SLOT(changeText()));
	   connect(demlayout->importPushButton,SIGNAL(clicked()),this,SLOT(import())); 
	   connect(demlayout->createPushButton,SIGNAL(clicked()),this,SLOT(create()));   	         
 
};

DemView::~DemView()
{
       changeText();
       delete demlayout;
};
