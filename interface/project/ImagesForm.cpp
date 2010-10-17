#include "ImagesForm.h"
ImagesForm :: ImagesForm(QWidget *parent) : AbstractForm(parent)
{
    setupUi(this);
    connect( imagesTable, SIGNAL( cellClicked(int,int)),
                 this   ,   SLOT(  emitSignal(int)    )
            );
	imagesTable->setColumnHidden(0,true);
	imagesTable->setHorizontalHeaderLabels(QString("key;Image ID;Arquivo").split(";"));
}

void ImagesForm::fillvalues(string values)
{
    EDomElement ede(values);
    int rows=ede.children().size();
    imagesTable->setRowCount(rows);
    imagesTable->setColumnCount(3);

    for (int i=0;i<rows;i++){
        EDomElement ima=ede.children().at(i);
        string tableParameter;
        stringstream auxStream;

        auxStream << ima.elementByTagName("filePath").toString().c_str();
        auxStream << "/";
        auxStream << ima.elementByTagName("fileName").toString().c_str();

        tableParameter = auxStream.str();

        QTableWidgetItem *keyItem = new QTableWidgetItem( QString::fromUtf8 (ima.attribute("key").c_str()) );
		QTableWidgetItem *idItem = new QTableWidgetItem(ima.elementByTagName("imageId").toString().c_str()) ;
        QTableWidgetItem *camItem = new QTableWidgetItem(tableParameter.c_str());

        imagesTable->setItem(i,0,keyItem);
        imagesTable->setItem(i,1,idItem);
        imagesTable->setItem(i,2,camItem);
    }

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
    // Provisóriamente nós enviamos a posição da linha clicada e não a key efetivamente.
    emit clicked(i);
}

bool ImagesForm::isForm(string formName)
{
	return !formName.compare("HeaderForm");
}

