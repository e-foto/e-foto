#include "ProjectUserInterface_Qt.h"

#include <qapplication.h>
#include <qvariant.h>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QLabel>
#include <QShortcut>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QProgressBar>

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
	this->changeModule = false;
	this->treeItems.clear();
	menuExecute->setEnabled(false);
	actionSave_file->setEnabled(false);
	actionSave_file_as->setEnabled(false);

	setWindowTitle(tr("efoto[Project Manager]"));
	imageForm.proj = this;

	// Realiza as conexões necessárias
	this->connect(actionNew, SIGNAL(triggered()), this, SLOT(newProject()));
	this->connect(actionLoad_file, SIGNAL(triggered()), this, SLOT(loadFile()));
	this->connect(actionSave_file, SIGNAL(triggered()), this, SLOT(saveFile()));
	this->connect(actionSave_file_as, SIGNAL(triggered()), this, SLOT(saveFileAs()));
	this->connect(actionInterior_Orientation, SIGNAL(triggered()), this, SLOT(executeIO()));
	this->connect(actionSpatial_resection, SIGNAL(triggered()), this, SLOT(executeSR()));
	this->connect(actionFoto_Tri, SIGNAL(triggered()), this, SLOT(executeFT()));
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

	connect(pointsForm.importButton, SIGNAL(clicked()), this, SLOT( importPointsFromTxt() ) );
	connect(pointsForm.exportToTxtButton, SIGNAL(clicked()), this, SLOT(exportPointsToTxt()) );
	connect(imagesForm.importButton, SIGNAL(clicked()), this, SLOT( importImagesFromTxt() ) );
	imagesForm.importButton->setEnabled(false);

	//setGeometry(qApp->desktop()->availableGeometry());
	setWindowState(this->windowState() | Qt::WindowMaximized);
	qApp->processEvents();
	// Bloqueia alguns dos dipositivos
	this->removeDockWidget(debuggerDockWidget);
	this->removeDockWidget(projectDockWidget);

	this->show();

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

	// Inserido pelo Paulo 05/09/2011
	// Adiciona um atalho para os desenvolvedores dar upload das coordenadas digitais do export do LPS
	QShortcut* shortcut2 = new QShortcut(QKeySequence(tr("Ctrl+Shift+P", "Import")),this);
	connect(shortcut2, SIGNAL(activated()), this, SLOT(importDigitalCoordinatesFromTxt()));

	QShortcut* shortcut3 = new QShortcut(QKeySequence(tr("Ctrl+Shift+O", "Import")),this);
	connect(shortcut3, SIGNAL(activated()), this, SLOT(importDigitalCoordinatesOIFromTxt()));


	actionFoto_Tri->setEnabled(availablePhotoTri());

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
	//qDebug() << changeModule;
	if (changeModule || confirmToClose())
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
	text +=	"<sensorId></sensorId>\n";
	text +=	"<type>\n";
	text +=	"<geometry>frame</geometry>\n";
	text +=	"<platform>aerial</platform>\n";
	text +=	"<detector>film</detector>\n";
	text +=	"<energySource>natural</energySource>\n";
	text +=	"<calculationMode>With Fiducial Marks</calculationMode>";
	text +=	"<spectralRanges uom=\"#um\">\n";
	//text +=	"<spectralRange band=\"\">\n";
	//text +=	"<inferiorLimit></inferiorLimit>\n";
	//text +=	"<superiorLimit></superiorLimit>\n";
	//text +=	"</spectralRange>\n";
	text +=	"</spectralRanges>\n";
	text +=	"</type>\n";
	text +=	"<description></description>\n";
	text +=	"<calibrationCertificate>\n";
	text +=	"<number></number>\n";
	text +=	"<dispatch></dispatch>\n";
	text +=	"<expiration></expiration>\n";
	text +=	"</calibrationCertificate>\n";
	text +=	"<focalDistance uom=\"#mm\">\n";
	text +=	"<value></value>\n";
	text +=	"<sigma></sigma>\n";
	text +=	"</focalDistance>\n";
	text +=	"<distortionCoefficients>\n";
	text +=	"<radialSymmetric considered=\"false\">\n";
	text +=	"<k0>\n";
	text +=	"<value></value>\n";
	text += "<sigma></sigma>\n";
	text +=	"</k0>\n";
	text +=	"<k1>\n";
	text +=	"<value></value>\n";
	text += "<sigma></sigma>\n";
	text +=	"</k1>\n";
	text +=	"<k2>\n";
	text +=	"<value></value>\n";
	text += "<sigma></sigma>\n";
	text +=	"</k2>\n";
	text +=	"<k3>\n";
	text +=	"<value></value>\n";
	text += "<sigma></sigma>\n";
	text +=	"</k3>\n";
	text +=	"</radialSymmetric>\n";
	text +=	"<decentered considered=\"false\">\n";
	text +=	"<P1>\n";
	text +=	"<value></value>\n";
	text += "<sigma></sigma>\n";
	text +=	"</P1>\n";
	text +=	"<P2>\n";
	text +=	"<value></value>\n";
	text += "<sigma></sigma>\n";
	text +=	"</P2>\n";
	text +=	"</decentered>\n";
	text +=	"</distortionCoefficients>\n";
	text +=	"<principalPointCoordinates uom=\"#mm\">\n";
	text +=	"<gml:pos></gml:pos>\n";
	text +=	"<sigma>\n";
	//text +=	"<mml:matrix>\n";
	//text +=	"<mml:matrixrow>\n";
	//text +=	"<mml:cn></mml:cn>\n";
	//text +=	"<mml:cn></mml:cn>\n";
	//text +=	"</mml:matrixrow>\n";
	//text +=	"<mml:matrixrow>\n";
	//text +=	"<mml:cn></mml:cn>\n";
	//text +=	"<mml:cn></mml:cn>\n";
	//text +=	"\t\t\t\t</mml:matrixrow>\n";
	//text +=	"\t\t\t</mml:matrix>\n";
	text +=	"</sigma>\n";
	text +=	"</principalPointCoordinates>\n";
	text +=	"<fiductialMarks uom=\"#mm\">\n";
	text +=	"<fiductialMark key=\"1\">\n";
	text +=	"<gml:pos></gml:pos>\n";
	text +=	"<sigma></sigma>\n";
	text +=	"</fiductialMark>\n";
	text +=	"<fiductialMark key=\"2\">\n";
	text +=	"<gml:pos></gml:pos>\n";
	text +=	"<sigma></sigma>\n";
	text +=	"</fiductialMark>\n";
	text +=	"<fiductialMark key=\"3\">\n";
	text +=	"<gml:pos></gml:pos>\n";
	text +=	"<sigma></sigma>\n";
	text +=	"</fiductialMark>\n";
	text +=	"<fiductialMark key=\"4\">\n";
	text +=	"<gml:pos></gml:pos>\n";
	text +=	"<sigma></sigma>\n";
	text +=	"</fiductialMark>\n";
	text +=	"</fiductialMarks>\n";
	text += "</sensor>";

	manager->addComponent(text, "sensors");

	text = "";

	text += "<flight key=\"1\" sensor_key=\"1\">\n";
	text += "<flightId></flightId>\n";
	text += "<description></description>\n";
	text += "<execution></execution>\n";
	text += "<producerName></producerName>\n";
	text += "<nominalScale>\n";
	text += "<mml:mfrac>\n";
	text += "<mml:mn>1</mml:mn>\n";
	text += "<mml:mn>10000</mml:mn>\n";
	text += "</mml:mfrac>\n";
	text += "</nominalScale>\n";
	text += "<flightHeight uom=\"#m\"></flightHeight>\n";
	text += "<overlap>\n";
	text += "<longitudinal uom=\"#%\">60</longitudinal>\n";
	text += "<transversal uom=\"#%\">20</transversal>\n";
	text += "</overlap>\n";
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
			/*//deprecated
			if (!manager->testFileVersion())
			{
				QMessageBox* alert = new QMessageBox(QMessageBox::Warning,"Unable to open file","The e-foto software was unable to open the selected file.\nThis may be due to:\n\n - Unsupported file version;\n - The file is not a valid .epp (e-foto Photogrammetric Project) file;\n - A bug in the program.\n\nTry changing the file or version of the software and try again.");
				alert->show();
				return;
			}
			*/
			EDomElement imagesXml(manager->getXml("images").c_str());
			deque<EDomElement> imagesEdom=imagesXml.elementsByTagName("image");

			QDir dirImage(filename.left(filename.lastIndexOf('/')));
			QFileInfo imageFileInfo;

			QString imagesMissing="";
			int contMissings=0;
			for (int i=0 ;i < imagesEdom.size() ;i++)
			{
				QString imagesName(imagesEdom.at(i).elementByTagName("filePath").toString().append("/").c_str());
				dirImage.setCurrent(imagesName);
				imagesName.append(imagesEdom.at(i).elementByTagName("fileName").toString().c_str());
				imageFileInfo.setFile(dirImage.absoluteFilePath(imagesName));
				if (!imageFileInfo.exists())
				{
					imagesMissing.append(contMissings%4==3 ? imageFileInfo.fileName().append("  \n") : imageFileInfo.fileName().append(" , "));
					contMissings++;
				}
			}
			imagesMissing.chop(3);
			if(imagesMissing.compare("")!=0)
			{
				QMessageBox* alertImages= new QMessageBox(QMessageBox::Warning,"Images missing",imagesMissing.prepend("Those images are missing:\n").append("."));
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
			//qDebug("load savedIn: %s",savedIn.c_str());

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

			QDir dir(filename.left(i));
			dir.setCurrent(dir.absolutePath());


			newTree();
			// Inserido pelo Paulo 05/09/2011
			actionFoto_Tri->setEnabled(availablePhotoTri());
//			actionInterior_Orientation->setEnabled(availabeOI());
	//		actionSpatial_resection->setEnabled(availableOE());
		}
		else
		{
			QMessageBox* alert = NULL;
			switch (manager->informFileVersionError())
			{
			case 0:
				alert = new QMessageBox(QMessageBox::Warning,"Unable to open file","The e-foto software was unable to open the selected file.\nThis may be due to:\n\n - Unsupported characters in the file's name or path (maybe accented characters or whitespace);\n - The file does not exist;\n - A bug in the program.\n\nTry changing the file's name or path and try again.");
				break;
			case 1:
			case 2:
			case 3:
			case 4:
				alert = new QMessageBox(QMessageBox::Warning,"Unable to load file","The e-foto software was unable to load the selected file.\nThis may be due to:\n\n - Unsupported file version;\n - The file is not a valid .epp (e-foto Photogrammetric Project) file;\n - A bug in the program.\n\nTry changing the file or version of the software and try again.");
				break;
			default:
				alert = new QMessageBox(QMessageBox::Warning,"Unable to open or load file","The e-foto software was unable to open or load the selected file.\nThis may be due to:\n\n - Unsupported characters in the file's name or path (maybe accented characters or whitespace);\n - Unsupported file version;\n - The file is not a valid .epp (e-foto Photogrammetric Project) file;\n - A bug in the program.\n\nTry changing the file's name or path or changing the file or version of the software and try again.");
				break;
			}
			alert->show();
			//delete alert;
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
		if (extension.toLower() != ".epp")
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
			changeModule = true;
			confirmToClose();
			LoadingScreen::instance().show();
			qApp->processEvents();
			this->close();
			manager->startModule("InteriorOrientation", value);
			changeModule = false;
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
			changeModule = true;
			confirmToClose();
			LoadingScreen::instance().show();
			qApp->processEvents();
			this->close();
			manager->startModule("SpatialRessection", value);
			changeModule = false;
		}
	}
}

