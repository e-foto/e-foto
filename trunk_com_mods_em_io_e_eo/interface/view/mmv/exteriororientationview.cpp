#include "exteriororientationview.h"
#include "EDom.h"

ExteriorOrientationModel::ExteriorOrientationModel(string numberkey, string nrgcp_id, string unit)
{
	cout <<"Chamada de construtor de ExteriorOrientationModel" << endl;
	QString countvalues = (QString::fromUtf8(nrgcp_id.c_str()));
	key = (QString::fromUtf8(numberkey.c_str()));
	QString unitCPS = (QString::fromUtf8(unit.c_str()));
	numberpoints = countvalues.count("</point>",Qt::CaseInsensitive);
	for(int i=0; i<numberpoints ; i++)
	{
     for(int j=0; j<=8 ; j++)
       {     
        QStandardItem *item = new QStandardItem();   
        setItem(i, j, item);
       }
      
    }
     setHeaderData(0, Qt::Horizontal, QObject::tr("gcp_id"));
     setHeaderData(1, Qt::Horizontal, QObject::tr("description"));
     if (unitCPS.contains("UTM", Qt::CaseInsensitive))
     { 
      setHeaderData(2, Qt::Horizontal, QObject::tr("E"));
      setHeaderData(3, Qt::Horizontal, QObject::tr("N"));
      setHeaderData(4, Qt::Horizontal, QObject::tr("H"));
     } 
      else
       {
       	setHeaderData(2, Qt::Horizontal, QObject::tr("X"));
      	setHeaderData(3, Qt::Horizontal, QObject::tr("Y"));
      	setHeaderData(4, Qt::Horizontal, QObject::tr("Z"));
       }
     setHeaderData(5, Qt::Horizontal, QObject::tr("row"));
     setHeaderData(6, Qt::Horizontal, QObject::tr("col"));
   	cout <<"saida de construtor de ExteriorOrientationModel" << endl;
}

void ExteriorOrientationModel::fillvalues(string numberkey, string strimageEO)
{
	cout <<"Chamada de fillvalues" << endl;
	EDomElement ede(strimageEO);
    string keystring, strpoint, strnumberkey;
    QString strcoord, strImgMeasurement, strImgCoord;
    int row, strpointbegin, strpointend;
    size_t found = 0;
    row =0;
    strnumberkey = "image_key=\"" + numberkey;
    strpointbegin = 0;
    strpointend = 0;
    
	for(int i=1; i<=numberpoints ; i++)
	{
     strpointbegin = strimageEO.find("<point",strpointbegin + 1);
     strpointend = strimageEO.find("</point>",strpointend + 1) + 7;
     cout << "strpointbegin: " << strpointbegin << endl;
     cout << "strpointend: " << strpointend << endl;
     strpoint = strimageEO.substr(strpointbegin, (strpointend -strpointbegin) +1);
     EDomElement edepoint(strpoint);
     found = strpoint.find(strnumberkey);
     strcoord = QString::fromUtf8(edepoint.elementByTagName("gml:pos").toString().c_str());
     EDomElement edeCoord(edepoint.elementByTagAtt("imageCoordinates","image_key",numberkey));
     strImgMeasurement = QString::fromUtf8(edeCoord.elementByTagName("gml:pos").toString().c_str());
     item(row,0)->setText(QString::fromUtf8(edepoint.elementByTagName("gcp_id").toString().c_str()));
     item(row,0)->setEditable(0);
     item(row,1)->setText(QString::fromUtf8(edepoint.elementByTagName("description").toString().c_str()));
     item(row,1)->setEditable(0);     
     item(row,2)->setText(strcoord.section(' ',0,0));
     item(row,2)->setEditable(0);
     item(row,3)->setText(strcoord.section(' ',1,1));
     item(row,3)->setEditable(0);     
     item(row,4)->setText(strcoord.section(' ',2,2));
     item(row,4)->setEditable(0);
     if (found!=string::npos)
        {          
          strImgMeasurement = QString::fromUtf8(edeCoord.elementByTagName("gml:pos").toString().c_str());
          item(row,5)->setText(strImgMeasurement.section(' ',0,0));
          item(row,5)->setEditable(1);     
          item(row,6)->setText(strImgMeasurement.section(' ',1,1));
          item(row,6)->setEditable(1);
        }
     else
        {
          item(row,5)->setText(" ");
          item(row,5)->setEditable(1);     
          item(row,6)->setText(" ");
          item(row,6)->setEditable(1);
        }               
     item(row,7)->setText(QString::fromUtf8(edepoint.elementByTagName("point").attribute("key").c_str()));
     item(row,8)->setText(QString::fromUtf8(edepoint.elementByTagName("point").attribute("type").c_str()));
     row = row +1;
     strpointbegin = strpointend;   
    }
    startDataOE = new QStandardItemModel;
    
    for(int i=0; i<numberpoints ; i++)
	{
     for(int j=0; j<=8 ; j++)
       { 
       	QStandardItem *item = new QStandardItem();   
        startDataOE->setItem(i, j, item);    
        startDataOE->item(i,j)->setText(this->item(i,j)->text());
       } 
    }	

    cout <<"saida de fillvalues" << endl;	
};

