#include "ProjectUserInterface_Qt.h"

#include <qapplication.h>
#include <qvariant.h>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QLabel>
#include <QShortcut>

#include "EDomValidator.h"



ProjectUserInterface_Qt::ProjectUserInterface_Qt(ProjectManager* manager, QWidget* parent, Qt::WindowFlags fl)
	: QMainWindow(parent, fl)
{
	setupUi(this);

	//  Inicia variáveis
	this->manager = manager;
	this->currentForm = NULL;
	this->currentItemId = 0;
	this->savedIn = "";
	this->editState = false;
	this->addNewState = false;
	this->treeItems.clear();
	menuExecute->setEnabled(false);
	actionSave_file->setEnabled(false);
	actionSave_file_as->setEnabled(false);
	setWindowTitle(tr("efoto[Project Manager]"));

	// Realiza as conexões necessárias
	this->connect(actionNew, SIGNAL(triggered()), this, SLOT(newProject()));
	this->connect(actionLoad_file, SIGNAL(triggered()), this, SLOT(loadFile()));
	this->connect(actionSave_file, SIGNAL(triggered()), this, SLOT(saveFile()));
	this->connect(actionSave_file_as, SIGNAL(triggered()), this, SLOT(saveFileAs()));
	this->connect(actionInterior_Orientation, SIGNAL(triggered()), this, SLOT(executeIO()));
	this->connect(actionSpatial_resection, SIGNAL(triggered()), this, SLOT(executeSR()));
	this->connect(actionExport_Stereo, SIGNAL(triggered()), this, SLOT(exportSPFile()));
	this->connect(actionAbout,SIGNAL(triggered()), this, SLOT(showAbout()));
	this->connect(treeWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(processTreeClick(QModelIndex)));
	this->connect(&imagesForm, SIGNAL(clicked(int)), this, SLOT(selectImage(int)));
	this->connect(&pointsForm, SIGNAL(clicked(int)), this, SLOT(selectPoint(int)));
	this->connect(imageForm.imageIDLine, SIGNAL(editingFinished()), this , SLOT( validatingImage()) );
	this->connect(imageForm.fileNameLine, SIGNAL(textChanged(QString)),this , SLOT( validatingImage()) );
	this->connect(pointForm.lineEdit_gcp_id, SIGNAL(editingFinished()), this, SLOT( validatingPoint()) );
	this->connect(pointForm.sigmaController, SIGNAL(validateChanged()), this, SLOT(validatingPoint()) );
	this->connect(imageForm.gnssSigmaController, SIGNAL(validateChanged()), this, SLOT(validatingImage()) );
	this->connect(imageForm.insSigmaController, SIGNAL(validateChanged()), this, SLOT(validatingImage()) );
	this->connect(flightForm.flightIDLineEdit, SIGNAL(editingFinished()), this , SLOT( validatingFlight()) );
	this->connect(sensorForm.sensorIdLineEdit, SIGNAL(editingFinished()), this , SLOT( validatingSensor()) );
	this->connect(sensorForm.fid0SigmaController, SIGNAL(validateChanged()), this, SLOT(validatingSensor()) );
	this->connect(sensorForm.fid1SigmaController, SIGNAL(validateChanged()), this, SLOT(validatingSensor()) );
	this->connect(sensorForm.fid2SigmaController, SIGNAL(validateChanged()), this, SLOT(validatingSensor()) );
	this->connect(sensorForm.fid3SigmaController, SIGNAL(validateChanged()), this, SLOT(validatingSensor()) );
	this->connect(sensorForm.fid4SigmaController, SIGNAL(validateChanged()), this, SLOT(validatingSensor()) );
	this->connect(sensorForm.fid5SigmaController, SIGNAL(validateChanged()), this, SLOT(validatingSensor()) );
	this->connect(sensorForm.fid6SigmaController, SIGNAL(validateChanged()), this, SLOT(validatingSensor()) );
	this->connect(sensorForm.fid7SigmaController, SIGNAL(validateChanged()), this, SLOT(validatingSensor()) );
	this->connect(sensorForm.calibratedSigmaController, SIGNAL(validateChanged()), this, SLOT(validatingSensor()) );
	this->connect(sensorForm.radialSigmaController, SIGNAL(validateChanged()), this, SLOT(validatingSensor()) );
	this->connect(sensorForm.decenteredSigmaController, SIGNAL(validateChanged()), this, SLOT(validatingSensor()) );
	this->connect(sensorForm.principalSigmaController, SIGNAL(validateChanged()), this, SLOT(validatingSensor()) );


	// Bloqueia alguns dos  dipositivos
	this->showMaximized();
	this->removeDockWidget(debuggerDockWidget);
	this->removeDockWidget(projectDockWidget);

	// Coloca s forms na QStackedWidget
	QScrollArea *headerArea = new QScrollArea;
	headerArea->setWidget(&headerForm);
	headerArea->setAlignment(Qt::AlignHCenter);
	centerArea.addWidget(headerArea);
	QScrollArea *terrainArea = new QScrollArea;
	terrainArea->setWidget(&terrainForm);
	terrainArea->setAlignment(Qt::AlignHCenter);
	centerArea.addWidget(terrainArea);
	QScrollArea *sensorArea = new QScrollArea;
	sensorArea->setWidget(&sensorForm);
	sensorArea->setAlignment(Qt::AlignHCenter);
	centerArea.addWidget(sensorArea);
	QScrollArea *flightArea = new QScrollArea;
	flightArea->setWidget(&flightForm);
	flightArea->setAlignment(Qt::AlignHCenter);
	centerArea.addWidget(flightArea);
	QScrollArea *imagesArea = new QScrollArea;
	imagesArea->setWidget(&imagesForm);
	imagesArea->setAlignment(Qt::AlignHCenter);
	centerArea.addWidget(imagesArea);
	QScrollArea *imageArea = new QScrollArea;
	imageArea->setWidget(&imageForm);
	imageArea->setAlignment(Qt::AlignHCenter);
	centerArea.addWidget(imageArea);
	QScrollArea *pointsArea = new QScrollArea;
	pointsArea->setWidget(&pointsForm);
	pointsArea->setAlignment(Qt::AlignHCenter);
	centerArea.addWidget(pointsArea);
	QScrollArea *pointArea = new QScrollArea;
	pointArea->setWidget(&pointForm);
	pointArea->setAlignment(Qt::AlignHCenter);
	centerArea.addWidget(pointArea);

	offset.setVisible(false);
	centerArea.setVisible(false);
	controlButtons.setVisible(false);
	centralWidget()->layout()->addWidget(&offset);
	centralWidget()->layout()->addWidget(&centerArea);
	centralWidget()->layout()->addWidget(&controlButtons);

	// Adiciona um atalho para os desenvolvedores observarem as mudanças no XML durante o runtime
	QShortcut* shortcut = new QShortcut(QKeySequence(tr("Ctrl+Shift+D", "Debug")),this);
	connect(shortcut, SIGNAL(activated()), this, SLOT(toggleDebug()));
}

ProjectUserInterface_Qt::~ProjectUserInterface_Qt()
{
	// no need to delete child widgets, Qt does it all for us
}

void ProjectUserInterface_Qt::closeEvent(QCloseEvent *event)
{
	/*
	if (editState || addNewState)
	{
		if (controlButtons.saveButton->isEnabled())
		{
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this, tr(" Warning: leaving form in edit mode"),
										  "Do you want to keep all changes?",
										  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
			if (reply == QMessageBox::Yes)
			{
				controlButtons.saveButton->click();
			}
			else if (reply == QMessageBox::No)
			{
				controlButtons.cancelButton->click();
			}
			else
			{
				event->ignore();
				return;
			}
		}
		else
		{
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this, tr(" Warning: leaving form in edit mode"),
										  "Registration data is not complete and will be lost. Continue?",
										  QMessageBox::Yes | QMessageBox::Cancel);
			if (reply == QMessageBox::Yes)
			{
				controlButtons.cancelButton->click();
			}
			else
			{
				event->ignore();
				return;
			}
		}
	}
	if (actionSave_file->isEnabled())
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, tr(" Warning: You have unsaved data"),
									  tr("Do you want to save all changes?"),
									  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if (reply == QMessageBox::Yes)
		{
			actionSave_file->trigger();
		}
		else if (reply == QMessageBox::Cancel)
		{
			event->ignore();
			return;
		}
	}
	*/
	if (confirmToClose())
		event->accept();
	else
		event->ignore();
}