void ProjectUserInterface_Qt::executeFT()
{
	bool ok;
	/*
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
			changeModule = true;
			confirmToClose();
			LoadingScreen::instance().show();
			qApp->processEvents();
			this->close();
			manager->startModule("FotoTri", value);
			changeModule = false;
		}
	}*/
	changeModule = true;
	confirmToClose();
	LoadingScreen::instance().show();
	qApp->processEvents();
	this->close();
	manager->startModule("FotoTriangulation", 0);
	changeModule = false;

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
	actionSave_file->setEnabled(!manager->getSavedState());
	actionFoto_Tri->setEnabled(availablePhotoTri());
//	actionInterior_Orientation->setEnabled(availabeOI());
//	actionSpatial_resection->setEnabled(availableOE());
	//PAULO -> codigo para dar um refresh no formulario
	updateCurrentForm();
	this->show();
	LoadingScreen::instance().close();
	qApp->processEvents();
	changeModule = false;

	//if (qApp->exec())
		//return false;
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
	debuggerTextEdit->setText(QString::fromUtf8(node.indent('\t').getContent().c_str()));
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
	debuggerTextEdit->setText(QString::fromUtf8(node.indent('\t').getContent().c_str()));
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
debuggerTextEdit->setText(QString::fromUtf8(node.indent('\t').getContent().c_str()));
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
		sensorForm.clearForm();
		sensorForm.setFormLocale(QLocale(QLocale::system()));
	sensorForm.fillvalues(node.getContent());
	centerArea.setStyleSheet("QScrollArea, QWidget {background: #FFFFFF} QScrollArea {border: 0px}");
	centerArea.setCurrentIndex(2);
	currentForm = &sensorForm;
	currentForm->setReadOnly(true);
	validatingSensor();

	debuggerTextEdit->clear();
	debuggerTextEdit->setText(QString::fromUtf8(node.indent('\t').getContent().c_str()));
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
debuggerTextEdit->setText(QString::fromUtf8(node.indent('\t').getContent().c_str()));
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
	debuggerTextEdit->setText(QString::fromUtf8(node.indent('\t').getContent().c_str()));
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

	EDomElement node2(manager->getXml("interiorOrientation"));
	if (node2.getContent()!="")
	{
		imagesForm.setIOsAvailable(node2.getContent());
	}

	EDomElement node3(manager->getXml("exteriorOrientation"));
	if (node3.getContent()!="")
	{
		imagesForm.setEOsAvailable(node3.getContent());
	}
	int height=(imagesForm.imagesTable->rowCount()+2)*imagesForm.imagesTable->rowHeight(0)+imagesForm.label->height()+imagesForm.importButton->height();
	imagesForm.setMinimumSize(imagesForm.imagesTable->width(),height);
	debuggerTextEdit->clear();
	debuggerTextEdit->setText(QString::fromUtf8(node.indent('\t').getContent().c_str()));
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

	EDomElement node2(manager->getXml("imageIO", "image_key", intToString(id)));
	if (node2.getContent()!="")
	{
		imageForm.setIOAvailable(true);
	}
	else
	{
		imageForm.setIOAvailable(false);
	}

	EDomElement node3(manager->getXml("imageEO", "image_key", intToString(id)));
	if (node3.getContent()!="")
	{
		imageForm.setEOAvailable(true);
	}
	else
	{
		imageForm.setEOAvailable(false);
	}

	debuggerTextEdit->clear();
	debuggerTextEdit->setText(QString::fromUtf8(node.indent('\t').getContent().c_str()));
	debuggerTextEdit->setReadOnly(true);

	menuProject->setEnabled(true);
	menuExecute->setEnabled(true);
//	actionInterior_Orientation->setEnabled(availabeOI());
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
	int height=(pointsForm.pointsTable->rowCount()+2)*pointsForm.pointsTable->rowHeight(0)+pointsForm.importButton->height()+pointsForm.label->height();
	pointsForm.setMinimumSize(pointsForm.pointsTable->width(),height);
	debuggerTextEdit->clear();
	debuggerTextEdit->setText(QString::fromUtf8(node.indent('\t').getContent().c_str()));
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
	debuggerTextEdit->setText(QString::fromUtf8(node.indent('\t').getContent().c_str()));
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
text +=	"<sensorId></sensorId>\n";
text +=	"<type>\n";
text +=	"<geometry></geometry>\n";
text +=	"<platform></platform>\n";
text +=	"<detector></detector>\n";
text +=	"<energySource></energySource>\n";
text +=	"<spectralRanges uom=\"#um\">\n";
text +=	"<spectralRange band=\"\">\n";
text +=	"<inferiorLimit></inferiorLimit>\n";
text +=	"<superiorLimit></superiorLimit>\n";
text +=	"</spectralRange>\n";
text +=	"</spectralRanges>\n";
text +=	"</type>\n";
text +=	"<description></description>\n";
text +=	"<calibrationCertificate>\n";
text +=	"<number></number>\n";
text +=	"<dispatch></dispatch>\n";
text +=	"<expiration></expiration>\n";
text +=	"</calibrationCertificate>\n";
text +=	"<focalDistance uom=\"#mm\">\n";
text +=	"<value></value>\n";
text +=	"<sigma></sigma>\n";
text +=	"</focalDistance>\n";
text +=	"<distortionCoefficients>\n";
text +=	"<radialSymmetric>\n";
text +=	"<k0>\n";
text +=	"<value></value>\n";
text += "<sigma></sigma>\n";
text +=	"</k0>\n";
text +=	"<k1>\n";
text +=	"<value></value>\n";
text += "<sigma></sigma>\n";
text +=	"</k1>\n";
text +=	"<k2>\n";
text +=	"<value></value>\n";
text += "<sigma></sigma>\n";
text +=	"</k2>\n";
text +=	"<k3>\n";
text +=	"<value></value>\n";
text += "<sigma></sigma>\n";
text +=	"</k3>\n";
text +=	"</radialSymmetric>\n";
text +=	"<decentered>\n";
text +=	"<P1>\n";
text +=	"<value></value>\n";
text += "<sigma></sigma>\n";
text +=	"</P1>\n";
text +=	"<P2>\n";
text +=	"<value></value>\n";
text += "<sigma></sigma>\n";
text +=	"</P2>\n";
text +=	"</decentered>\n";
text +=	"</distortionCoefficients>\n";
text +=	"<principalPointCoordinates uom=\"#mm\">\n";
text +=	"<gml:pos></gml:pos>\n";
text +=	"<sigma>\n";
text +=	"<mml:matrix>\n";
text +=	"<mml:matrixrow>\n";
text +=	"<mml:cn></mml:cn>\n";
text +=	"<mml:cn></mml:cn>\n";
text +=	"</mml:matrixrow>\n";
text +=	"<mml:matrixrow>\n";
text +=	"<mml:cn></mml:cn>\n";
text +=	"<mml:cn></mml:cn>\n";
text +=	"</mml:matrixrow>\n";
text +=	"</mml:matrix>\n";
text +=	"</sigma>\n";
text +=	"</principalPointCoordinates>\n";
text +=	"<fiductialMarks uom=\"#mm\">\n";
text +=	"<fiductialMark key=\"1\">\n";
text +=	"<gml:pos></gml:pos>\n";
text +=	"<sigma></sigma>\n";
text +=	"</fiductialMark>\n";
text +=	"<fiductialMark key=\"2\">\n";
text +=	"<gml:pos></gml:pos>\n";
text +=	"<sigma></sigma>\n";
text +=	"</fiductialMark>\n";
text +=	"<fiductialMark key=\"3\">\n";
text +=	"<gml:pos></gml:pos>\n";
text +=	"<sigma></sigma>\n";
text +=	"</fiductialMark>\n";
text +=	"<fiductialMark key=\"4\">\n";
text +=	"<gml:pos></gml:pos>\n";
text +=	"<sigma></sigma>\n";
text +=	"</fiductialMark>\n";
text +=	"</fiductialMarks>\n";
text += "</sensor>";
*/
}

