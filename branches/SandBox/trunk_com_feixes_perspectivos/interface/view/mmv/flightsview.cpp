#include "flightsview.h"
#include "EDom.h"

FlightsModel::FlightsModel(void)
{
	cout <<"Chamada de construtor de ProjectHeaderModel" << endl;
	for(int i=0; i<=6 ; i++)
	{
     QStandardItem *item = new QStandardItem();   
     setItem(0, i, item);
    }	
}

void FlightsModel::fillvalues(string xmlFlightsValues)
{
	cout << "Chamada de fillvalues" << endl;
	EDomElement ede(xmlFlightsValues);
    string edefrac;
    int index = (xmlFlightsValues.find("</mml:mn>")) + 9;
    edefrac = xmlFlightsValues.substr(index);
    EDomElement edomfrac(edefrac);
    
	item(0, 0)->setText(QString::fromUtf8(ede.elementByTagName("description").toString().c_str()));
	item(0, 1)->setText(QString::fromUtf8(ede.elementByTagName("execution").toString().c_str()));
	item(0, 2)->setText(QString::fromUtf8(ede.elementByTagName("producerName").toString().c_str()));
	item(0, 3)->setText(/*QString::fromUtf8(ede.elementByTagName("mml:mn").toString().c_str())
                         + "/" + */QString::fromUtf8(edomfrac.elementByTagName("mml:mn").toString().c_str()));
	item(0, 4)->setText(QString::fromUtf8(ede.elementByTagName("flightHeight").toString().c_str()));
	item(0, 5)->setText(QString::fromUtf8(ede.elementByTagName("longitudinal").toString().c_str()));
	item(0, 6)->setText(QString::fromUtf8(ede.elementByTagName("transversal").toString().c_str()));

    cout << "Fim de fillvalues" << endl;
};

string FlightsModel::getvalues()
{
    cout << "Chamada de getvalues" << endl;
    string xmlflightsstring;
	stringstream auxStream;
	QString fracnum, fracden, frac;
	frac = (item(0,3)->text());
	int div = frac.indexOf("/");
	fracnum = frac.left(div);
	fracden = frac.mid(div+1);
    	
    auxStream << "<flights>";
	auxStream << "<flight key=\"1\" sensor_key=\"1\">";
	auxStream << "<description>" << item(0,0)->text().toUtf8().data() << "</description>";
	auxStream << "<execution>" << item(0,1)->text().toUtf8().data() << "</execution>";
	auxStream << "<producerName>" << item(0,2)->text().toUtf8().data() << "</producerName>";
	auxStream << "<nominalScale>";
	auxStream << "<mml:mfrac>";
	auxStream << "<mml:mn>" << fracnum.toUtf8().data() << "</mml:mn>";
	auxStream << "<mml:mn>" << fracden.toUtf8().data() << "</mml:mn>";
	auxStream << "</mml:mfrac>";
	auxStream << "</nominalScale>";
	auxStream << "<flightHeight uom=\"#m\">" << item(0,4)->text().toUtf8().data() << "</flightHeight>";
	auxStream << "<overlap>";
	auxStream << "<longitudinal uom=\"#%\" >" << item(0,5)->text().toUtf8().data() << "</longitudinal>";
	auxStream << "<transversal uom=\"#%\" >" << item(0,6)->text().toUtf8().data() << "</transversal>";
	auxStream << "</overlap>";
	auxStream << "</flight>";
	auxStream << "</flights>";
	xmlflightsstring=auxStream.str();
    return xmlflightsstring;
};

void FlightsModel::showModel()
{
	cout << "Chamada de showmodel" << endl;
	for(int i=0; i<=6 ; i++)
	{
       cout << item(0,i)->text().toUtf8().data() << endl; 
    }
	cout << "Fim de showmodel" << endl;
};

