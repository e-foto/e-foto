/*Copyright 2002-2014 e-foto team (UERJ)
  This file is part of e-foto.

	e-foto is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	e-foto is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with e-foto.  If not, see <http://www.gnu.org/licenses/>.
*/
/**************************************************************************
		  ReportManager.cpp
**************************************************************************/

#include "ReportManager.h"
#include "EFotoManager.h"
#include "ReportUserInterface.h"
#include "ReportUserInterface_Qt.h"
#include "ProjectiveRay.h"
#include "ProjectHeader.h"
#include "SpatialRessection.h"
#include "SensorWithFiducialMarks.h"
#include "SensorWithKnowDimensions.h"
#include "SensorWithKnowParameters.h"
#include "BundleAdjustment.h"
#include "SingleScene.h"
#include "Point.h"
#include "InteriorOrientation.h"
#include "Terrain.h"
#include "Flight.h"
#include "Image.h"

#include <sstream>
#include <fstream>

#include <QtGui>

// Constructors and destructors
//

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

ReportManager::ReportManager()
{
	efotoManager = NULL;
	project = NULL;
	started = false;
	status = false;
}

ReportManager::ReportManager(EFotoManager* manager)
{
	efotoManager = manager;
	project = manager->getProject();
	started = false;
	status = false;
}

ReportManager::~ReportManager()
{

}

// Association Methods
//

void ReportManager::setInterface(ReportUserInterface* newInterface)
{
	myInterface = newInterface;
}

ReportUserInterface* ReportManager::getInterface()
{
	return myInterface;
}

// Other Methods
//

bool ReportManager::exec()
{

		if (efotoManager != NULL)
		{
				if (efotoManager->getInterfaceType().compare("Qt") == 0)
				{
					myInterface = ReportUserInterface_Qt::instance(this);
				}
				started = true;
				if (myInterface != NULL)
				{
						myInterface->exec();
				}
		}
		return status;
}

void ReportManager::returnProject()
{
		efotoManager->reloadProject();
}

void ReportManager::checkTree(QList<QTreeWidgetItem*> treeItems)
{
	int spatialResectionCounter = 0;
	int phototriCounter = 0;
	if(NULL == project->header())
	{
		//header
		treeItems.at(1)->setCheckState(0,Qt::Unchecked);
		treeItems.at(1)->setDisabled(true);
	}
	if(NULL == project->terrain())
	{
		//terrain
		treeItems.at(2)->setCheckState(0,Qt::Unchecked);
		treeItems.at(2)->setDisabled(true);
	}
	if(NULL == project->sensor(1))
	{
		//sensor
		treeItems.at(3)->setCheckState(0,Qt::Unchecked);
		treeItems.at(3)->setDisabled(true);
	}
	if(NULL == project->flight(1))
	{
		//flight
		treeItems.at(4)->setCheckState(0,Qt::Unchecked);
		treeItems.at(4)->setDisabled(true);
	}
	if(project->allImages().size() < 1)
	{
		//images
		treeItems.at(5)->setCheckState(0,Qt::Unchecked);
		treeItems.at(5)->setDisabled(true);
		//images/metadata
		treeItems.at(6)->setCheckState(0,Qt::Unchecked);
		treeItems.at(6)->setDisabled(true);
		//images/orientation parameters
		treeItems.at(7)->setCheckState(0,Qt::Unchecked);
		treeItems.at(7)->setDisabled(true);
	}
	if(project->allPoints().size() < 1)
	{
		//BlockPoints
		treeItems.at(8)->setCheckState(0,Qt::Unchecked);
		treeItems.at(8)->setDisabled(true);
		//BlockPoints/Coordinates in Object Space Frame
		treeItems.at(9)->setCheckState(0,Qt::Unchecked);
		treeItems.at(9)->setDisabled(true);
		//BlockPoints/Coordinates in Image Space Frame
		treeItems.at(10)->setCheckState(0,Qt::Unchecked);
		treeItems.at(10)->setDisabled(true);
	}
	if(project->allIOs().size() < 1)
	{
		//AffineTransformation
		treeItems.at(11)->setCheckState(0,Qt::Unchecked);
		treeItems.at(11)->setDisabled(true);
		//AffineTransformation/InitializationData
		treeItems.at(12)->setCheckState(0,Qt::Unchecked);
		treeItems.at(12)->setDisabled(true);
		//AffineTransformation/QualityData
		treeItems.at(13)->setCheckState(0,Qt::Unchecked);
		treeItems.at(13)->setDisabled(true);
		//InteriorOrientation
		treeItems.at(21)->setCheckState(0,Qt::Unchecked);
		treeItems.at(21)->setDisabled(true);
		//InteriorOrientation/Xa
		treeItems.at(22)->setCheckState(0,Qt::Unchecked);
		treeItems.at(22)->setDisabled(true);
		//InteriorOrientation/MatrixV
		treeItems.at(23)->setCheckState(0,Qt::Unchecked);
		treeItems.at(23)->setDisabled(true);
		//InteriorOrientation/SigmaXa
		treeItems.at(24)->setCheckState(0,Qt::Unchecked);
		treeItems.at(24)->setDisabled(true);
	}
	if(project->allEOs().size() < 1)
	{
		//SpatialResection
		treeItems.at(14)->setCheckState(0,Qt::Unchecked);
		treeItems.at(14)->setDisabled(true);
		//SpatialResection/InitializationData
		treeItems.at(15)->setCheckState(0,Qt::Unchecked);
		treeItems.at(15)->setDisabled(true);
		//SpatialResection/QualityData
		treeItems.at(16)->setCheckState(0,Qt::Unchecked);
		treeItems.at(16)->setDisabled(true);
		//SpatialResection/Values at each iteration
		treeItems.at(17)->setCheckState(0,Qt::Unchecked);
		treeItems.at(17)->setDisabled(true);
		//Phototriangulation
		treeItems.at(18)->setCheckState(0,Qt::Unchecked);
		treeItems.at(18)->setDisabled(true);
		//Phototriangulation/InitializationData
		treeItems.at(19)->setCheckState(0,Qt::Unchecked);
		treeItems.at(19)->setDisabled(true);
		//Phototriangulation/Values at each iteration
		treeItems.at(20)->setCheckState(0,Qt::Unchecked);
		treeItems.at(20)->setDisabled(true);
		//ExteriorOrientation
		treeItems.at(25)->setCheckState(0,Qt::Unchecked);
		treeItems.at(25)->setDisabled(true);
		//ExteriorOrientation/EOType
		treeItems.at(26)->setCheckState(0,Qt::Unchecked);
		treeItems.at(26)->setDisabled(true);
		//ExteriorOrientation/NumberofIterationsForConvergence
		treeItems.at(27)->setCheckState(0,Qt::Unchecked);
		treeItems.at(27)->setDisabled(true);
		//ExteriorOrientation/Xa
		treeItems.at(28)->setCheckState(0,Qt::Unchecked);
		treeItems.at(28)->setDisabled(true);
		//ExteriorOrientation/MatrixV
		treeItems.at(29)->setCheckState(0,Qt::Unchecked);
		treeItems.at(29)->setDisabled(true);
		//ExteriorOrientation/SigmaXa
		treeItems.at(30)->setCheckState(0,Qt::Unchecked);
		treeItems.at(30)->setDisabled(true);
		//StereoPairs
		treeItems.at(31)->setCheckState(0,Qt::Unchecked);
		treeItems.at(31)->setDisabled(true);
	}
	if(project->allEOs().size() < 1 && project->allIOs().size() < 1)
	{
		//images/orientation parameters
		treeItems.at(7)->setCheckState(0,Qt::Unchecked);
		treeItems.at(7)->setDisabled(true);
	}
	for(unsigned int i = 0; i < project->allEOs().size(); i++)
	{
		SpatialRessection* sr = (SpatialRessection*) project->allEOs().at(i);
		if (sr->getType() == "spatialResection")
			spatialResectionCounter++;
		else
			if(sr->getType() == "photoTriangulation")
				phototriCounter++;
	}
	if(spatialResectionCounter==0)
	{
		//SpatialResection
		treeItems.at(14)->setCheckState(0,Qt::Unchecked);
		treeItems.at(14)->setDisabled(true);
		//SpatialResection/InitializationData
		treeItems.at(15)->setCheckState(0,Qt::Unchecked);
		treeItems.at(15)->setDisabled(true);
		//SpatialResection/QualityData
		treeItems.at(16)->setCheckState(0,Qt::Unchecked);
		treeItems.at(16)->setDisabled(true);
		//SpatialResection/Values at each iteration
		treeItems.at(17)->setCheckState(0,Qt::Unchecked);
		treeItems.at(17)->setDisabled(true);
	}
	if(phototriCounter==0)
	{
		//Phototriangulation
		treeItems.at(18)->setCheckState(0,Qt::Unchecked);
		treeItems.at(18)->setDisabled(true);
		//Phototriangulation/InitializationData
		treeItems.at(19)->setCheckState(0,Qt::Unchecked);
		treeItems.at(19)->setDisabled(true);
		//Phototriangulation/Values at each iteration
		treeItems.at(20)->setCheckState(0,Qt::Unchecked);
		treeItems.at(20)->setDisabled(true);
	}
}

std::string ReportManager::eprHeader()
{
	ProjectHeader *cabecalho = project->header();
	std::string txt;
	txt = "<projectHeader>\n";
	txt += "<name>" + cabecalho->getName() + "</name>\n";
	txt += "<description>" + cabecalho->getDescription() + "</description>\n";
	txt += "<creation>" + cabecalho->getCreation() + "</creation>\n";
	txt += "<modification>" + cabecalho->getModification() + "</modification>\n";
	txt += "<owner>" + cabecalho->getOwner() + "</owner>\n";
	txt += "</projectHeader>\n";
	return txt;
}

std::string ReportManager::eprTerrain()
{
	Terrain *terreno = project->terrain();
	std::string txt,direction;
	Dms lat(terreno->getCentralCoordLat());
	Dms lon(terreno->getCentralCoordLong());
	txt = "<terrain>\n";
	txt += "<meanAltitude>" + Conversion::doubleToString(terreno->getMeanAltitude(),3) + "</meanAltitude>\n";
	txt += "<minAltitude>" + Conversion::doubleToString(terreno->getMinAltitude(),3) + "</minAltitude>\n";
	txt += "<maxAltitude>" + Conversion::doubleToString(terreno->getMaxAltitude(),3) + "</maxAltitude>\n";
	txt += "<GRS>" + terreno->getGRS() + "</GRS>\n";
	txt += "<CPS>" + terreno->getCPS() + "</CPS>\n";
	txt += "<workAreaCenterCoordinates>\n";
	txt += "<lat>\n";
	txt += "<degrees>" + Conversion::intToString(lat.getDegree()) + "</degrees>\n";
	txt += "<minutes>" + Conversion::intToString(lat.getMinute()) + "</minutes>\n";
	txt += "<seconds>" + Conversion::doubleToString(lat.getSeconds(),2) + "</seconds>\n";
	if(lat.hasSignal())
		txt += "<direction>N</direction>\n";
	else
		txt += "<direction>S</direction>\n";
	txt += "</lat>\n";
	txt += "<long>\n";
	txt += "<degrees>" + Conversion::intToString(lon.getDegree()) + "</degrees>\n";
	txt += "<minutes>" + Conversion::intToString(lon.getMinute()) + "</minutes>\n";
	txt += "<seconds>" + Conversion::doubleToString(lon.getSeconds(),2) + "</seconds>\n";
	if(lon.hasSignal())
		txt += "<direction>W</direction>\n";
	else
		txt += "<direction>E</direction>\n";
	txt += "</long>\n";
	txt += "<utmFuse>" + Conversion::intToString(terreno->getUtmFuse()) + "</utmFuse>\n";
	txt += "</workAreaCenterCoordinates>\n";
	txt += "</terrain>\n";
	return txt;
}

std::string ReportManager::eprSensors()
{
	Sensor *sensores = project->sensor(1);
	std::string txt;
	txt = "<sensors>\n";
	txt += "<sensorId>" + sensores->getSensorId() + "</sensorId>\n";
	txt += "<description>" + sensores->getDescription() + "</description>\n";
	txt += "<focalDistance>\n";
	txt += "<value>" + Conversion::doubleToString(sensores->getFocalDistance(),3) + "</value>\n";
	txt += "<sigma>" + Conversion::doubleToString(sensores->getFocalDistanceSigma(),3) + "</sigma>\n";
	txt += "</focalDistance>\n";
	txt += "</sensors>\n";
	return txt;
}