void ProjectUserInterface_Qt::newFlight()
{
	// Por enquanto este metodo esta fora de uso.
	/*
string text = "";

text += "<flight key=\"\" sensor_key=\"\">\n";
text += "<flightId></flightId>\n";
text += "<description></description>\n";
text += "<execution></execution>\n";
text += "<producerName></producerName>\n";
text += "<nominalScale>\n";
text += "<mml:mfrac>\n";
text += "<mml:mn></mml:mn>\n";
text += "<mml:mn></mml:mn>\n";
text += "</mml:mfrac>\n";
text += "</nominalScale>\n";
text += "<flightHeight uom=\"#m\"></flightHeight>\n";
text += "<overlap>\n";
text += "<longitudinal uom=\"#%\"></longitudinal>\n";
text += "<transversal uom=\"#%\"></transversal>\n";
text += "</overlap>\n";
text += "</flight>";
*/
}

void ProjectUserInterface_Qt::newImage()
{
	addNewState = true;
	currentItemId = manager->getFreeImageId();

	string text = "";
	text += "<image key=\"" + intToString(currentItemId) + "\" sensor_key=\"1\" flight_key=\"1\">\n";
	text += "<imageId></imageId>\n";
	text += "<width uom=\"#px\"></width>\n";
	text += "<height uom=\"#px\"></height>\n";
	text += "<fileName></fileName>\n";
	text += "<filePath>.</filePath>\n";
	text += "<flightId></flightId>\n";
	text += "<resolution uom=\"#dpi\">N/A</resolution>\n";
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
	text += "<pointId></pointId>\n";
	text += "<description></description>\n";
	text += "<spatialCoordinates uom=\"#m\">\n";
	text += "<gml:pos></gml:pos>\n";
	text += "<sigma>Not Available</sigma>\n";
	text += "</spatialCoordinates>\n";
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
	//qDebug() << actionSave_file->isEnabled();
	if (actionSave_file->isEnabled())
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, tr(" Warning: You have unsaved data"),
									  tr("Do you want to save all changes?"),
									  (changeModule ? QMessageBox::Yes | QMessageBox::No : QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel));
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
	//HeaderForm* parent =(HeaderForm*)&(this->headerForm);
	//qDebug("saved in %s",savedIn.c_str());

	//cout<< "cout "<<savedIn.c_str()<<"\n"<<endl;
	//qDebug()<<headerForm.lineEditFilePath->text();

	return  QString(savedIn.c_str()).left(savedIn.find_last_of('/'));

	//return parent->lineEditFilePath->text();
}

