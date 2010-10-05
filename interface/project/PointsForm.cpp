#include "PointsForm.h"
#include <QLabel>
#include <QLayout>

PointsForm::PointsForm(QWidget *parent,string XmlString) : AbstractForm(parent)
{
//    QLabel *MyQlabel = new QLabel(parent);

    // nenhuma das duas opções resolve a questão do view ocupar toda a largura disponível
    QVBoxLayout* MyLayout = new QVBoxLayout(this);


 //    QVBoxLayout* MyLayout = new QVBoxLayout(parent);

//    MyQlabel->setText("Guilherme e Bruno sao legais");
//    MyLayout->addWidget(MyQlabel);
    ppointsmodel= new PointsModel;
    vector<string> StrVector;
    StrVector.push_back(XmlString);
    StrVector.push_back("UTM");
    ppointsmodel->fillvalues(StrVector);
    ppointsview= new PointsView(this,ppointsmodel);

    //gambiarra code!!!!
       // ppointsview->setFixedWidth(parent->width()-400);
	//Codigo removido pois ele gerava um crash na hora de construir um objeto que contivesse este objeto aqui.

    MyLayout->addWidget(ppointsview);

    MyProxyModel = new SinglePointModel;
    MyProxyModel->setDynamicSortFilter(true);
    MyProxyModel->setSourceModel( ppointsmodel);
    
    /*Rafael : Chirin Chirion para os singles,
    porém a tabela do coletivo é redimensionada para um tamanho menor do que antes
      
    psinglepointview2 = new SinglePointView(this, MyProxyModel);
    psinglepointview2->setToUTM();
    MyLayout->addWidget(psinglepointview2);
            
    */
    QWidget* MyButtonContainer = new QWidget(this);
    QHBoxLayout* MyHlayout = new QHBoxLayout(MyButtonContainer);
    QPushButton* MyAddButton = new QPushButton;
    MyAddButton->setText("Add");
    MyHlayout->addStretch();
    MyHlayout->addWidget(MyAddButton);
    QPushButton* MyRemoveButton = new QPushButton;
    MyRemoveButton->setText("Remove");
	MyHlayout->addWidget(MyRemoveButton);
	MyLayout->addWidget(MyButtonContainer);
	MyButtonContainer->setVisible(false);
    QAbstractItemView* AuxAbstractItemView= (QAbstractItemView*) ppointsview->getPointsTable();// ppointsview->pointslayout->PointsTable;
    QObject::connect(AuxAbstractItemView,SIGNAL(clicked (const QModelIndex &)),MyProxyModel,SLOT(selectRow(const QModelIndex &))  );
    QObject::connect(AuxAbstractItemView,SIGNAL(clicked (const QModelIndex &)),ppointsmodel,SLOT(selectRow(const QModelIndex &))  );
    QObject::connect(MyRemoveButton,SIGNAL(clicked(void)),ppointsmodel,SLOT(deleteSelectedRow(void)));
    QObject::connect(MyRemoveButton,SIGNAL(clicked(void)),psinglepointview2,SLOT(clean(void)));
    QObject::connect(AuxAbstractItemView,SIGNAL(clicked (const QModelIndex &)),psinglepointview2,SLOT(remap())  );
    QObject::connect(MyAddButton,SIGNAL(clicked(void)),ppointsmodel,SLOT(addpoint(void)));


    setLayout(MyLayout);
    /* Rafael: forçando o tamanho da tabela pra evitar o redimensonamento defeituoso*/
    this->setGeometry(0,0,700,500);

    QTableView* myTableView = ppointsview->getPointsTable();
    this->connect(myTableView, SIGNAL(clicked(QModelIndex)), this, SLOT(emitSignal(QModelIndex)));
}