void ExteriorOrientationModel::measure(string strID, string strcolumn, string strrow)
{
	 cout << "Chamada de measure" << endl;
	 for(int i=0; i<numberpoints ; i++)
	   {
	   	QString auxgcp_id = item(i,0)->text();
	   	QString auxstrID = QString::fromUtf8(strID.c_str());
	   	 if( auxgcp_id == auxstrID)
	   	   {
	   	   	item(i,5)->setText(QString::fromUtf8(strcolumn.c_str()));
	   	   	item(i,6)->setText(QString::fromUtf8(strrow.c_str()));
		   }                                                                                                   
	   }
	 cout << "fim de measure" << endl;
};	   	   

string ExteriorOrientationModel::getvalues()
{
	cout << "Chamada de getvalues" << endl;
	string xmlEOString;
    stringstream auxStream;
    QString strmodelexit, strmodelentry;
    bool boolmodify = 0;
    auxStream << "<points>";
    for(int i=0; i<numberpoints ; i++)
    {
		for(int j=0; j<=8 ; j++)
		 {
			  strmodelexit = item(i,j)->text();
			  strmodelentry = startDataOE->item(i,j)->text();
			  if( strmodelexit != strmodelentry )
			   {
			   	boolmodify = 1;
			   }
		 }
		 if	(boolmodify)
		 {
		 	  auxStream << "<point key=\"" << item(i,7)->text().toUtf8().data();
		 	  auxStream << "\" type=\"" << item(i,8)->text().toUtf8().data() << "\">";
		 	  auxStream << "<imagesMeasurements>";
		 	  auxStream << "<imageCoordinates uom=\"#px\" image_key=\"" << key.toUtf8().data() << "\"><gml:pos>";
		 	  auxStream << item(i,5)->text().toUtf8().data() << " " << item(i,6)->text().toUtf8().data();
		 	  auxStream << "</gml:pos></imageCoordinates></imagesMeasurements></point>";
		 }
		boolmodify = 0;
	 }	 
	 auxStream << "</points>";
	 
	 xmlEOString=auxStream.str();
     return xmlEOString;
};

void ExteriorOrientationModel::showModel()
{
	cout << "Chamada de showmodel" << endl;
	for(int i=0; i<numberpoints ; i++)
	{
     for(int j=0; j<=6 ; j++)
       {     
        cout << item(i,j)->text().toUtf8().data() << endl;
       } 
    }	
	cout << "Fim de showmodel" << endl;
};

ExteriorOrientationLayout::ExteriorOrientationLayout(QWidget* Owner):QVBoxLayout(Owner)
{
    cout << "Chamada de Construtor de ExteriorOrientationLayout" << endl;
    ExteriorOrientationTable = new QTableView;
//	ExteriorOrientationTable->verticalHeader()->setResizeMode(QHeaderView::Stretch);
	ExteriorOrientationTable->verticalHeader()->hide();
    ExteriorOrientationTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ExteriorOrientationTable->setMinimumSize(650,200);
    ExteriorOrientationTable->setMaximumHeight(600);

    
         	
	addWidget(ExteriorOrientationTable);
	addStretch();     
	
	cout << "Fim de Construtor de ExteriorOrientationLayout" << endl;

};

ExteriorOrientationLayout::~ExteriorOrientationLayout()
{
    cout << "Chamada de destrutor de ExteriorOrientationLayout" << endl;
    
    delete ExteriorOrientationTable;
	
	cout << "Fim de destrutor de ExteriorOrientationLayout" << endl;

}

ExteriorOrientationView::ExteriorOrientationView( QWidget *parent, ExteriorOrientationModel* exteriororientationmodel) : QWidget(parent)
{
       exteriororientationlayout = new ExteriorOrientationLayout(this/*MainWidget*/);
 	   this->setLayout(exteriororientationlayout);
       exteriororientationlayout->ExteriorOrientationTable->setModel(exteriororientationmodel);
       exteriororientationlayout->ExteriorOrientationTable->setColumnHidden(7,1);
       exteriororientationlayout->ExteriorOrientationTable->setColumnHidden(8,1);
};

ExteriorOrientationView::~ExteriorOrientationView()
{
       cout << "Chamada de destrutor de ExteriorOrientationView" << endl; 
       delete exteriororientationlayout;
       cout << "Fim de destrutor de ExteriorOrientationView" << endl;        		
};