void ProjectUserInterface_Qt::newProject()
{
	if (!confirmToClose())
		return;

	if (!saveFileAs(true))
		return;

	addDockWidget(Qt::LeftDockWidgetArea,projectDockWidget);
	addDockWidget(Qt::BottomDockWidgetArea,debuggerDockWidget);
	setCorner(Qt::TopLeftCorner,Qt::LeftDockWidgetArea);
	setCorner(Qt::TopRightCorner,Qt::RightDockWidgetArea);
	projectDockWidget->setVisible(true);
	introWidget->setVisible(false);
	centerArea.setVisible(true);
	controlButtons.setVisible(true);
	offset.setVisible(true);

	manager->newProject(savedIn);
	//savedIn = "";
	actionSave_file->setEnabled(true);
	actionSave_file_as->setEnabled(true);
	//viewHeader();
	newTree();

	//Os comandos a seguir so serao uteis enquanto o projeto ficar restrito a apenas um sensor e um flight

	string text = "";

	text += "<sensor key=\"1\">\n";
	text +=	"\t<sensorId></sensorId>\n";
	text +=	"\t<type>\n";
	text +=	"\t\t<geometry>frame</geometry>\n";
	text +=	"\t\t<platform>aerial</platform>\n";
	text +=	"\t\t<detector>film</detector>\n";
	text +=	"\t\t<energySource>natural</energySource>\n";
	text +=	"\t\t<spectralRanges uom=\"#um\">\n";
	//text +=	"\t\t\t<spectralRange band=\"\">\n";
	//text +=	"\t\t\t\t<inferiorLimit></inferiorLimit>\n";
	//text +=	"\t\t\t\t<superiorLimit></superiorLimit>\n";
	//text +=	"\t\t\t</spectralRange>\n";
	text +=	"\t\t</spectralRanges>\n";
	text +=	"\t</type>\n";
	text +=	"\t<description></description>\n";
	text +=	"\t<calibrationCertificate>\n";
	text +=	"\t\t<number></number>\n";
	text +=	"\t\t<dispatch></dispatch>\n";
	text +=	"\t\t<expiration></expiration>\n";
	text +=	"\t</calibrationCertificate>\n";
	text +=	"\t<focalDistance uom=\"#mm\">\n";
	text +=	"\t\t<value></value>\n";
	text +=	"\t\t<sigma></sigma>\n";
	text +=	"\t</focalDistance>\n";
	text +=	"\t<distortionCoefficients>\n";
	text +=	"\t\t<radialSymmetric>\n";
	text +=	"\t\t\t<k0>\n";
	text +=	"\t\t\t\t<value></value>\n";
	text += "\t\t\t\t<sigma></sigma>\n";
	text +=	"\t\t\t</k0>\n";
	text +=	"\t\t\t<k1>\n";
	text +=	"\t\t\t\t<value></value>\n";
	text += "\t\t\t\t<sigma></sigma>\n";
	text +=	"\t\t\t</k1>\n";
	text +=	"\t\t\t<k2>\n";
	text +=	"\t\t\t\t<value></value>\n";
	text += "\t\t\t\t<sigma></sigma>\n";
	text +=	"\t\t\t</k2>\n";
	text +=	"\t\t\t<k3>\n";
	text +=	"\t\t\t\t<value></value>\n";
	text += "\t\t\t\t<sigma></sigma>\n";
	text +=	"\t\t\t</k3>\n";
	text +=	"\t\t</radialSymmetric>\n";
	text +=	"\t\t<decentered>\n";
	text +=	"\t\t\t<P1>\n";
	text +=	"\t\t\t\t<value></value>\n";
	text += "\t\t\t\t<sigma></sigma>\n";
	text +=	"\t\t\t</P1>\n";
	text +=	"\t\t\t<P2>\n";
	text +=	"\t\t\t\t<value></value>\n";
	text += "\t\t\t\t<sigma></sigma>\n";
	text +=	"\t\t\t</P2>\n";
	text +=	"\t\t</decentered>\n";
	text +=	"\t</distortionCoefficients>\n";
	text +=	"\t<principalPointCoordinates uom=\"#mm\">\n";
	text +=	"\t\t<gml:pos></gml:pos>\n";
	text +=	"\t\t<sigma>\n";
	//text +=	"\t\t\t<mml:matrix>\n";
	//text +=	"\t\t\t\t<mml:matrixrow>\n";
	//text +=	"\t\t\t\t\t<mml:cn></mml:cn>\n";
	//text +=	"\t\t\t\t\t<mml:cn></mml:cn>\n";
	//text +=	"\t\t\t\t</mml:matrixrow>\n";
	//text +=	"\t\t\t\t<mml:matrixrow>\n";
	//text +=	"\t\t\t\t\t<mml:cn></mml:cn>\n";
	//text +=	"\t\t\t\t\t<mml:cn></mml:cn>\n";
	//text +=	"\t\t\t\t</mml:matrixrow>\n";
	//text +=	"\t\t\t</mml:matrix>\n";
	text +=	"\t\t</sigma>\n";
	text +=	"\t</principalPointCoordinates>\n";
	text +=	"\t<fiductialMarks uom=\"#mm\">\n";
	text +=	"\t\t<fiductialMark key=\"1\">\n";
	text +=	"\t\t\t<gml:pos></gml:pos>\n";
	text +=	"\t\t\t<sigma></sigma>\n";
	text +=	"\t\t</fiductialMark>\n";
	text +=	"\t\t<fiductialMark key=\"2\">\n";
	text +=	"\t\t\t<gml:pos></gml:pos>\n";
	text +=	"\t\t\t<sigma></sigma>\n";
	text +=	"\t\t</fiductialMark>\n";
	text +=	"\t\t<fiductialMark key=\"3\">\n";
	text +=	"\t\t\t<gml:pos></gml:pos>\n";
	text +=	"\t\t\t<sigma></sigma>\n";
	text +=	"\t\t</fiductialMark>\n";
	text +=	"\t\t<fiductialMark key=\"4\">\n";
	text +=	"\t\t\t<gml:pos></gml:pos>\n";
	text +=	"\t\t\t<sigma></sigma>\n";
	text +=	"\t\t</fiductialMark>\n";
	text +=	"\t</fiductialMarks>\n";
	text += "</sensor>";

	manager->addComponent(text, "sensors");

	text = "";

	text += "<flight key=\"1\" sensor_key=\"1\">\n";
	text += "\t<flightId></flightId>\n";
	text += "\t<description></description>\n";
	text += "\t<execution></execution>\n";
	text += "\t<producerName></producerName>\n";
	text += "\t<nominalScale>\n";
	text += "\t\t<mml:mfrac>\n";
	text += "\t\t\t<mml:mn>1</mml:mn>\n";
	text += "\t\t\t<mml:mn>10000</mml:mn>\n";
	text += "\t\t</mml:mfrac>\n";
	text += "\t</nominalScale>\n";
	text += "\t<flightHeight uom=\"#m\"></flightHeight>\n";
	text += "\t<overlap>\n";
	text += "\t\t<longitudinal uom=\"#%\">60</longitudinal>\n";
	text += "\t\t<transversal uom=\"#%\">20</transversal>\n";
	text += "\t</overlap>\n";
	text += "</flight>";

	manager->addComponent(text, "flights");

	// Estas linhas fazem parte realmente deste código e não são parte da gambiarra a cima
	saveFile();
	controlButtons.editButton->click();
	//treeWidget->setCurrentItem(treeItems.at(0));
	//processTreeClick(treeWidget->currentIndex());
}