std::string ReportManager::eprFlights()
{
	Flight *voos = project->flight(1);
	std::string txt;
	txt = "<flights>\n";
	txt += "<flightId>" + voos->getFlightId() + "</flightId>\n";
	txt += "<execution>" + voos->getExecution() + "</execution>\n";
	txt += "<nominalScale>\n";
	txt += "<mml:mfrac>\n";
	txt += "<mml:mn>1</mml:mn>\n";
	txt += "<mml:mn>" + Conversion::doubleToString(voos->getScaleDen(),0) + "</mml:mn>\n";
	txt += "</mml:mfrac>\n";
	txt += "</nominalScale>\n";
	txt += "<overlap>\n";
	txt += "<longitudinal>" + Conversion::doubleToString(voos->getLongitudinalOverlap(),0) + "</longitudinal>\n";
	txt += "<transversal>" + Conversion::doubleToString(voos->getTransversalOverlap(),0) + "</transversal>\n";
	txt += "</overlap>\n";
	txt += "</flights>\n";
	return txt;
}

std::string ReportManager::eprImages(QList<QTreeWidgetItem*> treeItems)
{
	std::string txt;
	Image *img;
	txt = "<images>\n";
	for(unsigned int i=0;i<project->allImages().size();i++)
	{
		txt += "<image>\n";
		img = project->allImages().at(i);
		if(treeItems.at(6)->checkState(0)==2)
		{
			txt += "<imageId>" + img->getImageId() + "</imageId>\n";
			txt += "<width>" + Conversion::intToString(img->getWidth()) + "</width>\n";
			txt += "<heigth>" + Conversion::intToString(img->getHeight()) + "</heigth>\n";
			txt += "<fileName>" + img->getFilename() + "</fileName>\n";
			txt += "<filePath>" + img->getFilepath() + "</filePath>\n";
			// Pensar! adicionar o caminho absoluto
			txt += "<resolution>" + Conversion::intToString(img->getResolution()) + "</resolution>\n";
		}
		if(treeItems.at(7)->checkState(0)==2)
		{
			if(project->allIOs().size() > 0)
			{
				InteriorOrientation* io = img->getIO();
				if (NULL != io)
				{
					txt += "<interiorOrientation>\n";
					txt += "<ioType>Affine</ioType>\n";
					txt +=  "<Xa>\n";
										txt +=  "<a0>" + Conversion::doubleToString(io->getXa().get(1,1)) + "</a0>\n";
										txt +=  "<a1>" + Conversion::doubleToString(io->getXa().get(2,1)) + "</a1>\n";
										txt +=  "<a2>" + Conversion::doubleToString(io->getXa().get(3,1)) + "</a2>\n";
										txt +=  "<b0>" + Conversion::doubleToString(io->getXa().get(4,1)) + "</b0>\n";
										txt +=  "<b1>" + Conversion::doubleToString(io->getXa().get(5,1)) + "</b1>\n";
										txt +=  "<b2>" + Conversion::doubleToString(io->getXa().get(6,1)) + "</b2>\n";
					txt +=  "</Xa>\n";
					txt += "</interiorOrientation>\n";
				}
			}
			if(project->allEOs().size() > 0)
			{
				ExteriorOrientation* eo = img->getEO();
				if (NULL != eo)
				{
					txt += "<exteriorOrientation>\n";
										txt += "<eoType>" + eo->getType() + "</eoType>\n";
					txt +=  "<Xa>\n";
										txt +=  "<X0>" + Conversion::doubleToString(eo->getXa().get(1,1)) + "</X0>\n";
										txt +=  "<Y0>" + Conversion::doubleToString(eo->getXa().get(2,1)) + "</Y0>\n";
										txt +=  "<Z0>" + Conversion::doubleToString(eo->getXa().get(3,1)) + "</Z0>\n";
										txt +=  "<phi>" + Conversion::doubleToString(eo->getXa().get(4,1)) + "</phi>\n";
										txt +=  "<omega>" + Conversion::doubleToString(eo->getXa().get(5,1)) + "</omega>\n";
										txt +=  "<kappa>" + Conversion::doubleToString(eo->getXa().get(6,1)) + "</kappa>\n";
					txt +=  "</Xa>\n";
					txt += "</exteriorOrientation>\n";
				}
			}
		/*    string reportPath = "";
			txt += "<img src=\"" + reportPath + "thumb_" + img->getFilename() + "\">";
			QImage thumb = makeThumbnail(QString(img->getFilepath().c_str()).append("/").append(img->getFilename().c_str()),200,200);
			thumb.save(QString::fromStdString(reportPath + "thumb_" + img->getFilename()));
		*/
		}

		txt += "</image>\n";

	}
	txt += "</images>\n";
	return txt;
}

std::string ReportManager::eprBlockPoints(QList<QTreeWidgetItem*> treeItems)
{
	std::string txt;
	txt = "<blockPoints>\n";
	for(unsigned int i=0;i<project->allPoints().size();i++)
	{
		Point *pnt = project->allPoints().at(i);
		txt += "<point>\n";
		txt += "<pointId>" + pnt->getPointId() + "</pointId>\n";


		if(pnt->getType() == Point::CONTROL)
		{
			txt += "<pointType>control</pointType>\n";
		}
		else if(pnt->getType() == Point::PHOTOGRAMMETRIC)
		{
			txt += "<pointType>photogrammetric</pointType>\n";
		}
		else if(pnt->getType() == Point::CHECKING)
		{
			txt += "<pointType>checking</pointType>\n";
		}

		if(treeItems.at(9)->checkState(0)==2)
		{
			txt += "<spatialCoordinates>\n";
			txt += "<x>" + Conversion::doubleToString(pnt->getObjectCoordinate().getX()) + "</x>\n";
			txt += "<y>" + Conversion::doubleToString(pnt->getObjectCoordinate().getY()) + "</y>\n";
			txt += "<z>" + Conversion::doubleToString(pnt->getObjectCoordinate().getZ()) + "</z>\n";
			txt += "<unit>" + pnt->getObjectCoordinate().getUnit().substr(1, pnt->getObjectCoordinate().getUnit().size()) + "</unit>\n";
			txt += "</spatialCoordinates>\n";
		}
		if(treeItems.at(10)->checkState(0)==2)
		{
			txt += "<imagesMeasurements>\n";
			Image *img;
			for(unsigned int j=0;j < pnt->getImageCoordinates().size();j++)
			{
				img = project->image(pnt->getImageCoordinateAt(j).getImageId());
				if (img)
				{
					txt += "<imageCoordinates>\n";
					txt += "<imageId>" + img->getImageId() + "</imageId>\n";
					txt += "<position>\n";
					txt += "<lin>" + Conversion::doubleToString(pnt->getImageCoordinateAt(j).getLin()) + "</lin>\n";
					txt += "<col>" + Conversion::doubleToString(pnt->getImageCoordinateAt(j).getCol()) + "</col>\n";
					txt += "<unit>" + pnt->getImageCoordinateAt(j).getUnit().substr(1, pnt->getImageCoordinateAt(j).getUnit().size())+ "</unit>\n";
					txt += "</position>\n";
					txt += "</imageCoordinates>\n";
				}
			}
			txt += "</imagesMeasurements>\n";
		}
		txt += "</point>\n";
	}
	txt += "</blockPoints>\n";
	return txt;
}

std::string ReportManager::eprAffineTransformation(QList<QTreeWidgetItem*> treeItems)
{
	std::string txt;
	txt = "<affineTransformation>\n";
	for (unsigned int i = 0; i < project->allIOs().size(); i++)
	{
		InteriorOrientation* io = project->allIOs().at(i);
		if( NULL != io)
		{
			if(NULL != io->getImage())
			{
				txt += "<IO>\n";
				txt += "<imageId>" + io->getImage()->getImageId() + "</imageId>\n";
				if(treeItems.at(12)->checkState(0)==2)
				{
					txt += "<initializationData>\n";
					txt += "<calculationMode>" + io->getImage()->getSensor()->getCalculationMode() + "</calculationMode>\n";
					if (io->getImage()->getSensor()->getCalculationMode() == "With Fiducial Marks")
					{
						std::deque<ImageFiducialMark> fidMarks = io->getImage()->getDigFidMarks();
						SensorWithFiducialMarks* sensor = (SensorWithFiducialMarks*) io->getImage()->getSensor();
						txt += "<fidMarks>\n";
						for(unsigned int j = 0; j < fidMarks.size(); j++)
						{
							txt += "<fidMark>\n";
							QStringList gmlpos = QString::fromStdString(sensor->getAnaFidMarkAt(j).getGmlPos()).split(" ");
							txt += "<fidMarkNo>" + Conversion::intToString(j+1) + "</fidMarkNo>\n";
							txt += "<gmlX>" + gmlpos.at(0).toStdString() + "</gmlX>\n";
							txt += "<gmlY>" + gmlpos.at(1).toStdString() + "</gmlY>\n";
							txt += "<lin>" + Conversion::doubleToString(io->getImage()->getDigFidMarkAt(j).getLin()) + "</lin>\n";
							txt += "<col>" + Conversion::doubleToString(io->getImage()->getDigFidMarkAt(j).getCol()) + "</col>\n";
							txt += "</fidMark>\n";
						}
						txt += "</fidMarks>\n";
					}
					else if (io->getImage()->getSensor()->getCalculationMode() == "With Sensor Dimensions")
					{
						SensorWithKnowDimensions* sensor = (SensorWithKnowDimensions*) io->getImage()->getSensor();
						txt += "<sensorCols>" + Conversion::intToString(sensor->getFrameColumns()) + "</sensorCols>\n";
						txt += "<sensorRows>" + Conversion::intToString(sensor->getFrameRows()) + "</sensorRows>\n";
						txt += "<sensorPxsize>" + Conversion::doubleToString(sensor->getPixelSize()) + "</sensorPxsize>\n";
					}
					else if (io->getImage()->getSensor()->getCalculationMode() == "Fixed Parameters")
					{
						SensorWithKnowParameters* sensor = (SensorWithKnowParameters*) io->getImage()->getSensor();
						txt += "<Xa>\n";
						txt += "<a0>" + Conversion::doubleToString(sensor->getXa().get(1,1)) + "</a0>\n";
						txt += "<a1>" + Conversion::doubleToString(sensor->getXa().get(2,1)) + "</a1>\n";
						txt += "<a2>" + Conversion::doubleToString(sensor->getXa().get(3,1)) + "</a2>\n";
						txt += "<b0>" + Conversion::doubleToString(sensor->getXa().get(4,1)) + "</b0>\n";
						txt += "<b1>" + Conversion::doubleToString(sensor->getXa().get(5,1)) + "</b1>\n";
						txt += "<b2>" + Conversion::doubleToString(sensor->getXa().get(6,1)) + "</b2>\n";
						txt += "</Xa>\n";
					}
					txt += "</initializationData>\n";
				}

				if(treeItems.at(13)->checkState(0)==2)
				{
					txt += "<qualityData>\n";
					txt += "<V>\n";
					txt += io->getQuality().getV().xmlGetData(6);
					txt += "</V>\n";
					txt += "<sigmaXa>\n";
					txt += io->getQuality().getSigmaXa().xmlGetData(6);
					txt += "</sigmaXa>\n";
					txt += "<sigmaLa>\n";
					txt += io->getQuality().getSigmaLa().xmlGetData(6);
					txt += "</sigmaLa>\n";
					txt += "<sigma0Squared>" + Conversion::doubleToString(io->getQuality().getsigma0Squared()) + "</sigma0Squared>\n";
					txt += "</qualityData>\n";
				}
				txt += "</IO>\n";
			}
		}
	}

	txt += "</affineTransformation>\n";
	return txt;
}

