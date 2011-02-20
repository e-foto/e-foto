#include "pointview.h"
#include "EDom.h"

PointModel::PointModel(void)
{
	cout <<"Chamada de construtor de PointModel" << endl;
	for(int i=0; i<=6 ; i++)
	{
     QStandardItem *item = new QStandardItem();   
     setItem(0, i, item);
    }	
}

void PointModel::fillvalues(string strkeypoint, string xmlpointValues, string strunit)
{
	QString strcoord;
	EDomElement ede(xmlpointValues);
	EDomElement edepoint(ede.elementByTagAtt("point","key",strkeypoint).getContent());
		
	strcoord = QString::fromUtf8(edepoint.elementByTagName("gml:pos").toString().c_str());
	item(0,0)->setText(QString::fromUtf8(edepoint.elementByTagName("gcp_id").toString().c_str()));
    item(0,1)->setText(QString::fromUtf8(edepoint.elementByTagName("point").attribute("type").c_str()));
    item(0,2)->setText(QString::fromUtf8(edepoint.elementByTagName("description").toString().c_str()));
    item(0,3)->setText(strcoord.section(' ',0,0));
    item(0,4)->setText(strcoord.section(' ',1,1));
    item(0,5)->setText(strcoord.section(' ',2,2));
    item(0,6)->setText(QString::fromUtf8(edepoint.elementByTagName("sigma").toString().c_str()));
    
    strimgmeasurements = edepoint.elementByTagName("imagesMeasurements").getContent();
    
    strunitvalue = strunit;
    strkeyvalue = strkeypoint;
};

string PointModel::getvalues()
{
    string xmlPointString;
    stringstream auxStream;
    
    auxStream << "<point key=\"" << strkeyvalue;
	auxStream << "\" type=\"" << item(0,1)->text().toUtf8().data() << "\">";
	auxStream << "<gcp_id>" << item(0,0)->text().toUtf8().data() << "</gcp_id>";
	auxStream << "<description>" << item(0,2)->text().toUtf8().data() << "</description>";
	auxStream << "<spatialCoordinates uom=\"#m\">";
	auxStream << "<gml:pos>" << item(0,3)->text().toUtf8().data() << " ";
	auxStream << item(0,4)->text().toUtf8().data() << " ";
	auxStream << item(0,5)->text().toUtf8().data() << "</gml:pos>";
	auxStream << "<sigma>" << item(0,5)->text().toUtf8().data() << "</sigma></spatialCoordinates>";
	auxStream << strimgmeasurements;

    xmlPointString=auxStream.str();
    return xmlPointString;
};

void PointModel::showModel()
{
	cout << "Chamada de showmodel" << endl;
	for(int i=0; i<=6 ; i++)
	{
       cout << item(0,i)->text().toUtf8().data() << endl; 
    }
	cout << "Fim de showmodel" << endl;
};

PointLayout::PointLayout(QWidget* Owner)
{
    cout << "Chamada de Construtor de PointLayout" << endl;
    
    gcpIDLabel = new QLabel(tr("gcpID:"),Owner);
    gcpIDLineEdit = new QLineEdit(Owner);
    
    typeLabel = new QLabel(tr("Type:"),Owner);
    typeComboBox = new QComboBox(Owner);
    typeComboBox->addItem(tr("control"));
    typeComboBox->addItem(tr("verification"));
    typeComboBox->addItem(tr("photogrametric"));
    
    gcpIDLayout = new QHBoxLayout;
    gcpIDLayout->addWidget(gcpIDLabel);
    gcpIDLayout->addWidget(gcpIDLineEdit);
    gcpIDLayout->addSpacing(20);
    gcpIDLayout->addWidget(typeLabel);
    gcpIDLayout->addWidget(typeComboBox);
    gcpIDLayout->addStretch();
    
    descriptionLabel = new QLabel(tr("Description"),Owner);
    descriptionLineEdit = new QLineEdit(Owner);
    
    descriptionLayout = new QHBoxLayout;
    descriptionLayout->addWidget(descriptionLabel);
    descriptionLayout->addWidget(descriptionLineEdit);
    descriptionLayout->addStretch();
    
    spatialCoordGroupBox = new QGroupBox(tr("Spatial Coordinates"),Owner);
    
    spatialCoord01Label = new QLabel(tr("X"),Owner);
    spatialCoord01LineEdit = new QLineEdit(Owner);
    spatialCoord01LineEdit->setValidator(new QDoubleValidator(spatialCoord01LineEdit));
    spatialCoord02Label = new QLabel(tr("Y"),Owner);
    spatialCoord02LineEdit = new QLineEdit(Owner);
    spatialCoord02LineEdit->setValidator(new QDoubleValidator(spatialCoord02LineEdit));
    spatialCoord03Label = new QLabel(tr("Z"),Owner);
    spatialCoord03LineEdit = new QLineEdit(Owner);
    spatialCoord03LineEdit->setValidator(new QDoubleValidator(spatialCoord03LineEdit));
    
    spatialCoordHLayout = new QHBoxLayout;
    spatialCoordHLayout->addWidget(spatialCoord01Label);
    spatialCoordHLayout->addWidget(spatialCoord01LineEdit);
    spatialCoordHLayout->addSpacing(10);
    spatialCoordHLayout->addWidget(spatialCoord02Label);
    spatialCoordHLayout->addWidget(spatialCoord02LineEdit);
    spatialCoordHLayout->addSpacing(10);
    spatialCoordHLayout->addWidget(spatialCoord03Label);
    spatialCoordHLayout->addWidget(spatialCoord03LineEdit);
    spatialCoordHLayout->addStretch();
    
    sigmaLabel = new QLabel(tr("Sigma"),Owner);
    sigmaLineEdit = new QLineEdit(Owner);
    
    sigmaLayout = new QHBoxLayout;
    sigmaLayout->addWidget(sigmaLabel);
    sigmaLayout->addWidget(sigmaLineEdit);
    sigmaLayout->addStretch();
    
    spatialCoordVLayout = new QVBoxLayout;
    spatialCoordVLayout->addLayout(spatialCoordHLayout);
    spatialCoordVLayout->addLayout(sigmaLayout);
    
    spatialCoordGroupBox->setLayout(spatialCoordVLayout);
    
    addLayout(gcpIDLayout);
    addLayout(descriptionLayout);
    addWidget(spatialCoordGroupBox);
    addStretch();
    

};