/** This function import points from a *.txt file with a one point in each line
*  in pattern GCPID\tE\tN\tH\tdE\tdN\tdH or GCPID\tE\tN\tH
*/
void ProjectUserInterface_Qt::importPointsFromTxt()
{
	QString importFileName = QFileDialog::getOpenFileName(this,tr("Open Import File"),".","*.txt");
	if(importFileName=="")
		return;
	QFile *importFile = new QFile(importFileName);
	QStringList pointsList;

	importFile->open(QIODevice::ReadOnly);

	while(!importFile->atEnd())
	{
		QByteArray line = importFile->readLine();
		QString aux(line);
		//pointsList << aux.left(aux.lastIndexOf('\n'));
		pointsList << aux.remove('\n');
	}
	importFile->close();

	QWidget *loadWidget= new QWidget();
	loadWidget->setAttribute(Qt::WA_DeleteOnClose,true);
	QProgressBar loading;
	QPushButton cancelButton("Cancel");
	loading.setRange(0,pointsList.size());

	QVBoxLayout loadLayout;
	loadLayout.addWidget(&loading,Qt::AlignCenter);
	loadLayout.addWidget(&cancelButton,Qt::AlignCenter);
	connect(&cancelButton,SIGNAL(clicked()),loadWidget,SLOT(close()));

	loadWidget->setLayout(&loadLayout);
	loadWidget->setWindowTitle(tr("Loading Points"));
	loading.setMinimumSize(300,30);
	loadWidget->show();

	string newPointXML;
	for (int i=0; i<pointsList.length() && loadWidget!=NULL;i++)
	{
		loading.setFormat(tr("%v/%m : %p%"));
		loading.setValue(i+1);
		newPointXML+=pointTxtToXml(pointsList.at(i),manager->getFreePointId()+i,i+1);
		/*if ( newPointXML.hasTagName("pointId") )
		{
			manager->addComponent(newPointXML.getContent().data(),"points");
		}*/
	}
	manager->addComponent(newPointXML,"points");
	loadWidget->close();

	updateTree();
	viewPoints();
	actionSave_file->setEnabled(true);
}

