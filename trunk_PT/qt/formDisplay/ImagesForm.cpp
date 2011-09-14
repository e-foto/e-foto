#include "ImagesForm.h"
ImagesForm :: ImagesForm(QWidget *parent) : AbstractForm(parent)
{
	setupUi(this);
	connect(imagesTable,SIGNAL(cellClicked(int,int)),this,SLOT(emitSignal(int)));
	imagesTable->setColumnHidden(0,true);
	imagesTable->setColumnWidth(1,80);
	imagesTable->setColumnWidth(2,30);
	imagesTable->setColumnWidth(3,30);
}

void ImagesForm::fillvalues(string values)
{
	EDomElement ede(values);
	int rows=ede.children().size();
	imagesTable->setRowCount(rows);

	deque<EDomElement> imaChildren = ede.children();
	for (int i=0;i<rows;i++){
		string tableParameter;
		stringstream auxStream;

		auxStream << imaChildren.at(i).elementByTagName("filePath").toString().c_str();
		auxStream << "/";
		auxStream << imaChildren.at(i).elementByTagName("fileName").toString().c_str();

		tableParameter = auxStream.str();

		QTableWidgetItem *keyItem = new QTableWidgetItem( QString::fromUtf8 (imaChildren.at(i).attribute("key").c_str()) );
		QTableWidgetItem *idItem = new QTableWidgetItem(imaChildren.at(i).elementByTagName("imageId").toString().c_str()) ;
		QTableWidgetItem *fileItem = new QTableWidgetItem(tableParameter.c_str());
		keyItem->setTextAlignment(Qt::AlignCenter);
		idItem->setTextAlignment(Qt::AlignCenter);

		imagesTable->setItem(i,0,keyItem);
		imagesTable->setItem(i,1,idItem);
		imagesTable->setItem(i,4,fileItem);
	}
	imaChildren.clear();
}
string ImagesForm::getvalues()
{
    string xmlString;
    stringstream auxStream;

    auxStream << "\nNot Available\n";

    xmlString = auxStream.str();
    return xmlString;
}

void ImagesForm ::setReadOnly(bool state)
{
}

//emite o sinal da linha(row) correspondente a image key
void ImagesForm :: emitSignal(int i)
{
    //int key=stringToInt(imagesTable->item(i,0)->text().toStdString().c_str());
    //emit clicked(key);
    // Provisóriamente nós enviamos a posição da linha clicada e não a key efetivamente.
    emit clicked(i);
}

bool ImagesForm::isForm(string formName)
{
	return !formName.compare("HeaderForm");
}

void ImagesForm::setIOsAvailable(string xmlIOs)
{
	EDomElement ede(xmlIOs);

	for (int i=0;i<imagesTable->rowCount();i++){
		int key = imagesTable->item(i,0)->text().toInt();
		EDomElement IOXml = ede.elementByTagAtt("imageIO","image_key",intToString(key));
		QTableWidgetItem *IOItem = new QTableWidgetItem();
		IOItem->setTextAlignment(Qt::AlignHCenter);
		if(IOXml.getContent() != "")
		{
			IOItem->setTextColor(QColor("green"));
			IOItem->setText(QString::fromUtf8("✓"));
			IOItem->setFont(QFont("Sans",20,QFont::Bold));
		}
		else
		{
			IOItem->setTextColor(QColor("red"));
			IOItem->setText("x");
			IOItem->setFont(QFont("Sans",20));
		}
		imagesTable->setItem(i,2,IOItem);
	}
}

void ImagesForm::setEOsAvailable(string xmlEOs)
{
	EDomElement ede(xmlEOs);

	for (int i=0;i<imagesTable->rowCount();i++){
		int key = imagesTable->item(i,0)->text().toInt();
		EDomElement EOXml = ede.elementByTagAtt("imageEO","image_key",intToString(key));
		QTableWidgetItem *EOItem = new QTableWidgetItem();
		EOItem->setTextAlignment(Qt::AlignHCenter);
		if(EOXml.getContent() != "")
		{
			EOItem->setTextColor(QColor("green"));
			EOItem->setText(QString::fromUtf8("✓"));
			EOItem->setFont(QFont("Sans",20,QFont::Bold));
		}
		else
		{
			EOItem->setTextColor(QColor("red"));
			EOItem->setText("x");
			EOItem->setFont(QFont("Sans",20));
		}
		imagesTable->setItem(i,3,EOItem);
	}
}
