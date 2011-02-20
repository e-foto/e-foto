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

#include "pointsview.h"
#include <set>

int nextKeyAvailable( vector<int> VectInt)
{
    set<int> intSet;
    unsigned int i;
    for (i=0; i<VectInt.size();i++)
    {
        intSet.insert(VectInt[i]);
    }

    i=0;
    unsigned int oldsize = intSet.size();

    do
    {
        i++;
        intSet.insert(i);
    }
    while (intSet.size()==oldsize);
    return i;
};

PointsModel::PointsModel(void) 
{
	numberpoints = 0; 
        selectedRow = -1;
}


void PointsModel::fillPoint(string PointTag, int row)
{
    EDomElement edepoint(PointTag);
    item(row,0)->setText(QString::fromUtf8(edepoint.elementByTagName("point").attribute("key").c_str()));
    item(row,1)->setText(QString::fromUtf8(edepoint.elementByTagName("gcp_id").toString().c_str()));
    item(row,1)->setEditable(0);
    item(row,2)->setText(QString::fromUtf8(edepoint.elementByTagName("point").attribute("type").c_str()));
    item(row,2)->setEditable(0);
    item(row,3)->setText(QString::fromUtf8(edepoint.elementByTagName("description").toString().c_str()));
    item(row,3)->setEditable(0);
    QString strcoord = QString::fromUtf8(edepoint.elementByTagName("spatialCoordinates").elementByTagName("gml:pos").toString().c_str());
    item(row,4)->setText(strcoord.section(' ',0,0));
    item(row,4)->setEditable(0);
    item(row,5)->setText(strcoord.section(' ',1,1));
    item(row,5)->setEditable(0);
    item(row,6)->setText(strcoord.section(' ',2,2));
    item(row,6)->setEditable(0);
    item(row,7)->setText(QString::fromUtf8(edepoint.elementByTagName("spatialCoordinates").elementByTagName("sigma").toString().c_str()));
    item(row,7)->setEditable(0);
    item(row,8)->setText(QString::fromUtf8(edepoint.elementByTagName("imagesMeasurements").getContent().c_str()));
    item(row,8)->setEditable(0);
}

int PointsModel::getrow(string gcp_id)
{
    string XMLString = getvalues();
    stringstream auxStream;
    deque <EDomElement> xmlTree;

    EDomElement pointsRoot(XMLString);
    xmlTree = pointsRoot.children();
    auxStream << "<points>";
    for (unsigned int i=0;i<xmlTree.size();i++)
    {
        if(xmlTree[i].elementByTagName("gcp_id").toString().c_str()==gcp_id)
            return int(i);
        auxStream << xmlTree[i].getContent();
    }
    return -1;
}

void PointsModel::fillvalues(vector <string> StrVector)
{
    string pointsString = StrVector[0];
    EDomElement edePoints(pointsString);
    deque <EDomElement> pointsList = edePoints.children();
    numberpoints= pointsList.size();
    //keyVector.clear();
    for(int i=0; i<numberpoints ; i++)
    {
        for(int j=0; j<=8 ; j++)
       	{     
            QStandardItem *item = new QStandardItem();
            setItem(i, j, item);
        }
        //imageMeasurementsVector.push_back("");
        //keyVector.push_back("");
    }
    // este if é uma gambiarra que evita que um SinglePointView tenha no início seu conteúdo deletado em vão !!!!
    if (numberpoints>0)
        selectedRow=0;

    EDomElement edePointAux;
    string aux;
    int i=0;
    while (!pointsList.empty())
    {
        fillPoint((string) pointsList.front().elementByTagName("point").getContent(),i);
    	pointsList.pop_front(); i++;
    }
    setHeaderData(1, Qt::Horizontal, QObject::tr("gcp_id"));
    setHeaderData(2, Qt::Horizontal, QObject::tr("type"));
     setHeaderData(3, Qt::Horizontal, QObject::tr("description"));
     if (StrVector.size()>=2)//unitCPS.contains("UTM", Qt::CaseInsensitive))
     { 
     	QString unitCPS = (QString::fromUtf8(StrVector[1].c_str()));
     	if (unitCPS.contains("UTM", Qt::CaseInsensitive))
     	{
                setHeaderData(4, Qt::Horizontal, QObject::tr("E"));
                setHeaderData(5, Qt::Horizontal, QObject::tr("N"));
                setHeaderData(6, Qt::Horizontal, QObject::tr("H"));
     	} 
      	else
       	{
                setHeaderData(4, Qt::Horizontal, QObject::tr("X"));
                setHeaderData(5, Qt::Horizontal, QObject::tr("Y"));
                setHeaderData(6, Qt::Horizontal, QObject::tr("Z"));
       	}
        setHeaderData(7, Qt::Horizontal, QObject::tr("sigma"));
	}
};