/** This function convert a point data from a *.txt line in a children point XML valid
*/
string ProjectUserInterface_Qt::pointTxtToXml(QString point, int key, int line, string typePoint)
{
	stringstream aux;
	string gcpIdField, typeField, eField, nField, hField, dEField, dNField, dHField;
	QStringList fields= point.split("\t");
// check	control	 tie
	if (fields.length() == 7)
	{

		gcpIdField = fields.at(0).toStdString().c_str();
		/*
		typeField = point.split("\t").at(1).toStdString().c_str();
		if(typeField == "Tie")
			typePoint="photogrammetric";
		else if (typeField== "Control")
			typePoint="control";
		else if (typeField== "Check")
			typePoint="verification";
*/
		eField = fields.at(1).toStdString();
		nField = fields.at(2).toStdString();
		hField = fields.at(3).toStdString();
		dEField = fields.at(4).toStdString();
		dNField = fields.at(5).toStdString();
		dHField = fields.at(6).toStdString();

		aux << "<point key=\""<< intToString(key)<<"\" type=\"" << typePoint << "\">\n";
		aux << "<pointId>" << gcpIdField << "</pointId>\n";
		aux << "<description>" << "Put point description here" << "</description>\n";
		aux << "<spatialCoordinates uom=\"#" << "m" << "\">\n";
		aux << "<gml:pos>" << eField << " " << nField << " " << hField << "</gml:pos>\n";
		aux << "<sigma>\n";
		aux << "<mml:matrix>\n";
		aux << "<mml:matrixrow>\n";
		aux << "<mml:cn>" << dEField << "</mml:cn>\n";
		aux << "</mml:matrixrow>\n";
		aux << "<mml:matrixrow>\n";
		aux << "<mml:cn>" << dNField << "</mml:cn>\n";
		aux << "</mml:matrixrow>\n";
		aux << "<mml:matrixrow>\n";
		aux << "<mml:cn>" << dHField << "</mml:cn>\n";
		aux << "</mml:matrixrow>\n";
		aux << "</mml:matrix>\n";
		aux << "</sigma>\n";
		aux << "</spatialCoordinates>\n";
		aux << "<imagesMeasurements>\n";
		aux << "</imagesMeasurements>\n";
		aux << "</point>";
	}
	else if (point.split("\t").length() == 4)
	{
		//gcpIdField = point.split("\t").at(0).toStdString().c_str();
		gcpIdField = fields.at(0).toStdString();
		/*
		typeField = point.split("\t").at(1).toStdString().c_str();
		if(typeField == "Tie")
			typePoint="photogrammetric";
		else if (typeField== "Control")
			typePoint="control";
		else if (typeField== "Check")
			typePoint="verification";
*/
		eField = fields.at(1).toStdString();
		nField = fields.at(2).toStdString();
		hField = fields.at(3).toStdString();

		aux << "<point key=\""<< intToString(key)<<"\" type=\"" << typePoint << "\">\n";
		aux << "<pointId>" << gcpIdField << "</pointId>\n";
		aux << "<description>" << "Put point description here" << "</description>\n";
		aux << "<spatialCoordinates uom=\"#" << "m" << "\">\n";
		aux << "<gml:pos>" << eField << " " << nField << " " << hField << "</gml:pos>\n";
		aux << "<sigma>Not Available</sigma>\n";
		aux << "</spatialCoordinates>\n";
		aux << "<imagesMeasurements>\n";
		aux << "</imagesMeasurements>\n";
		aux << "</point>";
	}else{
		QMessageBox::warning(this, tr(" Warning "), tr("The point in line %1 from imported file\nhas incomplete or corrupted data").arg(line));
		return "";
	}

	return aux.str();
	//EDomElement newPointXml(newXml.c_str());

}

