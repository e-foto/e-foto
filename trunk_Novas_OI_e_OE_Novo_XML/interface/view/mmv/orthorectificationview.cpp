#include "orthorectificationview.h"
#include "EDom.h"
#include <QFileDialog>
#include <QDir>

OrthorectificationModel::OrthorectificationModel()
{
	cout <<"Chamada de construtor de OrthorectificationModel" << endl;
	for(int i=0; i<=9 ; i++)
	{
     QStandardItem *item = new QStandardItem();   
     setItem(0, i, item);
    }
};

void OrthorectificationModel::fillvalues(string xmlOrthorectificationValues)
{
	EDomElement ede(xmlOrthorectificationValues); 
	EDomElement edeUpperLeft(ede.elementByTagName("upperLeft"));
	EDomElement edeBottomRight(ede.elementByTagName("bottomRight"));
	QString StrspatialCoord, StrleftImageCoord, StrrightImageCoord;
         
	item(0,0)->setText(QString::fromUtf8(ede.elementByTagName("filePath").toString().c_str()));
	item(0,1)->setText(QString::fromUtf8(ede.elementByTagName("fileName").toString().c_str()));
	
	EDomElement spatialCoordValues(edeUpperLeft.elementByTagName("spatialCoordinates"));
	StrspatialCoord = QString::fromUtf8(spatialCoordValues.elementByTagName("gml:pos").toString().c_str());
	item(0,2)->setText(StrspatialCoord.section(' ',0,0));
	item(0,3)->setText(StrspatialCoord.section(' ',1,1));
	item(0,4)->setText(StrspatialCoord.section(' ',2,2));

    EDomElement botoomSpatialCoord(edeBottomRight.elementByTagName("spatialCoordinates"));	
	StrspatialCoord = QString::fromUtf8(botoomSpatialCoord.elementByTagName("gml:pos").toString().c_str());
	item(0,5)->setText(StrspatialCoord.section(' ',0,0));
	item(0,6)->setText(StrspatialCoord.section(' ',1,1));
	item(0,7)->setText(StrspatialCoord.section(' ',2,2));

    item(0,8)->setText(QString::fromUtf8(ede.elementByTagName("GSD").toString().c_str()));
	
	item(0,9)->setText(QString::fromUtf8(ede.elementByTagName("greyLevelInterpolation").toString().c_str()));
	
	strorthoimagekey = ede.elementByTagName("orthoimage").attribute("orthoimage_key");
};

string OrthorectificationModel::getvalues()
{
    string xmlOrthorectificationString;
    stringstream auxStream;
    auxStream << "<orthoimage orthoimage_key=\"" << strorthoimagekey << "\">\"";
	auxStream << "<filePath>" << item(0,0)->text().toUtf8().data() << "</filePath>";
	auxStream << "<fileName>" << item(0,1)->text().toUtf8().data() << "</fileName>";
	auxStream << "<upperLeft><spatialCoordinates uom=\"#m\">";
	auxStream << "<gml:pos>" << item(0,2)->text().toUtf8().data() << " ";
	auxStream <<  item(0,3)->text().toUtf8().data() << " ";
	auxStream << item(0,4)->text().toUtf8().data() << "</gml:pos></spatialCoordinates></upperLeft>";
	auxStream << "<bottomRight><spatialCoordinates uom=\"#m\"><gml:pos>";
	auxStream <<  item(0,5)->text().toUtf8().data() << " ";
	auxStream <<  item(0,6)->text().toUtf8().data() << " ";
	auxStream <<  item(0,7)->text().toUtf8().data() << "</gml:pos></spatialCoordinates></bottomRight>";
	auxStream <<  "<GSD><spatialCoordinates uom=\"#m\"><gml:pos>";
	auxStream <<  item(0,8)->text().toUtf8().data() << "</gml:pos></spatialCoordinates></GSD>";
	auxStream <<  "<greyLevelInterpolation>" << item(0,9)->text().toUtf8().data();
	auxStream <<  "</greyLevelInterpolation></orthoimage>";
	
    xmlOrthorectificationString=auxStream.str();
    return xmlOrthorectificationString;
};

void OrthorectificationModel::showModel()
{
	cout << "Chamada de showmodel" << endl;
	for(int i=0; i<=9 ; i++)
	{
       cout << item(0,i)->text().toUtf8().data() << endl; 
    }
	cout << "Fim de showmodel" << endl;
};