string PointsModel::getvalues()
{
    stringstream auxStream;
    auxStream << "<points>";
    for(int i=0; i<numberpoints; i++)
    {
        auxStream << "<point key=\""<<item(i,0)->text().toUtf8().data() <<"\" ";
        auxStream << "type=\"" << item(i,2)->text().toUtf8().data() << "\">";
        auxStream << "<gcp_id>" << item(i,1)->text().toUtf8().data()<<"</gcp_id>";
        auxStream << "<description>" << item(i,3)->text().toUtf8().data()<<"</description>";
        auxStream << "<spatialCoordinates uom=\"#m\"><gml:pos>"
                <<item(i,4)->text().toUtf8().data()<<" "
                <<item(i,5)->text().toUtf8().data()<<" "
                <<item(i,6)->text().toUtf8().data()<< "</gml:pos>"
                <<"<sigma>"<<item(i,7)->text().toUtf8().data()
		<<"</sigma></spatialCoordinates>";
        auxStream<< item(i,8)->text().toUtf8().data()<< "</point>";
    }
    auxStream << "</points>";
    return auxStream.str();
};

void PointsModel::selectRow(const QModelIndex &index)
{
   selectedRow=index.row();
}

void PointsModel::deleteSelectedRow(void)
{
    if ((selectedRow!=-1)&&(numberpoints>1)) // esta condição evita um falha de segmentação que ocorre quando deleta-se o último ponto
    {
        removeRow(selectedRow);
        selectedRow=-1;
        numberpoints--;
    }
}
void PointsModel::showModel()
{
    cout << "Chamada de showmodel" << endl;
    for(int i=0; i<numberpoints ; i++)
    {
        for(int j=0; j<=8 ; j++)
        {
            cout << item(i,j)->text().toUtf8().data() << endl;
        }
    }	
    cout << "Fim de showmodel" << endl;
};

PointsLayout::PointsLayout(QWidget* Owner):QVBoxLayout(Owner)
{
    PointsTable = new QTableView;
    PointsTable->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    PointsTable->verticalHeader()->hide();
    PointsTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    //PointsTable->setMinimumSize(600,250);
    
    addWidget(PointsTable);
    addStretch();


};


PointsLayout::~PointsLayout()
{
    
    delete PointsTable;
	

}

TypeComboBoxDelegate::TypeComboBoxDelegate(QObject *parent)
     : QItemDelegate(parent)
 {
 //	cout << "Chamada de TypeComboBoxDelegate" << endl;
 }

QWidget *TypeComboBoxDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex &/* index */) const
 {
     
     QComboBox *editor = new QComboBox(parent);
     editor->addItem(tr("control"));
     editor->addItem(tr("verification"));
     editor->addItem(tr("photogrammetric"));
 
     return editor;
 }

 void TypeComboBoxDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
 {
     QString typeString = index.model()->data(index, Qt::DisplayRole).toString();

     QComboBox *typeComboBox = static_cast<QComboBox*>(editor);
     int typevalue = typeComboBox->findText(typeString);
     typeComboBox->setCurrentIndex(typevalue);
 }

 void TypeComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *pointsmodel,
                                    const QModelIndex &index) const
 {
     QComboBox *typeComboBox = static_cast<QComboBox*>(editor);
     typeComboBox->update();
     QString typevalue = typeComboBox->currentText();

     pointsmodel->setData(index, typevalue, Qt::EditRole);
 }

 void TypeComboBoxDelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
 {
       editor->setGeometry(option.rect);
 }

void PointsModel::removepoint(string pointID)
{
	string XMLString = getvalues();
	stringstream auxStream;
	deque <EDomElement> xmlTree;
	EDomElement pointsRoot(XMLString);
	xmlTree = pointsRoot.children();
	auxStream << "<points>";
	for (unsigned int i=0;i<xmlTree.size();i++)
	{	
		if(xmlTree[i].elementByTagName("gcp_id").toString().c_str()==pointID)
			continue;
		auxStream << xmlTree[i].getContent();
	}	
	auxStream << "</points>";
	XMLString =  auxStream.str();
	vector<string> StrVector;
	StrVector.push_back(XMLString);
	fillvalues(StrVector);
}				

