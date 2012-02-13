#include <iostream>

using namespace std;

#include <QApplication>
#include <QTranslator>

#include <QTreeView>

#include "LoadingScreen.h"
#include "EFotoManager.h"
#include "testTable.h"
#include "DemFeatures.h"
#include "ImageViewers.h"
#include "SingleDisplay.h"
#include "ProjectiveRay.h"
#include "DigitalImageSpaceCoordinate.h"
//#include "Tool3D.h"

/*
vector<Coordinate3d> extractCoordinates(br::uerj::eng::efoto::DemFeatures* features, int& lCount)
{
	vector<Coordinate3d> points;
	lCount = 0;
	int size = features->getNumFeatures();
	for (int i = 0; i < size; i++)
	{
		br::uerj::eng::efoto::DemFeature df = features->getFeature(i);
		for (int k = 0; k < df.points.size(); k++)
		{
			Coordinate3d coord(df.points.at(k).X, df.points.at(k).Y, df.points.at(k).Z);
			points.push_back(coord);
		}
	}
	vector<Coordinate3d> lines;
	for (int i = 0; i < points.size() -1; i++)
	{
		lines.push_back(points.at(i));
		lines.push_back(points.at(i+1));
		lCount++;
	}
	return lines;
}
*/

void extractProjections(br::uerj::eng::efoto::SeparatedStereoViewer* sv, br::uerj::eng::efoto::DemFeatures* features, int leftKey, int rightKey)
{
	// Precciso de um projeto carrregado e estou imrovisando isso aqui:
	br::uerj::eng::efoto::EFotoManager manager;
	stringstream myData;
        ifstream myFile("/home/marts/Documentos/e-foto/eppsHomePage/UERJ_io_eo.epp");
	if (myFile.is_open())
	{
		string line;
		while (!myFile.eof())
		{
			getline (myFile,line);
			myData << line << endl;
		}
	}
	myFile.close();
	manager.xmlSetData(myData.str());

	// Dai é ter o trabalho de abrir as imagens e criar raios projetivos
	br::uerj::eng::efoto::Image* leftImage = manager.instanceImage(leftKey);
	br::uerj::eng::efoto::Image* rightImage = manager.instanceImage(rightKey);
	br::uerj::eng::efoto::Sensor* sensor = manager.instanceSensor(1);
	br::uerj::eng::efoto::InteriorOrientation* lio = manager.instanceIO(leftKey);
	br::uerj::eng::efoto::InteriorOrientation* rio = manager.instanceIO(rightKey);
	br::uerj::eng::efoto::SpatialRessection* lsr = (br::uerj::eng::efoto::SpatialRessection*)manager.instanceEO(leftKey);
	br::uerj::eng::efoto::SpatialRessection* rsr = (br::uerj::eng::efoto::SpatialRessection*)manager.instanceEO(rightKey);
	leftImage->setSensor(sensor); leftImage->setIO(lio); leftImage->setEO(lsr);
	rightImage->setSensor(sensor); rightImage->setIO(rio); rightImage->setEO(rsr);
	if (leftImage == NULL || rightImage == NULL)
		return;
	br::uerj::eng::efoto::ProjectiveRay prL(leftImage);
	br::uerj::eng::efoto::ProjectiveRay prR(rightImage);
	sv->loadLeftImage(QString(leftImage->getFilename().c_str())); // Deste modo as imagens ainda precisam estar no diretório bin, depois isso será melhorado.
	sv->loadRightImage(QString(rightImage->getFilename().c_str()));

	// Aqui enquanto não definimos uma estrutura para as geometrias 2d eu vou coordenar a tradução para o que eu já possuia no display para pontos e retas.
	for (int i = 0; i < features->getNumFeatures(); i++)
	{
		br::uerj::eng::efoto::DemFeature df = features->getFeature(i);
		for (int j = 0; j < df.points.size(); j++)
		{
			int k = (j+1)%df.points.size();
			// Isso vai ficar um pouco ruim, mas vai melhorar em breve quando a classe DigitalImageCoordinates for substituida por algo equivalente usando double (completando o suporte a subpixels)
			br::uerj::eng::efoto::DigitalImageSpaceCoordinate pL0 = prL.objectToDigital(df.points.at(j).X, df.points.at(j).Y, df.points.at(j).Z,false);
			br::uerj::eng::efoto::DigitalImageSpaceCoordinate pL1 = prL.objectToDigital(df.points.at(k).X, df.points.at(k).Y, df.points.at(k).Z,false);
			sv->getLeftDisplay()->getCurrentScene()->geometry()->addLine(QPointF(pL0.getCol(), pL0.getLin()), QPointF(pL1.getCol(), pL1.getLin()));

			br::uerj::eng::efoto::DigitalImageSpaceCoordinate pR0 = prR.objectToDigital(df.points.at(j).X, df.points.at(j).Y, df.points.at(j).Z,false);
			br::uerj::eng::efoto::DigitalImageSpaceCoordinate pR1 = prR.objectToDigital(df.points.at(k).X, df.points.at(k).Y, df.points.at(k).Z,false);
			sv->getRightDisplay()->getCurrentScene()->geometry()->addLine(QPointF(pR0.getCol(), pR0.getLin()), QPointF(pR1.getCol(), pR1.getLin()));
		}
	}
	sv->update();
}

int main( int argc, char ** argv )
{

	QApplication efoto(argc, argv);
	efoto.setStyle("plastique");
	efoto.quitOnLastWindowClosed();

	qApp->processEvents();

        br::uerj::eng::efoto::EFotoManager manager;
        manager.setInterfaceType("Qt");
        manager.exec();
/*
	br::uerj::eng::efoto::DemFeatures myFeatures;
	myFeatures.loadFeatures("/home/irvingbadolato/Desktop/FEICOES_UERJ_3.txt",1,false);
	QString txt = QString::fromStdString(myFeatures.getFeaturesList());

	//txt = ("Getting Started\tHow to familiarize yourself with Qt Designer\n\t\tLaunching Designer\tRunning the Qt Designer application\n\t\tThe User Interface\tHow to interact with Qt Designer\n\tConnection Editing Mode\tConnecting widgets together with signals and slots\n\t\tConnecting Objects\tMaking connections in Qt Designer\n\t\tEditing Connections\tChanging existing connections");
	QTreeView *treeViewer = new QTreeView;
	br::uerj::eng::efoto::TreeFeatures* tree = new br::uerj::eng::efoto::TreeFeatures(txt);
	treeViewer->setModel(tree);
	treeViewer->show();

	br::uerj::eng::efoto::SeparatedStereoViewer* sv = new br::uerj::eng::efoto::SeparatedStereoViewer();
	extractProjections(sv, &myFeatures, 1, 2);
	sv->show();
*/
	/*
	Tool3D* tool3d = new Tool3D();
	tool3d->setCentralWidget(tool3d->getDisplay3D());
	int lCount;
	Model* m = tool3d->addModel(extractCoordinates(&myFeatures, lCount));
	m->setLinesCount(lCount);
	tool3d->model0->setEnabled(true);
	tool3d->model0->toggle();
	tool3d->show();
	*/

	return efoto.exec();

}