OrthorectificationLayout::OrthorectificationLayout(QWidget* Owner, string strunitlabel)
{
    cout << "Chamada de Construtor de OrthorectificationLayout" << endl;
    
    if (strunitlabel == "UTM")
     {
     	strcoord01 = "E:";
     	strcoord02 = "N:";
     	strcoord03 = "H:";
     }
     else
     	{
     		strcoord01 = "X:";
     		strcoord02 = "Y:";
     		strcoord03 = "Z:";
     	}
     	
    filePathLabel = new QLabel(tr("FilePath"),Owner);
    filePathLineEdit = new QLineEdit(Owner);
    filePathLineEdit->setReadOnly(1);

    
    fileNameLabel = new QLabel(tr("FileName"),Owner);
    fileNameLineEdit = new QLineEdit(Owner);
    fileNameLineEdit->setReadOnly(1);
    
    importPushButton = new QPushButton(tr("import..."),Owner);
    createPushButton = new QPushButton(tr("create..."),Owner); 
    
    upperLeftGroupBox = new QGroupBox(tr("Upper Left"),Owner);
    QLabel *spatialCoordinatesupperLabel = new QLabel(tr("Spatial Coordinates(m)"),Owner);
    upperleftCoordXLabel = new QLabel(strcoord01,Owner);
    upperleftCoordYLabel = new QLabel(strcoord02,Owner);
    upperleftCoordZLabel = new QLabel(strcoord03,Owner);
    upperleftCoordXLineEdit = new QLineEdit(Owner);
    upperleftCoordXLineEdit->setValidator(new QDoubleValidator(upperleftCoordXLineEdit));
    upperleftCoordYLineEdit = new QLineEdit(Owner);
    upperleftCoordYLineEdit->setValidator(new QDoubleValidator(upperleftCoordYLineEdit));    
    upperleftCoordZLineEdit = new QLineEdit(Owner);
    upperleftCoordZLineEdit->setValidator(new QDoubleValidator(upperleftCoordZLineEdit));    

    bottomRightGroupBox = new QGroupBox(tr("Bottom Right"),Owner);
    QLabel *spatialCoordinatesbottomLabel = new QLabel(tr("Spatial Coordinates(m)"),Owner);    
    bottomrightCoordXLabel = new QLabel(strcoord01,Owner);
    bottomrightCoordYLabel = new QLabel(strcoord02,Owner);
    bottomrightCoordZLabel = new QLabel(strcoord03,Owner);
    bottomrightCoordXLineEdit = new QLineEdit(Owner);
    bottomrightCoordXLineEdit->setValidator(new QDoubleValidator(bottomrightCoordXLineEdit));    
    bottomrightCoordYLineEdit = new QLineEdit(Owner);
    bottomrightCoordYLineEdit->setValidator(new QDoubleValidator(bottomrightCoordYLineEdit));    
    bottomrightCoordZLineEdit = new QLineEdit(Owner);
    bottomrightCoordZLineEdit->setValidator(new QDoubleValidator(bottomrightCoordZLineEdit));    
    
    GSDLabel = new QLabel(tr("GSD(m):"), Owner);
    GSDLineEdit = new QLineEdit(Owner);
    GSDLineEdit->setValidator(new QDoubleValidator(GSDLineEdit));
    GSDLineEdit->setFixedWidth(150);    
    
    greylevelLabel = new QLabel(tr("Grey level interpolation"),Owner);
    greylevelComboBox = new QComboBox(Owner);
    greylevelComboBox->addItem(tr("Not Available"));
    greylevelComboBox->addItem(tr("nearest neighbor"));
    greylevelComboBox->addItem(tr("bilinear"));    
    greylevelComboBox->addItem(tr("bicubic"));    

    filePathLayout = new QHBoxLayout;
    filePathLayout->addWidget(filePathLabel);
    filePathLayout->addWidget(filePathLineEdit);
    filePathLayout->addWidget(importPushButton);
    filePathLayout->addWidget(createPushButton);
    fileNameLayout = new QHBoxLayout;
    fileNameLayout->addWidget(fileNameLabel);
    fileNameLayout->addWidget(fileNameLineEdit);
    
    upperLeftCoordLayout = new QHBoxLayout;
    upperLeftCoordLayout->addWidget(spatialCoordinatesupperLabel);
    upperLeftCoordLayout->addWidget(upperleftCoordXLabel);
    upperLeftCoordLayout->addWidget(upperleftCoordXLineEdit);
    upperLeftCoordLayout->addWidget(upperleftCoordYLabel);    
    upperLeftCoordLayout->addWidget(upperleftCoordYLineEdit);
    upperLeftCoordLayout->addWidget(upperleftCoordZLabel);
    upperLeftCoordLayout->addWidget(upperleftCoordZLineEdit);    
    upperLeftGroupBox->setLayout(upperLeftCoordLayout);
    upperLeftGroupBox->setFixedWidth(600);
    
    bottomRightCoordLayout = new QHBoxLayout;
    bottomRightCoordLayout->addWidget(spatialCoordinatesbottomLabel);
    bottomRightCoordLayout->addWidget(bottomrightCoordXLabel);
    bottomRightCoordLayout->addWidget(bottomrightCoordXLineEdit);
    bottomRightCoordLayout->addWidget(bottomrightCoordYLabel);    
    bottomRightCoordLayout->addWidget(bottomrightCoordYLineEdit);
    bottomRightCoordLayout->addWidget(bottomrightCoordZLabel);
    bottomRightCoordLayout->addWidget(bottomrightCoordZLineEdit);    
    bottomRightGroupBox->setLayout(bottomRightCoordLayout);
    bottomRightGroupBox->setFixedWidth(600);   
    
    GSDLayout = new QHBoxLayout;
    GSDLayout->addWidget(GSDLabel);
    GSDLayout->addWidget(GSDLineEdit);
    GSDLayout->addStretch();    
    
	greylevelLayout = new QHBoxLayout;
	greylevelLayout->addWidget(greylevelLabel);
	greylevelLayout->addWidget(greylevelComboBox);
	greylevelLayout->addStretch();	    
    	
//	mainLayout = new QVBoxLayout;
	addLayout(filePathLayout);
	addLayout(fileNameLayout);
	addWidget(upperLeftGroupBox);
	addWidget(bottomRightGroupBox);
	addLayout(GSDLayout);
	addLayout(greylevelLayout);
	addStretch();		
 //   mainLayout->addWidget(viewunit);	
	
//	addLayout(mainLayout);
	

	cout << "Fim de Construtor de OrthorectificationLayout" << endl;

};

