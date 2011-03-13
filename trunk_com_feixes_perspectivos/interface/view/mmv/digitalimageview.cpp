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

#include "digitalimageview.h"

int getComboIndex(int Code, vector<int> CodeVector)
{
    int result = -1;
    for (int i=0;i< (int) CodeVector.size();i++)
    {
        if (CodeVector.at(i)==Code)
        {
            result = i;
            break;
        }
    }
    return result;
}

DigitalImageModel::DigitalImageModel(void)
{
    for(int i=0; i<=11 ; i++)
    {
        QStandardItem *item = new QStandardItem();
        setItem(0, i, item);
    }
    this->item(0,1)->setText("-1");
    this->item(0,2)->setText("-1");
}

void DigitalImageModel::fillvalues(vector<string> StrVector)
{

    string xmlDigitalImageValues = StrVector[0];

     QString AuxString;
    fillComboValues(StrVector);
    EDomElement ede(xmlDigitalImageValues);
    item(0, 0)->setText(QString::fromUtf8(ede.elementByTagName("image").attribute("key").c_str()));
    AuxString = ede.elementByTagName("image").attribute("sensor_key").c_str();
    AuxString.setNum(getComboIndex(AuxString.toInt(), SensorComboCode));
    item(0, 1)->setText(AuxString.toAscii().data());
    AuxString =ede.elementByTagName("image").attribute("flight_key").c_str();
    AuxString.setNum(getComboIndex(AuxString.toInt(), FlightComboCode));
    item(0, 2)->setText(AuxString);
    item(0, 3)->setText(QString::fromUtf8(ede.elementByTagName("image_id").toString().c_str()));
    item(0, 4)->setText(QString::fromUtf8(ede.elementByTagName("fileName").toString().c_str()));
    item(0, 5)->setText(QString::fromUtf8(ede.elementByTagName("filePath").toString().c_str()));
    item(0, 6)->setText(QString::fromUtf8(ede.elementByTagName("width").toString().c_str()));
    item(0, 7)->setText(QString::fromUtf8(ede.elementByTagName("height").toString().c_str()));
    item(0, 8)->setText(QString::fromUtf8(ede.elementByTagName("resolution").toString().c_str()));
    QString MyQString = QString::fromUtf8(ede.elementByTagName("gml:pos").toString().c_str());
    item(0, 9)->setText(MyQString.section ( ' ',0,0,QString::SectionSkipEmpty));
    item(0, 10)->setText(MyQString.section ( ' ',1,1,QString::SectionSkipEmpty));
    item(0, 11)->setText(MyQString.section ( ' ',2,2,QString::SectionSkipEmpty));
}

void DigitalImageModel::fillComboValues(vector<string> StrVector)
{
    SensorComboCode.clear();
    SensorComboText.clear();
    FlightComboCode.clear();
    FlightComboText.clear();

    EDomElement EDESensor(StrVector[1]); // sensor xmlString
    deque <EDomElement> dequeSensor = EDESensor.children();
    QString AuxString;
    for (int i=0; i<(int)dequeSensor.size();i++)
    {
        AuxString= dequeSensor.at(i).elementByTagName("sensor").attribute("key").c_str();
        SensorComboCode.push_back(AuxString.toInt());//SensorComboCode.push_back(QString::toInt())
        AuxString= dequeSensor.at(i).elementByTagName("description").toString().c_str();
        SensorComboText.push_back(AuxString.toAscii().data());
    }

    EDomElement EDEFlight(StrVector[2]); // flight xmlString
    deque <EDomElement> dequeFlight = EDEFlight.children();

    for (int i=0; i<(int)dequeFlight.size();i++)
    {
        AuxString= dequeFlight.at(i).elementByTagName("flight").attribute("key").c_str();
        FlightComboCode.push_back(AuxString.toInt());//SensorComboCode.push_back(QString::toInt())
        AuxString= dequeFlight.at(i).elementByTagName("description").toString().c_str();
        FlightComboText.push_back(AuxString.toAscii().data());
    }
}