string PointsModel::inttostring(int intconvert)
{
        stringstream convertstrstream;
	convertstrstream << intconvert;
	string strconvert = convertstrstream.str();
	return strconvert;
};	

void PointsModel::addpoint()
{
    vector<int> MyVectInt;
    for (int i=0; i< numberpoints; i++)
        MyVectInt.push_back(item(i,0)->text().toInt());

    for(int j=0; j<=8 ; j++)
    {
        QStandardItem *item = new QStandardItem();
        setItem(numberpoints, j, item);
        item->setEditable(0);
    }
    item(numberpoints,0)->setText(QString::number(nextKeyAvailable( MyVectInt)));
    numberpoints++;
};


PointsView::PointsView( QWidget *parent, QAbstractItemModel* /*PointsModel*/ pointmodel) : QWidget(parent)
{
       pointslayout = new PointsLayout(this);
 	   this->setLayout(pointslayout);
       pointslayout->PointsTable->setModel(pointmodel);
       pointslayout->PointsTable->hideColumn(0);
       pointslayout->PointsTable->hideColumn(8);
       QAbstractItemView* AuxAbsItemView = (QAbstractItemView*) pointslayout->PointsTable;
       AuxAbsItemView->setSelectionBehavior (QAbstractItemView::SelectRows);
};

PointsView::~PointsView()
{
       delete pointslayout;
};



void SinglePointModel::setSourceModel( QAbstractItemModel* paramSourceModel)
{
    QSortFilterProxyModel::setSourceModel(paramSourceModel);
    QStandardItemModel* auxModel= (QStandardItemModel*) paramSourceModel;

    if (auxModel->rowCount()>0)
        selectRow(auxModel->item(0,0)->text());
};

void SinglePointModel::selectRow(QString PointKey)
{
    setFilterKeyColumn(0);
    QRegExp regExp(PointKey, Qt::CaseSensitive, QRegExp::RegExp);
    this->setFilterRegExp(regExp);
    emit dataChanged(index(0,0), index(0,8));

};

void SinglePointModel::selectRow(const QModelIndex & index)
{
    QProxyModel* AuxProxyModel = (QProxyModel*) this;
    QStandardItemModel * auxModel = (QStandardItemModel *) AuxProxyModel->model();
    QString MyString = auxModel->item(index.row(),0)->text().toUtf8().data();
    selectRow(MyString);
};


SinglePointLayout::SinglePointLayout(QWidget* Owner)
{
    gcpIDLabel = new QLabel(tr("gcpID:"),Owner);
    gcpIDLineEdit = new QLineEdit(Owner);

    typeLabel = new QLabel(tr("Type:"),Owner);
    typeComboBox = new QComboBox(Owner);
    typeComboBox->addItem(tr("control"));
    typeComboBox->addItem(tr("verification"));
    typeComboBox->addItem(tr("photogrammetric"));

    gcpIDLayout = new QHBoxLayout;
    gcpIDLayout->addWidget(gcpIDLabel);
    gcpIDLayout->addWidget(gcpIDLineEdit);
    gcpIDLayout->addSpacing(20);
    gcpIDLayout->addWidget(typeLabel);
    gcpIDLayout->addWidget(typeComboBox);
    //gcpIDLayout->addStretch();

    descriptionLabel = new QLabel(tr("Description:"),Owner);
    descriptionTextEdit = new QTextEdit(Owner);//= new QLineEdit(Owner);
    descriptionTextEdit->setFixedHeight(40);

    descriptionLayout = new QHBoxLayout;
    descriptionLayout->addWidget(descriptionLabel);
    descriptionLayout->addWidget(descriptionTextEdit);
    //descriptionLayout->addStretch();

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
    sigmaLayout->addStretch();

    spatialCoordVLayout = new QVBoxLayout;
    spatialCoordVLayout->addLayout(spatialCoordHLayout);
    spatialCoordVLayout->addLayout(sigmaLayout);

    spatialCoordGroupBox->setLayout(spatialCoordVLayout);

    verticalGeneralLayout = new QVBoxLayout;
    verticalGeneralLayout->addLayout(gcpIDLayout);
    verticalGeneralLayout->addLayout(descriptionLayout);
    verticalGeneralLayout->addWidget(spatialCoordGroupBox);
    verticalGeneralLayout->addStretch();
    addLayout(verticalGeneralLayout);
    addStretch();
};

