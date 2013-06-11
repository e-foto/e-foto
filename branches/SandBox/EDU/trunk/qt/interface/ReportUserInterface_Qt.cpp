#include "ReportUserInterface_Qt.h"

#include <qapplication.h>
#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qaction.h>
#include <qstring.h>
#include <QRadioButton>
#include "ETableWidget.h"

#include <QTreeWidget>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

//#include <iostream>

ReportUserInterface_Qt* ReportUserInterface_Qt::ReportInst = NULL;

ReportUserInterface_Qt* ReportUserInterface_Qt::instance(ReportManager* manager)
{
        if (ReportInst = NULL)
	{
		delete ReportInst;
		ReportInst = NULL;
	}
	if (ReportInst == NULL)
	{
		ReportInst = new ReportUserInterface_Qt(manager);
	}
	return ReportInst;
}

 ReportUserInterface_Qt::ReportUserInterface_Qt(ReportManager* manager, QWidget* parent, Qt::WindowFlags fl)
	: QMainWindow(parent, fl)
{
        setupUi(this);
        
        this->manager = manager;

        //setWindowState(this->windowState() | Qt::WindowMaximized);
	qApp->processEvents();
	init();
}

ReportUserInterface_Qt::~ReportUserInterface_Qt()
{
	// no need to delete child widgets, Qt does it all for us
}

void ReportUserInterface_Qt::languageChange()
{
	retranslateUi(this);
}

void ReportUserInterface_Qt::init()
{
    newTree();    
    connect(generateButton,SIGNAL(clicked()),this,SLOT(saveEPR()));
}

void ReportUserInterface_Qt::closeEvent(QCloseEvent *e)
{
    LoadingScreen::instance().show();
	qApp->processEvents();
	manager->returnProject();
	QMainWindow::closeEvent(e);
}

void ReportUserInterface_Qt::newTree()
{	
        QTreeWidget *treeWidget = new QTreeWidget(this);

        connect(treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(selectAllChildren(QTreeWidgetItem*)));

        connect(treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(unselectFatherByKid(QTreeWidgetItem*)));

        treeWidget->clear();
        treeItems.clear();

        QTreeWidgetItem* item = new QTreeWidgetItem(treeWidget);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Select All");
        item->setExpanded(true);
        treeItems.push_back(item);
        QTreeWidgetItem* fatherTree = item;

        item = new QTreeWidgetItem(fatherTree);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Header");          
        treeItems.push_back(item);

        item = new QTreeWidgetItem(fatherTree);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Terrain");
        treeItems.push_back(item);

        item = new QTreeWidgetItem(fatherTree);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Sensors");
        treeItems.push_back(item);

        item = new QTreeWidgetItem(fatherTree);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Flights");
        treeItems.push_back(item);        	 

        item = new QTreeWidgetItem(fatherTree);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Images");
        treeItems.push_back(item);
        QTreeWidgetItem* n1RootItem = item;

        item = new QTreeWidgetItem(n1RootItem);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Metadata");
        treeItems.push_back(item);

        item = new QTreeWidgetItem(n1RootItem);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Orientation parameters");
        treeItems.push_back(item);

        item = new QTreeWidgetItem(fatherTree);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Points");
        treeItems.push_back(item);
        n1RootItem = item;

        item = new QTreeWidgetItem(n1RootItem);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Coordinates in Object Space Frame");
        treeItems.push_back(item);

        item = new QTreeWidgetItem(n1RootItem);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Coordinates in Image Space Frame");
        treeItems.push_back(item);

        item = new QTreeWidgetItem(fatherTree);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Affine Transformation");
        treeItems.push_back(item);        
        n1RootItem = item;

        item = new QTreeWidgetItem(n1RootItem);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Initialization data");
        treeItems.push_back(item);

        item = new QTreeWidgetItem(n1RootItem);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Quality data");
        treeItems.push_back(item);

        item = new QTreeWidgetItem(fatherTree);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Spatial Resection");
        treeItems.push_back(item);
        n1RootItem = item;

        item = new QTreeWidgetItem(n1RootItem);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Initialization data");
        treeItems.push_back(item);        

        item = new QTreeWidgetItem(n1RootItem);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Quality data");
        treeItems.push_back(item);

        item = new QTreeWidgetItem(n1RootItem);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Values at each iteration");
        treeItems.push_back(item);

        item = new QTreeWidgetItem(fatherTree);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Photogrammetric Block");
        treeItems.push_back(item);
        n1RootItem = item;

        item = new QTreeWidgetItem(n1RootItem);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Initialization data");
        treeItems.push_back(item);

        item = new QTreeWidgetItem(n1RootItem);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Values at each iteration");
        treeItems.push_back(item);

        item = new QTreeWidgetItem(fatherTree);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Interior Orientation");
        treeItems.push_back(item);
        n1RootItem = item;

        item = new QTreeWidgetItem(n1RootItem);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Xa");
        treeItems.push_back(item);

        item = new QTreeWidgetItem(n1RootItem);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Matrix V");
        treeItems.push_back(item);

        item = new QTreeWidgetItem(n1RootItem);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "SigmaXa");
        treeItems.push_back(item);        

        item = new QTreeWidgetItem(fatherTree);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Exterior Orientation");
        treeItems.push_back(item);
        n1RootItem = item;

        item = new QTreeWidgetItem(n1RootItem);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "EO Type");
        treeItems.push_back(item);

        item = new QTreeWidgetItem(n1RootItem);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Number of Iterations for Convergence");
        treeItems.push_back(item);

        item = new QTreeWidgetItem(n1RootItem);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Xa");
        treeItems.push_back(item);

        item = new QTreeWidgetItem(n1RootItem);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Matrix V");
        treeItems.push_back(item);

        item = new QTreeWidgetItem(n1RootItem);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "SigmaXa");
        treeItems.push_back(item);

        item = new QTreeWidgetItem(fatherTree);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Stereo Pairs");
        treeItems.push_back(item);

      /*  item = new QTreeWidgetItem(fatherTree);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Stereo Plotting");
        treeItems.push_back(item);

        item = new QTreeWidgetItem(fatherTree);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "DSM");
        treeItems.push_back(item);

        item = new QTreeWidgetItem(fatherTree);
        item->setCheckState(0,Qt::Checked);
        item->setText(0, "Orthorectification");
        treeItems.push_back(item);*/

    manager->checkTree(treeItems);

	treeWidget->setCurrentItem(treeItems.at(0));
	treeWidget->header()->hide();

    treeArea->setLayout(new QVBoxLayout());
	treeArea->layout()->addWidget(treeWidget);
	//processTreeClick(treeWidget->currentIndex());
}

