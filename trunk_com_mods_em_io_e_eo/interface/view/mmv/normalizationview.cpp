#include "normalizationview.h"
#include "EDom.h"

NormalizationModel::NormalizationModel(string xmlnormalizationmodel)
{
	cout <<"Chamada de construtor de NormalizationModel" << endl;
	QString countvalues = (QString::fromUtf8(xmlnormalizationmodel.c_str()));
	numberpoints = countvalues.count("</point>",Qt::CaseInsensitive);
	for(int i=0; i<numberpoints ; i++)
	{
     for(int j=0; j<=5 ; j++)
       {     
        QStandardItem *item = new QStandardItem();   
        setItem(i, j, item);
       }
    }
    
     setHeaderData(0, Qt::Horizontal, QObject::tr("left column"));
     setHeaderData(1, Qt::Horizontal, QObject::tr("right column"));
     setHeaderData(2, Qt::Horizontal, QObject::tr("difference column"));
     setHeaderData(3, Qt::Horizontal, QObject::tr("left row"));
	 setHeaderData(4, Qt::Horizontal, QObject::tr("right row")); 
     setHeaderData(5, Qt::Horizontal, QObject::tr("difference row"));
     
     normalizedpairmodel = new QStandardItemModel(0,4);
     
      for(int k=0; k<=3 ; k++)
       {     
        QStandardItem *itemnormalized = new QStandardItem();   
        normalizedpairmodel->setItem(0, k, itemnormalized);
       }                 

   	cout <<"saida de construtor de NormalizedModel" << endl;
}

void NormalizationModel::fillvalues(string xmlNormalization)
{
	cout <<"Chamada de fillvalues" << endl;
	EDomElement ede(xmlNormalization);
    string strpoint, strnormalizedpair;
    QString strleft, strright, strdifference;
    int intpointbegin, intpointend, intnormalizedpairbegin;
    intpointbegin = 0;
    intpointend = 0;
    
	for(int i=0; i<numberpoints ; i++)
	{
     intpointbegin = xmlNormalization.find("<point",intpointbegin + 1);
     intpointend = xmlNormalization.find("</point>",intpointend + 1) + 7;
     strpoint = xmlNormalization.substr(intpointbegin, (intpointend -intpointbegin) +1);
     EDomElement edepoint(strpoint);
     EDomElement edenormalizedpair(edepoint.elementByTagName("normalizedPair").getContent());

     EDomElement edeleft(edenormalizedpair.elementByTagName("left").getContent());
     strleft = QString::fromUtf8(edeleft.elementByTagName("gml:pos").toString().c_str());
     item(i,0)->setText(strleft.section(' ',0,0));
     item(i,0)->setEditable(0);
     item(i,3)->setText(strleft.section(' ',1,1));
     item(i,3)->setEditable(0);      
     
     EDomElement ederight(edenormalizedpair.elementByTagName("right").getContent());
     strright = QString::fromUtf8(ederight.elementByTagName("gml:pos").toString().c_str());
     item(i,1)->setText(strright.section(' ',0,0));
     item(i,1)->setEditable(0);
     item(i,4)->setText(strright.section(' ',1,1));
     item(i,4)->setEditable(0); 

     EDomElement ededifference(edenormalizedpair.elementByTagName("difference").getContent());
     strdifference = QString::fromUtf8(ededifference.elementByTagName("gml:pos").toString().c_str());
     item(i,2)->setText(strdifference.section(' ',0,0));
     item(i,2)->setEditable(0);
     item(i,5)->setText(strdifference.section(' ',1,1));
     item(i,5)->setEditable(0);
 
     intpointbegin = intpointend;   
    }

    intnormalizedpairbegin = xmlNormalization.rfind("<normalizedPair>");
    strnormalizedpair = xmlNormalization.substr(intnormalizedpairbegin,(xmlNormalization.length()-1));
    EDomElement edenormalizeddifference(strnormalizedpair);
    EDomElement edecolumndifference(edenormalizeddifference.elementByTagName("columnDifference").toString().c_str());
    EDomElement ederowdifference(edenormalizeddifference.elementByTagName("rowDifference").toString().c_str());    
    normalizedpairmodel->item(0,0)->setText(QString::fromUtf8(edecolumndifference.elementByTagName("mean").toString().c_str()));   
    normalizedpairmodel->item(0,1)->setText(QString::fromUtf8(edecolumndifference.elementByTagName("standardDeviation").toString().c_str()));     
    normalizedpairmodel->item(0,2)->setText(QString::fromUtf8(ederowdifference.elementByTagName("mean").toString().c_str()));   
    normalizedpairmodel->item(0,3)->setText(QString::fromUtf8(ederowdifference.elementByTagName("standardDeviation").toString().c_str()));     
 
    cout <<"saida de fillvalues" << endl;	
};

string NormalizationModel::getvalues()
{

     return "end";
};

