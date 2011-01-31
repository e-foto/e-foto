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

#include "projectheaderview.h"

ProjectHeaderModel::ProjectHeaderModel(void)
{
	for(int i=0; i<=8 ; i++)
	{
     QStandardItem *item = new QStandardItem();   
     setItem(0, i, item);
    }	
}

void ProjectHeaderModel::fillvalues(vector<string> StrVector)
{
	
	
	string xmlString;
	xmlString = StrVector[0];
	
	EDomElement ede(xmlString); 
	item(0, 0)->setText(QString::fromUtf8(ede.elementByTagName("name").toString().c_str()));
	item(0, 1)->setText(QString::fromUtf8(ede.elementByTagName("description").toString().c_str()));
	item(0, 2)->setText(QString::fromUtf8(ede.elementByTagName("filePath").toString().c_str()));
	item(0, 3)->setText(QString::fromUtf8(ede.elementByTagName("fileName").toString().c_str()));
	item(0, 4)->setText(QString::fromUtf8(ede.elementByTagName("creation").toString().c_str()));
	item(0, 5)->setText(QString::fromUtf8(ede.elementByTagName("modification").toString().c_str()));
	item(0, 6)->setText(QString::fromUtf8(ede.elementByTagName("owner").toString().c_str()));
	item(0, 7)->setText(QString::fromUtf8(ede.elementByTagName("aims").toString().c_str()));
	item(0, 8)->setText(QString::fromUtf8(ede.elementByTagName("context").toString().c_str()));
 
}

string ProjectHeaderModel::getvalues()
{
    string xmlString;
    stringstream auxStream;
    auxStream << "<projectHeader>";
    auxStream << "<name>"<<item(0,0)->text().toUtf8().data()<<"</name>";
    auxStream << "<description>"<<item(0,1)->text().toUtf8().data()<<"</description>";
    auxStream << "<filePath>"<<item(0,2)->text().toUtf8().data()<<"</filePath>";
    auxStream << "<fileName>"<<item(0,3)->text().toUtf8().data()<<"</fileName>";
    auxStream << "<creation>"<<item(0,4)->text().toUtf8().data()<<"</creation>";
    auxStream << "<modification>"<<item(0,5)->text().toUtf8().data()<<"</modification>";
    auxStream << "<owner>"<<item(0,6)->text().toUtf8().data()<<"</owner>";
    auxStream << "<aims>"<<item(0,7)->text().toUtf8().data()<<"</aims>";
    auxStream << "<context>"<<item(0,8)->text().toUtf8().data()<<"</context>";
    auxStream << "</projectHeader>";
    xmlString=auxStream.str();
    return xmlString;
}