bool ReportUserInterface_Qt::exec()
{
	this->show();
        LoadingScreen::instance().close();
	qApp->processEvents();
	return true;
}


void ReportUserInterface_Qt::selectAllChildren(QTreeWidgetItem* father)
{

	int numChild= father->childCount();

	if (father->checkState(0)==Qt::Checked)
	{
		for (int i=0;i<numChild;i++)
		{
			QTreeWidgetItem *aux=father->child(i);
            if(!aux->isDisabled())
                aux->setCheckState(0,Qt::Checked);
			if (aux->childCount()!=0)
			{
				selectAllChildren(aux);
			}
		}
	}
	else if (father->checkState(0)==Qt::Unchecked)
	{
		for (int i=0;i<numChild;i++)
		{
			QTreeWidgetItem *aux=father->child(i);
			aux->setCheckState(0,Qt::Unchecked);
			if (aux->childCount()!=0)
			{
				selectAllChildren(aux);
			}
		}
	}

}

void ReportUserInterface_Qt::unselectFatherByKid(QTreeWidgetItem* kid)
{
	QTreeWidgetItem* father = kid->parent();

	if (father==NULL)
		return;

    int numChild = father->childCount();
	int count_uncheck = 0;
	int count_check = 0;

	for (int i=0;i<numChild;i++)
	{
        if(!father->child(i)->isDisabled())
        {
            if(father->child(i)->checkState(0)==Qt::Unchecked)
            {
                ++count_uncheck;
            }
            else if(father->child(i)->checkState(0)==Qt::Checked)
            {
                ++count_check;
            }
            else if(father->child(i)->checkState(0)==Qt::PartiallyChecked)
            {
                father->setCheckState(0,Qt::PartiallyChecked);
            }
        }
	}

	if(count_uncheck > 0)
	{
		if(count_uncheck==numChild)
		{
			father->setCheckState(0,Qt::Unchecked);
		}
		else
		{
			father->setCheckState(0,Qt::PartiallyChecked);
		}
	}

	if(count_check > 0)
	{
		if(count_check==numChild)
		{
			father->setCheckState(0,Qt::Checked);

		}
	}
	unselectFatherByKid(father);

}