PointLayout::~PointLayout()
{
    cout << "Chamada de destrutor de PointLayout" << endl;
    delete gcpIDLabel;
    delete gcpIDLineEdit;
    delete typeLabel;
    delete typeComboBox;
    delete descriptionLabel;
    delete descriptionLineEdit;
    delete spatialCoord01Label;
    delete spatialCoord01LineEdit;
    delete spatialCoord02Label;
    delete spatialCoord02LineEdit;
    delete spatialCoord03Label;
    delete spatialCoord03LineEdit;
    delete sigmaLabel;
    delete sigmaLineEdit;
    delete spatialCoordGroupBox;    

	cout << "Fim de destrutor de PointLayout" << endl;

}

void PointView::changeText()
{
		cout << "Chamada de changetext" << endl;
		PointModel *MyPointModel;
 		MyPointModel = (PointModel*) pointmapper.model();
 		MyPointModel->item(0,1)->setText(pointlayout->typeComboBox->currentText());
		MyPointModel->getvalues();
		cout << "fim de changetext" << endl;
};

void PointMapper::makeMapper(PointLayout* getlayoutMapper, PointModel *getMapperModel)
{
	cout << "Chamada de makemapper" << endl;
	setModel(getMapperModel);
    addMapping(getlayoutMapper->gcpIDLineEdit,0);
    addMapping(getlayoutMapper->typeComboBox,1,"currentindex");
    addMapping(getlayoutMapper->descriptionLineEdit,2); 
    addMapping(getlayoutMapper->spatialCoord01LineEdit,3);
    addMapping(getlayoutMapper->spatialCoord02LineEdit,4);   
    addMapping(getlayoutMapper->spatialCoord03LineEdit,5);  
    addMapping(getlayoutMapper->sigmaLineEdit,6);
    toFirst();
    if ((getMapperModel->strunitvalue) == "UTM")
       {
        getlayoutMapper->spatialCoord01Label->setText(" E:");
        getlayoutMapper->spatialCoord01Label->setText(" N:");
        getlayoutMapper->spatialCoord01Label->setText(" H:");
       }

    getMapperModel->showModel();
};



PointView::PointView( QWidget *parent, PointModel* pointmodel) : QWidget(parent)
{
       pointlayout = new PointLayout(this/*MainWidget*/);
	   this->setLayout(pointlayout);
	   indexGridsType = pointlayout->typeComboBox->findText(pointmodel->item(0,1)->text());
	   pointlayout->typeComboBox->setCurrentIndex(indexGridsType.toInt());
	   pointmapper.makeMapper(pointlayout,pointmodel);
       connect(pointmodel,SIGNAL(itemChanged(QStandardItem *)),this,SLOT(changeText()));
};

PointView::~PointView()
{
       cout << "Chamada de destrutor de PointView" << endl; 
       changeText();
       delete pointlayout;
       cout << "Fim de destrutor de PointView" << endl;        		
};