void ProjectUserInterface_Qt::loadFile(string filenameAtStart)
{
	if (!confirmToClose())
		return;

	/*
	if (editState || addNewState)
	{
		if (controlButtons.saveButton->isEnabled())
		{
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this, tr(" Warning: leaving form in edit mode"),
										  "Do you want to keep all changes?",
										  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
			if (reply == QMessageBox::Yes)
			{
				controlButtons.saveButton->click();
			}
			else if (reply == QMessageBox::No)
			{
				controlButtons.cancelButton->click();
			}
			else
			{
				return;
			}
		}
		else
		{
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this, tr(" Warning: leaving form in edit mode"),
										  "Registration data is not complete and will be lost. Continue?",
										  QMessageBox::Yes | QMessageBox::Cancel);
			if (reply == QMessageBox::Yes)
			{
				controlButtons.cancelButton->click();
			}
			else
			{
				return;
			}
		}
	}
	if (actionSave_file->isEnabled())
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, tr(" Warning: You have unsaved data"),
									  tr("Do you want to save all changes?"),
									  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if (reply == QMessageBox::Yes)
		{
			actionSave_file->trigger();
		}
		else if (reply == QMessageBox::Cancel)
		{
			return;
		}
	}
	*/

	QString filename;
	if (filenameAtStart == "")
	{
		filename = QFileDialog::getOpenFileName(this, "Open File", ".", "*.epp");
	}
	else
	{
		filename = QString(filenameAtStart.c_str());
	}

	if (filename == "")
		return;
	else
	{
		if (manager->loadFile(filename.toStdString()))
		{
			if (!manager->testFileVersion())
			{
				QMessageBox* alert = new QMessageBox(QMessageBox::Warning,"Unable to open file","The e-foto software was unable to open the selected file.\nThis may be due to:\n\n - Unsupported file version;\n - The file is not a valid .epp (e-foto Photogrammetric Project) file;\n - A bug in the program.\n\nTry changing the file or version of the software and try again.");
				alert->show();
				return;
			}
			QString imagesMissing="";

			for (int i=0 ;i < manager->getFreeImageId() ;i++)
			{
				EDomElement img(manager->getXml("image","key",QString::number(i).toStdString().c_str()));

				QString imagesName(img.elementByTagName("filePath").toString().append("/").c_str());
				imagesName.append(img.elementByTagName("fileName").toString().c_str());
				QFileInfo image(imagesName);
				if (!image.exists())
				{
					imagesMissing.append(image.fileName());
					imagesMissing.append("\n");
				}
			}
			if(imagesMissing.compare("")!=0)
			{

				QMessageBox* alertImages= new QMessageBox(QMessageBox::Warning,"Images missing",imagesMissing);
				alertImages->show();
			}

			addDockWidget(Qt::LeftDockWidgetArea,projectDockWidget);
			addDockWidget(Qt::BottomDockWidgetArea,debuggerDockWidget);
			setCorner(Qt::TopLeftCorner,Qt::LeftDockWidgetArea);
			setCorner(Qt::TopRightCorner,Qt::RightDockWidgetArea);
			projectDockWidget->setVisible(true);
			introWidget->setVisible(false);
			centerArea.setVisible(true);
			controlButtons.setVisible(true);
			offset.setVisible(true);

			savedIn = filename.toStdString();
			actionSave_file->setEnabled(false);
			actionSave_file_as->setEnabled(true);

			//***************************************************************************************************
			// Este tratamento pode precisar de ajustes para cumprir o requisito do e-foto de ser CrossPlataform
			int i=filename.lastIndexOf("/");

			QString fileName = "<fileName>"+filename.right(filename.length()-i-1)+"</fileName>";
			QString filePath = "<filePath>"+filename.left(i)+"</filePath>";

			EDomElement node(manager->getXml("projectHeader"));

			node.replaceChildByTagName("fileName",fileName.toStdString());
			node.replaceChildByTagName("filePath",filePath.toStdString());

			manager->editComponent("Header", node.getContent());
			//***************************************************************************************************

			//viewHeader();
			newTree();
		}
		else
		{
			QMessageBox* alert = new QMessageBox(QMessageBox::Warning,"Unable to open file","The e-foto software was unable to open the selected file.\nThis may be due to:\n\n - Unsupported characters in the file's name or path (maybe accented characters or whitespace);\n - The file does not exist;\n - A bug in the program.\n\nTry changing the file's name or path and try again.");
			alert->show();
		}
	}
}

void ProjectUserInterface_Qt::saveFile()
{
	if (!savedIn.empty())
	{
		QDateTime dateTimeBackup = headerForm.dateTimeEditModificationDate->dateTime();
		//headerForm.dateTimeEditModificationDate->setTime(QTime::currentTime());
		headerForm.dateTimeEditModificationDate->setDateTime(QDateTime::currentDateTime());
		manager->editComponent("Header", headerForm.getvalues());
		if (manager->saveFile(savedIn))
		{
			actionSave_file->setEnabled(false);
		}
		else
		{
			headerForm.dateTimeEditModificationDate->setDateTime(dateTimeBackup);
			manager->editComponent("Header", headerForm.getvalues());
			QMessageBox* alert = new QMessageBox(QMessageBox::Warning,"Unable to save file", "The e-foto software was unable to save the file.\nThis may be due to:\n\n - The disk does not have enought free space;\n - You do not have the needed permissions;\n - A bug in the program.\n\nCheck your disk space and permissions and try again.");
			alert->show();
		}
		if (centerArea.currentIndex() == 0)
			viewHeader();
		updateTree();
	}
	else
		saveFileAs(); // Isso já não executa nunca mais. Eu acho!
}

bool ProjectUserInterface_Qt::saveFileAs(bool onNewProject)
{
	QString filename;

	if (onNewProject)
		filename = QFileDialog::getSaveFileName(this, tr("Save A New File"), ".", "*.epp");
	else
		filename = QFileDialog::getSaveFileName(this, tr("Save File As"), ".", "*.epp");

	if (filename == "")
		return false;
	else
	{
		//***************************************************************************************************
		// Este tratamento pode precisar de ajustes para cumprir o requisito do e-foto de ser CrossPlataform
		QString extension = filename.right(4);
		if (extension != ".epp")
			filename.append(".epp");
		int i=filename.lastIndexOf("/");

		QString fileName = filename.right(filename.length()-i-1);
		QString filePath = filename.left(i);

		QString fileNameBackup = headerForm.lineEditFileName->text();
		QString filePathBackup = headerForm.lineEditFilePath->text();
		QDateTime dateTimeCreatBackup = headerForm.dateTimeEditCreationDate->dateTime();
		QDateTime dateTimeModifBackup = headerForm.dateTimeEditModificationDate->dateTime();

		headerForm.lineEditFilePath->setText(filePath);
		headerForm.lineEditFileName->setText(fileName);
		headerForm.dateTimeEditCreationDate->setDateTime(QDateTime::currentDateTime());
		headerForm.dateTimeEditModificationDate->setDateTime(QDateTime::currentDateTime());

		manager->editComponent("Header", headerForm.getvalues());
		//***************************************************************************************************

		if (manager->saveFile(filename.toStdString()))
		{
			savedIn = filename.toStdString();
			actionSave_file->setEnabled(false);
		}
		else
		{
			headerForm.lineEditFilePath->setText(filePathBackup);
			headerForm.lineEditFileName->setText(fileNameBackup);
			headerForm.dateTimeEditCreationDate->setDateTime(dateTimeCreatBackup);
			headerForm.dateTimeEditModificationDate->setDateTime(dateTimeModifBackup);
			manager->editComponent("Header", headerForm.getvalues());

			QMessageBox* alert = new QMessageBox(QMessageBox::Warning,"Unable to save file", "The e-foto software was unable to save the file.\nThis may be due to:\n\n - The disk does not have enought free space;\n - You do not have the needed permissions;\n - The file's name or path is invalid (maybe accented characters or whitespace);\n - A bug in the program.\n\nCheck your disk space and permissions and try again.");
			alert->show();
			return false;
		}
		if (centerArea.currentIndex() == 0)
			viewHeader();
		updateTree();
	}
	return true;
}

void ProjectUserInterface_Qt::executeIO()
{
	bool ok;
	QStringList items;
	deque<string> strItems = manager->listImages();
	for (unsigned int i = 0; i < strItems.size(); i++)
		items << strItems.at(i).c_str();
	QString chosen = QInputDialog::getItem(this, tr("Select your image!"), tr("Image name:"), items, 0, false, &ok);
	if (ok)
	{
		int value = manager->getImageId(chosen.toStdString());
		if (value != -1)
		{
			manager->startModule("InteriorOrientation", value);
			this->close();
		}
	}
}

