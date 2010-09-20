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

#include "matrixview.h"

MatrixModel::MatrixModel(unsigned int prows, unsigned int pcolumns, unsigned int pviewheader)
{
	if ((pviewheader!=1)&&(pviewheader!=2)&&(pviewheader!=3))
	{
		viewheader=0;
		if ((pcolumns>0)&&(prows>0))
		{
		rows=prows;
		columns=pcolumns;
		}
		else
		{
			rows=1;
			columns=1;
		}
	}
	else
	{
		rows =6;
		columns=1;
		viewheader=pviewheader;
 	}
	
	for(unsigned int i=0; i<rows ; i++)
 	{
    	for(unsigned int j=0; j<columns ; j++)
	    {
			QStandardItem *pitem = new QStandardItem("0");   
			setItem(i, j, pitem); item((int)i,(int)j)->setEditable(0);
			
		}
	}
	putVertHeader();
}

void MatrixModel::putVertHeader(void)
{
	QString Label_1,Label_2,Label_3,Label_4,Label_5,Label_6;
	
	switch (viewheader)
	{
		case (1):
    		Label_1 = "a"; Label_1.append(QChar(0x2080));
    		Label_2 = "a"; Label_2.append(QChar(0x2081));
    		Label_3 = "a"; Label_3.append(QChar(0x2082));
    		Label_4 = "b"; Label_4.append(QChar(0x2080));
    		Label_5 = "b"; Label_5.append(QChar(0x2081));
    		Label_6 = "b"; Label_6.append(QChar(0x2082));
    		break;
    	
    	case (2):
			Label_1 = QChar(0x03C9);
			Label_2 = QChar(0x03D5);
    		Label_3 = QChar(0x03F0);
    		Label_4 = "X";  Label_4.append(QChar(0x2080));
			Label_5 = "Y";  Label_5.append(QChar(0x2080));    	
			Label_6 = "Z";  Label_6.append(QChar(0x2080));
    		break;

     	case (3):
			Label_1 = QChar(0x03C9);Label_1.append(QChar(0x2080));
			Label_2 = QChar(0x03D5);Label_2.append(QChar(0x2080));
    		Label_3 = QChar(0x03F0);Label_3.append(QChar(0x2080));
    		Label_4 = "X";  Label_4.append(QChar(0x2080));Label_4.append(QChar(0x2080));
			Label_5 = "Y";  Label_5.append(QChar(0x2080));Label_5.append(QChar(0x2080));    	
			Label_6 = "Z";  Label_6.append(QChar(0x2080));Label_6.append(QChar(0x2080));
    		break;
   	
	};    	
	if (viewheader!=0)
	{
		setHeaderData(0, Qt::Vertical, Label_1);	
    	setHeaderData(1, Qt::Vertical, Label_2);
    	setHeaderData(2, Qt::Vertical, Label_3);
    	setHeaderData(3, Qt::Vertical, Label_4);
    	setHeaderData(4, Qt::Vertical, Label_5);
    	setHeaderData(5, Qt::Vertical, Label_6);
	}
}

MatrixModel::MatrixModel(vector <string> StrVector)
{
	string xmlMatrixValues = StrVector[0];
    fillvalues(StrVector); 
};