string DigitalImageModel::getvalues()
{
    QString Sensor_Key, Flight_Key;
    Sensor_Key = item(0,1)->text().toUtf8().data();
    if (Sensor_Key.toInt()>-1)
        Sensor_Key.setNum(SensorComboCode.at(Sensor_Key.toInt()));//Sensor_Key.toInt()));

    Flight_Key= item(0,2)->text().toUtf8().data();
    if (Flight_Key.toInt()>-1)
        Flight_Key.setNum(FlightComboCode.at(Flight_Key.toInt()));

    stringstream auxStream;
    auxStream << "<image key=\"" << item(0,0)->text().toUtf8().data() << "\" sensor_key=\""<<Sensor_Key.toAscii().data(); //item(0,1)->text().toUtf8().data();
    auxStream << "\" " << "flight_key=\""<<Flight_Key.toAscii().data()<< "\">";//item(0,2)->text().toUtf8().data() << "\"" <<">";
    auxStream << "<image_id>" << item(0,3)->text().toUtf8().data() << "</image_id>";
    auxStream << "<width uom=\"#px\">" << item(0,6)->text().toUtf8().data() << "</width>";
    auxStream << "<height uom=\"#px\">" << item(0,7)->text().toUtf8().data() << "</height>";
    auxStream << "<fileName>" << item(0,4)->text().toUtf8().data() << "</fileName>";
    auxStream << "<filePath>" << item(0,5)->text().toUtf8().data() << "</filePath>";
    auxStream << "<resolution uom=\"#dpi\">" << item(0,8)->text().toUtf8().data() << "</resolution>";
    auxStream << "<spatialCoordinates uom=\"#m\"><gml:pos>" << item(0,9)->text().toUtf8().data() <<
            " " << item(0,10)->text().toUtf8().data()<< " " << item(0,11)->text().toUtf8().data()<<
            "</gml:pos>" << "</spatialCoordinates>";
    auxStream << "</image>";
    xmldigitalimagestring=auxStream.str();
    return xmldigitalimagestring;
}

void DigitalImageModel::showModel()
{
    for(int i=0; i<=11 ; i++)
    {
        cout << item(0,i)->text().toUtf8().data() << " | ";
    }
    cout << endl;
}