/** This function exports all points in current XML to *.txt file with a one point in
*   each line in pattern GCPID\tE\tN\tH\tdE\tdN\tdH or GCPID\tE\tN\tH
*/
void ProjectUserInterface_Qt::exportPointsToTxt()
{
	QString fileSaveName= QFileDialog::getSaveFileName(this,tr("Export file"),".","*.txt");
	if (fileSaveName=="")
		return;
	QFile *exportFileName= new QFile(fileSaveName);
	exportFileName->setFileName(fileSaveName);
	exportFileName->open(QIODevice::WriteOnly);

	EDomElement points(manager->getXml("points").c_str());

	QWidget *loadWidget= new QWidget();
	loadWidget->setAttribute(Qt::WA_DeleteOnClose,true);
	QProgressBar loading;
	QPushButton cancelButton("Cancel");
	loading.setRange(0,points.children().size());

	QVBoxLayout loadLayout;
	loadLayout.addWidget(&loading,Qt::AlignCenter);
	loadLayout.addWidget(&cancelButton,Qt::AlignCenter);
	connect(&cancelButton,SIGNAL(clicked()),loadWidget,SLOT(close()));

	loadWidget->setLayout(&loadLayout);
	loadWidget->setWindowTitle(tr("Exporting Points"));
	loading.setMinimumSize(300,30);
	loadWidget->show();

	for (int i=1; i<=points.children().size(); i++)
	{
		loading.setFormat(tr("Point %v/%m : %p%"));
		loading.setValue(i);
		exportFileName->write(edomPointToTxt(points.elementByTagAtt("point","key",intToString(i))).data() );
	}
	loadWidget->close();
	exportFileName->close();
}