void MatrixModel::fillvalues(vector <string> StrVector)
{
    string xmlMatrixValues = StrVector[0];
 	
 	if((xmlMatrixValues.find("<mml:matrix>"))!=string::npos)
	{
		QString countvalues = (QString::fromUtf8(xmlMatrixValues.c_str()));
    	rows = countvalues.count("<mml:matrixrow>",Qt::CaseInsensitive);
    	columns = (countvalues.count("<mml:cn>",Qt::CaseInsensitive))/rows;
	}
	else
	{
		rows=6;
		columns=1;
	}
	for(unsigned int i=0; i<rows ; i++)
	{
		for(unsigned int j=0; j<columns ; j++)
		{
			QStandardItem *pitem = new QStandardItem(); 
			setItem(i, j, pitem); item((int)i,(int)j)->setEditable(0);
		}
	}
	
    strMatrix = xmlMatrixValues;
    QString stringmodel,rowstring,columnstring;
    stringmodel = (QString::fromUtf8(xmlMatrixValues.c_str()));
    if((xmlMatrixValues.find("<mml:matrix>"))!=string::npos)
    {
    	viewheader = 0; //sei não
    	int startrow,endrow,startcolumn,endcolumn;
    	startrow = 0;
    	endrow = 0; 
   		startcolumn = 0;
    	endcolumn = 0;
   		for(unsigned int i=0; i<rows ; i++)
	 	{
       		startrow =  stringmodel.indexOf("<mml:matrixrow>",startrow);
       		endrow = (stringmodel.indexOf("</mml:matrixrow>",endrow)) + 16;
       		rowstring = stringmodel.mid(startrow,endrow - startrow);
            	for(unsigned int j=0; j<columns ; j++)
	        	{
                	startcolumn =  rowstring.indexOf("<mml:cn>",startcolumn);
                	endcolumn = rowstring.indexOf("</mml:cn>",endcolumn);
                	item(i,j) -> setText((rowstring.mid(startcolumn+8,endcolumn - startcolumn - 8)));
                	startcolumn = endcolumn + 9;
                	endcolumn = startcolumn;
            	}
       		startrow =  endrow;
       		startcolumn =0;
       		endcolumn =0;     
     	}
     }
     
     if((xmlMatrixValues.find("<a0>"))!=string::npos)
     {
    	viewheader = 1;
     	EDomElement edematrix(xmlMatrixValues);
     	item(0,0)->setText(QString::fromUtf8(edematrix.elementByTagName("a0").toString().c_str()));
     	item(1,0)->setText(QString::fromUtf8(edematrix.elementByTagName("a1").toString().c_str()));
     	item(2,0)->setText(QString::fromUtf8(edematrix.elementByTagName("a2").toString().c_str()));
     	item(3,0)->setText(QString::fromUtf8(edematrix.elementByTagName("b0").toString().c_str()));
     	item(4,0)->setText(QString::fromUtf8(edematrix.elementByTagName("b1").toString().c_str()));
     	item(5,0)->setText(QString::fromUtf8(edematrix.elementByTagName("b2").toString().c_str()));
     }
     
      if (((xmlMatrixValues.find("<omega"))!=string::npos)&&((xmlMatrixValues.find("<omega0"))==string::npos))
     {
    	viewheader = 2;
     	EDomElement edematrix(xmlMatrixValues);
     	item(0,0)->setText(QString::fromUtf8(edematrix.elementByTagName("omega").toString().c_str()));
     	item(1,0)->setText(QString::fromUtf8(edematrix.elementByTagName("phi").toString().c_str()));
     	item(2,0)->setText(QString::fromUtf8(edematrix.elementByTagName("kappa").toString().c_str()));
     	item(3,0)->setText(QString::fromUtf8(edematrix.elementByTagName("X0").toString().c_str()));
     	item(4,0)->setText(QString::fromUtf8(edematrix.elementByTagName("Y0").toString().c_str()));
     	item(5,0)->setText(QString::fromUtf8(edematrix.elementByTagName("Z0").toString().c_str()));
     }    

      if((xmlMatrixValues.find("<omega0"))!=string::npos)
     {
    	viewheader = 3;
     	EDomElement edematrix(xmlMatrixValues);
     	item(0,0)->setText(QString::fromUtf8(edematrix.elementByTagName("omega0").toString().c_str()));
     	item(1,0)->setText(QString::fromUtf8(edematrix.elementByTagName("phi0").toString().c_str()));
     	item(2,0)->setText(QString::fromUtf8(edematrix.elementByTagName("kappa0").toString().c_str()));
     	item(3,0)->setText(QString::fromUtf8(edematrix.elementByTagName("X00").toString().c_str()));
     	item(4,0)->setText(QString::fromUtf8(edematrix.elementByTagName("Y00").toString().c_str()));
     	item(5,0)->setText(QString::fromUtf8(edematrix.elementByTagName("Z00").toString().c_str()));
     } 
     putVertHeader();   
};