std::string ReportManager::eprSpatialRessection(QList<QTreeWidgetItem*> treeItems)
{
	std::string txt;
	txt = "<spatialResection>\n";
	for(unsigned int i = 0; i < project->allEOs().size(); i++)
	{
		SpatialRessection* sr = (SpatialRessection*) project->allEOs().at(i);
		if (sr->getType() != "spatialResection")
			continue;
		txt += "<SR>\n";
		txt += "<imageId>" + sr->getImage()->getImageId() + "</imageId>\n";
		txt += "<flightDirection>" + Conversion::doubleToString(sr->getImage()->getFlightDirection()) + "</flightDirection>\n";
		txt += "<unit>rad</unit>\n";
		if(treeItems.at(15)->checkState(0)==2)
		{
			//INICIALIZACAO
			//ver a imagem
			//ver se tem Gnss
			//se tem Gnss, verificar se o tipo é inicial ou fixo ou nao considerado
			//ver se tem ins
			//se tem ins, verificar se o tipo é inicial ou fixo ou nao considerado
			//ver quais pontos foram selecionados (talvez nao tenha implementado)
			//perguntar pros pontos selecionados quais foram as coodernadas que foram medidos (E,N,H | col e lin)
			//perguntar para a imagem qual a direcao de voo
			txt += "<initializationData>\n";
			if(sr->getImage()->isGnssAvailable())
			{
				txt += "<gnss>\n";
				txt += "<type>" + sr->getImage()->getGnssType() + "</type>\n";
				txt += "<x0>" + Conversion::doubleToString(sr->getImage()->getGnssX0()) + "</x0>";
				txt += "<y0>" + Conversion::doubleToString(sr->getImage()->getGnssY0()) + "</y0>";
				txt += "<z0>" + Conversion::doubleToString(sr->getImage()->getGnssZ0()) + "</z0>";
				txt += "<gnss>\n";
			}
			if(sr->getImage()->isInsAvailable())
			{
				txt += "<ins>\n";
				txt += "<type>" + sr->getImage()->getInsType() + "</type>\n";
				txt += "<omega>" + Conversion::doubleToString(sr->getImage()->getInsOmega()) + "</omega>\n";
				txt += "<phi>" + Conversion::doubleToString(sr->getImage()->getInsPhi()) + "</phi>\n";
				txt += "<kappa>" + Conversion::doubleToString(sr->getImage()->getInsKappa()) + "</kappa>\n";
				txt += "</ins>\n";
			}
			if(sr->getSelectedPoints().size() > 0)
			{
				txt += "<selectedPoints>\n";
				for(unsigned int j = 0; j < sr->getSelectedPoints().size(); j++)
				{
					Point* point = project->point(sr->getSelectedPoints().at(j));
					txt += "<point>\n";
					txt += "<x>" + Conversion::doubleToString(point->getObjectCoordinate().getX()) + "</x>\n";
					txt += "<y>" + Conversion::doubleToString(point->getObjectCoordinate().getY()) + "</y>\n";
					txt += "<z>" + Conversion::doubleToString(point->getObjectCoordinate().getZ()) + "</z>\n";
					txt += "<col>" + Conversion::doubleToString(point->getImageCoordinate(sr->getImageId()).getCol()) + "</col>\n";
					txt += "<lin>" + Conversion::doubleToString(point->getImageCoordinate(sr->getImageId()).getLin()) + "</lin>\n";
					txt += "</point>\n";
				}
				txt += "</selectedPoints>\n";
			}
			txt += "</initializationData>\n";
		}

		if(treeItems.at(16)->checkState(0)==2)
		{
			txt += "<qualityData>\n";
			txt += "<V>\n";
			txt += sr->getQuality().getV().xmlGetData(6);
			txt += "</V>\n";
			txt += "<sigmaXa>\n";
			txt += sr->getQuality().getSigmaXa().xmlGetData(6);
			txt += "</sigmaXa>\n";
			txt += "<sigmaLa>\n";
			txt += sr->getQuality().getSigmaLa().xmlGetData(6);
			txt += "</sigmaLa>\n";
			txt += "<sigma0Squared>" + Conversion::doubleToString(sr->getQuality().getsigma0Squared()) + "</sigma0Squared>\n";
			txt += "</qualityData>\n";
		}
		if(treeItems.at(17)->checkState(0)==2)
		{

			sr->initialize();
			sr->calculate(sr->getTotalIterations(),0.001,0.001);
			std::deque<double> rmse = sr->getRMSE();
			txt += "<rmseOfEachIteration>\n";
			for (size_t a = 0; a < rmse.size(); a++)
			{
				//qDebug("%f",rmse.at(a));
				txt += "<rmse>\n";
				txt += "<iteration>" + Conversion::intToString(a) + "</iteration>\n";
				txt += "<value>" + Conversion::doubleToString(rmse.at(a)) + "</value>\n";
				txt += "</rmse>\n";
			}
			txt += "</rmseOfEachIteration>\n";
		}
		txt += "</SR>\n";
	}
	txt += "</spatialResection>\n";
	return txt;
}

std::string ReportManager::eprPhotogrammetricBlock(QList<QTreeWidgetItem*> treeItems)
{
	std::string txt;
	if (project->photoTri() == NULL)
		return "";

	std::deque<Image*> usedImages = project->photoTri()->getImages();
	std::deque<Point*> usedPoints = project->photoTri()->getPoints();
	BundleAdjustment* bundle = new BundleAdjustment(usedImages, usedPoints);

	txt = "<photogrammetricBlock>\n";

	if(treeItems.at(19)->checkState(0)==2)
	{
		txt += "<totalIterations>" + Conversion::doubleToString(project->photoTri()->getTotalIterations()) + "</totalIterations>\n";
		txt += "<angularConvergency>" + Conversion::doubleToString(project->photoTri()->getAngularConvergency()) + "</angularConvergency>\n";
		txt += "<metricConvergency>" + Conversion::doubleToString(project->photoTri()->getMetricConvergency()) + "</metricConvergency>\n";

		bundle->calculateInicialsValues();
		Matrix inicialValues = bundle->getMatrixInicialValues();
		Image* img;
		for(unsigned int i = 1; i < inicialValues.getRows()+1;i++)
		{
			img = usedImages.at(i-1);
			txt += "<initialValues>\n";
			txt += "<imageId>" + img->getImageId() + "</imageId>\n";
			txt += "<matrix>\n";
			txt += "<omega>" + Conversion::doubleToString(inicialValues.get(i,1)) + "</omega>\n";
			txt += "<phi>" + Conversion::doubleToString(inicialValues.get(i,2)) + "</phi>\n";
			txt += "<kappa>" + Conversion::doubleToString(inicialValues.get(i,3)) + "</kappa>\n";
			txt += "<x0>" + Conversion::doubleToString(inicialValues.get(i,4)) + "</x0>\n";
			txt += "<y0>" + Conversion::doubleToString(inicialValues.get(i,5)) + "</y0>\n";
			txt += "<z0>" + Conversion::doubleToString(inicialValues.get(i,6)) + "</z0>\n";
			txt += "</matrix>\n";
			txt += "</initialValues>\n";
		}
	}

	if(treeItems.at(20)->checkState(0)==2)
	{
		bundle->setAngularConvergencyValue(project->photoTri()->getAngularConvergency());
		bundle->setMetricConvergencyValue(project->photoTri()->getMetricConvergency());
		bundle->setMaxNumberIterations(project->photoTri()->getTotalIterations());
		bundle->calculate(true);
		std::deque<double> RMSEs = bundle->getListRMSE();
		txt += "<rmseOfEachIteration>\n";
		for(unsigned int j=0;j<RMSEs.size();j++)
		{
			txt += "<rmse>\n";
			txt += "<iteration>" + Conversion::intToString(j+1) + "</iteration>\n";
			txt += "<value>" + Conversion::doubleToString(RMSEs.at(j)) + "</value>\n";
			txt += "</rmse>\n";
		}
		txt += "</rmseOfEachIteration>\n";

	}

	txt += "</photogrammetricBlock>\n";
	return txt;
}

std::string ReportManager::eprInteriorOrientation(QList<QTreeWidgetItem*> treeItems)
{
	std::string txt;
	txt = "<interiorOrientation>\n";
	Image *img;
	InteriorOrientation* io;
	for(unsigned int i=0;i<project->allImages().size();i++)
	{
		img = project->allImages().at(i);
		io = img->getIO();
		if (NULL != io)
		{
			txt += "<IO>\n";
			txt += "<imageId>" + img->getImageId() + "</imageId>\n";
			txt += "<ioType>Affine</ioType>\n";
			if(treeItems.at(22)->checkState(0)==2)
			{
				txt += "<Xa>\n";
				txt +=  "<a0>" + Conversion::doubleToString(io->getXa().get(1,1)) + "</a0>\n";
				txt +=  "<a1>" + Conversion::doubleToString(io->getXa().get(2,1)) + "</a1>\n";
				txt +=  "<a2>" + Conversion::doubleToString(io->getXa().get(3,1)) + "</a2>\n";
				txt +=  "<b0>" + Conversion::doubleToString(io->getXa().get(4,1)) + "</b0>\n";
				txt +=  "<b1>" + Conversion::doubleToString(io->getXa().get(5,1)) + "</b1>\n";
				txt +=  "<b2>" + Conversion::doubleToString(io->getXa().get(6,1)) + "</b2>\n";
				txt += "</Xa>\n";
			}
			if(treeItems.at(23)->checkState(0)==2)
			{
				txt += "<V>\n";
				txt += io->getQuality().getV().xmlGetData(6);
				txt += "</V>\n";
			}
			if(treeItems.at(24)->checkState(0)==2)
			{
				txt += "<sigmaXa>\n";
				txt += io->getQuality().getSigmaXa().xmlGetData(6);
				txt += "</sigmaXa>\n";
			}
			txt += "</IO>\n";
		}
	}
	txt += "</interiorOrientation>\n";
	return txt;
}

std::string ReportManager::eprExteriorOrientation(QList<QTreeWidgetItem*> treeItems)
{
	std::string txt;
	Image *img;
	ExteriorOrientation* eo;
	txt = "<exteriorOrientation>\n";
	for(unsigned int i=0;i<project->allImages().size();i++)
	{
		img = project->allImages().at(i);
		eo = img->getEO();
		if(NULL != eo)
		{
			txt += "<EO>\n";
			txt += "<imageId>" + img->getImageId() + "</imageId>\n";
			if(treeItems.at(26)->checkState(0)==2)
			{
								txt += "<eoType>" + eo->getType() + "</eoType>\n";
			}
			if(treeItems.at(27)->checkState(0)==2)
			{
				txt += "<noIterations>";
				if (eo->getType() == "photoTriangulation" && project->photoTri())
					txt += Conversion::intToString(project->photoTri()->getTotalIterations());
				else if (eo->getType() == "spatialResection")
					txt += Conversion::intToString(eo->getTotalIterations());
				else
					txt += "unknow";
				txt += "</noIterations>\n";
			}
			if(treeItems.at(28)->checkState(0)==2)
			{
				txt +=  "<Xa>\n";
								txt +=  "<X0>" + Conversion::doubleToString(eo->getXa().get(1,1)) + "</X0>\n";
								txt +=  "<Y0>" + Conversion::doubleToString(eo->getXa().get(2,1)) + "</Y0>\n";
								txt +=  "<Z0>" + Conversion::doubleToString(eo->getXa().get(3,1)) + "</Z0>\n";
								txt +=  "<phi>" + Conversion::doubleToString(eo->getXa().get(4,1)) + "</phi>\n";
								txt +=  "<omega>" + Conversion::doubleToString(eo->getXa().get(5,1)) + "</omega>\n";
								txt +=  "<kappa>" + Conversion::doubleToString(eo->getXa().get(6,1)) + "</kappa>\n";
				txt +=  "</Xa>\n";
			}
			if(treeItems.at(29)->checkState(0)==2)
			{
				txt +=  "<V>\n";
								txt +=  eo->getQuality().getV().xmlGetData(6);
				txt +=  "</V>\n";
			}
			if(treeItems.at(30)->checkState(0)==2)
			{
				txt +=  "<sigmaXa>\n";
								txt +=  eo->getQuality().getSigmaXa().xmlGetData(6);
				txt +=  "</sigmaXa>\n";
			}
			txt += "</EO>\n";
		}
	}
	txt += "</exteriorOrientation>\n";
	return txt;
}

double fixAngle(double angle)
{
	bool negative = angle < 0.0;

	// Positive signal
	angle = fabs(angle);

	// Bring angle to the first lap
	if (angle > 2*M_PI)
	{
		int laps = floor(angle/(2*M_PI));
		angle = angle - laps*(2*M_PI);
	}

	if (negative)
		angle = 2*M_PI - angle;

	return angle;
}

double getAngleBetweenImages(double X1, double Y1, double X2, double Y2)
{
	double delta_X = X2 - X1;
	double delta_Y = Y2 - Y1;

		if (fabs(delta_X - 0.0) < 0.0000000000001)
	{
		if (delta_Y > 0.0)
			return M_PI/2.0;
		else
			return 3*M_PI/2.0;
	}

		return fixAngle(atan(delta_Y/delta_X));
}

bool checkAnglesAlligned(double angle1, double angle2, double tolerance)
{
	angle1 = fixAngle(angle1);
	angle2 = fixAngle(angle2);

	double distance = fabs(angle1 - angle2);

	if (distance > M_PI)
		distance = 2*M_PI - distance;

	return (distance < tolerance || fabs(M_PI - distance) < tolerance);
}