SinglePointLayout::~SinglePointLayout()
{
    delete gcpIDLabel;
    delete gcpIDLineEdit;
    delete typeLabel;
    delete typeComboBox;
    delete descriptionLabel;
    delete descriptionTextEdit;
    delete spatialCoord01Label;
    delete spatialCoord01LineEdit;
    delete spatialCoord02Label;
    delete spatialCoord02LineEdit;
    delete spatialCoord03Label;
    delete spatialCoord03LineEdit;
    delete sigmaLabel;
    delete sigmaLineEdit;
    delete spatialCoordGroupBox;

}


void SinglePointMapper::makeMapper(SinglePointLayout* getlayoutMapper,  QStandardItemModel *getMapperModel)
{
    setModel(getMapperModel);
    addMapping(getlayoutMapper->gcpIDLineEdit,1);
    addMapping(getlayoutMapper->typeComboBox,2,"currentText");
    addMapping(getlayoutMapper->descriptionTextEdit,3);
    addMapping(getlayoutMapper->spatialCoord01LineEdit,4);
    addMapping(getlayoutMapper->spatialCoord02LineEdit,5);
    addMapping(getlayoutMapper->spatialCoord03LineEdit,6);
    addMapping(getlayoutMapper->sigmaLineEdit,7);
    toFirst();
    if (getMapperModel->rowCount()>0)
    {
        QStandardItemModel * auxModel = (QStandardItemModel *)getMapperModel;
        QString MyString =auxModel->data(auxModel->index(0,2)).toString().toAscii().data();
        if (MyString=="control")
            getlayoutMapper->typeComboBox->setCurrentIndex(0);
        if (MyString=="verification")
            getlayoutMapper->typeComboBox->setCurrentIndex(1);
        if (MyString=="photogrammetric")
            getlayoutMapper->typeComboBox->setCurrentIndex(2);
        if (MyString=="")
            getlayoutMapper->typeComboBox->setCurrentIndex(-1);
    }

};



SinglePointView::SinglePointView( QWidget *parent,  QAbstractItemModel * singlepointmodel) : QWidget(parent)
{
    singlepointlayout = new SinglePointLayout(this);
    this->setLayout(singlepointlayout);
    singlepointlayout->typeComboBox->setCurrentIndex(-1);
    singlepointmapper.makeMapper(singlepointlayout,  (QStandardItemModel*) singlepointmodel);
};

SinglePointView::~SinglePointView()
{
       delete singlepointlayout;
};

void SinglePointView::setToUTM(void)
{
    singlepointlayout->spatialCoord01Label->setText("E");
    singlepointlayout->spatialCoord02Label->setText("N");
    singlepointlayout->spatialCoord03Label->setText("H");
};

void SinglePointView::setReadOnly(bool enabled)
{
    singlepointlayout->gcpIDLineEdit->setReadOnly(enabled);
    singlepointlayout->typeComboBox->setDisabled(enabled);
    singlepointlayout->descriptionTextEdit->setReadOnly(enabled);
    singlepointlayout->spatialCoord01LineEdit->setReadOnly(enabled);
    singlepointlayout->spatialCoord02LineEdit->setReadOnly(enabled);
    singlepointlayout->spatialCoord03LineEdit->setReadOnly(enabled);
    singlepointlayout->sigmaLineEdit->setReadOnly(enabled);
}


void SinglePointView::remap(void)
{
    singlepointmapper.makeMapper(singlepointlayout, (QStandardItemModel*) singlepointmapper.model());

    // gambiarra necessária para atualizar o valor no maldito combobox é feio mais funciona!!!
    QStandardItemModel * auxModel = (QStandardItemModel *)singlepointmapper.model();
    QString MyString =auxModel->data(auxModel->index(0,2)).toString().toAscii().data();
    if (MyString=="control")
        singlepointlayout->typeComboBox->setCurrentIndex(0);
    if (MyString=="verification")
        singlepointlayout->typeComboBox->setCurrentIndex(1);
    if (MyString=="photogrammetric")
        singlepointlayout->typeComboBox->setCurrentIndex(2);
    if (MyString=="")
        singlepointlayout->typeComboBox->setCurrentIndex(-1);
};

void SinglePointView::clean(void)
{
    singlepointlayout->gcpIDLineEdit->setText("");
    singlepointlayout->descriptionTextEdit->setText("");
    singlepointlayout->spatialCoord01LineEdit->setText("");
    singlepointlayout->spatialCoord02LineEdit->setText("");
    singlepointlayout->spatialCoord03LineEdit->setText("");
    singlepointlayout->sigmaLineEdit->setText("");
    singlepointlayout->typeComboBox->setCurrentIndex(-1);
}