void ProjectUserInterface_Qt::executeSR()
{
	bool ok;
	QStringList items;
	deque<string> strItems = manager->listImages();
	for (unsigned int i = 0; i < strItems.size(); i++)
		items << strItems.at(i).c_str();
	QString chosen = QInputDialog::getItem(this, tr("Select your image!"), tr("Image name:"), items, 0, false, &ok);
	if (ok)
	{
		int value = manager->getImageId(chosen.toStdString());
		if (value != -1)
		{
			manager->startModule("SpatialRessection", value);
			this->close();
		}
	}
}

void ProjectUserInterface_Qt::processTreeClick(QModelIndex index)
{
    if ((editState && index!=currentIndex)||addNewState)
    {
        if (controlButtons.saveButton->isEnabled())
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, tr(" Warning: leaving form in edit mode"),
                                          "Do you want to keep all changes?",
                                          QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if (reply == QMessageBox::Yes)
            {
                controlButtons.saveButton->click();
            }
            else if (reply == QMessageBox::No)
            {
                controlButtons.cancelButton->click();
            }
            else
            {
                treeWidget->setCurrentIndex(currentIndex);
                return;
            }
        }
        else
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, tr(" Warning: leaving form in edit mode"),
                                          "Registration data is not complete and will be lost. Continue?",
                                          QMessageBox::Yes | QMessageBox::Cancel);
            if (reply == QMessageBox::Yes)
            {
                controlButtons.cancelButton->click();
            }
            else
            {
                //if (addNewState)
				//  treeWidget->setCurrentIndex(0);
                //else
				treeWidget->setCurrentIndex(currentIndex);
                return;
            }
        }
    }
    currentIndex = index;
	ETreeModel* etm = manager->getTreeModel();
	if (index.parent() == QModelIndex())
	{
		if (etm->dataAt(index.row()) == "projectHeader")
		{
			viewHeader();
		}
		else if (etm->dataAt(index.row()) == "terrain")
		{
			viewTerrain();
		}
		else if (etm->dataAt(index.row()) == "sensors")
		{
			//viewSensors(); mudança temporaria
			viewSensor(1);
		}
		else if (etm->dataAt(index.row()) == "flights")
		{
			//viewFlights(); mudança temporaria
			viewFlight(1);
		}
		else if (etm->dataAt(index.row()) == "images")
		{
			viewImages();
		}
		else if (etm->dataAt(index.row()) == "points")
		{
			viewPoints();
		}
	}
	else
	{
		if (etm->dataAt(index.parent().row()) == "sensors")
		{
			//viewSensor(etm->idAt(index.parent().row(), index.row())); mudança temporaria
			viewSensor(1);
		}
		else if (etm->dataAt(index.parent().row()) == "flights")
		{
			//viewFlight(etm->idAt(index.parent().row(), index.row())); mudança temporaria
			viewFlight(1);
		}
		else if (etm->dataAt(index.parent().row()) == "images")
		{
			viewImage(etm->idAt(index.parent().row(), index.row()));
		}
		else if (etm->dataAt(index.parent().row()) == "points")
		{
			viewPoint(etm->idAt(index.parent().row(), index.row()));
		}
	}
}

void ProjectUserInterface_Qt::exportSPFile()
{
	bool ok;
	QStringList items;
	deque<string> strItems = manager->listImages();
	for (unsigned int i = 0; i < strItems.size(); i++)
		items << strItems.at(i).c_str();

	QString chosen1 = QInputDialog::getItem(this, tr("Select left image"), tr("Image name:"), items, 0, false, &ok);
	if (!ok) return;

	items.removeOne(chosen1);

	QString chosen2 = QInputDialog::getItem(this, tr("Select right image!"), tr("Image name:"), items, 0, false, &ok);
	if (!ok) return;

	QString filename = QFileDialog::getSaveFileName(this, "Save File", ".", "*.txt");

	int image1 = manager->getImageId(chosen1.toStdString());
	int image2 = manager->getImageId(chosen2.toStdString());
	if (image1 != -1 && image2 != -1)
	{
		bool result = manager->makeSPFile(filename.toStdString(), image1, image2);
		if (result == false)
		{
			QMessageBox msgBox;
			msgBox.setText("Error: invalid input parameters.");
			msgBox.exec();
		}
	}
}

//Metodo que sera reescrito depois que trabalharmos esta interface grafica com o QLinguist
void ProjectUserInterface_Qt::languageChange()
{
	//retranslateUi(this);
}

void ProjectUserInterface_Qt::newTree()
{
	treeWidget->clear();
	treeItems.clear();
	//this->treeWidget->setHeaderHidden(false);
	if (savedIn == "")
	{
		//treeWidget->setHeaderLabel(tr("New Project"));
		projectDockWidget->setWindowTitle(tr("Open Project: *Unsaved"));
	}
	else
	{
		//treeWidget->setHeaderLabel(headerForm.lineEditFileName->text());
		if (actionSave_file->isEnabled())
			projectDockWidget->setWindowTitle(QString(tr("Open Project: *")) + headerForm.lineEditFileName->text());
		else
			projectDockWidget->setWindowTitle(QString(tr("Open Project: ")) + headerForm.lineEditFileName->text());
	}
	if (headerForm.lineEditName->text() != "")
		setWindowTitle(headerForm.lineEditName->text() + tr(" - efoto[Project Manager]"));
	else
		setWindowTitle(tr("efoto[Project Manager]"));

	ETreeModel* etm = manager->getTreeModel();

	for (unsigned int i = 0; i < etm->countChildren(); i++)
	{
		QTreeWidgetItem* rootItem = new QTreeWidgetItem(treeWidget);
		treeItems.push_back(rootItem);
		if (etm->dataAt(i) == "projectHeader")
			rootItem->setText(0, tr("Project Header"));
		else if (etm->dataAt(i) == "terrain")
			rootItem->setText(0, tr("Terrain"));
		else if (etm->dataAt(i) == "flights")
			rootItem->setText(0, tr("Flight"));
		else if (etm->dataAt(i) == "sensors")
			rootItem->setText(0, tr("Sensor"));
		else if (etm->dataAt(i) == "images")
			rootItem->setText(0, tr("Images"));
		else if (etm->dataAt(i) == "points")
			rootItem->setText(0, tr("Points"));
		if (!(etm->dataAt(i) == "projectHeader" || etm->dataAt(i) == "terrain" || etm->dataAt(i) == "sensors" || etm->dataAt(i) == "flights"))
		{
			for (unsigned int j = 0; j < etm->countGrandchildren(i); j++)
			{
				QTreeWidgetItem* leafItem = new QTreeWidgetItem(rootItem);
				leafItem->setText(0, tr(etm->dataAt(i,j).c_str()));
			}
		}
	}
	treeWidget->setCurrentItem(treeItems.at(0));
	processTreeClick(treeWidget->currentIndex());
}