std::string ReportManager::eprStereoPairs()
{
	std::string txt;
	txt = "<stereoPairs>\n";

	//
	// Derivado do código nas interfaces implementadas pelo Marcelo.
	//
	if (project->allImages().size() < 2)
		return "<stereoPairs></stereoPairs>\n";

	if(project->allEOs().size() < 1)
		return "<stereoPairs></stereoPairs>\n";

	Image *img;
	double X1, Y1, X2, Y2, R, dist, best_dist, bX2, bY2, overlap, align_ang, kappa;
	int best_img;
	Matrix Xa;

	// Calculate Image Radius
   // ObjectSpaceCoordinate osc;
	img = project->allImages().at(0);
   // ProjectiveRay pr(img);
   // Xa = img->getEO()->getXa();
   // X1 = Xa.get(1,1);
   // Y1 = Xa.get(2,1);
   // p1 = img->getWidth();
   // p2 = img->getHeight()/2;
   // osc = pr.imageToObject(p1,p2,img->getHeight(),false);
   // X2 = osc.getX();
   // Y2 = osc.getY();
   // R = sqrt(pow(X1-X2,2) + pow(Y1-Y2,2));
	// Calculate Images Radius, using the first image as reference

		// New R calcul width*scale*(1 pol in meters)/resolution
		R = img->getWidth() * img->getFlight()->getScaleDen() * 0.0254 / img->getResolution();
	for (unsigned int i=0; i<project->allImages().size()-1; i++)
	{
		// Get base image center
		img = project->allImages().at(i);
		Xa = img->getEO()->getXa();
		X1 = Xa.get(1,1);
		Y1 = Xa.get(2,1);
		kappa = fabs(Xa.get(6,1));

		best_dist = 10e100;

		// Calculate the shortest image center
		for (unsigned int j=i+1; j<project->allImages().size(); j++)
		{
			img = project->allImages().at(j);
			Xa = img->getEO()->getXa();
			X2 = Xa.get(1,1);
			Y2 = Xa.get(2,1);

			// Calculate dist
			dist = sqrt(pow(X1-X2,2) + pow(Y1-Y2,2));

			if (dist < best_dist)
			{
				best_dist = dist;
				best_img = j;
				bX2 = X2;
				bY2 = Y2;
			}
		}

		// Check images overlapping
		overlap = 100*(2*R-best_dist)/(2*R);

		if (overlap < 60.0 || overlap > 100.0)
			continue;

		// Check images alignment and kappa
		align_ang = getAngleBetweenImages(X1, Y1, bX2, bY2);

		if (!checkAnglesAlligned(align_ang, kappa, 0.174532925)) // 10 Degrees
			continue;

		txt += "<stereoPair>\n";
		txt += "<imageId>" + project->allImages().at(i)->getImageId() + "</imageId>\n";
		txt += "<imageId>" + project->allImages().at(best_img)->getImageId() + "</imageId>\n";
		txt += "</stereoPair>\n";
	}

	txt += "</stereoPairs>\n";

	return txt;
}

std::string ReportManager::eprStereoPlotting()
{
	std::string txt;
	txt = "<stereoPlotting>Teste 13</stereoPlotting>\n";
	return txt;
}

std::string ReportManager::eprDSM()
{
	std::string txt;
	txt = "<dsm>Teste 14</dsm>\n";
	return txt;
}

std::string ReportManager::eprOrthorectification()
{
	std::string txt;
	txt = "<orthoRectification>Teste 15</orthoRectification>\n";
	return txt;
}

bool ReportManager::makeFile(std::string filename, int idExt, QList<QTreeWidgetItem*> treeItems)
{
	std::stringstream out;
	std::ofstream myFile (filename.c_str());
	if (myFile.is_open())
	{
		int numChild = treeItems.count();
		out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

		if(idExt == 1){
			out << "<?xml-stylesheet type=\"text/xsl\" href=\"epr_txt.xsl\"?>\n";
		} else {
			if(idExt ==2){
				out << "<?xml-stylesheet type=\"text/xsl\" href=\"epr_html.xsl\"?>\n";
			}
		}

		out << "<efotoPhotogrammetricReport xmlns:mml=\"http://www.w3.org/1998/Math/MathML\">\n";

		for (int i=0;i<numChild;i++)
		{
			QTreeWidgetItem *aux= treeItems.at(i);
			if(aux->checkState(0) == 2 || aux->checkState(0) == 1)
			{
				switch(i)
				{
					case 1:
						out << eprHeader();
					break;
					case 2:
						out << eprTerrain();
					break;
					case 3:
						out << eprSensors();
					break;
					case 4:
						out << eprFlights();
					break;
					case 5:
						out << eprImages(treeItems);
					break;
					case 8:
						out << eprBlockPoints(treeItems);
					break;
					case 11:
						out << eprAffineTransformation(treeItems);
					break;
					case 14:
						out << eprSpatialRessection(treeItems);
					break;
					case 18:
						out << eprPhotogrammetricBlock(treeItems);
					break;
					case 21:
						out << eprInteriorOrientation(treeItems);
					break;
					case 25:
						out << eprExteriorOrientation(treeItems);
					break;
					case 31:
						out << eprStereoPairs();
					break;
					case 32:
						out << eprStereoPlotting();
					break;
					case 33:
						out << eprDSM();
					break;
					case 34:
						out << eprOrthorectification();
					break;
				}
			}
		}

		out << "</efotoPhotogrammetricReport>";
		EDomElement xml(out.str());
		myFile << xml.removeBlankLines(true).indent('\t').getContent();
		myFile.close();

		return true;
	}
	else
	{
		return false;
	}
}