DigitalImageLayout::DigitalImageLayout(QWidget* Owner)
{
    MainLayout = new QVBoxLayout;//CoordinatesGroupBox

    BrowseButton = new QPushButton(tr("&Browse..."),Owner);

    // CoordinatesGroupBox
    Coord1LineEdit = new QLineEdit;
    Coord2LineEdit = new QLineEdit;
    Coord3LineEdit = new QLineEdit;
    Coord1LineEdit->setValidator( new QDoubleValidator(Coord1LineEdit) );
    Coord2LineEdit->setValidator( new QDoubleValidator(Coord1LineEdit) );
    Coord3LineEdit->setValidator( new QDoubleValidator(Coord1LineEdit) );

    Coord1Label = new QLabel;
    Coord2Label = new QLabel;
    Coord3Label = new QLabel;
    Coord1Label->setText("X:");
    Coord2Label->setText("Y:");
    Coord3Label->setText("Z:");
    Coord1UnitLabel = new QLabel;
    Coord2UnitLabel = new QLabel;
    Coord3UnitLabel = new QLabel;
    Coord1UnitLabel->setText("m");
    Coord2UnitLabel->setText("m");
    Coord3UnitLabel->setText("m");

    CoordinatesLayout = new QHBoxLayout;
    CoordinatesLayout->addWidget(Coord1Label);
    CoordinatesLayout->addWidget(Coord1LineEdit);
    CoordinatesLayout->addWidget(Coord1UnitLabel);
    CoordinatesLayout->addSpacing(10);
    CoordinatesLayout->addWidget(Coord2Label);
    CoordinatesLayout->addWidget(Coord2LineEdit);
    CoordinatesLayout->addWidget(Coord2UnitLabel);
    CoordinatesLayout->addSpacing(10);
    CoordinatesLayout->addWidget(Coord3Label);
    CoordinatesLayout->addWidget(Coord3LineEdit);
    CoordinatesLayout->addWidget(Coord3UnitLabel);
    CoordinatesGroupBox = new QGroupBox(tr("Image coordinates:"));
    CoordinatesGroupBox->setLayout(CoordinatesLayout);

    //  ImageSizeGroupBox
    WidthLabel = new QLabel(tr("Width:"),Owner);
    WidthLineEdit = new QLineEdit(Owner);
    WidthLineEdit->setReadOnly(1);
    WidthpixLabel = new QLabel(tr("pixels"),Owner);
    HeightLabel = new QLabel(tr("Height:"),Owner);
    HeightLineEdit = new QLineEdit(Owner);
    HeightLineEdit->setReadOnly(1);
    HeightpixLabel = new QLabel(tr("pixels"),Owner);

    ImageSizeLayout = new QHBoxLayout;
    ImageSizeLayout-> addWidget(WidthLabel);
    ImageSizeLayout-> addWidget(WidthLineEdit);
    ImageSizeLayout-> addWidget(WidthpixLabel);
    ImageSizeLayout-> addSpacing(10);//
    ImageSizeLayout-> addWidget(HeightLabel);
    ImageSizeLayout -> addWidget(HeightLineEdit);
    ImageSizeLayout-> addWidget(HeightpixLabel);
    //HeightHorizLayout ->addStretch();
    ImageSizeGroupBox= new QGroupBox(tr("Size:"));
    ImageSizeGroupBox->setLayout(ImageSizeLayout);

    QLabel *FileNameLabel = new QLabel(tr("Name:"));
    ImageNameLineEdit = new QLineEdit(Owner);
    ImageNameLineEdit->setReadOnly(1);
    ImageFileVBoxLayout = new QVBoxLayout;
    ImageNameHLayout = new QHBoxLayout;

    ImageNameHLayout->addWidget(FileNameLabel);
    ImageNameHLayout->addWidget(ImageNameLineEdit);
    ImageNameHLayout->addWidget(BrowseButton);

    ImageFileVBoxLayout->addLayout(ImageNameHLayout);

     ImagePathHBoxLayout = new QHBoxLayout;
    ImageLabel = new QLabel(tr("Path:"),Owner);
    PathImageLineEdit = new QLineEdit(Owner);
    PathImageLineEdit->setReadOnly(1);
    ImagePathHBoxLayout -> addWidget(ImageLabel);

    ResolutionLabel = new QLabel(tr("Resolution:"),Owner);
    ResolutionSpinBox = new QSpinBox(Owner);
    ResolutionUnitLabel = new QLabel(tr("dpi"));
    ResolutionSpinBox->setRange(75,1500);
    ResolutionHorizLayout = new QHBoxLayout;

    ImagePathHBoxLayout -> addWidget(PathImageLineEdit);

    ImageFileVBoxLayout->addLayout(ImagePathHBoxLayout);
    ResolutionHorizLayout -> addWidget(ResolutionLabel);
    ResolutionHorizLayout -> addWidget(ResolutionSpinBox);
    ResolutionHorizLayout->addWidget(ResolutionUnitLabel);
    ResolutionHorizLayout ->addStretch();
    ImageFileVBoxLayout->addLayout(ResolutionHorizLayout);

    ImageFileHBoxLayout=new QHBoxLayout;
    ImageFileVBoxLayout->addWidget(ImageSizeGroupBox);
    ImageFileHBoxLayout->addStretch();

    FlightidLabel = new QLabel(tr("FlightID:"),Owner);
    FlightidComboBox = new QComboBox(Owner);
    SensorLabel = new QLabel(tr("Sensor:"),Owner);
    SensorComboBox = new QComboBox(Owner);
    ImageIdLabel = new QLabel("Image Id:");
    ImageIdLineEdit = new QLineEdit(Owner);
    ImageIdLineEdit->setMinimumWidth(50);

    QHBoxLayout *idHorizLayout = new QHBoxLayout;
    idHorizLayout->addWidget(ImageIdLabel);
    idHorizLayout->addWidget(ImageIdLineEdit);
    idHorizLayout ->addStretch();
    idHorizLayout->addWidget(SensorLabel);
    idHorizLayout->addWidget(SensorComboBox);
    idHorizLayout ->addStretch();
    idHorizLayout -> addWidget(FlightidLabel);
    idHorizLayout -> addWidget(FlightidComboBox);

    MainLayout->addLayout(idHorizLayout);
    ImageFileGroupBox= new QGroupBox(tr("Image file:"));
    ImageFileGroupBox->setLayout(ImageFileVBoxLayout);

    MainLayout->addWidget(ImageFileGroupBox);
    MainLayout->addWidget(CoordinatesGroupBox);
    MainLayout->addStretch();


    addLayout(MainLayout);
    addStretch();
 };