/** This function convert a EDomElement children Point in a line *.txt point format
*/
string ProjectUserInterface_Qt::edomPointToTxt(EDomElement points)
{
	stringstream aux;
	stringstream stdev;
	QString gmlpos=points.elementByTagName("gml:pos").toString().c_str();
	aux << points.elementByTagName("pointId").toString().c_str()<< "\t";
	//aux << points.attribute("type")<< "\t";
	
	aux << (gmlpos.split(" ").at(0)).toStdString().c_str() <<"\t"<<(gmlpos.split(" ").at(1)).toStdString().c_str()<<"\t"<<(gmlpos.split(" ").at(2)).toStdString().c_str();

	Matrix stdevMatrix;
	stdevMatrix.xmlSetData(points.elementByTagName("mml:matrix").getContent());

	if (points.hasTagName("mml:matrix"))
	{
	   /*
	   aux << "\t" << points.elementsByTagName("mml:cn").at(0).toString().c_str() <<"\t";
	   aux << points.elementsByTagName("mml:cn").at(1).toString().c_str() <<"\t";
	   aux << points.elementsByTagName("mml:cn").at(2).toString().c_str();
	   */
	   aux << "\t" << stdevMatrix.get(1,1) <<"\t";
	   aux << stdevMatrix.get(2,2) <<"\t";
	   aux << stdevMatrix.get(3,3);
	}

	aux <<"\n";
	string result=aux.str();
	//qDebug("tamanho: %d",points.elementsByTagName("mml:matrix").size());

	//qDebug("stdev:%s",stdev.str().c_str());
	//qDebug("result:%s",result.c_str());

	return result.c_str();
}

/** This function import images from a *.txt file with a one image in each line
*   in pattern ...
*/
void ProjectUserInterface_Qt::importImagesFromTxt()
{
	QString importFileName = QFileDialog::getOpenFileName(this,tr("Open Import File"),".","*.txt");
	QFile *importFile = new QFile(importFileName);
	QStringList imagesList;

	importFile->open(QIODevice::ReadOnly);

	while(!importFile->atEnd())
	{
		QByteArray line = importFile->readLine();
		QString aux(line);
		imagesList << aux.remove('\n');
	}

	importFile->close();

	for (int i=0; i<imagesList.length();i++)
	{
		EDomElement newImageXML=imageTxtToXml(imagesList.at(i),manager->getFreeImageId(),i+1);
		if ( newImageXML.hasTagName("imageId") )
		{
			manager->addComponent(newImageXML.getContent().data(),"images");
		}
	}
	updateTree();
	viewImages();

}

/** This function convert image data from a *.txt line in a children image XML valid
*
*/
EDomElement ProjectUserInterface_Qt::imageTxtToXml(QString image, int key, int line, int sensorKey, int flightKey)
{
	stringstream aux;

	// implementar o xml de images aqui
	/*
	aux << "<image key=\""<< intToString(key)<<"\" sensor_key=\"" << sensorKey << "\">" << "\" flight_key=\"" << flightKey << "\">\n";
	aux << "<imageId>" << "" << "</imageId>\n";
	aux << "<width uom=\"#px\"" << "" << "</width>\n";
	aux << "<height uom=\"#px\"" << ""  << "</height>\n";
	aux << "<fileName>" << "" << "</fileName>\n";
	aux << "<filePath>" << "" << "</filePath>\n";
	aux << "<resolution uom=\"#dpi\">" << "" << "</resolution>\n";
	aux << "<GNSS uom=\"#m\" type=\"Unknown\">\n";
	aux << "<gml:pos>" << "E" << " " << "N" << " " << "H" << "</gml:pos>\n";
	aux << "<sigma>Not Available</sigma>\n";
	aux << "</GNSS>\n";
	aux << "<INS uom=\"#rad\" type=\"Unknown\">\n";
	aux << "<omega>" << "" << "</omega>\n";
	aux << "<phi>" << "" << "</phi>\n";
	aux << "<kappa>" << "" << "</kappa>\n";
	aux << "<sigma>Not Available</sigma>\n";
	aux << "</INS>\n";
	aux << "</image>";

	QMessageBox::warning(this, tr(" Warning "), tr("The point in line %1 from imported file\nhas incomplete or corrupted data").arg(line));

	*/
	string newXml=aux.str();
	EDomElement newImageXml(newXml.c_str());

	return newImageXml;
}