void ProjectUserInterface_Qt::updateTree()
{
    if (savedIn == "")
    {
        //treeWidget->setHeaderLabel(tr("New Project"));
        projectDockWidget->setWindowTitle(tr("Open Project: *Unsaved"));
    }
    else
    {
        //treeWidget->setHeaderLabel(headerForm.lineEditFileName->text());
        if (actionSave_file->isEnabled())
            projectDockWidget->setWindowTitle(QString(tr("Open Project: *")) + headerForm.lineEditFileName->text());
        else
            projectDockWidget->setWindowTitle(QString(tr("Open Project: ")) + headerForm.lineEditFileName->text());
    }
    if (headerForm.lineEditName->text() != "")
        setWindowTitle(headerForm.lineEditName->text() + tr(" - efoto[Project Manager]"));
    else
        setWindowTitle(tr("efoto[Project Manager]"));

	ETreeModel* etm = manager->getTreeModel();

	for (unsigned int i = 0; i < etm->countChildren(); i++)
	{
		if (i < treeItems.size())
		{
			if (etm->dataAt(i) == "projectHeader")
				treeItems.at(i)->setText(0, tr("Project Header"));
			else if (etm->dataAt(i) == "terrain")
				treeItems.at(i)->setText(0, tr("Terrain"));
			else if (etm->dataAt(i) == "flights")
				treeItems.at(i)->setText(0, tr("Flight"));
			else if (etm->dataAt(i) == "sensors")
				treeItems.at(i)->setText(0, tr("Sensor"));
			else if (etm->dataAt(i) == "images")
				treeItems.at(i)->setText(0, tr("Images"));
			else if (etm->dataAt(i) == "points")
				treeItems.at(i)->setText(0, tr("Points"));
			if (!(etm->dataAt(i) == "projectHeader" || etm->dataAt(i) == "terrain" || etm->dataAt(i) == "sensors" || etm->dataAt(i) == "flights"))
				for (unsigned int j = 0; j < etm->countGrandchildren(i); j++)
				{
				if (j < treeItems.at(i)->childCount())
				{
					treeItems.at(i)->child(j)->setText(0, tr(etm->dataAt(i,j).c_str()));
				}
				else
				{
					QTreeWidgetItem* leafItem = new QTreeWidgetItem(treeItems.at(i));
					leafItem->setText(0, tr(etm->dataAt(i,j).c_str()));
				}
			}
		}
		else
		{
			QTreeWidgetItem* rootItem = new QTreeWidgetItem(treeWidget);
			treeItems.push_back(rootItem);
			if (etm->dataAt(i) == "projectHeader")
				rootItem->setText(0, tr("Project Header"));
			else if (etm->dataAt(i) == "terrain")
				rootItem->setText(0, tr("Terrain"));
			else if (etm->dataAt(i) == "flights")
				rootItem->setText(0, tr("Flight"));
			else if (etm->dataAt(i) == "sensors")
				rootItem->setText(0, tr("Sensor"));
			else if (etm->dataAt(i) == "images")
				rootItem->setText(0, tr("Images"));
			else if (etm->dataAt(i) == "points")
				rootItem->setText(0, tr("Points"));
			if (!(etm->dataAt(i) == "projectHeader" || etm->dataAt(i) == "terrain" || etm->dataAt(i) == "sensors" || etm->dataAt(i) == "flights"))
			{
				for (unsigned int j = 0; j < etm->countGrandchildren(i); j++)
				{
					QTreeWidgetItem* leafItem = new QTreeWidgetItem(rootItem);
					leafItem->setText(0, tr(etm->dataAt(i,j).c_str()));
				}
			}
		}
	}
}

bool ProjectUserInterface_Qt::exec()
{
	this->show();
	if (qApp->exec())
		return false;
	return true;
}

void ProjectUserInterface_Qt::selectImage(int pos)
{
	treeWidget->setCurrentItem(treeItems.at(4)->child(pos));
	processTreeClick(treeWidget->currentIndex());
}

void ProjectUserInterface_Qt::selectPoint(int pos)
{
	treeWidget->setCurrentItem(treeItems.at(5)->child(pos));
	processTreeClick(treeWidget->currentIndex());
}

void ProjectUserInterface_Qt::viewHeader()
{
	controlButtons.disconnectAll();
	controlButtons.singleConnect();
	connect((&controlButtons)->editButton, SIGNAL(clicked()), this, SLOT(enableForm()));
	connect((&controlButtons)->saveButton, SIGNAL(clicked()), this, SLOT(saveHeader()));
	connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this, SLOT(cancelHeader()));

	EDomElement node(manager->getXml("projectHeader"));

	headerForm.fillvalues(node.getContent());
	centerArea.setStyleSheet("QScrollArea, QWidget {background: #FFFFFF} QScrollArea {border: 0px}");
	centerArea.setCurrentIndex(0);
	currentForm = &headerForm;
	currentForm->setReadOnly(true);

	debuggerTextEdit->clear();
	debuggerTextEdit->setText(QString::fromUtf8(node.indent("\t").c_str()));
	debuggerTextEdit->setReadOnly(true);

	menuProject->setEnabled(true);
	menuExecute->setEnabled(true);
	controlButtons.saveButton->setEnabled(true);
}

void ProjectUserInterface_Qt::viewTerrain()
{
	controlButtons.disconnectAll();
	controlButtons.singleConnect();
	connect((&controlButtons)->editButton, SIGNAL(clicked()), this, SLOT(enableForm()));
	connect((&controlButtons)->saveButton, SIGNAL(clicked()), this, SLOT(saveTerrain()));
	connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this, SLOT(cancelTerrain()));

	EDomElement node(manager->getXml("terrain"));

	terrainForm.fillvalues(node.getContent());
	centerArea.setStyleSheet("QScrollArea, QWidget {background: #FFFFFF} QScrollArea {border: 0px}");
	centerArea.setCurrentIndex(1);
	currentForm = &terrainForm;
	currentForm->setReadOnly(true);

	debuggerTextEdit->clear();
	debuggerTextEdit->setText(QString::fromUtf8(node.indent("\t").c_str()));
	debuggerTextEdit->setReadOnly(true);

	menuProject->setEnabled(true);
	menuExecute->setEnabled(true);
	controlButtons.saveButton->setEnabled(true);
}

void ProjectUserInterface_Qt::viewSensors()
{
	// Por enquanto este metodo esta fora de uso.
	/*
controlButtons.disconnectAll();
controlButtons.multiConnect();
connect((&controlButtons)->newButton, SIGNAL(clicked()), this, SLOT(newSensor()));
connect((&controlButtons)->saveButton, SIGNAL(clicked()), this, SLOT(saveNewSensor()));
connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this, SLOT(cancelSensors()));

EDomElement node(manager->getXml("sensors"));

sensorsForm.fillvalues(node.getContent());
centerArea.setStyleSheet("QScrollArea, QWidget {background: #FFFFFF} QScrollArea {border: 0px}");
centerArea.setCurrentIndex(X); // falta definir este indice
currentForm = &sensorsForm;
currentForm->setReadOnly(true);

debuggerTextEdit->clear();
debuggerTextEdit->setText(QString::fromUtf8(node.indent("\t").c_str()));
debuggerTextEdit->setReadOnly(true);

menuProject->setEnabled(true);
menuExecute->setEnabled(true);
*/
}

void ProjectUserInterface_Qt::viewSensor(int id)
{
	currentItemId = id;
	controlButtons.disconnectAll();
	controlButtons.singleConnect();
	connect((&controlButtons)->editButton, SIGNAL(clicked()), this, SLOT(enableForm()));
	connect((&controlButtons)->saveButton, SIGNAL(clicked()), this, SLOT(saveSensor()));
	connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this, SLOT(cancelSensor()));
	connect((&controlButtons)->deleteButton, SIGNAL(clicked()), this, SLOT(deleteSensor()));

	EDomElement node(manager->getXml("sensor", "key", intToString(id)));

	sensorForm.fillvalues(node.getContent());
	centerArea.setStyleSheet("QScrollArea, QWidget {background: #FFFFFF} QScrollArea {border: 0px}");
	centerArea.setCurrentIndex(2);
	currentForm = &sensorForm;
	currentForm->setReadOnly(true);
	validatingSensor();

	debuggerTextEdit->clear();
	debuggerTextEdit->setText(QString::fromUtf8(node.indent("\t").c_str()));
	debuggerTextEdit->setReadOnly(true);

	menuProject->setEnabled(true);
	menuExecute->setEnabled(true);
}

void ProjectUserInterface_Qt::viewFlights()
{
	// Por enquanto este metodo esta fora de uso.
	/*
controlButtons.disconnectAll();
controlButtons.multiConnect();
connect((&controlButtons)->newButton, SIGNAL(clicked()), this, SLOT(newFlight()));
connect((&controlButtons)->saveButton, SIGNAL(clicked()), this, SLOT(saveNewFlight()));
connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this, SLOT(cancelFlights()));

EDomElement node(manager->getXml("flights"));

flightsForm.fillvalues(node.getContent());
centerArea.setStyleSheet("QScrollArea, QWidget {background: #FFFFFF} QScrollArea {border: 0px}");
centerArea.setCurrentIndex(X); // falta definir este indice
currentForm = &flightsForm;
currentForm->setReadOnly(true);

debuggerTextEdit->clear();
debuggerTextEdit->setText(QString::fromUtf8(node.indent("\t").c_str()));
debuggerTextEdit->setReadOnly(true);

menuProject->setEnabled(true);
menuExecute->setEnabled(true);
*/
}