/*void OrthorectificationView::browse()
{	
	 cout << "Entrada Browse" << endl;
	 QDir *strRelativePath = new QDir(QDir::current());
     QWidget *OpenImage = new QWidget;
     QString strPath = QFileDialog::getSaveFileName(OpenImage,
                                      tr("Open File"),strRelativePath->path(),
                                      tr("All Files(*.*)"));
     orthorectificationlayout->fileNameLineEdit->setText(strPath.section('/',-1));
     orthorectificationlayout->filePathLineEdit->setText("./" + strRelativePath->relativeFilePath(strPath).section('/',0,-2));  
     orthorectificationmapper.submit();
     cout << "Fim de Browse" << endl;     
};*/

 void OrthorectificationView::import()
{	
	 cout << "Entrada import" << endl;
	 QDir *strRelativePath = new QDir(QDir::current());
     QFileDialog importDialog(this,tr("import file"),strRelativePath->path(),tr("All Files(*.*)"));
	 importDialog.setFileMode(QFileDialog::ExistingFile);
     importDialog.setLabelText(QFileDialog::Accept,tr("import..."));
     importDialog.exec();
     QStringList strListPath = importDialog.selectedFiles();
     
	 QString strPath = strListPath.at(0);
     orthorectificationlayout->fileNameLineEdit->setText(strPath.section('/',-1));
     orthorectificationlayout->filePathLineEdit->setText("./" + strRelativePath->relativeFilePath(strPath).section('/',0,-2));  
     orthorectificationmapper.submit();
     cout << "Fim de Import" << endl;
};

 void OrthorectificationView::create()
{	
	 cout << "Entrada Create" << endl;
	 QDir *strRelativePath = new QDir(QDir::current());
     QFileDialog createDialog(this,tr("Create file"),strRelativePath->path(),tr("All Files"));
	 createDialog.setFileMode(QFileDialog::AnyFile);
     createDialog.setLabelText(QFileDialog::Accept,tr("create..."));
     createDialog.exec();
     QStringList strListPath = createDialog.selectedFiles();
     
	 QString strPath = strListPath.at(0);
     orthorectificationlayout->fileNameLineEdit->setText(strPath.section('/',-1));
     orthorectificationlayout->filePathLineEdit->setText("./" + strRelativePath->relativeFilePath(strPath).section('/',0,-2));  
     orthorectificationmapper.submit();
     cout << "Fim de Create" << endl;
};