bool ReportManager::makeXslt(int idExt, std::string path)
{
	std::string xsltransformation;
	std::stringstream outxsl;
	xsltransformation = path.c_str();
	outxsl << "<xsl:stylesheet version=\"1.0\" xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" xmlns:mml=\"http://www.w3.org/1998/Math/MathML\">\n";

	if(idExt == TXTTYPE)
	{
		xsltransformation += "/epr_txt.xsl";
		outxsl << "<xsl:output method=\"xml\" indent=\"yes\" encoding=\"UTF-8\" omit-xml-declaration=\"yes\"/>\n";
		outxsl << "<xsl:template match=\"efotoPhotogrammetricReport\">\n";
		outxsl << "    Begin of E-Foto Photogrammetric Report\n";
		outxsl << "\n";
		outxsl << "<xsl:if test=\"projectHeader\">\n";
		outxsl << "    =======================================\n";
		outxsl << "    Project Header:\n";
		outxsl << "\n";
		outxsl << "    Name: <xsl:value-of select=\"projectHeader/name\"/>\n";
		outxsl << "    Description: <xsl:value-of select=\"projectHeader/description\"/>\n";
		outxsl << "    Creation Date: <xsl:value-of select=\"projectHeader/creation\"/>\n";
		outxsl << "    Modification Date: <xsl:value-of select=\"projectHeader/modification\"/>\n";
		outxsl << "    Owner: <xsl:value-of select=\"projectHeader/owner\"/>\n";
		outxsl << "    =======================================\n";
		outxsl << "</xsl:if>\n";
		outxsl << "\n\n";
		outxsl << "<xsl:if test=\"terrain\">\n";
		outxsl << "    =======================================\n";
		outxsl << "    Terrain:\n";
		outxsl << "\n";
		outxsl << "    Mean Altitude: <xsl:value-of select=\"terrain/meanAltitude\"/>\n";
		outxsl << "    Minimum Altitude: <xsl:value-of select=\"terrain/minAltitude\"/>\n";
		outxsl << "    Maximum Altitude: <xsl:value-of select=\"terrain/maxAltitude\"/>\n";
		outxsl << "    GRS: <xsl:value-of select=\"terrain/GRS\"/>\n";
		outxsl << "    CPS: <xsl:value-of select=\"terrain/CPS\"/>\n";
		outxsl << "    Work Area Center Coordinates:";
		outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/lat/degrees\"/>º ";
		outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/lat/minutes\"/>' ";
		outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/lat/seconds\"/>'' ";
		outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/lat/direction\"/> ";
		outxsl << "<xsl:text> | </xsl:text>";
		outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/long/degrees\"/>º ";
		outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/long/minutes\"/>' ";
		outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/long/seconds\"/>'' ";
		outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/long/direction\"/> ";
		outxsl << "<xsl:text> | </xsl:text> ";
		outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/utmFuse\"/> UTM\n";
		outxsl << "    =======================================\n";
		outxsl << "</xsl:if>\n";
		outxsl << "\n\n";
		outxsl << "<xsl:if test=\"sensors\">\n";
		outxsl << "    =======================================\n";
		outxsl << "    Sensors:\n\n";
		outxsl << "    Sensor Id: <xsl:value-of select=\"sensors/sensorId\"/>\n";
		outxsl << "    Description: <xsl:value-of select=\"sensors/description\"/>\n";
		outxsl << "    Focal Distance: <xsl:value-of select=\"sensors/focalDistance/value\"/>\n";
		outxsl << "    =======================================\n";
		outxsl << "</xsl:if>\n";
		outxsl << "\n\n";
		outxsl << "<xsl:if test=\"flights\">\n";
		outxsl << "    =======================================\n";
		outxsl << "    Flights:\n\n";
		outxsl << "    Flight Id: <xsl:value-of select=\"flights/flightId\"/>\n";
		outxsl << "    Execution Date: <xsl:value-of select=\"flights/execution\"/>\n";
		outxsl << "    Nominal Scale: <xsl:value-of select=\"flights/nominalScale/mml:mfrac/*[1]\"/>:";
		outxsl << "<xsl:value-of select=\"flights/nominalScale/mml:mfrac/*[2]\"/>\n";
		outxsl << "    Longitudinal Overlap: <xsl:value-of select=\"flights/overlap/longitudinal\"/>\n";
		outxsl << "    Transversal Overlap: <xsl:value-of select=\"flights/overlap/transversal\"/>\n";
		outxsl << "    =======================================\n";
		outxsl << "</xsl:if>\n";
		outxsl << "\n\n";
		outxsl << "<xsl:if test=\"images\">\n";
		outxsl << "    =======================================\n";
		outxsl << "    Images:\n\n";
		outxsl << "    ---------------------------------------\n";
		outxsl << "<xsl:for-each select=\"images/image\">\n";
		outxsl << "    Image Id: <xsl:value-of select=\"imageId\"/>\n";
		outxsl << "    Dimension: <xsl:value-of select=\"width\"/>x<xsl:value-of select=\"heigth\"/> px\n";
		outxsl << "    Filename: <xsl:value-of select=\"fileName\"/>\n";
		outxsl << "    Filepath: <xsl:value-of select=\"filePath\"/>\n";
		outxsl << "    Resolution: <xsl:value-of select=\"resolution\"/> dpi\n";
		outxsl << "\n";
		outxsl << "<xsl:if test=\"interiorOrientation\">\n";
		outxsl << "    Interior Orientation - <xsl:value-of select=\"interiorOrientation/ioType\"/>:\n";
		outxsl << "\n";
		outxsl << "           Xa\n";
		outxsl << "    |  a0  |  <xsl:value-of select=\"interiorOrientation/Xa/a0\"/>\n";
		outxsl << "    |  a1  |  <xsl:value-of select=\"interiorOrientation/Xa/a1\"/>\n";
		outxsl << "    |  a2  |  <xsl:value-of select=\"interiorOrientation/Xa/a2\"/>\n";
		outxsl << "    |  b0  |  <xsl:value-of select=\"interiorOrientation/Xa/b0\"/>\n";
		outxsl << "    |  b1  |  <xsl:value-of select=\"interiorOrientation/Xa/b1\"/>\n";
		outxsl << "    |  b2  |  <xsl:value-of select=\"interiorOrientation/Xa/b2\"/>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "\n";
		outxsl << "<xsl:if test=\"exteriorOrientation\">\n";
		outxsl << "\n";
		outxsl << "    Exterior Orientation - <xsl:value-of select=\"exteriorOrientation/eoType\"/>:\n";
		outxsl << "\n";
		outxsl << "               Xa\n";
		outxsl << "    |   X0      |  <xsl:value-of select=\"exteriorOrientation/Xa/X0\"/>\n";
		outxsl << "    |   Y0      |  <xsl:value-of select=\"exteriorOrientation/Xa/Y0\"/>\n";
		outxsl << "    |   Z0      |  <xsl:value-of select=\"exteriorOrientation/Xa/Z0\"/>\n";
		outxsl << "    |   Omega   |  <xsl:value-of select=\"exteriorOrientation/Xa/omega\"/>\n";
		outxsl << "    |   Phi     |  <xsl:value-of select=\"exteriorOrientation/Xa/phi\"/>\n";
		outxsl << "    |   Kappa   |  <xsl:value-of select=\"exteriorOrientation/Xa/kappa\"/>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "    ---------------------------------------\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "    =======================================\n";
		outxsl << "</xsl:if>\n";
		outxsl << "\n\n";
		outxsl << "<xsl:if test=\"blockPoints\">\n";
		outxsl << "    =======================================\n";
		outxsl << "    Points:\n\n";
		outxsl << "    ---------------------------------------\n";
		outxsl << "<xsl:for-each select=\"blockPoints/point\">\n";
		outxsl << "    Point Id: <xsl:value-of select=\"pointId\"/>\n";
		outxsl << "    Type: <xsl:value-of select=\"pointType\"/>\n";
		outxsl << "<xsl:if test=\"spatialCoordinates/x\">\n";
		outxsl << "\n";
		outxsl << "     Spatial Coordinates\n";
		outxsl << "    |   X   |  <xsl:value-of select=\"spatialCoordinates/x\"/>\n";
		outxsl << "    |   Y   |  <xsl:value-of select=\"spatialCoordinates/y\"/>\n";
		outxsl << "    |   Z   |  <xsl:value-of select=\"spatialCoordinates/z\"/>\n";
		outxsl << "    |  Uom  |  <xsl:if test=\"spatialCoordinates/unit = 'm'\">Meters</xsl:if>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"imagesMeasurements/imageCoordinates\">\n";
		outxsl << "\n";
		outxsl << "    Image Coordinates\n";
		outxsl << "<xsl:for-each select=\"imagesMeasurements/imageCoordinates\">\n";
		outxsl << "\n";
		outxsl << "    Image Id: <xsl:value-of select=\"imageId\"/>\n";
		outxsl << "    Row: <xsl:value-of select=\"position/lin\"/>\n";
		outxsl << "    Column: <xsl:value-of select=\"position/col\"/>\n";
		outxsl << "    Uom: <xsl:if test=\"position/unit = 'px'\">pixels</xsl:if>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "\n";
		outxsl << "    ---------------------------------------\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "    =======================================\n";
		outxsl << "</xsl:if>\n";
		outxsl << "\n";
		outxsl << "<xsl:if test=\"affineTransformation\">\n";
		outxsl << "    =======================================\n";
		outxsl << "    Affine Transformation:\n";
		outxsl << "\n";
		outxsl << "    ---------------------------------------\n";
		outxsl << "<xsl:for-each select=\"affineTransformation/IO\">\n";
		outxsl << "    Image Id: <xsl:value-of select=\"imageId\"/>\n";
		outxsl << "<xsl:if test=\"initializationData\">\n";
		outxsl << "    Calculation Mode: <xsl:value-of select=\"initializationData/calculationMode\"/>\n";
		outxsl << "<xsl:if test=\"initializationData/calculationMode = 'With Fiducial Marks'\">\n";
		outxsl << "\n";
		outxsl << "    Fiducial Marks:\n";
		outxsl << "<xsl:for-each select=\"initializationData/fidMarks/fidMark\">\n";
		outxsl << "\n";
		outxsl << "    Fiducial Mark No: <xsl:value-of select=\"fidMarkNo\"/>\n";
		outxsl << "    |   X    |  <xsl:value-of select=\"gmlX\"/>\n";
		outxsl << "    |   Y    |  <xsl:value-of select=\"gmlY\"/>\n";
		outxsl << "    |  Row   |  <xsl:value-of select=\"lin\"/>\n";
		outxsl << "    | Column |  <xsl:value-of select=\"col\"/>\n";
		outxsl << "\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"initializationData/calculationMode = 'With Sensor Dimensions'\">\n";
		outxsl << "\n";
		outxsl << "    Sensor Dimensions:\n";
		outxsl << "    Sensor Columns: <xsl:value-of select=\"initializationData/sensorCols\"/>\n";
		outxsl << "    Sensor Rows: <xsl:value-of select=\"initializationData/sensorRows\"/>\n";
		outxsl << "   Sensor Pixel Size: <xsl:value-of select=\"initializationData/sensorPxsize\"/>\n";
		outxsl << "\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"initializationData/calculationMode = 'Fixed Parameters'\">\n";
		outxsl << "    Fixed Parameters:\n";
		outxsl << "\n";
		outxsl << "           Xa\n";
		outxsl << "    |  a0  |  <xsl:value-of select=\"initializationData/Xa/a0\"/>\n";
		outxsl << "    |  a1  |  <xsl:value-of select=\"initializationData/Xa/a1\"/>\n";
		outxsl << "    |  a2  |  <xsl:value-of select=\"initializationData/Xa/a2\"/>\n";
		outxsl << "    |  b0  |  <xsl:value-of select=\"initializationData/Xa/b0\"/>\n";
		outxsl << "    |  b1  |  <xsl:value-of select=\"initializationData/Xa/b1\"/>\n";
		outxsl << "    |  b2  |  <xsl:value-of select=\"initializationData/Xa/b2\"/>\n";
		outxsl << "\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"qualityData\">\n";
		outxsl << "\n";
		outxsl << "    Matrix V\n";
		outxsl << "<xsl:for-each select=\"qualityData/sigmaXa/mml:matrix/mml:matrixrow\">\n";
		outxsl << "    | <xsl:value-of select=\"mml:cn\"/> |\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "\n";
		outxsl << "    Sigma Xa\n";
		outxsl << "<xsl:for-each select=\"qualityData/sigmaXa/mml:matrix/mml:matrixrow\">\n";
		outxsl << "    | <xsl:value-of select=\"*[1]\"/> | <xsl:value-of select=\"*[2]\"/> | <xsl:value-of select=\"*[3]\"/> | <xsl:value-of select=\"*[4]\"/> | <xsl:value-of select=\"*[5]\"/> | <xsl:value-of select=\"*[6]\"/> |\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "\n";
		outxsl << "    Sigma La\n";
		outxsl << "<xsl:for-each select=\"qualityData/sigmaLa/mml:matrix/mml:matrixrow\">\n";
		outxsl << "    | <xsl:value-of select=\"*[1]\"/> | <xsl:value-of select=\"*[2]\"/> | <xsl:value-of select=\"*[3]\"/> | <xsl:value-of select=\"*[4]\"/> | <xsl:value-of select=\"*[5]\"/> | <xsl:value-of select=\"*[6]\"/> | <xsl:value-of select=\"*[7]\"/> | <xsl:value-of select=\"*[8]\"/> |\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "\n";
		outxsl << "    Sigma0Squared: <xsl:value-of select=\"qualityData/sigma0Squared\"/>\n";
		outxsl << "\n";
		outxsl << "</xsl:if>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "    ---------------------------------------\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "    =======================================\n";
		outxsl << "</xsl:if>\n";
		outxsl << "\n";
		outxsl << "<xsl:if test=\"spatialResection\">\n";
		outxsl << "    =======================================\n";
		outxsl << "    Spatial Ressection:\n";
		outxsl << "\n";
		outxsl << "    ---------------------------------------\n";
		outxsl << "<xsl:for-each select=\"spatialResection/SR\">\n";
		outxsl << "    Image Id: <xsl:value-of select=\"imageId\"/>\n";
		outxsl << "    Flight Direction: <xsl:value-of select=\"flightDirection\"/> <xsl:if test=\"unit = 'rad'\"> Radian</xsl:if>\n";
		outxsl << "<xsl:if test=\"initializationData\">\n";
		outxsl << "<xsl:if test=\"initializationData/gnss\">\n";
		outxsl << "\n";
		outxsl << "    Ground Coordinates:\n";
		outxsl << "    Type: <xsl:value-of select=\"initializationData/gnss/type\"/>\n";
		outxsl << "    E0: <xsl:value-of select=\"initializationData/gnss/x0\"/>\n";
		outxsl << "    N0: <xsl:value-of select=\"initializationData/gnss/y0\"/>\n";
		outxsl << "    H0: <xsl:value-of select=\"initializationData/gnss/z0\"/>\n";
		outxsl << "\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"initializationData/ins\">\n";
		outxsl << "\n";
		outxsl << "    Inertial Navigation System:\n";
		outxsl << "    Type: <xsl:value-of select=\"initializationData/ins/type\"/>\n";
		outxsl << "    Omega: <xsl:value-of select=\"initializationData/ins/omega\"/>\n";
		outxsl << "    Phi: <xsl:value-of select=\"initializationData/ins/phi\"/>\n";
		outxsl << "    Kappa: <xsl:value-of select=\"initializationData/ins/kappa\"/>\n";
		outxsl << "\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"initializationData/selectedPoints\">\n";
		outxsl << "\n";
		outxsl << "    Selected Points:\n";
		outxsl << "\n";
		outxsl << "<xsl:for-each select=\"initializationData/selectedPoints/point\">\n";
		outxsl << "\n";
		outxsl << "    |   X    |  <xsl:value-of select=\"x\"/>\n";
		outxsl << "    |   Y    |  <xsl:value-of select=\"y\"/>\n";
		outxsl << "    |   Z    |  <xsl:value-of select=\"z\"/>\n";
		outxsl << "    |  Row   |  <xsl:value-of select=\"lin\"/>\n";
		outxsl << "    | Column |  <xsl:value-of select=\"col\"/>\n";
		outxsl << "\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"qualityData\">\n";
		outxsl << "\n";
		outxsl << "    Matrix V\n";
		outxsl << "<xsl:for-each select=\"qualityData/sigmaXa/mml:matrix/mml:matrixrow\">\n";
		outxsl << "    | <xsl:value-of select=\"mml:cn\"/> |\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "\n";
		outxsl << "    Sigma Xa\n";
		outxsl << "<xsl:for-each select=\"qualityData/sigmaXa/mml:matrix/mml:matrixrow\">\n";
		outxsl << "    | <xsl:value-of select=\"*[1]\"/> | <xsl:value-of select=\"*[2]\"/> | <xsl:value-of select=\"*[3]\"/> | <xsl:value-of select=\"*[4]\"/> | <xsl:value-of select=\"*[5]\"/> | <xsl:value-of select=\"*[6]\"/> |\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "\n";
		outxsl << "    Sigma La\n";
		outxsl << "<xsl:for-each select=\"qualityData/sigmaLa/mml:matrix/mml:matrixrow\">\n";
		outxsl << "    | <xsl:value-of select=\"*[1]\"/> | <xsl:value-of select=\"*[2]\"/> | <xsl:value-of select=\"*[3]\"/> | <xsl:value-of select=\"*[4]\"/> | <xsl:value-of select=\"*[5]\"/> | <xsl:value-of select=\"*[6]\"/> | <xsl:value-of select=\"*[7]\"/> | <xsl:value-of select=\"*[8]\"/> |\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "\n";
		outxsl << "    Sigma0Squared: <xsl:value-of select=\"qualityData/sigma0Squared\"/>\n";
		outxsl << "\n";
		outxsl << "</xsl:if>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "    ---------------------------------------\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "    =======================================\n";
		outxsl << "</xsl:if>\n";
		outxsl << "\n";
		outxsl << "<xsl:if test=\"photogrammetricBlock\">\n";
		outxsl << "    =======================================\n";
		outxsl << "    Photogrammetric Block:\n";
		outxsl << "\n";
		outxsl << "    Total Iterations: <xsl:value-of select=\"photogrammetricBlock/totalIterations\"/>\n";
		outxsl << "    Angular Convergency: <xsl:value-of select=\"photogrammetricBlock/angularConvergency\"/> Radian\n";
		outxsl << "    Metric Convergency: <xsl:value-of select=\"photogrammetricBlock/metricConvergency\"/> Meters\n";
		outxsl << "\n";
		outxsl << "<xsl:if test=\"photogrammetricBlock/initialValues\">\n";
		outxsl << "        Initial Values:\n";
		outxsl << "<xsl:for-each select=\"photogrammetricBlock/initialValues\">\n";
		outxsl << "    | Image Id |  <xsl:value-of select=\"imageId\"/>\n";
		outxsl << "    |  Omega   |  <xsl:value-of select=\"matrix/omega\"/>\n";
		outxsl << "    |   Phi    |  <xsl:value-of select=\"matrix/phi\"/>\n";
		outxsl << "    |   Kappa  |  <xsl:value-of select=\"matrix/kappa\"/>\n";
		outxsl << "    |    X0    |  <xsl:value-of select=\"matrix/x0\"/>\n";
		outxsl << "    |    Y0    |  <xsl:value-of select=\"matrix/y0\"/>\n";
		outxsl << "    |    Z0    |  <xsl:value-of select=\"matrix/z0\"/>\n";
		outxsl << "\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "\n";
		outxsl << "<xsl:if test=\"photogrammetricBlock/rmseOfEachIteration\">\n";
		outxsl << "\n";
		outxsl << "\n";
		outxsl << "    Rmse Of Each Iteration:\n";
		outxsl << "<xsl:for-each select=\"photogrammetricBlock/rmseOfEachIteration/rmse\">\n";
		outxsl << "    Iteration: <xsl:value-of select=\"iteration\"/>\n";
		outxsl << "    Value: <xsl:value-of select=\"value\"/>\n";
		outxsl << "\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "\n";
		outxsl << "</xsl:if>\n";
		outxsl << "    =======================================\n";
		outxsl << "</xsl:if>\n";
		outxsl << "\n";
		outxsl << "<xsl:if test=\"interiorOrientation\">\n";
		outxsl << "    =======================================\n";
		outxsl << "    Interior Orientation:\n";
		outxsl << "\n";
		outxsl << "    ---------------------------------------\n";
		outxsl << "<xsl:for-each select=\"interiorOrientation/IO\">\n";
		outxsl << "    ---------------------------------------\n";
		outxsl << "    Image Id: <xsl:value-of select=\"imageId\"/>\n";
		outxsl << "    IO Type: <xsl:value-of select=\"ioType\"/>\n";
		outxsl << "<xsl:if test=\"Xa\">\n";
		outxsl << "\n";
		outxsl << "           Xa\n";
		outxsl << "    |  a0  |  <xsl:value-of select=\"Xa/a0\"/>\n";
		outxsl << "    |  a1  |  <xsl:value-of select=\"Xa/a1\"/>\n";
		outxsl << "    |  a2  |  <xsl:value-of select=\"Xa/a2\"/>\n";
		outxsl << "    |  b0  |  <xsl:value-of select=\"Xa/b0\"/>\n";
		outxsl << "    |  b1  |  <xsl:value-of select=\"Xa/b1\"/>\n";
		outxsl << "    |  b2  |  <xsl:value-of select=\"Xa/b2\"/>\n";
		outxsl << "\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"V\">\n";
		outxsl << "\n";
		outxsl << "    Matrix V\n";
		outxsl << "<xsl:for-each select=\"V/mml:matrix/mml:matrixrow\">\n";
		outxsl << "    | <xsl:value-of select=\"mml:cn\"/> |\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "\n";
		outxsl << "</xsl:if>\n";
		outxsl << "\n";
		outxsl << "<xsl:if test=\"sigmaXa\">\n";
		outxsl << "    Sigma Xa\n";
		outxsl << "<xsl:for-each select=\"sigmaXa/mml:matrix/mml:matrixrow\">\n";
		outxsl << "    | <xsl:value-of select=\"*[1]\"/> | <xsl:value-of select=\"*[2]\"/> | <xsl:value-of select=\"*[3]\"/> | <xsl:value-of select=\"*[4]\"/> | <xsl:value-of select=\"*[5]\"/> | <xsl:value-of select=\"*[6]\"/> |\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "\n";
		outxsl << "</xsl:if>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "    =======================================\n";
		outxsl << "</xsl:if>\n";
		outxsl << "\n";
		outxsl << "<xsl:if test=\"exteriorOrientation\">\n";
		outxsl << "    =======================================\n";
		outxsl << "    Exterior Orientation:\n";
		outxsl << "\n";
		outxsl << "    ---------------------------------------\n";
		outxsl << "<xsl:for-each select=\"exteriorOrientation/EO\">\n";
		outxsl << "\n";
		outxsl << "    Image Id: <xsl:value-of select=\"imageId\"/>\n";
		outxsl << "    EO Type: <xsl:value-of select=\"eoType\"/>\n";
		outxsl << "    Number Of Iterations: <xsl:value-of select=\"noIterations\"/>\n";
		outxsl << "<xsl:if test=\"Xa\">\n";
		outxsl << "\n";
		outxsl << "               Xa\n";
		outxsl << "    |   X0      |  <xsl:value-of select=\"Xa/X0\"/>\n";
		outxsl << "    |   Y0      |  <xsl:value-of select=\"Xa/Y0\"/>\n";
		outxsl << "    |   Z0      |  <xsl:value-of select=\"Xa/Z0\"/>\n";
		outxsl << "    |   Omega   |  <xsl:value-of select=\"Xa/omega\"/>\n";
		outxsl << "    |   Phi     |  <xsl:value-of select=\"Xa/phi\"/>\n";
		outxsl << "    |   Kappa   |  <xsl:value-of select=\"Xa/kappa\"/>\n";
		outxsl << "\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"V\">\n";
		outxsl << "\n";
		outxsl << "    Matrix V\n";
		outxsl << "<xsl:for-each select=\"V/mml:matrix/mml:matrixrow\">\n";
		outxsl << "    | <xsl:value-of select=\"mml:cn\"/> |\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "\n";
		outxsl << "</xsl:if>\n";
		outxsl << "\n";
		outxsl << "<xsl:if test=\"sigmaXa\">\n";
		outxsl << "    Sigma Xa\n";
		outxsl << "<xsl:for-each select=\"sigmaXa/mml:matrix/mml:matrixrow\">\n";
		outxsl << "    | <xsl:value-of select=\"*[1]\"/> | <xsl:value-of select=\"*[2]\"/> | <xsl:value-of select=\"*[3]\"/> | <xsl:value-of select=\"*[4]\"/> | <xsl:value-of select=\"*[5]\"/> | <xsl:value-of select=\"*[6]\"/> |\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "\n";
		outxsl << "</xsl:if>\n";
		outxsl << "    ---------------------------------------\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "    =======================================\n";
		outxsl << "</xsl:if>\n";
		outxsl << "\n";
		outxsl << "<xsl:if test=\"stereoPairs\">\n";
		outxsl << "    =======================================\n";
		outxsl << "    Stereo Pairs:\n";
		outxsl << "    ---------------------------------------\n";
		outxsl << "<xsl:for-each select=\"stereoPairs/stereoPair\">\n";
		outxsl << "    Stereo Pair: <xsl:for-each select=\"imageId\"> Image Id <xsl:value-of select=\"text()\"/> | </xsl:for-each>\n";
		outxsl << "    ---------------------------------------\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "    =======================================\n";
		outxsl << "</xsl:if>\n";
		outxsl << "    End of E-Foto Photogrammetric Report\n";
		outxsl << "</xsl:template>\n";
		outxsl << "</xsl:stylesheet>\n";
		std::ofstream myXsltFile(xsltransformation.c_str());
		myXsltFile << outxsl.str();
		myXsltFile.close();
	} else {
		xsltransformation += "/epr_html.xsl";
		outxsl << "<xsl:template match=\"efotoPhotogrammetricReport\">\n";
		outxsl << "<html>\n";
		outxsl << "<head>\n";
		outxsl << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/>\n";
		outxsl << "<script type=\"text/javascript\">\n";
		outxsl << "<xsl:text>\n";
		outxsl << "function ShowHide(divId)\n";
		outxsl << "{\n";
		outxsl << "if(document.getElementById(divId).style.display == 'none')\n";
		outxsl << "{\n";
		outxsl << "document.getElementById(divId).style.display='block';\n";
		outxsl << "}\n";
		outxsl << "else\n";
		outxsl << "{\n";
		outxsl << "document.getElementById(divId).style.display = 'none';\n";
		outxsl << "}\n";
		outxsl << "}\n";
		outxsl << "</xsl:text>\n";
		outxsl << "</script>\n";
		outxsl << "</head>\n";
		outxsl << "<body>\n";
		outxsl << "<a name=\"top\"><h2>E-Foto Photogrammetric Report</h2></a>\n";
		outxsl << "<xsl:if test=\"projectHeader\">\n";
		outxsl << "<a onclick =\"javascript:ShowHide('projectHeader')\" href=\"javascript:;\"><h3>Project Header</h3></a>\n";
		outxsl << "<div id=\"projectHeader\" style=\"DISPLAY: none\">\n";
		outxsl << "Name: <xsl:value-of select=\"projectHeader/name\"/><br/>\n";
		outxsl << "Description: <xsl:value-of select=\"projectHeader/description\"/><br/>\n";
		outxsl << "Creation Date: <xsl:value-of select=\"projectHeader/creation\"/><br/>\n";
		outxsl << "Modification Date: <xsl:value-of select=\"projectHeader/modification\"/><br/>\n";
		outxsl << "Owner: <xsl:value-of select=\"projectHeader/owner\"/><br/>\n";
		outxsl << "</div>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"terrain\">\n";
		outxsl << "<a onclick =\"javascript:ShowHide('terrain')\" href=\"javascript:;\"><h3>Terrain</h3></a>\n";
		outxsl << "<div id=\"terrain\" style=\"DISPLAY: none\">\n";
		outxsl << "Mean Altitude: <xsl:value-of select=\"terrain/meanAltitude\"/><br/>\n";
		outxsl << "Minimum Altitude: <xsl:value-of select=\"terrain/minAltitude\"/><br/>\n";
		outxsl << "Maximum Altitude: <xsl:value-of select=\"terrain/maxAltitude\"/><br/>\n";
		outxsl << "GRS: <xsl:value-of select=\"terrain/GRS\"/><br/>\n";
		outxsl << "CPS: <xsl:value-of select=\"terrain/CPS\"/><br/>\n";
		outxsl << "Work Area Center Coordinates: \n";
		outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/lat/degrees\"/>º\n";
		outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/lat/minutes\"/>'\n";
		outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/lat/seconds\"/>''\n";
		outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/lat/direction\"/>\n";
		outxsl << "<xsl:text> | </xsl:text>\n";
		outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/long/degrees\"/>º\n";
		outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/long/minutes\"/>'\n";
		outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/long/seconds\"/>''\n";
		outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/long/direction\"/>\n";
		outxsl << "<xsl:text> | </xsl:text>\n";
		outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/utmFuse\"/> UTM\n";
		outxsl << "</div>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"sensors\">\n";
		outxsl << "<a onclick =\"javascript:ShowHide('sensors')\" href=\"javascript:;\"><h3>Sensors</h3></a>\n";
		outxsl << "<div id=\"sensors\" style=\"DISPLAY: none\">\n";
		outxsl << "Sensor Id: <xsl:value-of select=\"sensors/sensorId\"/><br/>\n";
		outxsl << "Description: <xsl:value-of select=\"sensors/description\"/><br/>\n";
		outxsl << "Focal Distance: <xsl:value-of select=\"sensors/focalDistance\"/><br/>\n";
		outxsl << "</div>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"flights\">\n";
		outxsl << "<a onclick =\"javascript:ShowHide('flights')\" href=\"javascript:;\"><h3>Flights</h3></a>\n";
		outxsl << "<div id=\"flights\" style=\"DISPLAY: none\">\n";
		outxsl << "Flight Id: <xsl:value-of select=\"flights/flightId\"/><br/>\n";
		outxsl << "Execution Date: <xsl:value-of select=\"flights/execution\"/><br/>\n";
		outxsl << "Nominal Scale: <xsl:value-of select=\"flights/nominalScale/mml:mfrac/*[1]\"/>:\n";
		outxsl << "<xsl:value-of select=\"flights/nominalScale/mml:mfrac/*[2]\"/><br/>\n";
		outxsl << "Longitudinal Overlap: <xsl:value-of select=\"flights/overlap/longitudinal\"/><br/>\n";
		outxsl << "Transversal Overlap: <xsl:value-of select=\"flights/overlap/transversal\"/><br/>\n";
		outxsl << "</div>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"images\">\n";
		outxsl << "<a onclick =\"javascript:ShowHide('images')\" href=\"javascript:;\"><h3>Images</h3></a>\n";
		outxsl << "<div id=\"images\" style=\"DISPLAY: none;\">\n";
		outxsl << "<xsl:for-each select=\"images/image\">\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('images_image_<xsl:value-of select=\"imageId\" />')</xsl:attribute>Image Id <xsl:value-of select=\"imageId\"/></a><br/><br/>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">images_image_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "Dimension: <xsl:value-of select=\"width\"/>x<xsl:value-of select=\"heigth\"/> px<br/>\n";
		outxsl << "Filename: <xsl:value-of select=\"fileName\"/><br/>\n";
		outxsl << "Filepath: <xsl:value-of select=\"filePath\"/><br/>\n";
		outxsl << "Resolution: <xsl:value-of select=\"resolution\"/> dpi<br/>\n";
		outxsl << "<xsl:if test=\"interiorOrientation\">\n";
		outxsl << "<br/>\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('image_IO_<xsl:value-of select=\"imageId\" />')</xsl:attribute><i>Interior Orientation</i></a>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">image_IO_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "<br/>\n";
		outxsl << "<table border=\"1\">\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"interiorOrientation/ioType\"/></td>\n";
		outxsl << "<td align=\"center\"><b>X<sub>a</sub></b></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">a0</td>\n";
		outxsl << "<td><xsl:value-of select=\"interiorOrientation/Xa/a0\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">a1</td>\n";
		outxsl << "<td><xsl:value-of select=\"interiorOrientation/Xa/a1\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">a2</td>\n";
		outxsl << "<td><xsl:value-of select=\"interiorOrientation/Xa/a2\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">b0</td>\n";
		outxsl << "<td><xsl:value-of select=\"interiorOrientation/Xa/b0\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">b1</td>\n";
		outxsl << "<td><xsl:value-of select=\"interiorOrientation/Xa/b1\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">b2</td>\n";
		outxsl << "<td><xsl:value-of select=\"interiorOrientation/Xa/b2\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "</table>\n";
		outxsl << "</div>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"exteriorOrientation\">\n";
		outxsl << "<br/><br/>\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('image_EO_<xsl:value-of select=\"imageId\" />')</xsl:attribute><i>Exterior Orientation</i></a>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">image_EO_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "<br/>\n";
		outxsl << "<table border=\"1\">\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"exteriorOrientation/eoType\"/></td>\n";
		outxsl << "<td align=\"center\"><b>X<sub>a</sub></b></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">X0</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"exteriorOrientation/Xa/X0\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">Y0</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"exteriorOrientation/Xa/Y0\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">Z0</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"exteriorOrientation/Xa/Z0\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">&#x3C9;</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"exteriorOrientation/Xa/omega\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">&#x3C6;</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"exteriorOrientation/Xa/phi\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">&#x3BA;</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"exteriorOrientation/Xa/kappa\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "</table>\n";
		outxsl << "</div><br/><br/>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "</div>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "<a href=\"#top\">Top</a>\n";
		outxsl << "</div>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"blockPoints\">\n";
		outxsl << "<a onclick =\"javascript:ShowHide('blockOfPoints')\" href=\"javascript:;\"><h3>Points</h3></a>\n";
		outxsl << "<div id=\"blockOfPoints\" style=\"DISPLAY: none\">\n";
		outxsl << "<xsl:for-each select=\"blockPoints/point\">\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('blockPoints_point_<xsl:value-of select=\"pointId\" />')</xsl:attribute>Point Id <xsl:value-of select=\"pointId\"/></a><br/><br/>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">blockPoints_point_<xsl:value-of select=\"pointId\" /></xsl:attribute>\n";
		outxsl << "Type: <xsl:value-of select=\"pointType\"/><br/>\n";
		outxsl << "<xsl:if test=\"spatialCoordinates/x\">\n";
		outxsl << "<br/>\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('point_spatialCoordinates_<xsl:value-of select=\"pointId\" />')</xsl:attribute><i>Spatial Coordinates</i></a>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">point_spatialCoordinates_<xsl:value-of select=\"pointId\" /></xsl:attribute>\n";
		outxsl << "<br/>\n";
		outxsl << "<table border=\"1\">\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">X</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"spatialCoordinates/x\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">Y</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"spatialCoordinates/y\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">Z</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"spatialCoordinates/z\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">Uom </td>\n";
		outxsl << "<td align=\"center\"><xsl:if test=\"spatialCoordinates/unit = 'm'\">Meters</xsl:if></td>\n";
		outxsl << "</tr>\n";
		outxsl << "</table>\n";
		outxsl << "</div><br/><br/>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"imagesMeasurements/imageCoordinates\">\n";
		outxsl << "<br/>\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('point_imageCoordinates_<xsl:value-of select=\"pointId\" />')</xsl:attribute><i>Image Coordinates</i></a>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">point_imageCoordinates_<xsl:value-of select=\"pointId\" /></xsl:attribute>\n";
		outxsl << "<br/>\n";
		outxsl << "<table border=\"1\">\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">Image Id</td>\n";
		outxsl << "<td align=\"center\">Line</td>\n";
		outxsl << "<td align=\"center\">Column</td>\n";
		outxsl << "<td align=\"center\">Uom</td>\n";
		outxsl << "</tr>\n";
		outxsl << "<xsl:for-each select=\"imagesMeasurements/imageCoordinates\">\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"imageId\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"position/lin\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"position/col\"/></td>\n";
		outxsl << "<td align=\"center\">\n";
		outxsl << "<xsl:if test=\"position/unit = 'px'\">\n";
		outxsl << "pixels\n";
		outxsl << "</xsl:if>\n";
		outxsl << "</td>\n";
		outxsl << "</tr>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "</table>\n";
		outxsl << "</div><br/><br/>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "</div>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "<a href=\"#top\">Top</a>\n";
		outxsl << "</div>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"affineTransformation\">\n";
		outxsl << "<a onclick =\"javascript:ShowHide('affineTransformation')\" href=\"javascript:;\"><h3>Affine Transformation</h3></a>\n";
		outxsl << "<div id=\"affineTransformation\" style=\"DISPLAY: none\">\n";
		outxsl << "<xsl:for-each select=\"affineTransformation/IO\">\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('affine_image_<xsl:value-of select=\"imageId\" />')</xsl:attribute>Image Id <xsl:value-of select=\"imageId\"/></a><br/><br/>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">affine_image_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "<xsl:if test=\"initializationData\">\n";
		outxsl << "Calculation Mode: <xsl:value-of select=\"initializationData/calculationMode\"/><br/>\n";
		outxsl << "<xsl:if test=\"initializationData/calculationMode = 'With Fiducial Marks'\">\n";
		outxsl << "<br/>\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('fiducialMarks_<xsl:value-of select=\"imageId\" />')</xsl:attribute><i>Fiducial Marks</i></a>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">fiducialMarks_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "<br/>\n";
		outxsl << "<table border=\"1\">\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">FidMark</td>\n";
		outxsl << "<td align=\"center\">X</td>\n";
		outxsl << "<td align=\"center\">Y</td>\n";
		outxsl << "<td align=\"center\">Column</td>\n";
		outxsl << "<td align=\"center\">Row</td>\n";
		outxsl << "</tr>\n";
		outxsl << "<xsl:for-each select=\"initializationData/fidMarks/fidMark\">\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"fidMarkNo\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"gmlX\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"gmlY\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"col\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"lin\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "</table>\n";
		outxsl << "</div><br/><br/>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"initializationData/calculationMode = 'With Sensor Dimensions'\">\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('sensorDimensions_<xsl:value-of select=\"imageId\" />')</xsl:attribute><i>Sensor Dimensions</i></a>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">sensorDimensions_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "<br/>\n";
		outxsl << "Sensor Columns: <xsl:value-of select=\"initializationData/sensorCols\"/><br/>\n";
		outxsl << "Sensor Rows: <xsl:value-of select=\"initializationData/sensorRows\"/><br/>\n";
		outxsl << "Sensor Pixel Size: <xsl:value-of select=\"initializationData/sensorPxsize\"/><br/>\n";
		outxsl << "</div><br/><br/>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"initializationData/calculationMode = 'Fixed Parameters'\">\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('fixedParameters_<xsl:value-of select=\"imageId\" />')</xsl:attribute><i>Fixed Parameters</i></a>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">fixedParameters_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "<br/>\n";
		outxsl << "<table border=\"1\">\n";
		outxsl << "<tr>\n";
		outxsl << "<td colspan=\"2\" align=\"center\"><b>X<sub>a</sub></b></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">a0</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"initializationData/Xa/a0\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">a1</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"initializationData/Xa/a1\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">a2</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"initializationData/Xa/a2\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">b0</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"initializationData/Xa/b0\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">b1</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"initializationData/Xa/b1\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">b2</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"initializationData/Xa/b2\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "</table>\n";
		outxsl << "</div><br/><br/>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"qualityData\">\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('affine_quality_V_<xsl:value-of select=\"imageId\" />')</xsl:attribute><i>Matrix V</i></a>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">affine_quality_V_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "<br/>\n";
		outxsl << "<table border=\"1\">\n";
		outxsl << "<xsl:for-each select=\"qualityData/V/mml:matrix/mml:matrixrow\">\n";
		outxsl << "<tr><td align=\"center\"><xsl:value-of select=\"mml:cn\"/></td></tr>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "</table>\n";
		outxsl << "</div><br/><br/>\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('affine_quality_sigmaXa_<xsl:value-of select=\"imageId\" />')</xsl:attribute><i>&#x3A3;X<sub>a</sub></i></a>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">affine_quality_sigmaXa_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "<br/>\n";
		outxsl << "<table border=\"1\">\n";
		outxsl << "<xsl:for-each select=\"qualityData/sigmaXa/mml:matrix/mml:matrixrow\">\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[1]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[2]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[3]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[4]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[5]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[6]\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "</table>\n";
		outxsl << "</div><br/><br/>\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('affine_quality_sigmaLa_<xsl:value-of select=\"imageId\" />')</xsl:attribute><i>&#x3A3;L<sub>a</sub></i></a>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">affine_quality_sigmaLa_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "<br/>\n";
		outxsl << "<table border=\"1\">\n";
		outxsl << "<xsl:for-each select=\"qualityData/sigmaLa/mml:matrix/mml:matrixrow\">\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[1]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[2]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[3]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[4]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[5]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[6]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[7]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[8]\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "</table>\n";
		outxsl << "</div><br/><br/>\n";
		outxsl << "&#x3C3;<sub>0</sub><sup>2</sup>: <xsl:value-of select=\"qualityData/sigma0Squared\"/><br/>\n";
		outxsl << "</xsl:if><br/>\n";
		outxsl << "</div>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "<a href=\"#top\">Top</a>\n";
		outxsl << "</div>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"spatialResection\">\n";
		outxsl << "<a onclick =\"javascript:ShowHide('spatialResection')\" href=\"javascript:;\"><h3>Spatial Ressection</h3></a>\n";
		outxsl << "<div id=\"spatialResection\" style=\"DISPLAY: none\">\n";
		outxsl << "<xsl:for-each select=\"spatialResection/SR\">\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('spatialResection_image_<xsl:value-of select=\"imageId\" />')</xsl:attribute>Image Id <xsl:value-of select=\"imageId\"/></a>\n";
		outxsl << "<br/><br/>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">spatialResection_image_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "Flight Direction: <xsl:value-of select=\"flightDirection\"/> <xsl:if test=\"unit = 'rad'\"> Radian</xsl:if><br/>\n";
		outxsl << "<xsl:if test=\"initializationData\">\n";
		outxsl << "<xsl:if test=\"initializationData/gnss\">\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('spatialResection_gnss_<xsl:value-of select=\"imageId\" />')</xsl:attribute>Ground Coordinates</a>\n";
		outxsl << "<br/><br/>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">spatialResection_gnss_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "<br/>\n";
		outxsl << "Type: <xsl:value-of select=\"initializationData/gnss/type\"/><br/>\n";
		outxsl << "E<sub>0</sub>: <xsl:value-of select=\"initializationData/gnss/x0\"/><br/>\n";
		outxsl << "N<sub>0</sub>: <xsl:value-of select=\"initializationData/gnss/y0\"/><br/>\n";
		outxsl << "H<sub>0</sub>: <xsl:value-of select=\"initializationData/gnss/z0\"/><br/>\n";
		outxsl << "<br/>\n";
		outxsl << "</div>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"initializationData/ins\">\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('spatialResection_ins_<xsl:value-of select=\"imageId\" />')</xsl:attribute>Inertial Navigation System</a>\n";
		outxsl << "<br/><br/>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">spatialResection_ins_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "<br/>\n";
		outxsl << "Type: <xsl:value-of select=\"initializationData/ins/type\"/><br/>\n";
		outxsl << "&#x3C9;: <xsl:value-of select=\"initializationData/ins/omega\"/><br/>\n";
		outxsl << "&#x3C6;: <xsl:value-of select=\"initializationData/ins/phi\"/><br/>\n";
		outxsl << "&#x3BA;: <xsl:value-of select=\"initializationData/ins/kappa\"/><br/>\n";
		outxsl << "<br/>\n";
		outxsl << "</div>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"initializationData/selectedPoints\">\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('spatialResection_selectedPoints_<xsl:value-of select=\"imageId\" />')</xsl:attribute>Selected Points</a>\n";
		outxsl << "<br/><br/>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">spatialResection_selectedPoints_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "<br/>\n";
		outxsl << "<table border=\"1\">\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">X</td>\n";
		outxsl << "<td align=\"center\">Y</td>\n";
		outxsl << "<td align=\"center\">Z</td>\n";
		outxsl << "<td align=\"center\">Column</td>\n";
		outxsl << "<td align=\"center\">Row</td>\n";
		outxsl << "</tr>\n";
		outxsl << "<xsl:for-each select=\"initializationData/selectedPoints/point\">\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"x\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"y\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"z\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"col\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"lin\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "</table>\n";
		outxsl << "</div>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<br/>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"qualityData\">\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('spatialResection_quality_V_<xsl:value-of select=\"imageId\" />')</xsl:attribute><i>Matrix V</i></a>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">spatialResection_quality_V_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "<br/>\n";
		outxsl << "<table border=\"1\">\n";
		outxsl << "<xsl:for-each select=\"qualityData/V/mml:matrix/mml:matrixrow\">\n";
		outxsl << "<tr><td align=\"center\"><xsl:value-of select=\"mml:cn\"/></td></tr>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "</table>\n";
		outxsl << "</div><br/><br/>\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('spatialResection_quality_sigmaXa_<xsl:value-of select=\"imageId\" />')</xsl:attribute><i>&#x3A3;X<sub>a</sub></i></a>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">spatialResection_quality_sigmaXa_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "<br/>\n";
		outxsl << "<table border=\"1\">\n";
		outxsl << "<xsl:for-each select=\"qualityData/sigmaXa/mml:matrix/mml:matrixrow\">\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[1]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[2]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[3]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[4]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[5]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[6]\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "</table>\n";
		outxsl << "</div><br/><br/>\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('spatialResection_quality_sigmaLa_<xsl:value-of select=\"imageId\" />')</xsl:attribute><i>&#x3A3;L<sub>a</sub></i></a>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">spatialResection_quality_sigmaLa_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "<br/>\n";
		outxsl << "<table border=\"1\">\n";
		outxsl << "<xsl:for-each select=\"qualityData/sigmaLa/mml:matrix/mml:matrixrow\">\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[1]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[2]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[3]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[4]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[5]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[6]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[7]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[8]\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "</table>\n";
		outxsl << "</div><br/><br/>\n";
		outxsl << "&#x3C3;<sub>0</sub><sup>2</sup>: <xsl:value-of select=\"qualityData/sigma0Squared\"/><br/>\n";
		outxsl << "</xsl:if><br/>\n";
		outxsl << "</div>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "<a href=\"#top\">Top</a>\n";
		outxsl << "</div>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"photogrammetricBlock\">\n";
		outxsl << "<a onclick =\"javascript:ShowHide('photogrammetricBlock')\" href=\"javascript:;\"><h3>Photogrammetric Block</h3></a>\n";
		outxsl << "<div id=\"photogrammetricBlock\" style=\"DISPLAY: none\">\n";
		outxsl << "Total Iterations: <xsl:value-of select=\"photogrammetricBlock/totalIterations\"/><br/>\n";
		outxsl << "Angular Convergency: <xsl:value-of select=\"photogrammetricBlock/angularConvergency\"/> Radian<br/>\n";
		outxsl << "Metric Convergency: <xsl:value-of select=\"photogrammetricBlock/metricConvergency\"/> Meters<br/>\n";
		outxsl << "<xsl:if test=\"photogrammetricBlock/initialValues\"><br/>\n";
		outxsl << "<a onclick =\"javascript:ShowHide('photogrammetricBlock_initialValues')\" href=\"javascript:;\"><i>Initial Values</i></a><br/>\n";
		outxsl << "<div id=\"photogrammetricBlock_initialValues\" style=\"DISPLAY: none\">\n";
		outxsl << "<table border=\"1\">\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">Image Id</td>\n";
		outxsl << "<td align=\"center\">&#x3C9;</td>\n";
		outxsl << "<td align=\"center\">&#x3C6;</td>\n";
		outxsl << "<td align=\"center\">&#x3BA;</td>\n";
		outxsl << "<td align=\"center\">X<sub>0</sub></td>\n";
		outxsl << "<td align=\"center\">Y<sub>0</sub></td>\n";
		outxsl << "<td align=\"center\">Z<sub>0</sub></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<xsl:for-each select=\"photogrammetricBlock/initialValues\">\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"imageId\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"matrix/omega\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"matrix/phi\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"matrix/kappa\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"matrix/x0\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"matrix/y0\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"matrix/z0\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "</table>\n";
		outxsl << "</div>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"photogrammetricBlock/rmseOfEachIteration\"><br/>\n";
		outxsl << "<a onclick =\"javascript:ShowHide('photogrammetricBlock_rmse')\" href=\"javascript:;\"><i>Rmse Of Each Iteration</i></a><br/>\n";
		outxsl << "<div id=\"photogrammetricBlock_rmse\" style=\"DISPLAY: none\">\n";
		outxsl << "<table border=\"1\">\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">Iteration</td>\n";
		outxsl << "<td align=\"center\">Value</td>\n";
		outxsl << "</tr>\n";
		outxsl << "<xsl:for-each select=\"photogrammetricBlock/rmseOfEachIteration/rmse\">\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"iteration\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"value\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "</table>\n";
		outxsl << "</div><br/>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<a href=\"#top\">Top</a>\n";
		outxsl << "</div>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"interiorOrientation\">\n";
		outxsl << "<a onclick =\"javascript:ShowHide('interiorOrientation')\" href=\"javascript:;\"><h3>Interior Orientations</h3></a>\n";
		outxsl << "<div id=\"interiorOrientation\" style=\"DISPLAY: none\">\n";
		outxsl << "<xsl:for-each select=\"interiorOrientation/IO\">\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('interiorOrientation_image_<xsl:value-of select=\"imageId\" />')</xsl:attribute>Image Id <xsl:value-of select=\"imageId\"/></a>\n";
		outxsl << "<br/><br/>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">interiorOrientation_image_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "IO Type: <xsl:value-of select=\"ioType\"/><br/><br/>\n";
		outxsl << "<xsl:if test=\"Xa\">\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('interiorOrientation_Xa_<xsl:value-of select=\"imageId\" />')</xsl:attribute>X<sub>a</sub></a>\n";
		outxsl << "<br/><br/>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">interiorOrientation_Xa_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "<table border=\"1\">\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">a0</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"Xa/a0\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">a1</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"Xa/a1\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">a2</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"Xa/a2\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">b0</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"Xa/b0\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">b1</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"Xa/b1\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">b2</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"Xa/b2\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "</table>\n";
		outxsl << "</div><br/>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"V\">\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('interiorOrientation_V_<xsl:value-of select=\"imageId\" />')</xsl:attribute>Matrix V</a>\n";
		outxsl << "<br/><br/>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">interiorOrientation_V_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "<table border=\"1\">\n";
		outxsl << "<xsl:for-each select=\"V/mml:matrix/mml:matrixrow\">\n";
		outxsl << "<tr><td align=\"center\"><xsl:value-of select=\"mml:cn\"/></td></tr>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "</table>\n";
		outxsl << "</div><br/>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"sigmaXa\">\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('interiorOrientation_sigmaXa_<xsl:value-of select=\"imageId\" />')</xsl:attribute>&#x3A3;X<sub>a</sub></a>\n";
		outxsl << "<br/><br/>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">interiorOrientation_sigmaXa_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "<table border=\"1\">\n";
		outxsl << "<xsl:for-each select=\"sigmaXa/mml:matrix/mml:matrixrow\">\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[1]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[2]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[3]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[4]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[5]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[6]\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "</table>\n";
		outxsl << "</div><br/>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "</div>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "<a href=\"#top\">Top</a>\n";
		outxsl << "</div>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"exteriorOrientation\">\n";
		outxsl << "<a onclick =\"javascript:ShowHide('exteriorOrientation')\" href=\"javascript:;\"><h3>Exterior Orientations</h3></a>\n";
		outxsl << "<div id=\"exteriorOrientation\" style=\"DISPLAY: none\">\n";
		outxsl << "<xsl:for-each select=\"exteriorOrientation/EO\">\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('exteriorOrientation_image_<xsl:value-of select=\"imageId\" />')</xsl:attribute>Image Id <xsl:value-of select=\"imageId\"/></a>\n";
		outxsl << "<br/><br/>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">exteriorOrientation_image_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "EO Type: <xsl:value-of select=\"eoType\"/><br/>\n";
		outxsl << "Number Of Iterations: <xsl:value-of select=\"noIterations\"/><br/>\n";
		outxsl << "<xsl:if test=\"Xa\">\n";
		outxsl << "<br/>\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('exteriorOrientation_Xa_<xsl:value-of select=\"imageId\" />')</xsl:attribute>X<sub>a</sub></a>\n";
		outxsl << "<br/><br/>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">exteriorOrientation_Xa_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "<table border=\"1\">\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">X0</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"Xa/X0\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">Y0</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"Xa/Y0\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">Z0</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"Xa/Z0\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">&#x3C9;</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"Xa/omega\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">&#x3C6;</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"Xa/phi\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">&#x3BA;</td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"Xa/kappa\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "</table>\n";
		outxsl << "</div><br/>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"V\">\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('exteriorOrientation_V_<xsl:value-of select=\"imageId\" />')</xsl:attribute>Matrix V</a>\n";
		outxsl << "<br/><br/>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">exteriorOrientation_V_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "<table border=\"1\">\n";
		outxsl << "<xsl:for-each select=\"V/mml:matrix/mml:matrixrow\">\n";
		outxsl << "<tr><td align=\"center\"><xsl:value-of select=\"mml:cn\"/></td></tr>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "</table>\n";
		outxsl << "</div><br/>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"sigmaXa\">\n";
		outxsl << "<a href=\"javascript:;\"><xsl:attribute name=\"onclick\">javascript:ShowHide('exteriorOrientation_sigmaXa_<xsl:value-of select=\"imageId\" />')</xsl:attribute>&#x3A3;X<sub>a</sub></a>\n";
		outxsl << "<br/><br/>\n";
		outxsl << "<div style=\"DISPLAY: none;\"><xsl:attribute name=\"id\">exteriorOrientation_sigmaXa_<xsl:value-of select=\"imageId\" /></xsl:attribute>\n";
		outxsl << "<table border=\"1\">\n";
		outxsl << "<xsl:for-each select=\"sigmaXa/mml:matrix/mml:matrixrow\">\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[1]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[2]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[3]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[4]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[5]\"/></td>\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"*[6]\"/></td>\n";
		outxsl << "</tr>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "</table>\n";
		outxsl << "</div><br/>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "</div>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "<a href=\"#top\">Top</a>\n";
		outxsl << "</div>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "<xsl:if test=\"stereoPairs\">\n";
		outxsl << "<a onclick =\"javascript:ShowHide('stereoPairs')\" href=\"javascript:;\"><h3>Stereo Pairs</h3></a>\n";
		outxsl << "<div id=\"stereoPairs\" style=\"DISPLAY: none\">\n";
		outxsl << "<table border=\"1\">\n";
		outxsl << "<tr><td colspan=\"2\" align=\"center\">Stereo Pair</td></tr>\n";
		outxsl << "<tr>\n";
		outxsl << "<td align=\"center\">Image Id(1)</td>\n";
		outxsl << "<td align=\"center\">Image Id(2)</td>\n";
		outxsl << "</tr>\n";
		outxsl << "<xsl:for-each select=\"stereoPairs/stereoPair\">\n";
		outxsl << "<tr>\n";
		outxsl << "<xsl:for-each select=\"imageId\">\n";
		outxsl << "<td align=\"center\"><xsl:value-of select=\"text()\"/></td>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "</tr>\n";
		outxsl << "</xsl:for-each>\n";
		outxsl << "</table>\n";
		outxsl << "<a href=\"#top\">Top</a>\n";
		outxsl << "</div>\n";
		outxsl << "</xsl:if>\n";
		outxsl << "</body>\n";
		outxsl << "</html>\n";
		outxsl << "</xsl:template>\n";
		outxsl << "</xsl:stylesheet>\n";
		std::ofstream myXsltFile(xsltransformation.c_str());
		EDomElement xsl(outxsl.str());
		myXsltFile << xsl.removeBlankLines(true).indent('\t').getContent() ;
		myXsltFile.close();
	}

	return true;
}

QImage ReportManager::makeThumbnail(QString filename, int maxW, int maxH)
{
	SingleScene ss(0, filename);
	return ss.getThumb(QSize(maxW, maxH));
}

/*string ReportManager::getImageIds(int )
{

}*/


} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