void ProjectUserInterface_Qt::viewFlight(int id)
{
	currentItemId = id;
	controlButtons.disconnectAll();
	controlButtons.singleConnect();
	connect((&controlButtons)->editButton, SIGNAL(clicked()), this, SLOT(enableForm()));
	connect((&controlButtons)->saveButton, SIGNAL(clicked()), this, SLOT(saveFlight()));
	connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this, SLOT(cancelFlight()));
	connect((&controlButtons)->deleteButton, SIGNAL(clicked()), this, SLOT(deleteFlight()));

	EDomElement node(manager->getXml("flight", "key", intToString(id)));

	flightForm.fillvalues(node.getContent());
	centerArea.setStyleSheet("QScrollArea, QWidget {background: #FFFFFF} QScrollArea {border: 0px}");
	centerArea.setCurrentIndex(3);
	currentForm = &flightForm;
	currentForm->setReadOnly(true);
	validatingFlight();

	debuggerTextEdit->clear();
	debuggerTextEdit->setText(QString::fromUtf8(node.indent("\t").c_str()));
	debuggerTextEdit->setReadOnly(true);

	menuProject->setEnabled(true);
	menuExecute->setEnabled(true);
}

void ProjectUserInterface_Qt::viewImages()
{
	controlButtons.disconnectAll();
	controlButtons.multiConnect();
	connect((&controlButtons)->newButton, SIGNAL(clicked()), this, SLOT(newImage()));
	connect((&controlButtons)->saveButton, SIGNAL(clicked()), this, SLOT(saveNewImage()));
	connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this, SLOT(cancelImages()));

	EDomElement node(manager->getXml("images"));

	imagesForm.fillvalues(node.getContent());
	centerArea.setStyleSheet("QScrollArea, QWidget {background: #FFFFFF} QScrollArea, QTableWidget {border: 0px}");
	centerArea.setCurrentIndex(4);
	currentForm = &imagesForm;
	currentForm->setReadOnly(true);

	debuggerTextEdit->clear();
	debuggerTextEdit->setText(QString::fromUtf8(node.indent("\t").c_str()));
	debuggerTextEdit->setReadOnly(true);

	menuProject->setEnabled(true);
	menuExecute->setEnabled(true);
}

void ProjectUserInterface_Qt::viewImage(int id)
{
	currentItemId = id;
	controlButtons.disconnectAll();
	controlButtons.singleConnect();
	connect((&controlButtons)->editButton, SIGNAL(clicked()), this, SLOT(enableForm()));
	connect((&controlButtons)->saveButton, SIGNAL(clicked()), this, SLOT(saveImage()));
	connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this, SLOT(cancelImage()));
	connect((&controlButtons)->deleteButton, SIGNAL(clicked()), this, SLOT(deleteImage()));

	EDomElement node(manager->getXml("image", "key", intToString(id)));

	imageForm.fillvalues(node.getContent());
	centerArea.setStyleSheet("QScrollArea, QWidget {background: #FFFFFF} QScrollArea {border: 0px}");
	centerArea.setCurrentIndex(5);
	currentForm = &imageForm;
	currentForm->setReadOnly(true);
	validatingImage();

	debuggerTextEdit->clear();
	debuggerTextEdit->setText(QString::fromUtf8(node.indent("\t").c_str()));
	debuggerTextEdit->setReadOnly(true);

	menuProject->setEnabled(true);
	menuExecute->setEnabled(true);
}

void ProjectUserInterface_Qt::viewPoints()
{
	controlButtons.disconnectAll();
	controlButtons.multiConnect();
	connect((&controlButtons)->newButton, SIGNAL(clicked()), this, SLOT(newPoint()));
	connect((&controlButtons)->saveButton, SIGNAL(clicked()), this, SLOT(saveNewPoint()));
	connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this, SLOT(cancelPoints()));

	EDomElement node(manager->getXml("points"));

	pointsForm.fillvalues(node.getContent());
	centerArea.setStyleSheet("QScrollArea, QWidget {background: #FFFFFF} QScrollArea, QTableWidget {border: 0px}");
	centerArea.setCurrentIndex(6);
	currentForm = &pointsForm;
	currentForm->setReadOnly(true);

	debuggerTextEdit->clear();
	debuggerTextEdit->setText(QString::fromUtf8(node.indent("\t").c_str()));
	debuggerTextEdit->setReadOnly(true);

	menuProject->setEnabled(true);
	menuExecute->setEnabled(true);
}

void ProjectUserInterface_Qt::viewPoint(int id)
{
	currentItemId = id;
	controlButtons.disconnectAll();
	controlButtons.singleConnect();
	connect((&controlButtons)->editButton, SIGNAL(clicked()), this, SLOT(enableForm()));
	connect((&controlButtons)->saveButton, SIGNAL(clicked()), this, SLOT(savePoint()));
	connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this, SLOT(cancelPoint()));
	connect((&controlButtons)->deleteButton, SIGNAL(clicked()), this, SLOT(deletePoint()));

	EDomElement node(manager->getXml("point", "key", intToString(id)));

	pointForm.setImageList(manager->listImageKeys(), manager->listImages());
	pointForm.fillvalues(node.getContent());
	centerArea.setStyleSheet("QScrollArea, QWidget {background: #FFFFFF} QScrollArea, QTableWidget {border: 0px}");
	centerArea.setCurrentIndex(7);
	currentForm = &pointForm;
	currentForm->setReadOnly(true);
	validatingPoint();

	debuggerTextEdit->clear();
	debuggerTextEdit->setText(QString::fromUtf8(node.indent("\t").c_str()));
	debuggerTextEdit->setReadOnly(true);

	menuProject->setEnabled(true);
	menuExecute->setEnabled(true);
}

// Habilitando cada view...
void ProjectUserInterface_Qt::enableForm()
{
	currentForm->setReadOnly(false);
	debuggerTextEdit->setReadOnly(false);
	centerArea.setStyleSheet(this->styleSheet());
	currentForm->setStyleSheet(this->styleSheet());
	menuProject->setEnabled(false);
	menuExecute->setEnabled(false);
	editState = true;
	currentForm->setFocus();
}

// Salvando cada view...
void ProjectUserInterface_Qt::saveHeader()
{
    editState = false;
	manager->editComponent("Header", headerForm.getvalues());
	viewHeader();
	actionSave_file->setEnabled(true);
	updateTree();
}

void ProjectUserInterface_Qt::saveTerrain()
{
    editState = false;
	manager->editComponent("Terrain", terrainForm.getvalues());
	viewTerrain();
	actionSave_file->setEnabled(true);
	updateTree();
}

void ProjectUserInterface_Qt::saveSensor()
{
    editState = false;
	manager->editComponent("Sensor", currentItemId, sensorForm.getvalues());
	viewSensor(currentItemId);
	actionSave_file->setEnabled(true);
	updateTree();
}

void ProjectUserInterface_Qt::saveFlight()
{
    editState = false;
	manager->editComponent("Flight", currentItemId, flightForm.getvalues());
	viewFlight(currentItemId);
	actionSave_file->setEnabled(true);
	updateTree();
}

void ProjectUserInterface_Qt::saveImage()
{
    editState = false;
	manager->editComponent("Image", currentItemId, imageForm.getvalues());
	viewImage(currentItemId);
	actionSave_file->setEnabled(true);
	updateTree();
}

void ProjectUserInterface_Qt::savePoint()
{
    editState = false;
	manager->editComponent("Point", currentItemId, pointForm.getvalues());
	viewPoint(currentItemId);
	actionSave_file->setEnabled(true);
	updateTree();
}

// Cancelando cada view...
void ProjectUserInterface_Qt::cancelHeader()
{
    editState = false;
	viewHeader();
}

void ProjectUserInterface_Qt::cancelTerrain()
{
    editState = false;
	viewTerrain();
}

void ProjectUserInterface_Qt::cancelSensor()
{
    editState = false;
	viewSensor(currentItemId);
}

void ProjectUserInterface_Qt::cancelFlight()
{
    editState = false;
	viewFlight(currentItemId);
}

void ProjectUserInterface_Qt::cancelImage()
{
    editState = false;
	viewImage(currentItemId);
}

void ProjectUserInterface_Qt::cancelPoint()
{
    editState = false;
	viewPoint(currentItemId);
}

void ProjectUserInterface_Qt::cancelSensors()
{
    addNewState = false;
	viewSensors();
}

void ProjectUserInterface_Qt::cancelFlights()
{
    addNewState = false;
	viewFlights();
}

void ProjectUserInterface_Qt::cancelImages()
{
    addNewState = false;
	viewImages();
}