/*PointsForm::PointsForm(QWidget *parent) : AbstractForm(parent)
{
//    QLabel *MyQlabel = new QLabel(parent);
    QVBoxLayout* MyLayout = new QVBoxLayout(this);

//    MyQlabel->setText("Guilherme e Bruno sao legais");
//    MyLayout->addWidget(MyQlabel);
    ppointsmodel= new PointsModel;
    ppointsview= new PointsView(this,ppointsmodel);

    //gambiarra code!!!!
    ppointsview->setFixedWidth(parent->width()-400);;

    MyLayout->addWidget(ppointsview);

    MyProxyModel = new SinglePointModel;
    MyProxyModel->setDynamicSortFilter(true);
    MyProxyModel->setSourceModel( ppointsmodel);
    psinglepointview2 = new SinglePointView(this, MyProxyModel);
    psinglepointview2->setToUTM();
    MyLayout->addWidget(psinglepointview2);


    QAbstractItemView* AuxAbstractItemView= (QAbstractItemView*) ppointsview->getPointsTable();// ppointsview->pointslayout->PointsTable;
    QObject::connect(AuxAbstractItemView,SIGNAL(clicked (const QModelIndex &)),MyProxyModel,SLOT(selectRow(const QModelIndex &))  );
    QObject::connect(AuxAbstractItemView,SIGNAL(clicked (const QModelIndex &)),ppointsmodel,SLOT(selectRow(const QModelIndex &))  );
    //QObject::connect(MyRemoveButton,SIGNAL(clicked(void)),ppointsmodel,SLOT(deleteSelectedRow(void)));
    //QObject::connect(MyRemoveButton,SIGNAL(clicked(void)),psinglepointview2,SLOT(clean(void)));
    QObject::connect(AuxAbstractItemView,SIGNAL(clicked (const QModelIndex &)),psinglepointview2,SLOT(remap())  );
    //QObject::connect(MyAddButton,SIGNAL(clicked(void)),ppointsmodel,SLOT(addpoint(void)));


    setLayout(MyLayout);

   // MyQlabel->show();

    // QApplication *app= new QApplication(argc, argv);

    string efotostring,auxstring;
//    ifstream dataefoto("../../uerj_project.epp");
//    while (!dataefoto.eof())
//    {
//        getline(dataefoto,auxstring);
//        efotostring += auxstring;
//    }
//    EDomElement auxede(efotostring);

   // string xmlimagepoints = auxede.elementByTagName("points").getContent();
   // string xmlunit = auxede.elementByTagName("CPS").toString();

    QMainWindow *MyPointsWindow = new QMainWindow;


    PointsModel* ppointsmodel= new PointsModel;
   // vector<string> StrVector;
   // StrVector.push_back(xmlimagepoints);
  //  StrVector.push_back(xmlunit);
  //  ppointsmodel->fillvalues(StrVector); // vai para fillvalues

    SinglePointModel* MyProxyModel = new SinglePointModel;
    MyProxyModel->setDynamicSortFilter(true);
    MyProxyModel->setSourceModel( ppointsmodel);
    PointsView* ppointsview= new PointsView(MyPointsWindow,ppointsmodel);

 //   PointsView* psinglepointview = new PointsView(MyPointsWindow,MyProxyModel );
 //   SinglePointView* psinglepointview2 = new SinglePointView(MyPointsWindow, MyProxyModel);
 //   psinglepointview2->setToUTM();

 //   QWidget* MyCentralWidget = new QWidget(MyPointsWindow);
    QVBoxLayout* MyLayout = new QVBoxLayout(MyPointsWindow);

    MyLayout->addWidget(ppointsview);
 //   MyLayout->addWidget(psinglepointview);
 //   MyLayout->addWidget(psinglepointview2);
 //   QPushButton* MyAddButton = new QPushButton;
 //   MyAddButton->setText("Add");
 //   MyLayout->addWidget(MyAddButton);
 //   QPushButton* MyRemoveButton = new QPushButton;
 //   MyRemoveButton->setText("Remove");
 //   MyLayout->addWidget(MyRemoveButton);
 //   MyCentralWidget->setLayout(MyLayout);
 //   MyPointsWindow->setCentralWidget (MyCentralWidget);
 //   QAbstractItemView* AuxAbstractItemView= (QAbstractItemView*) ppointsview->getPointsTable();// ppointsview->pointslayout->PointsTable;
 //   QObject::connect(AuxAbstractItemView,SIGNAL(clicked (const QModelIndex &)),MyProxyModel,SLOT(selectRow(const QModelIndex &))  );
 //   QObject::connect(AuxAbstractItemView,SIGNAL(clicked (const QModelIndex &)),ppointsmodel,SLOT(selectRow(const QModelIndex &))  );
 //   QObject::connect(MyRemoveButton,SIGNAL(clicked(void)),ppointsmodel,SLOT(deleteSelectedRow(void)));
 //   QObject::connect(MyRemoveButton,SIGNAL(clicked(void)),psinglepointview2,SLOT(clean(void)));
 //   QObject::connect(AuxAbstractItemView,SIGNAL(clicked (const QModelIndex &)),psinglepointview2,SLOT(remap())  );
 //   QObject::connect(MyAddButton,SIGNAL(clicked(void)),ppointsmodel,SLOT(addpoint(void)));
 //   MyPointsWindow->show(); 
 // setupUi(this);
}*/

void PointsForm::fillvalues(string values)
{
    vector<string> StrVector;
    StrVector.push_back(values);
    StrVector.push_back("UTM");
    ppointsmodel->fillvalues(StrVector);
    // gambiarra!!! No teste ficava ok, mas como aqui primeiro é lincado e depois é criado o view, as colunas voltam a aparecer.
    ppointsview->hideColumns();
    MyProxyModel->setSourceModel( ppointsmodel);
}

string PointsForm::getvalues()
{
    return ppointsmodel->getvalues();
}

void PointsForm::setReadOnly (bool state)
{
/*    spinBox->setReadOnly(state);
    lineEditDescription->setReadOnly(state);
    lineEdit_gml_pos->setReadOnly(state);
    lineEditSigma->setReadOnly(state);
    lineEditImageCoordinates1->setReadOnly(state);
    lineEditImageCoordinates2->setReadOnly(state);
*/
}

void PointsForm::emitSignal(QModelIndex myIndex)
{
    //int key=stringToInt(imagesTable->item(i,0)->text().toStdString().c_str());
    //emit clicked(key);
    // Provisóriamente nós enviamos a posição da linha clicada e não a key efetivamente.
    //imagesTable->setRangeSelected(QTableWidgetSelectionRange(i,0,i,2),true);

    emit clicked(myIndex.row());
}

bool PointsForm::isForm(string formName)
{

}
