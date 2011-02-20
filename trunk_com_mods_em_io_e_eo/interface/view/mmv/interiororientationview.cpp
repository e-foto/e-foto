#include "interiororientationview.h"
#include "EDom.h"

InteriorOrientationModel::InteriorOrientationModel(string StrSensor)
{
	cout <<"Chamada de construtor de InteriorOrientationModel" << endl;
	QString StrNumberRows = QString::fromUtf8(StrSensor.c_str());
	NumberRows = StrNumberRows.count("</fiductialMark>");
	for(int i=0; i<NumberRows ; i++)
	{
     for(int j=0; j<=4 ; j++)
       {     
        QStandardItem *item = new QStandardItem();   
        setItem(i, j, item);
       }
      
    }
     QVariant var1 = QChar(0x03BE);
     QVariant var2 = QChar(0x03B7);    
     setHeaderData(0, Qt::Horizontal, QObject::tr("nr"));
     setHeaderData(1, Qt::Horizontal, var1);
     setHeaderData(2, Qt::Horizontal, var2);
     setHeaderData(3, Qt::Horizontal, QObject::tr("col"));
     setHeaderData(4, Qt::Horizontal, QObject::tr("row"));
   	cout <<"saida de construtor de InteriorOrientationModel" << endl;	
}

void InteriorOrientationModel::fillvalues(string StrFromSensor, string StrFromIO)
{
	cout <<"Chamada de fillvalues" << endl;
//	EDomElement ede(xmlefoto);
    EDomElement edesensor(StrFromSensor); 
    EDomElement edeio(StrFromIO);
    cout <<"ponto 01" << endl;
    QString sensorstring,iostring;
    string idstring;
	for(int i=0; i<NumberRows ; i++)
	{
     stringstream idstringstream;
     idstringstream << (i+1);
     idstring = idstringstream.str();
     cout <<"ponto 02" << endl;
     EDomElement edesensorid(edesensor.elementByTagAtt("fiductialMark","key",idstring));
     cout <<"ponto 03" << endl;
     EDomElement edeioid(edeio.elementByTagAtt("fiductialMark","key",idstring));
     cout <<"ponto 04" << endl;
     sensorstring = QString::fromUtf8(edesensorid.elementByTagName("gml:pos").toString().c_str());
     cout <<"ponto 05" << endl;
     cout << sensorstring.toUtf8().data() << endl;
     iostring = QString::fromUtf8(edeioid.elementByTagName("gml:pos").toString().c_str());
     cout <<"ponto 06" << endl;
     item(i,0)->setText(QString::fromUtf8(idstring.c_str()));
     item(i,0)->setEditable(0);
     cout <<"ponto 07" << endl;
     item(i,1)->setText(sensorstring.section(' ',0,0));
     item(i,1)->setEditable(0);     
     cout <<"ponto 08" << endl;
     item(i,2)->setText(sensorstring.section(' ',1,1));
     item(i,2)->setEditable(0);
     cout <<"ponto 09" << endl;
     item(i,3)->setText(iostring.section(' ',0,0));
     cout <<"ponto 10" << endl;
     item(i,4)->setText(iostring.section(' ',1,1));
     cout <<"ponto 11" << endl;
    }
    cout <<"saida de fillvalues" << endl;	
};

string InteriorOrientationModel::getvalues()
{
    string xmlInteriorOrientationString;
    stringstream auxStream;
    auxStream << "<fiductialMarks uom=\"#px\">";
    for(int i=0; i<NumberRows ; i++)
    {
    	auxStream << "<fiductialMark key=\"" << (i+1) << "\">";
    	auxStream << "<gml:pos>" << item(i,3)->text().toUtf8().data() << " " << item(i,4)->text().toUtf8().data() << "</gml:pos>";
    	auxStream << "</fiductialMark>";
    } 
    auxStream << "</fiductialMarks>";   	
    xmlInteriorOrientationString=auxStream.str();
    return xmlInteriorOrientationString;
};

void InteriorOrientationModel::measure(string strkey, string strcolumn, string strrow)
{
	 cout << "Chamada de measure" << endl;
	 for(int i=0; i<NumberRows ; i++)
	   {
	   	cout << "passo: " << i << endl;
	   	QString auxgcp_id = item(i,0)->text();
	   	QString auxstrID = QString::fromUtf8(strkey.c_str());
	   	 if( auxgcp_id == auxstrID)
	   	   {
	   	   	item(i,3)->setText(QString::fromUtf8(strcolumn.c_str()));
	   	   	item(i,4)->setText(QString::fromUtf8(strrow.c_str()));
		   }                                                                                                   
	   }
	 cout << "fim de measure" << endl;
};	   	   


void InteriorOrientationModel::showModel()
{
	cout << "Chamada de showmodel" << endl;
	for(int i=0; i<NumberRows ; i++)
	{
     for(int j=0; j<=4 ; j++)
       {     
        cout << item(i,j)->text().toUtf8().data() << endl;
       } 
    }	
	cout << "Fim de showmodel" << endl;
};

InteriorOrientationLayout::InteriorOrientationLayout(QWidget* Owner):QVBoxLayout(Owner)
{
    cout << "Chamada de Construtor de InteriorOrientationLayout" << endl;
    InteriorOrientationTable = new QTableView;
	InteriorOrientationTable->verticalHeader()->setResizeMode(QHeaderView::Stretch);
	InteriorOrientationTable->verticalHeader()->hide();
    InteriorOrientationTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    InteriorOrientationTable->setFixedWidth(500);
//    InteriorOrientationTable->setFixedHeight(400,250);
         	
	addWidget(InteriorOrientationTable);
	addStretch();     
	
	cout << "Fim de Construtor de InteriorOrientationLayout" << endl;

};

InteriorOrientationLayout::~InteriorOrientationLayout()
{
    cout << "Chamada de destrutor de TerrainLayout" << endl;
    
    delete InteriorOrientationTable;
	
	cout << "Fim de destrutor de TerrainLayout" << endl;

}

InteriorOrientationView::InteriorOrientationView( QWidget *parent, InteriorOrientationModel* interiororientationmodel) : QWidget(parent)
{
       interiororientationlayout = new InteriorOrientationLayout(this/*MainWidget*/);
 	   this->setLayout(interiororientationlayout);
       interiororientationlayout->InteriorOrientationTable->setModel(interiororientationmodel);
       interiororientationlayout->InteriorOrientationTable->setFixedHeight(40*interiororientationmodel->NumberRows);
       
};

InteriorOrientationView::~InteriorOrientationView()
{
       cout << "Chamada de destrutor de InteriorOrientationView" << endl; 
       delete interiororientationlayout;
       cout << "Fim de destrutor de InteriorOrientationView" << endl;        		
};