void ProjectUserInterface_Qt::cancelPoints()
{
    addNewState = false;
	viewPoints();
}

// Criando um vazio de cada view...

void ProjectUserInterface_Qt::newSensor()
{
	// Por enquanto este metodo esta fora de uso.
	/*
string text = "";

text += "<sensor key=\"\">\n";
text +=	"\t<sensorId></sensorId>\n";
text +=	"\t<type>\n";
text +=	"\t\t<geometry></geometry>\n";
text +=	"\t\t<platform></platform>\n";
text +=	"\t\t<detector></detector>\n";
text +=	"\t\t<energySource></energySource>\n";
text +=	"\t\t<spectralRanges uom=\"#um\">\n";
text +=	"\t\t\t<spectralRange band=\"\">\n";
text +=	"\t\t\t\t<inferiorLimit></inferiorLimit>\n";
text +=	"\t\t\t\t<superiorLimit></superiorLimit>\n";
text +=	"\t\t\t</spectralRange>\n";
text +=	"\t\t</spectralRanges>\n";
text +=	"\t</type>\n";
text +=	"\t<description></description>\n";
text +=	"\t<calibrationCertificate>\n";
text +=	"\t\t<number></number>\n";
text +=	"\t\t<dispatch></dispatch>\n";
text +=	"\t\t<expiration></expiration>\n";
text +=	"\t</calibrationCertificate>\n";
text +=	"\t<focalDistance uom=\"#mm\">\n";
text +=	"\t\t<value></value>\n";
text +=	"\t\t<sigma></sigma>\n";
text +=	"\t</focalDistance>\n";
text +=	"\t<distortionCoefficients>\n";
text +=	"\t\t<radialSymmetric>\n";
text +=	"\t\t\t<k0>\n";
text +=	"\t\t\t\t<value></value>\n";
text += "\t\t\t\t<sigma></sigma>\n";
text +=	"\t\t\t</k0>\n";
text +=	"\t\t\t<k1>\n";
text +=	"\t\t\t\t<value></value>\n";
text += "\t\t\t\t<sigma></sigma>\n";
text +=	"\t\t\t</k1>\n";
text +=	"\t\t\t<k2>\n";
text +=	"\t\t\t\t<value></value>\n";
text += "\t\t\t\t<sigma></sigma>\n";
text +=	"\t\t\t</k2>\n";
text +=	"\t\t\t<k3>\n";
text +=	"\t\t\t\t<value></value>\n";
text += "\t\t\t\t<sigma></sigma>\n";
text +=	"\t\t\t</k3>\n";
text +=	"\t\t</radialSymmetric>\n";
text +=	"\t\t<decentered>\n";
text +=	"\t\t\t<P1>\n";
text +=	"\t\t\t\t<value></value>\n";
text += "\t\t\t\t<sigma></sigma>\n";
text +=	"\t\t\t</P1>\n";
text +=	"\t\t\t<P2>\n";
text +=	"\t\t\t\t<value></value>\n";
text += "\t\t\t\t<sigma></sigma>\n";
text +=	"\t\t\t</P2>\n";
text +=	"\t\t</decentered>\n";
text +=	"\t</distortionCoefficients>\n";
text +=	"\t<principalPointCoordinates uom=\"#mm\">\n";
text +=	"\t\t<gml:pos></gml:pos>\n";
text +=	"\t\t<sigma>\n";
text +=	"\t\t\t<mml:matrix>\n";
text +=	"\t\t\t\t<mml:matrixrow>\n";
text +=	"\t\t\t\t\t<mml:cn></mml:cn>\n";
text +=	"\t\t\t\t\t<mml:cn></mml:cn>\n";
text +=	"\t\t\t\t</mml:matrixrow>\n";
text +=	"\t\t\t\t<mml:matrixrow>\n";
text +=	"\t\t\t\t\t<mml:cn></mml:cn>\n";
text +=	"\t\t\t\t\t<mml:cn></mml:cn>\n";
text +=	"\t\t\t\t</mml:matrixrow>\n";
text +=	"\t\t\t</mml:matrix>\n";
text +=	"\t\t</sigma>\n";
text +=	"\t</principalPointCoordinates>\n";
text +=	"\t<fiductialMarks uom=\"#mm\">\n";
text +=	"\t\t<fiductialMark key=\"1\">\n";
text +=	"\t\t\t<gml:pos></gml:pos>\n";
text +=	"\t\t\t<sigma></sigma>\n";
text +=	"\t\t</fiductialMark>\n";
text +=	"\t\t<fiductialMark key=\"2\">\n";
text +=	"\t\t\t<gml:pos></gml:pos>\n";
text +=	"\t\t\t<sigma></sigma>\n";
text +=	"\t\t</fiductialMark>\n";
text +=	"\t\t<fiductialMark key=\"3\">\n";
text +=	"\t\t\t<gml:pos></gml:pos>\n";
text +=	"\t\t\t<sigma></sigma>\n";
text +=	"\t\t</fiductialMark>\n";
text +=	"\t\t<fiductialMark key=\"4\">\n";
text +=	"\t\t\t<gml:pos></gml:pos>\n";
text +=	"\t\t\t<sigma></sigma>\n";
text +=	"\t\t</fiductialMark>\n";
text +=	"\t</fiductialMarks>\n";
text += "</sensor>";
*/
}

void ProjectUserInterface_Qt::newFlight()
{
	// Por enquanto este metodo esta fora de uso.
	/*
string text = "";

text += "<flight key=\"\" sensor_key=\"\">\n";
text += "\t<flightId></flightId>\n";
text += "\t<description></description>\n";
text += "\t<execution></execution>\n";
text += "\t<producerName></producerName>\n";
text += "\t<nominalScale>\n";
text += "\t\t<mml:mfrac>\n";
text += "\t\t\t<mml:mn></mml:mn>\n";
text += "\t\t\t<mml:mn></mml:mn>\n";
text += "\t\t</mml:mfrac>\n";
text += "\t</nominalScale>\n";
text += "\t<flightHeight uom=\"#m\"></flightHeight>\n";
text += "\t<overlap>\n";
text += "\t\t<longitudinal uom=\"#%\"></longitudinal>\n";
text += "\t\t<transversal uom=\"#%\"></transversal>\n";
text += "\t</overlap>\n";
text += "</flight>";
*/
}

void ProjectUserInterface_Qt::newImage()
{
    addNewState = true;
	currentItemId = manager->getFreeImageId();

	string text = "";
	text += "<image key=\"" + intToString(currentItemId) + "\" sensor_key=\"1\" flight_key=\"1\">\n";
	text += "\t<imageId></imageId>\n";
	text += "\t<width uom=\"#px\"></width>\n";
	text += "\t<height uom=\"#px\"></height>\n";
	text += "\t<fileName></fileName>\n";
	text += "\t<filePath>.</filePath>\n";
	text += "\t<flightId></flightId>\n";
	text += "\t<resolution uom=\"#dpi\">600</resolution>\n";
	text += "</image>";

	imageForm.fillvalues(text);
	centerArea.setCurrentIndex(5);
	currentForm = &imageForm;
	currentForm->setReadOnly(false);
	centerArea.setStyleSheet(this->styleSheet());
	validatingImage();

	debuggerTextEdit->clear();
	debuggerTextEdit->setText(QString::fromUtf8(text.c_str()));
	debuggerTextEdit->setReadOnly(true);
}

void ProjectUserInterface_Qt::newPoint()
{
    addNewState = true;
	//updateView(new QLabel("Futuro view de point"));
	string text = "";

	currentItemId = manager->getFreePointId();
	text += "<point key=\"" + intToString(currentItemId) + "\" type=\"\">\n";
	text += "\t<pointId></pointId>\n";
	text += "\t<description></description>\n";
	text += "\t<spatialCoordinates uom=\"#m\">\n";
	text += "\t\t<gml:pos></gml:pos>\n";
	text += "\t\t<sigma>Not Available</sigma>\n";
	text += "\t</spatialCoordinates>\n";
	text += "</point>";

	pointForm.setImageList(manager->listImageKeys(), manager->listImages());
	pointForm.fillvalues(text);
	centerArea.setCurrentIndex(7);
	currentForm = &pointForm;
	currentForm->setReadOnly(false);
	centerArea.setStyleSheet(this->styleSheet());
	validatingPoint();

	debuggerTextEdit->clear();
	debuggerTextEdit->setText(QString::fromUtf8(text.c_str()));
	debuggerTextEdit->setReadOnly(false);
}