string MatrixModel::getvalues()
{
    string xmlMatrixString;
    stringstream auxstream;
    
 
    switch (viewheader)
    {
    case 0:
    	auxstream << "<mml:matrix>";
		for(unsigned int i=0; i<rows ; i++)
	 	{
       		auxstream << "<mml:matrixrow>";
       		for(unsigned int j=0; j<columns ; j++)
	        {
              auxstream << "<mml:cn>" << item(i,j)->text().toUtf8().data() << "</mml:cn>";
            }
       		auxstream << "</mml:matrixrow>";
     	}
    	auxstream << "</mml:matrix>";
    	break;
    
    case 1:
    	auxstream <<"<Xa><a0>" << item(0,0)->text().toUtf8().data() << "</a0>";
     	auxstream << "<a1>" << item(1,0)->text().toUtf8().data() << "</a1>";
     	auxstream << "<a2>" << item(2,0)->text().toUtf8().data() << "</a2>";
     	auxstream << "<b0>" << item(3,0)->text().toUtf8().data() << "</b0>";
     	auxstream << "<b1>" << item(4,0)->text().toUtf8().data() << "</b1>";
     	auxstream << "<b2>" << item(5,0)->text().toUtf8().data() << "</b2></Xa>";
   		break;
    
    
    case 2:  
       	auxstream << "<Xa><omega uom=\"#rad\">" << item(0,0)->text().toUtf8().data() << "</omega>";
     	auxstream << "<phi uom=\"#rad\">" << item(1,0)->text().toUtf8().data() << "</phi>";
     	auxstream << "<kappa uom=\"#rad\">" << item(2,0)->text().toUtf8().data() << "</kappa>";
     	auxstream << "<X0 uom=\"#m\">" << item(3,0)->text().toUtf8().data() << "</X0>";
     	auxstream << "<Y0 uom=\"#m\">" << item(4,0)->text().toUtf8().data() << "</Y0>";
     	auxstream << "<Z0 uom=\"#m\">" << item(5,0)->text().toUtf8().data() << "</Z0></Xa>";
     	break;

     case 3:  
       	auxstream << "<X0><omega0 uom=\"#rad\">" << item(0,0)->text().toUtf8().data() << "</omega0>";
     	auxstream << "<phi0 uom=\"#rad\">" << item(1,0)->text().toUtf8().data() << "</phi0>";
     	auxstream << "<kappa0 uom=\"#rad\">" << item(2,0)->text().toUtf8().data() << "</kappa0>";
     	auxstream << "<X00 uom=\"#m\">" << item(3,0)->text().toUtf8().data() << "</X00>";
     	auxstream << "<Y00 uom=\"#m\">" << item(4,0)->text().toUtf8().data() << "</Y00>";
     	auxstream << "<Z00 uom=\"#m\">" << item(5,0)->text().toUtf8().data() << "</Z00></X0>";
     	break;
    }    

    xmlMatrixString=auxstream.str();
    return xmlMatrixString;
};

void MatrixModel::showModel()
{
	for(unsigned int i=0; i<rows ; i++)
	 {
       for(unsigned int j=0; j<columns ; j++)
	        {
              cout << item(i,j)->text().toUtf8().data() << " "; 
            }
       cout << endl;
     }
};

MatrixLayout::MatrixLayout(QWidget* Owner,int viewheaderlayout):QVBoxLayout(Owner)
{
	
	matrixTable=new QTableView;
	matrixTable->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    matrixTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    matrixTable->horizontalHeader()->hide();
    
    if(viewheaderlayout == 0)
    {
     	   matrixTable->verticalHeader()->hide();
    }
	addWidget(matrixTable);
};

MatrixLayout::~MatrixLayout()
{
    delete matrixTable;

}

MatrixView::MatrixView(QWidget *parent, MatrixModel* matrixmodel) : QWidget(parent)
{
    matrixlayout = new MatrixLayout(this,matrixmodel->viewheader);
	this->setLayout(matrixlayout);
   	matrixlayout->matrixTable->setModel(matrixmodel);
};

MatrixView::~MatrixView()
{
	delete matrixlayout;
};