//inserido pelo Paulo 05/09/2011
// Coloca as coordenadas digitais linha e coluna nos pontos;
void ProjectUserInterface_Qt::importDigitalCoordinatesFromTxt()
{
	QString importFileName = QFileDialog::getOpenFileName(this,tr("Open Import File"),".","*.txt");
	if(importFileName=="")
		return;
	QFile *importFile = new QFile(importFileName);
	QStringList pointsList;

	importFile->open(QIODevice::ReadOnly);

	while(!importFile->atEnd())
	{
		QByteArray line = importFile->readLine();
		QString aux(line);
		pointsList << aux.remove('\n');
	}

	importFile->close();

	QWidget *loadWidget= new QWidget();
	loadWidget->setAttribute(Qt::WA_DeleteOnClose,true);
	QProgressBar loading;
	QPushButton cancelButton("Cancel");
	loading.setRange(0,pointsList.length());

	QVBoxLayout loadLayout;
	loadLayout.addWidget(&loading,Qt::AlignCenter);
	loadLayout.addWidget(&cancelButton,Qt::AlignCenter);
	connect(&cancelButton,SIGNAL(clicked()),loadWidget,SLOT(close()));

	loadWidget->setLayout(&loadLayout);
	loadWidget->setWindowTitle(tr("Loading Digitals Coordinates"));
	loading.setMinimumSize(300,30);
	loadWidget->show();

	for (int i=0; i<pointsList.length() ;i++)
	{
			loading.setFormat(tr("Coordinate %v/%m : %p%"));
			loading.setValue(i+1);
			if(!insertDigitalCoordinates(pointsList.at(i)))
				QMessageBox::warning(this, tr(" Warning "), tr("The point in line %1 from imported file\nhas incomplete or corrupted data").arg(i));
	}
	loadWidget->close();
	updateTree();
	viewPoints();
	actionSave_file->setEnabled(true);
}

bool ProjectUserInterface_Qt::insertDigitalCoordinates(QString coordinates)
{
	QStringList digitalPoint=coordinates.split("\t");
	if (digitalPoint.size()!=4)
		return false;
	else
	{
		string imagekey=digitalPoint.at(0).toStdString().c_str();
		string pointkey=digitalPoint.at(1).toStdString().c_str();
		string colValue=digitalPoint.at(2).toStdString().c_str();
		string linValue=digitalPoint.at(3).toStdString().c_str();

		EDomElement pointsXML(manager->getXml("points").c_str());
		EDomElement point(pointsXML.elementByTagAtt("point","key",pointkey));
		stringstream aux;
		aux << "<imageCoordinates uom=\"#px\" image_key=\""<< imagekey <<"\">\n";
		aux << "<gml:pos>" << colValue << " " << linValue << "</gml:pos>\n";
		aux << "</imageCoordinates>";
		point.addChildAtTagName("imagesMeasurements",aux.str());
		manager->editComponent("Point",stringToInt(pointkey),point.getContent());
		return true;
	}
}

bool ProjectUserInterface_Qt::availablePhotoTri()
{
	EDomElement ois(manager->getXml("interiorOrientation"));
	EDomElement images(manager->getXml(("images")));
	//qDebug("numero Images = %d, OIS feitas %d",images.children().size(),ois.children().size());

	if(images.children().size()==ois.children().size())
		return true;
	return false;
}

/*
// Se houver imagens cadastradas, poderá se fazer a Orientação interior
bool ProjectUserInterface_Qt::availabeOI()
{
	EDomElement images(manager->getXml(("images")));
	if(images.children().size()>0)
		return true;
	else
		return false;
}
// Paulo 24/09/2011
// Se houver pelo menos uma OI e um ponto cadastrado na imagem, poderá se fazer a Orientaçao exterior
// OBS.: Aqui não é feita a filtragem de quais imagens poderão ser feitas as OEs!!!!
bool ProjectUserInterface_Qt::availableOE()
{
	EDomElement ois(manager->getXml("interiorOrientation"));
	EDomElement points(manager->getXml("points"));
	deque<EDomElement> point=points.elementsByTagName("point");

	//points.elementByTagAtt("imageCoordinates","key",intToString(i))
	//points.hasTagName("imageCoordinates")
	for (int i=0;i<points.children().size();i++)
	{
		if (point.at(i).hasTagName("imageCoordinates"))
			return true;
	}
	return false;
}
*/
void ProjectUserInterface_Qt::updateCurrentForm()
{
	if(currentForm!=NULL)
		if (currentForm== &headerForm)
			viewHeader();
		else if (currentForm==&terrainForm)
			viewTerrain();
		else if (currentForm==&sensorForm)
			viewSensor(currentItemId);
		else if (currentForm==&flightForm)
			viewFlight(currentItemId);
		else if (currentForm==&imagesForm)
			viewImages();
		else if (currentForm==&imageForm)
			viewImage(currentItemId);
		else if (currentForm==&pointsForm)
			viewPoints();
		else if (currentForm==&pointForm)
			viewPoint(currentItemId);
}
