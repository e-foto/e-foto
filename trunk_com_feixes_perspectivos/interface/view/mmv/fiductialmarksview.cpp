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

#include "fiductialmarksview.h"

FiductialMarksModel::FiductialMarksModel(vector <string> StrVector)
{
    string StrSensor = StrVector.at(0);
    NewCol="-1";
    NewRow="-1";
    selectedRow=-1;
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
}

void FiductialMarksModel::fillvalues(vector<string> StrVector)
{
    string StrFromSensor, StrFromIO;
    StrFromSensor=StrVector.at(0);
    StrFromIO=StrVector.at(1);
    //cout<<"Sensor:"<<endl<<StrFromSensor<<endl<<"IO:"<<StrFromIO<<endl;
    EDomElement edesensor(StrFromSensor); 
    EDomElement edeio(StrFromIO);
    QString sensorstring,iostring;
    string idstring;
    for(int i=0; i<NumberRows ; i++)
    {
        stringstream idstringstream;
        idstringstream << (i+1);
        idstring = idstringstream.str();
        EDomElement edesensorid(edesensor.elementByTagAtt("fiductialMark","key",idstring));
        EDomElement edeioid(edeio.elementByTagAtt("fiductialMark","key",idstring));
        sensorstring = QString::fromUtf8(edesensorid.elementByTagName("gml:pos").toString().c_str());
        iostring = QString::fromUtf8(edeioid.elementByTagName("gml:pos").toString().c_str());
        //cout <<"@@@@@@@@@@@@@@@@@@@@@@"<< sensorstring.toAscii().data() << endl;
        item(i,0)->setText(QString::fromUtf8(idstring.c_str()));
        item(i,0)->setEditable(0);
        item(i,1)->setText(sensorstring.section(' ',0,0));
        item(i,1)->setEditable(0);
        item(i,2)->setText(sensorstring.section(' ',1,1));
        item(i,2)->setEditable(0);
        item(i,3)->setText(iostring.section(' ',0,0));
        item(i,4)->setText(iostring.section(' ',1,1));
    }
};

string FiductialMarksModel::getvalues()
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

//void FiductialMarksModel::measure(string strkey, string strcolumn, string strrow)
//{
//	 cout << "Chamada de measure" << endl;
//	 for(int i=0; i<NumberRows ; i++)
//	   {
//	   	cout << "passo: " << i << endl;
//	   	QString auxgcp_id = item(i,0)->text();
//	   	QString auxstrID = QString::fromUtf8(strkey.c_str());
//	   	 if( auxgcp_id == auxstrID)
//	   	   {
//	   	   	item(i,3)->setText(QString::fromUtf8(strcolumn.c_str()));
//	   	   	item(i,4)->setText(QString::fromUtf8(strrow.c_str()));
//		   }
//	   }
//	 cout << "fim de measure" << endl;
//};
//

void FiductialMarksModel::showModel()
{
    for(int i=0; i<NumberRows ; i++)
    {
        for(int j=0; j<=4 ; j++)
        {
            cout << item(i,j)->text().toUtf8().data() << " ";
        }
        cout << endl;
    }	
};

void FiductialMarksModel::selectRow(const QModelIndex &index)
{
   selectedRow=index.row();
}

void FiductialMarksModel::measure(QString Col, QString Row)
{
    if (selectedRow >-1)
    {
        item(selectedRow,3)->setText(Col);
        item(selectedRow,4)->setText(Row);
    }
}

void FiductialMarksModel::measure(void)
{
    measure(NewCol, NewRow);
}

void FiductialMarksModel::setNewCol(QString Col)
{
        NewCol=Col;
}

void FiductialMarksModel::setNewRow(QString Row)
{
        NewRow=Row;
}

FiductialMarksView::FiductialMarksView(QWidget* parent=0, FiductialMarksModel* pFiductialMarksModel=0):QTableView(parent)
{
    verticalHeader()->setResizeMode(QHeaderView::Stretch);
    verticalHeader()->hide();
    horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    setModel(pFiductialMarksModel);
    setSelectionBehavior (QAbstractItemView::SelectRows);
}