bool ReportUserInterface_Qt::saveEPR()
{
    QString* chosenExtension = new QString();
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File As"), ".",tr("*.xml;;*.txt;;*.html"),chosenExtension);
    QString filenameOriginal,filenameOriginalMask,filePathMask;
    int idExt = 0;
    if (filename == "")
    {
            return false;
    }
    else
    {        
        int j=filename.lastIndexOf("/");

        filenameOriginal = filename.right(filename.length()-j-1);

        if(!filename.endsWith(".xml"))
            filename.append(".xml");

        chosenExtension->remove('*');

        if (chosenExtension->toStdString() == ".xml"){
            idExt = 0;
        } else {
            if (chosenExtension->toStdString() == ".txt"){
                idExt = 1;
                filenameOriginalMask = filenameOriginal;
                filenameOriginalMask.prepend("~");
                filePathMask = filename.left(j);
                filename = filePathMask + "/" + filenameOriginalMask;
            } else {
                if (chosenExtension->toStdString() == ".html"){
                    idExt = 2;
                    filenameOriginalMask = filenameOriginal;
                    filenameOriginalMask.prepend("~");
                    filePathMask = filename.left(j);
                    filename = filePathMask + "/" + filenameOriginalMask;
                }
            }
        }
    }

    int i=filename.lastIndexOf("/");
    QString filePath = filename.left(i);

    bool done = manager->makeFile(filename.toStdString(),idExt,treeItems);
    bool doneXslt = false;

    if(idExt == 1)
    {
        doneXslt = manager->makeXslt(1,filePath.toStdString());
    } else {
        if(idExt == 2)
        {
            doneXslt = manager->makeXslt(2,filePath.toStdString());
        }
        else
        {
            doneXslt = true;
        }
    }

    if(done == true && doneXslt == true)
    {
        QProcess *pro = new QProcess();        
        if(idExt == 1)
        {            
            QString output;
            if(filenameOriginal.endsWith(".txt"))
                output = filePath + "/" + filenameOriginal;
            else
                output = filePath + "/" + filenameOriginal + ".txt";
            QString outxsl;
            outxsl = filePath + "/" + "epr_txt.xsl";
            QString outcmd;
            #ifdef Q_WS_X11 //LINUX
                outcmd = "xsltproc -o " + output + " " + filename + " " + outxsl;
                pro->start(outcmd);
                pro->waitForFinished(1000);
                pro->start("rm " + outxsl);
                pro->waitForFinished(1000);
                pro->start("rm " + filename);
            #endif
            #ifdef Q_WS_WIN //WINDOWS
                outcmd = "xsltproc -o \"" + output + "\" \"" + filename + "\" \"" + outxsl + "\"";
                pro->start(outcmd);
                pro->waitForFinished(1000);
                outxsl.replace("/","\\");
                filename.replace("/","\\");
                pro->start("cmd /C del \""+outxsl+"\"");
                pro->waitForFinished(1000);
                pro->start("cmd /C del \""+filename+"\"");
            #endif
        } else {
            if(idExt == 2){                   
                QString output;
                if(filenameOriginal.endsWith(".html"))
                    output = filePath + "/" + filenameOriginal;
                else
                    output = filePath + "/" + filenameOriginal + ".html";
                QString outxsl;
                outxsl = filePath + "/" + "epr_html.xsl";
                QString outcmd;                
                #ifdef Q_WS_X11 //LINUX
                    outcmd = "xsltproc -o " + output + " " + filename + " " + outxsl;
                    pro->start(outcmd);
                    pro->waitForFinished(1000);
                    pro->start("rm " + outxsl);
                    pro->waitForFinished(1000);
                    pro->start("rm " + filename);
                #endif
                #ifdef Q_WS_WIN //WINDOWS
                    outcmd = "xsltproc -o \"" + output + "\" \"" + filename + "\" \"" + outxsl + "\"";
                    pro->start(outcmd);
                    pro->waitForFinished(1000);
                    outxsl.replace("/","\\");
                    filename.replace("/","\\");
                    pro->start("cmd /C del \""+outxsl+"\"");
                    pro->waitForFinished(1000);
                    pro->start("cmd /C del \""+filename+"\"");
                #endif
            }
        }
        pro->kill();
/*
        // Removed
        QMessageBox msgBox;
        msgBox.setText("Foi salvo com sucesso.");
        msgBox.exec();
*/
        return true;
    }
    else
    {
        return false;
    }
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
