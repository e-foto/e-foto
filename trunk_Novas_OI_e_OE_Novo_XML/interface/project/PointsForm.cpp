#include "PointsForm.h"
#include <QFileDialog>
#include <QFile>
#include <QString>

PointsForm :: PointsForm(QWidget *parent) : AbstractForm(parent)
{
    setupUi(this);
	connect( pointsTable, SIGNAL( cellClicked(int,int)), this , SLOT( emitSignal(int) ) );
	pointsTable->setColumnHidden(0,true);
	pointsTable->setHorizontalHeaderLabels(QString("key;Point Id;Type;E;N;H").split(";"));

	//connect(importButton, SIGNAL(clicked()), this ,SLOT(importFromTxt()));
}

void PointsForm::fillvalues(string values)
{
   EDomElement ede(values);
    int rows=ede.children().size();
    pointsTable->setRowCount(rows);
    pointsTable->setColumnCount(6);

    for (int i=0;i<rows;i++){

        EDomElement pnt=ede.children().at(i);

		deque<double> values;

        QTableWidgetItem *keyItem = new QTableWidgetItem( QString::fromUtf8 (pnt.attribute("key").c_str()) );
		QTableWidgetItem *gcpItem = new QTableWidgetItem(pnt.elementByTagName("pointId").toString().c_str()) ;
        QTableWidgetItem *typeItem = new QTableWidgetItem(pnt.attribute("type").c_str()) ;
		values = pnt.elementByTagName("gml:pos").toGmlPos();
		if(values.size() == 3)
        {
			QTableWidgetItem *EItem = new QTableWidgetItem(QString::number(values.at(0),'f',3));
			QTableWidgetItem *NItem = new QTableWidgetItem(QString::number(values.at(1),'f',3));
			QTableWidgetItem *HItem = new QTableWidgetItem(QString::number(values.at(2),'f',3));
            pointsTable->setItem(i,0,keyItem);
            pointsTable->setItem(i,1,gcpItem);
            pointsTable->setItem(i,2,typeItem);
            pointsTable->setItem(i,3,EItem);
            pointsTable->setItem(i,4,NItem);
            pointsTable->setItem(i,5,HItem);
        };



    }

}
string PointsForm::getvalues()
{
    string xmlString;
    stringstream auxStream;

    auxStream << "\nNot Available\n";

    xmlString = auxStream.str();
    return xmlString;
}

void PointsForm ::setReadOnly(bool state)
{
}

void PointsForm :: emitSignal(int i)
{
    emit clicked(i);
}

bool PointsForm::isForm(string formName)
{
        return !formName.compare("PointsForm");
}