// Salvando um novo de cada view...

void ProjectUserInterface_Qt::saveNewSensor()
{
	/*
string currentData = debuggerTextEdit->toPlainText().toStdString();
if (EDomValidator::validateSensor(currentData))
{
manager->addComponent(currentData, "sensors");
newTree();
viewSensors();
}
else
{
QMessageBox msgBox;
msgBox.setText("Erro: Xml passado é inválido.");
msgBox.exec();
emit viewButtons->editButton->click();
}
*/
}

void ProjectUserInterface_Qt::saveNewFlight()
{
	/*
string currentData = debuggerTextEdit->toPlainText().toStdString();
if (EDomValidator::validateFlight(currentData))
{
manager->addComponent(currentData, "flights");
newTree();
viewFlights();
}
else
{
QMessageBox msgBox;
msgBox.setText("Erro: Xml passado é inválido.");
msgBox.exec();
emit viewButtons->editButton->click();
}
*/
}

void ProjectUserInterface_Qt::saveNewImage()
{
    addNewState = false;
	manager->addComponent(imageForm.getvalues(),"images");
	viewImages();
	actionSave_file->setEnabled(true);
	updateTree();
}

void ProjectUserInterface_Qt::saveNewPoint()
{
	addNewState = false;
	manager->addComponent(pointForm.getvalues(),"points");
	viewPoints();
	actionSave_file->setEnabled(true);
	updateTree();
}

// Apagando cada view...

void ProjectUserInterface_Qt::deleteFlight()
{
}

void ProjectUserInterface_Qt::deleteSensor()
{
}

void ProjectUserInterface_Qt::deletePoint()
{
}

void ProjectUserInterface_Qt::deleteImage()
{
}

void ProjectUserInterface_Qt::toggleDebug()
{
	if (debuggerDockWidget->isVisible())
		debuggerDockWidget->setVisible(false);
	else
		debuggerDockWidget->setVisible(true);
}

void ProjectUserInterface_Qt::showAbout()
{
	AboutForm* about = new AboutForm();
	about->show();
}

// Códigos das classes extras.

//#include <QStringList>

/*TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent)
{
	parentItem = parent;
	itemData = data;
}

TreeItem::~TreeItem()
{
	qDeleteAll(childItems);
}

void TreeItem::appendChild(TreeItem *item)
{
	childItems.append(item);
}

TreeItem *TreeItem::child(int row)
{
	return childItems.value(row);
}

int TreeItem::childCount() const
{
	return childItems.count();
}

int TreeItem::columnCount() const
{
	return itemData.count();
}

QVariant TreeItem::data(int column) const
{
	return itemData.value(column);
}

TreeItem *TreeItem::parent()
{
	return parentItem;
}

int TreeItem::row() const
{
	if (parentItem)
		return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));

	return 0;
}

TreeModel::TreeModel(const QString &data, QObject *parent)
	: QAbstractItemModel(parent)
{
	QList<QVariant> rootData;
	rootData << "";
	rootItem = new TreeItem(rootData);
	setupModelData(data.split(QString("\n")), rootItem);
}

TreeModel::~TreeModel()
{
	delete rootItem;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
	else
		return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

	return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
							   int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
		const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	TreeItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	TreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
	TreeItem *parentItem = childItem->parent();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
	TreeItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}

void TreeModel::setupModelData(const QStringList &lines, TreeItem *parent)
{
	QList<TreeItem*> parents;
	QList<int> indentations;
	parents << parent;
	indentations << 0;

	int number = 0;

	while (number < lines.count()) {
		int position = 0;
		while (position < lines[number].length()) {
			if (lines[number].mid(position, 1) != " ")
				break;
			position++;
		}

		QString lineData = lines[number].mid(position).trimmed();

		if (!lineData.isEmpty()) {
			// Read the column data from the rest of the line.
			QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
			QList<QVariant> columnData;
			for (int column = 0; column < columnStrings.count(); ++column)
				columnData << columnStrings[column];

			if (position > indentations.last()) {
				// The last child of the current parent is now the new parent
				// unless the current parent has no children.

				if (parents.last()->childCount() > 0) {
					parents << parents.last()->child(parents.last()->childCount()-1);
					indentations << position;
				}
			} else {
				while (position < indentations.last() && parents.count() > 0) {
					parents.pop_back();
					indentations.pop_back();
				}
			}

			// Append a new item to the current parent's list of children.
			parents.last()->appendChild(new TreeItem(columnData, parents.last()));
		}

		number++;
	}
}*/

void ProjectUserInterface_Qt::validatingSensor()
{
	if ((sensorForm.sensorIdLineEdit->text() == "") ||
		(!sensorForm.fid0SigmaController->getValidate()) ||
		(!sensorForm.fid1SigmaController->getValidate()) ||
		(!sensorForm.fid2SigmaController->getValidate()) ||
		(!sensorForm.fid3SigmaController->getValidate()) ||
		((!sensorForm.fid4SigmaController->getValidate())&&(sensorForm.fiductialMarksCombo->currentIndex()==1)) ||
		((!sensorForm.fid5SigmaController->getValidate())&&(sensorForm.fiductialMarksCombo->currentIndex()==1)) ||
		((!sensorForm.fid6SigmaController->getValidate())&&(sensorForm.fiductialMarksCombo->currentIndex()==1)) ||
		((!sensorForm.fid7SigmaController->getValidate())&&(sensorForm.fiductialMarksCombo->currentIndex()==1)) ||
		(!sensorForm.principalSigmaController->getValidate()) ||
		(!sensorForm.radialSigmaController->getValidate()) ||
		(!sensorForm.decenteredSigmaController->getValidate()) ||
		(!sensorForm.calibratedSigmaController->getValidate())){
		controlButtons.saveButton->setEnabled(false);
	}
	else
		controlButtons.saveButton->setEnabled(true);
}

void ProjectUserInterface_Qt::validatingFlight()
{
	if ((flightForm.flightIDLineEdit->text() == "")){
		controlButtons.saveButton->setEnabled(false);
	}
	else
		controlButtons.saveButton->setEnabled(true);
}

void ProjectUserInterface_Qt::validatingImage()
{
	if ((imageForm.imageIDLine->text() == "") || (imageForm.fileNameLine->text() == "") || (!imageForm.gnssSigmaController->getValidate()) || (!imageForm.insSigmaController->getValidate())){
		controlButtons.saveButton->setEnabled(false);
	}
	else
		controlButtons.saveButton->setEnabled(true);
}

void ProjectUserInterface_Qt::validatingPoint()
{
	if ((pointForm.lineEdit_gcp_id->text() == "") || (!pointForm.sigmaController->getValidate()))
		controlButtons.saveButton->setEnabled(false);
	else
		controlButtons.saveButton->setEnabled(true);
}

bool ProjectUserInterface_Qt::confirmToClose()
{
	if (editState || addNewState)
	{
		if (controlButtons.saveButton->isEnabled())
		{
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this, tr(" Warning: leaving form in edit mode"),
										  "Do you want to keep all changes?",
										  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
			if (reply == QMessageBox::Yes)
			{
				controlButtons.saveButton->click();
			}
			else if (reply == QMessageBox::No)
			{
				controlButtons.cancelButton->click();
			}
			else
			{
				return false;
			}
		}
		else
		{
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this, tr(" Warning: leaving form in edit mode"),
										  "Registration data is not complete and will be lost. Continue?",
										  QMessageBox::Yes | QMessageBox::Cancel);
			if (reply == QMessageBox::Yes)
			{
				controlButtons.cancelButton->click();
			}
			else
			{
				return false;
			}
		}
	}
	if (actionSave_file->isEnabled())
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, tr(" Warning: You have unsaved data"),
									  tr("Do you want to save all changes?"),
									  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if (reply == QMessageBox::Yes)
		{
			actionSave_file->trigger();
		}
		else if (reply == QMessageBox::Cancel)
		{
			return false;
		}
	}
	return true;
}

QString ProjectUserInterface_Qt::getSavedIn()
{
	return QString(savedIn.c_str());
}