ProjectHeaderLayout::ProjectHeaderLayout(QWidget* Owner):QGridLayout (Owner)
{
	mainGridLayout = new QGridLayout;

	NameLabel= new QLabel(tr("Name:"), Owner);
	NameLabel->setAlignment(Qt::AlignRight|Qt::AlignCenter);
	mainGridLayout->addWidget(NameLabel, 0,0,1,1);
	
	NameLineEdit =new QLineEdit(Owner);
	mainGridLayout->addWidget(NameLineEdit,0,1,1,1);
	
	DescriptionLabel= new QLabel(tr("Description:"), Owner);
	DescriptionLabel->setAlignment(Qt::AlignRight|Qt::AlignCenter);
	mainGridLayout->addWidget(DescriptionLabel, 1,0,1,1);
	DescriptionTextEdit =new QTextEdit (Owner);
	mainGridLayout->addWidget(DescriptionTextEdit,1,1,1,1);
	
	FilePathLabel= new QLabel(tr("FilePath:"),Owner);
	FilePathLabel->setAlignment(Qt::AlignRight|Qt::AlignCenter);	
	mainGridLayout->addWidget(FilePathLabel, 2,0,1,1);
	FilePathLineEdit =new QLineEdit (Owner);
	FilePathLineEdit->setReadOnly(true);
	mainGridLayout->addWidget(FilePathLineEdit,2,1,1,1);
	
	FileNameLabel= new QLabel(tr("File name:"),Owner);
	FileNameLabel->setAlignment(Qt::AlignRight|Qt::AlignCenter);	
	mainGridLayout->addWidget(FileNameLabel, 3,0,1,1);
	FileNameLineEdit =new QLineEdit (Owner);
	FileNameLineEdit->setReadOnly(true);
	mainGridLayout->addWidget(FileNameLineEdit,3,1,1,1);
	
	CreationLabel = new QLabel(tr("Creation date:"),Owner);
	CreationLabel->setAlignment(Qt::AlignRight|Qt::AlignCenter);	
	mainGridLayout->addWidget(CreationLabel, 4,0,1,1);
	
	CreationDateTimeEdit =new QDateTimeEdit (Owner); 
	CreationDateTimeEdit->setReadOnly(true);
	mainGridLayout->addWidget(CreationDateTimeEdit,4,1,1,1);
	
	ModificationLabel= new QLabel(tr("Modification date:"), Owner);
	ModificationLabel->setAlignment(Qt::AlignRight|Qt::AlignCenter);	
	mainGridLayout->addWidget(ModificationLabel, 5,0,1,1);
	
	ModificationDateTimeEdit =new QDateTimeEdit (Owner); 
	ModificationDateTimeEdit->setReadOnly(true);
	mainGridLayout->addWidget(ModificationDateTimeEdit,5,1,1,1);
	
	OwnerLabel= new QLabel(tr("Owner:"), Owner);
	OwnerLabel->setAlignment(Qt::AlignRight|Qt::AlignCenter);	
	mainGridLayout->addWidget(OwnerLabel, 6,0,1,1);
	
	OwnerLineEdit =new QLineEdit (Owner);
	mainGridLayout->addWidget(OwnerLineEdit,6,1,1,1);
	
	AimsLabel= new QLabel(tr("Aims:"), Owner);
	AimsLabel->setAlignment(Qt::AlignRight|Qt::AlignCenter);	
	mainGridLayout->addWidget(AimsLabel, 7,0,1,1);
	
	AimsLineEdit =new QLineEdit (Owner);
	mainGridLayout->addWidget(AimsLineEdit,7,1,1,1);

	ContextLabel= new QLabel(tr("Context:"), Owner);
	ContextLabel->setAlignment(Qt::AlignRight|Qt::AlignCenter);	
	mainGridLayout->addWidget(ContextLabel, 8,0,1,1);
	
	ContextLineEdit =new QLineEdit (Owner);
	mainGridLayout->addWidget(ContextLineEdit,8,1,1,1);
	
	mainVBoxLayout = new QVBoxLayout;
	mainVBoxLayout->addLayout(mainGridLayout);
	mainVBoxLayout->addStretch();
	mainHBoxLayout = new QHBoxLayout;
	mainHBoxLayout->addLayout(mainVBoxLayout);
	mainHBoxLayout->addStretch();
	
	addLayout(mainHBoxLayout,0,0,0);
	
};

ProjectHeaderLayout::~ProjectHeaderLayout()
{
	delete NameLabel;
	delete NameLineEdit;
	delete DescriptionLabel;
	delete DescriptionTextEdit;
	delete FilePathLabel;
	delete FilePathLineEdit;
	delete FileNameLabel;
	delete FileNameLineEdit;
	delete CreationLabel;
	delete CreationDateTimeEdit;
	delete ModificationLabel;
	delete ModificationDateTimeEdit;
	delete OwnerLabel;
	delete OwnerLineEdit;
	delete AimsLabel;
	delete AimsLineEdit;
	delete ContextLabel;
	delete ContextLineEdit;
}

void ProjectHeaderMapper::changeText()
{
	 	ProjectHeaderModel *MyModel;
	 	MyModel = (ProjectHeaderModel*) this->model(); 
	 	MyModel->getvalues();
};

void ProjectHeaderMapper::makeMapper(ProjectHeaderLayout* createMapper, ProjectHeaderModel *getMapperModel)
{
	setModel(getMapperModel);
	addMapping(createMapper->NameLineEdit,0);
	addMapping(createMapper->DescriptionTextEdit,1,"plainText");
	addMapping(createMapper->FilePathLineEdit,2);
	addMapping(createMapper->FileNameLineEdit,3);
	addMapping(createMapper->CreationDateTimeEdit,4);
	addMapping(createMapper->ModificationDateTimeEdit,5);
	addMapping(createMapper->OwnerLineEdit,6);
	addMapping(createMapper->AimsLineEdit,7);	
	addMapping(createMapper->ContextLineEdit,8);
    toFirst();
    connect(getMapperModel,SIGNAL(itemChanged(QStandardItem *)),this,SLOT(changeText()));
};

ProjectHeaderView::ProjectHeaderView(QWidget *parent, ProjectHeaderModel* projectheadermodel) : QWidget(parent)
{
     projectheaderlayout = new ProjectHeaderLayout(this);
     this->setLayout(projectheaderlayout);
     projectheadermapper.makeMapper(projectheaderlayout,projectheadermodel);
};

ProjectHeaderView::~ProjectHeaderView()
{
       delete projectheaderlayout;
};