FlightsLayout::FlightsLayout(QWidget* FlightsOwner)
{
    cout << "Chamada de Construtor de FlightsLayout" << endl;
	FlightsNameLabel= new QLabel(tr("Local Flight:"), FlightsOwner);
	FlightsNameLineEdit =new QLineEdit(FlightsOwner);
	FlightsNameLineEdit->setMinimumWidth(300);
	LocalFlightLayout = new QHBoxLayout;
	LocalFlightLayout->addWidget(FlightsNameLabel);
	LocalFlightLayout->addWidget(FlightsNameLineEdit);
	LocalFlightLayout->addStretch();
	
	ExecutionDateLabel = new QLabel(tr("Execution:"),FlightsOwner);
	ExecutionDateTimeEdit =new QDateEdit(FlightsOwner); 
//	ExecutionDateTimeEdit->setReadOnly(true);
	ExecutiondateHorizLayout = new QHBoxLayout;
	ExecutiondateHorizLayout->addWidget(ExecutionDateLabel);
	ExecutiondateHorizLayout->addWidget(ExecutionDateTimeEdit,0,Qt::AlignLeft);
	ExecutiondateHorizLayout->addStretch();
	
	ProducerNameLabel= new QLabel(tr("Producer Name:"),FlightsOwner);
	ProducerNameLineEdit =new QLineEdit(FlightsOwner);
	ProducerHorizLayout = new QHBoxLayout;
	ProducerHorizLayout->addWidget(ProducerNameLabel);
	ProducerHorizLayout->addWidget(ProducerNameLineEdit);
	ProducerHorizLayout->addStretch();
	
	NominalScaleLabel= new QLabel(tr("Nominal Scale:   1:"),FlightsOwner);
	NominalScaleLineEdit =new QLineEdit(FlightsOwner);
	NominalScaleLineEdit->setValidator(new QIntValidator(NominalScaleLineEdit));
	NominalScaleLineEdit->setMaximumWidth(80);
	NominalScaleHorizLayout = new QHBoxLayout;
	NominalScaleHorizLayout->addWidget(NominalScaleLabel);
	NominalScaleHorizLayout->addWidget(NominalScaleLineEdit);
	NominalScaleHorizLayout->addStretch();
	
	FlightHeightLabel= new QLabel(tr("FlightHeight:"),FlightsOwner);
	FlightHeightLineEdit =new QLineEdit(FlightsOwner);
	FlightHeightLineEdit->setValidator(new QDoubleValidator(FlightHeightLineEdit));
	FlightHeightLineEdit->setMaximumWidth(80);	
	QLabel *FlightHeightUnitLabel= new QLabel(tr("m"),FlightsOwner);
	FlightHeightHorizLayout = new QHBoxLayout;
	FlightHeightHorizLayout->addWidget(FlightHeightLabel);
	FlightHeightHorizLayout->addWidget(FlightHeightLineEdit);
	FlightHeightHorizLayout->addWidget(FlightHeightUnitLabel);
	FlightHeightHorizLayout->addStretch();	

	LongLabel= new QLabel(tr("Longitudinal:"),FlightsOwner);
	LongSpinBox =new QSpinBox(FlightsOwner);
	LongSpinBox->setRange(1,100);
	LongSpinBox->setMaximumWidth(60);
	QLabel *longpercentualLabel = new QLabel(tr("\%"));
	LongHorizLayout = new QHBoxLayout;
	LongHorizLayout->addWidget(LongLabel,0,Qt::AlignRight);
	LongHorizLayout->addWidget(LongSpinBox,0,Qt::AlignLeft);
	LongHorizLayout->addWidget(longpercentualLabel,0,Qt::AlignLeft);	
	
	TransvLabel= new QLabel(tr("Transversal:"),FlightsOwner);
	TransvSpinBox =new QSpinBox(FlightsOwner);
	TransvSpinBox->setRange(1,100);
	TransvSpinBox->setMaximumWidth(60);
	QLabel *transvpercentualLabel = new QLabel(tr("%"));
	TransvHorizLayout = new QHBoxLayout;
	TransvHorizLayout->addWidget(TransvLabel,0,Qt::AlignRight);
	TransvHorizLayout->addWidget(TransvSpinBox,0,Qt::AlignLeft);
	TransvHorizLayout->addWidget(transvpercentualLabel,0,Qt::AlignLeft);
    
    OverlapLayout = new QHBoxLayout;
    OverlapLayout->addLayout(LongHorizLayout);
    OverlapLayout->addStretch();	
    OverlapLayout->addLayout(TransvHorizLayout);
    	
    OverlapGroupBox = new QGroupBox(tr("OverLap"));
    OverlapGroupBox->setLayout(OverlapLayout);
    OverlapGroupBox->setFixedSize(400,60);
    
	FlightsVLayout = new QVBoxLayout;
	FlightsVLayout->addLayout(LocalFlightLayout);
    FlightsVLayout->addLayout(ExecutiondateHorizLayout);
    FlightsVLayout->addLayout(ProducerHorizLayout);
    FlightsVLayout->addLayout(NominalScaleHorizLayout);
    FlightsVLayout->addLayout(FlightHeightHorizLayout);
    FlightsVLayout->addWidget(OverlapGroupBox);
    FlightsVLayout->addStretch();
    
	addLayout(FlightsVLayout);
    
	cout << "Fim de Construtor de FlightsLayout" << endl;

};

FlightsLayout::~FlightsLayout()
{
    cout << "Chamada de destrutor de FlightsLayout" << endl;
	delete FlightsNameLabel;
	delete FlightsNameLineEdit;
	delete ExecutionDateLabel;
	delete ExecutionDateTimeEdit;
	delete ProducerNameLabel;
	delete ProducerNameLineEdit;
	delete NominalScaleLabel;
	delete NominalScaleLineEdit;
	delete FlightHeightLabel;
	delete FlightHeightLineEdit;
	delete LongLabel;
	delete LongSpinBox;
	delete TransvLabel;
	delete TransvSpinBox;
	
	cout << "Fim de destrutor de FlightsLayout" << endl;

}

void FlightsMapper::changeText()
{
	 	cout << "Chamada de changeText " << endl;
	 	FlightsModel *MyFlightsModel;
	 	MyFlightsModel = (FlightsModel*) this->model();
	 	MyFlightsModel->getvalues();
};

void FlightsMapper::makeMapper(FlightsLayout* LayoutMapper, FlightsModel *getMapperModel)
{
	cout << "Chamada de makemapper" << endl;
	setModel(getMapperModel);
	addMapping(LayoutMapper->FlightsNameLineEdit,0);
	addMapping(LayoutMapper->ExecutionDateTimeEdit,1);
	addMapping(LayoutMapper->ProducerNameLineEdit,2);
	addMapping(LayoutMapper->NominalScaleLineEdit,3);
	addMapping(LayoutMapper->FlightHeightLineEdit,4);
	addMapping(LayoutMapper->LongSpinBox,5);
	addMapping(LayoutMapper->TransvSpinBox,6);
    toFirst();
    connect(getMapperModel,SIGNAL(itemChanged(QStandardItem *)),this,SLOT(changeText()));
};

FlightsView::FlightsView(QWidget *parent, FlightsModel* flightsmodel) : QWidget(parent)
{
       flightslayout = new FlightsLayout(this);
	   //projectheaderlayout->show();

	   this->setLayout(flightslayout);
       flightsmapper.makeMapper(flightslayout,flightsmodel);
};

FlightsView::~FlightsView()
{
       cout << "Chamada de destrutor de FlightsView" << endl; 
       delete flightslayout;
       cout << "Fim de destrutor de FlightsView" << endl;        		
};