void DigitalImageView::flightIdComboFillvalues(vector<string> dataFlightId)
{
    for(int i=0; i < (int) dataFlightId.size(); i++)
    {
        digitalimagelayout->FlightidComboBox->addItem(QString::fromUtf8(dataFlightId[i].c_str()));
    }
};

void DigitalImageView::SensorComboFillvalues(vector<string> dataSensorId)
{
    for(int i=0; i < (int) dataSensorId.size(); i++)
    {
        digitalimagelayout->SensorComboBox->addItem(QString::fromUtf8(dataSensorId[i].c_str()));
    }
};        

void DigitalImageView::browse()
{
    QDir *strRelativePath = new QDir(QDir::current());
    QWidget *OpenImage = new QWidget;
    QString ImageName = QFileDialog::getOpenFileName(OpenImage,
                                                     tr("Select image"),strRelativePath->path(),
                                                     tr("Image Files(*.png *.jpg *.bmp *.tif *.tiff)"));
    QStandardItemModel *AuxModel;
    AuxModel = (QStandardItemModel*) digitalimagemapper.model();
    AuxModel->item(0,4)->setText(ImageName.section('/', -1));
    AuxModel->item(0,5)->setText("./" + strRelativePath->relativeFilePath(ImageName).section('/',0,-2));
    QImage image(ImageName);
    QString AuxString;
    AuxString.setNum(image.width());
    AuxModel->item(0,6)->setText(AuxString);
    AuxString.setNum(image.height());
    AuxModel->item(0,7)->setText(AuxString);
};

DigitalImageLayout::~DigitalImageLayout()
{
// É preciso rever este destrutor para que seja tudo destruído    

    delete ImageIdLineEdit;
    delete ImageLabel;
    delete PathImageLineEdit;
    delete BrowseButton;
    delete ImageNameLineEdit;
    delete WidthpixLabel;
    delete HeightpixLabel;
    delete WidthLineEdit;
    delete HeightLineEdit;
    delete FlightidComboBox;
    delete ResolutionSpinBox;
    delete SensorComboBox;
    delete FlightidLabel;
    delete WidthLabel;
    delete HeightLabel;
    delete ResolutionLabel;
    delete SensorLabel;
};

void DigitalImageView::changeText()
{
    DigitalImageModel *MyDigitalImageModel;
    MyDigitalImageModel = (DigitalImageModel*) digitalimagemapper.model();
};

void DigitalImageView::setToUTM(void)
{
    digitalimagelayout->Coord1Label->setText("E: ");
    digitalimagelayout->Coord2Label->setText("N: ");
    digitalimagelayout->Coord3Label->setText("H: ");
}


void DigitalImageMapper::makeMapper(DigitalImageLayout* layoutdata, DigitalImageModel *getMapperModel)
{

    setModel(getMapperModel);//
    addMapping(layoutdata->ImageIdLineEdit,3);
    addMapping(layoutdata->PathImageLineEdit,5,"text");
    addMapping(layoutdata->ImageNameLineEdit,4,"text");
    addMapping(layoutdata->FlightidComboBox,2,"currentIndex");
    addMapping(layoutdata->WidthLineEdit,6);
    addMapping(layoutdata->HeightLineEdit,7);
    addMapping(layoutdata->ResolutionSpinBox,8);
    addMapping(layoutdata->SensorComboBox,1,"currentIndex");
    addMapping(layoutdata->Coord1LineEdit,9);
    addMapping(layoutdata->Coord2LineEdit,10);
    addMapping(layoutdata->Coord3LineEdit,11);
    toFirst();
    //layoutdata->ImageData();
};

DigitalImageView::DigitalImageView(QWidget *parent, DigitalImageModel* digitalimagemodel /*,vector<string> newDataFlightId, string newDataSensor*/) : QWidget(parent)
{
    digitalimagelayout = new DigitalImageLayout(this);
    this->flightIdComboFillvalues(digitalimagemodel->getFlightTexts());
    this->SensorComboFillvalues(digitalimagemodel->getSensorTexts());
    this->setLayout(digitalimagelayout);
    digitalimagemapper.makeMapper(digitalimagelayout,digitalimagemodel);
    connect(digitalimagelayout->BrowseButton,SIGNAL(clicked()),this,SLOT(browse()));
};

DigitalImageView::~DigitalImageView()
{
    changeText();
    delete digitalimagelayout;
};