void NormalizationModel::showModel()
{
	cout << "Chamada de showmodel" << endl;
	for(int i=0; i<numberpoints ; i++)
	{
     for(int j=0; j<=5 ; j++)
       {     
        cout << item(i,j)->text().toUtf8().data() << endl;
       } 
    }
    
     for(int k=0; k<=3 ; k++)
       {     
        cout << normalizedpairmodel->item(0,k)->text().toUtf8().data() << endl;
       } 
           	
	cout << "Fim de showmodel" << endl;
};

NormalizationLayout::NormalizationLayout(QWidget* Owner):QVBoxLayout(Owner)
{
    cout << "Chamada de Construtor de NormalizationLayout" << endl;
    NormalizationTable = new QTableView(Owner);
//	NormalizationTable->verticalHeader()->setResizeMode(QHeaderView::Stretch);
	NormalizationTable->verticalHeader()->hide();
    NormalizationTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    NormalizationTable->setFixedSize(650,300);
 
    
	columnGroupBox = new QGroupBox(tr("Column difference"),Owner);
	meancolumnLabel = new QLabel(tr("Mean: "),Owner);
	meancolumnLineEdit = new QLineEdit(Owner);
	meancolumnHLayout = new QHBoxLayout;
	standardcolumnLabel = new QLabel(tr("StandardDeviation: "),Owner);
	standardcolumnLineEdit = new QLineEdit(Owner);
	standardcolumnHLayout = new QHBoxLayout;	
	meancolumnHLayout->addWidget(meancolumnLabel);
	meancolumnHLayout->addWidget(meancolumnLineEdit);
	meancolumnHLayout->addStretch();
	standardcolumnHLayout->addWidget(standardcolumnLabel);
	standardcolumnHLayout->addWidget(standardcolumnLineEdit);
	standardcolumnHLayout->addStretch();	
	columnLayout = new QVBoxLayout;
	columnLayout-> addLayout(meancolumnHLayout);
	columnLayout-> addLayout(standardcolumnHLayout);
	columnGroupBox->setLayout(columnLayout);
	columnGroupBox->setFixedWidth(320);
	
	rowGroupBox = new QGroupBox(tr("Row difference"),Owner);
	meanrowLabel = new QLabel(tr("Mean: "),Owner);
	meanrowLineEdit = new QLineEdit(Owner);
	meanrowHLayout = new QHBoxLayout;
	standardrowLabel = new QLabel(tr("StandardDeviation: "),Owner);
	standardrowLineEdit = new QLineEdit(Owner);
	standardrowHLayout = new QHBoxLayout;	
	meanrowHLayout->addWidget(meanrowLabel);
	meanrowHLayout->addWidget(meanrowLineEdit);
	meanrowHLayout->addStretch();
	standardrowHLayout->addWidget(standardrowLabel);
	standardrowHLayout->addWidget(standardrowLineEdit);
	standardrowHLayout->addStretch();	
	rowLayout = new QVBoxLayout;
	rowLayout-> addLayout(meanrowHLayout);
	rowLayout-> addLayout(standardrowHLayout);
	rowGroupBox->setLayout(rowLayout);
	rowGroupBox->setFixedWidth(320);
	
	differenceHLayout = new QHBoxLayout;					
    differenceHLayout->addWidget(columnGroupBox);
    differenceHLayout->addWidget(rowGroupBox);
    differenceHLayout->addStretch();    
             	
	addWidget(NormalizationTable); 
    addLayout(differenceHLayout); 
    addStretch();   
	
	cout << "Fim de Construtor de NormalizationLayout" << endl;

};

NormalizationLayout::~NormalizationLayout()
{
    cout << "Chamada de destrutor de NormalizationLayout" << endl;
    
    delete NormalizationTable;
	
	cout << "Fim de destrutor de NormalizationLayout" << endl;

}

void NormalizationMapper::makeMapper(NormalizationLayout* LayoutMapper, NormalizationModel *getMapperModel)
{
	cout << "Chamada de makemapper" << endl;
	setModel(getMapperModel);
	addMapping(LayoutMapper->meancolumnLineEdit,0);
	addMapping(LayoutMapper->standardcolumnLineEdit,1);
	addMapping(LayoutMapper->meanrowLineEdit,2);
	addMapping(LayoutMapper->standardrowLineEdit,3);
    toFirst();
 //   connect(getMapperModel->normalizedpairmodel,SIGNAL(itemChanged(QStandardItem *)),this,SLOT(changeText()));
};

NormalizationView::NormalizationView( QWidget *parent, NormalizationModel* normalizationmodel) : QWidget(parent)
{
       normalizationlayout = new NormalizationLayout(this/*MainWidget*/);
 	   this->setLayout(normalizationlayout);
       normalizationlayout->NormalizationTable->setModel(normalizationmodel);
       normalizationmapper.makeMapper(normalizationlayout,normalizationmodel);       
};

NormalizationView::~NormalizationView()
{
       cout << "Chamada de destrutor de NormalizationView" << endl; 
       delete normalizationlayout;
       cout << "Fim de destrutor de NormalizationView" << endl;        		
};