OrthorectificationLayout::~OrthorectificationLayout()
{
    cout << "Chamada de destrutor de OrthorectificationLayout" << endl;
    delete filePathLabel;
    delete filePathLineEdit;
    delete fileNameLabel;
    delete fileNameLineEdit;
    delete importPushButton;
    delete createPushButton;
    delete upperleftCoordXLabel;
    delete upperleftCoordYLabel;
    delete upperleftCoordZLabel;
    delete bottomrightCoordXLabel;
    delete bottomrightCoordYLabel;
    delete bottomrightCoordZLabel;
    delete GSDLabel;
    delete greylevelLabel;
    delete upperleftCoordXLineEdit;
    delete upperleftCoordYLineEdit;
    delete upperleftCoordZLineEdit;
    delete bottomrightCoordXLineEdit;
    delete bottomrightCoordYLineEdit;
    delete bottomrightCoordZLineEdit;
    delete GSDLineEdit;
    delete greylevelComboBox;
    delete upperLeftGroupBox;
    delete bottomRightGroupBox;

	cout << "Fim de destrutor de OrthorectificationLayout" << endl;

}

void OrthorectificationView::changeText()
{
	 	OrthorectificationModel *MyOrthorectificationModel;
 	 	MyOrthorectificationModel = (OrthorectificationModel*) orthorectificationmapper.model();
 	 	MyOrthorectificationModel->item(0,9)->setText(orthorectificationlayout->greylevelComboBox->currentText());
	 	MyOrthorectificationModel->getvalues();
};

void OrthorectificationMapper::makeMapper(OrthorectificationLayout* getlayoutMapper, OrthorectificationModel *getMapperModel)
{
	cout << "Chamada de makemapper" << endl;
	setModel(getMapperModel);
    addMapping(getlayoutMapper->filePathLineEdit,0);
    addMapping(getlayoutMapper->fileNameLineEdit,1);
    addMapping(getlayoutMapper->upperleftCoordXLineEdit,2); 
    addMapping(getlayoutMapper->upperleftCoordYLineEdit,3);
    addMapping(getlayoutMapper->upperleftCoordZLineEdit,4);   
    addMapping(getlayoutMapper->bottomrightCoordXLineEdit,5);  
    addMapping(getlayoutMapper->bottomrightCoordYLineEdit,6);
    addMapping(getlayoutMapper->bottomrightCoordZLineEdit,7);   
    addMapping(getlayoutMapper->GSDLineEdit,8);  
    addMapping(getlayoutMapper->greylevelComboBox,9,"currentindex");
    toFirst();
    getMapperModel->showModel();
};



OrthorectificationView::OrthorectificationView( QWidget *parent, OrthorectificationModel* orthorectificationmodel, string strunit) : QWidget(parent)
{
       orthorectificationlayout = new OrthorectificationLayout(this/*MainWidget*/, strunit);
	   this->setLayout(orthorectificationlayout);
	   indexGridsType = orthorectificationlayout->greylevelComboBox->findText(orthorectificationmodel->item(0,9)->text());
       orthorectificationmodel->item(0,9)->setText(indexGridsType.toString());	   
	   orthorectificationmapper.makeMapper(orthorectificationlayout,orthorectificationmodel);
       connect(orthorectificationmodel,SIGNAL(itemChanged(QStandardItem *)),this,SLOT(changeText()));
	   connect(orthorectificationlayout->importPushButton,SIGNAL(clicked()),this,SLOT(import()));
	   connect(orthorectificationlayout->createPushButton,SIGNAL(clicked()),this,SLOT(create()));  	           
};

OrthorectificationView::~OrthorectificationView()
{
       cout << "Chamada de destrutor de OrthorectificationView" << endl; 
       changeText();
       delete orthorectificationlayout;
       cout << "Fim de destrutor de OrthorectificationView" << endl;        		
};
