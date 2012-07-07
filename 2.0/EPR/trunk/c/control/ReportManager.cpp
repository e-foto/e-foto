/**************************************************************************
		  ReportManager.cpp
**************************************************************************/

#include "ReportManager.h"
#include "EFotoManager.h"
#include "ReportUserInterface.h"
#include "ReportUserInterface_Qt.h"
#include "ProjectiveRay.h"

#include "BundleAdjustment.h"



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
	/*
	if (manager != NULL && mySensor != NULL && myImage != NULL && myIO != NULL)
		if (myImage->getSensorId() == mySensor->getId() && myIO->getImageId() == myImage->getId())
		{
			if (manager->getInterfaceType().compare("Qt") == 0)
			{
				//myInterface = new IOUserInterface_Qt(this);
				myInterface = IOUserInterface_Qt::instance(this);
			}
			myImage->setSensor(mySensor);
			myIO->setImage(myImage);
			started = true;
			if (myInterface != NULL)
			{
				myInterface->exec();
			}
			//if (manager->getInterfaceType().compare("Qt") == 0)
			//{
			//IOUserInterface_Qt::createInstance(this)->exec();
			//}
		}
	return status;
	*/

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
        //efotoManager->reloadProject();
        LoadingScreen::instance().close();
}

string ReportManager::getAllXml()
{
    return project->getXml();
}

EDomElement ReportManager::getHeaderXml()
{
    return EDomElement (project->header()->xmlGetData());
}

EDomElement ReportManager::getTerrainXml()
{
    return EDomElement (project->terrain()->xmlGetData());
}

EDomElement ReportManager::getSensorXml()
{
    return EDomElement (project->sensor(1)->xmlGetData());
}

EDomElement ReportManager::getFlightXml()
{
    return EDomElement (project->flight(1)->xmlGetData());
}

EDomElement ReportManager::getImageXml(int id)
{
    return EDomElement (project->image(id)->xmlGetData());
}

EDomElement ReportManager::getInteriorOrientationXml(int id)
{
    return EDomElement (project->IO(id)->xmlGetData());
}

EDomElement ReportManager::getExteriorOrientationXml(int id)
{
    return EDomElement (project->EO(id)->xmlGetData());
}

void ReportManager::checkTree(QList<QTreeWidgetItem*> treeItems)
{
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
        //SpatialRessection
        treeItems.at(14)->setCheckState(0,Qt::Unchecked);
        treeItems.at(14)->setDisabled(true);
        //SpatialRessection/InitializationData
        treeItems.at(15)->setCheckState(0,Qt::Unchecked);
        treeItems.at(15)->setDisabled(true);
        //SpatialRessection/QualityData
        treeItems.at(16)->setCheckState(0,Qt::Unchecked);
        treeItems.at(16)->setDisabled(true);
        //SpatialRessection/Values at each iteration
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
    }
}

string ReportManager::eprHeader()
{
    ProjectHeader *cabecalho = project->header();
    string txt;
    txt = "<projectHeader>\n";
    txt += "<name>" + cabecalho->getName() + "</name>\n";
    txt += "<description>" + cabecalho->getDescription() + "</description>\n";
    txt += "<creation>" + cabecalho->getCreation() + "</creation>\n";
    txt += "<modification>" + cabecalho->getModification() + "</modification>\n";
    txt += "<owner>" + cabecalho->getOwner() + "</owner>\n";
    txt += "</projectHeader>\n";
    return txt;
}

string ReportManager::eprTerrain()
{
    Terrain *terreno = project->terrain();
    string txt,direction;
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

string ReportManager::eprSensors()
{
    Sensor *sensores = project->sensor(1);
    string txt;    
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

string ReportManager::eprFlights()
{
    Flight *voos = project->flight(1);
    string txt;    
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

string ReportManager::eprImages(QList<QTreeWidgetItem*> treeItems)
{
    string txt;
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
            InteriorOrientation* io = img->getIO();
            if (NULL != io)
            {
                txt += "<interiorOrientation>\n";
                txt += "<ioType>Affine</ioType>\n";
                txt +=  "<Xa>\n";
                txt +=  "<a0>" + Conversion::doubleToString(io->getXaa0()) + "</a0>\n";
                txt +=  "<a1>" + Conversion::doubleToString(io->getXaa1()) + "</a1>\n";
                txt +=  "<a2>" + Conversion::doubleToString(io->getXaa2()) + "</a2>\n";
                txt +=  "<b0>" + Conversion::doubleToString(io->getXab0()) + "</b0>\n";
                txt +=  "<b1>" + Conversion::doubleToString(io->getXab1()) + "</b1>\n";
                txt +=  "<b2>" + Conversion::doubleToString(io->getXab2()) + "</b2>\n";
                txt +=  "</Xa>\n";
                txt += "</interiorOrientation>\n";
            }
            SpatialRessection* eo = img->getEO();
            if (NULL != eo)
            {
                txt += "<exteriorOrientation>\n";
                txt += "<eoType>Spatial Ressection</eoType>\n";
                txt +=  "<Xa>\n";
                txt +=  "<X0>" + Conversion::doubleToString(eo->getXaX0()) + "</X0>\n";
                txt +=  "<Y0>" + Conversion::doubleToString(eo->getXaY0()) + "</Y0>\n";
                txt +=  "<Z0>" + Conversion::doubleToString(eo->getXaZ0()) + "</Z0>\n";
                txt +=  "<phi>" + Conversion::doubleToString(eo->getXaphi()) + "</phi>\n";
                txt +=  "<omega>" + Conversion::doubleToString(eo->getXaomega()) + "</omega>\n";
                txt +=  "<kappa>" + Conversion::doubleToString(eo->getXakappa()) + "</kappa>\n";
                txt +=  "</Xa>\n";
                txt += "</exteriorOrientation>\n";
            }
        }
        txt += "</image>\n";
    }
    txt += "</images>\n";
    return txt;
}

string ReportManager::eprBlockPoints(QList<QTreeWidgetItem*> treeItems)
{
    string txt;
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

string ReportManager::eprAffineTransformation(QList<QTreeWidgetItem*> treeItems)
{
    string txt;
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
                        deque<ImageFiducialMark> fidMarks = io->getImage()->getDigFidMarks();
                        SensorWithFiducialMarks* sensor = (SensorWithFiducialMarks*) io->getImage()->getSensor();
                        txt += "<fidMarks>\n";
                        for(unsigned int j = 0; j < fidMarks.size(); j++)
                        {
                            txt += "<fidMark>\n";
                            QStringList gmlpos = QString::fromStdString(sensor->getAnaFidMarkAt(j).getGmlPos()).split(" ");
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
                    txt += io->getQuality().getV().xmlGetData();
                    txt += "</V>\n";
                    txt += "<sigmaXa>\n";
                    txt += io->getQuality().getSigmaXa().xmlGetData();
                    txt += "</sigmaXa>\n";
                    txt += "<sigmaLa>\n";
                    txt += io->getQuality().getSigmaLa().xmlGetData();
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

string ReportManager::eprSpatialRessection(QList<QTreeWidgetItem*> treeItems)
{
    string txt;
    txt = "<spatialRessection>\n";
    for(unsigned int i = 0; i < project->allEOs().size(); i++)
    {
        SpatialRessection* sr = (SpatialRessection*) project->allEOs().at(i);
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
                txt += "<phi>" + Conversion::doubleToString(sr->getImage()->getInsPhi()) + "</phi>\n";
                txt += "<omega>" + Conversion::doubleToString(sr->getImage()->getInsOmega()) + "</omega>\n";
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
                    txt += "<lin>" + Conversion::doubleToString(point->getImageCoordinate(sr->getImageId()).getLin()) + "</lin>\n";
                    txt += "<col>" + Conversion::doubleToString(point->getImageCoordinate(sr->getImageId()).getCol()) + "</col>\n";
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
            txt += sr->getQuality().getV().xmlGetData();
            txt += "</V>\n";
            txt += "<sigmaXa>\n";
            txt += sr->getQuality().getSigmaXa().xmlGetData();
            txt += "</sigmaXa>\n";
            txt += "<sigmaLa>\n";
            txt += sr->getQuality().getSigmaLa().xmlGetData();
            txt += "</sigmaLa>\n";
            txt += "<sigma0Squared>" + Conversion::doubleToString(sr->getQuality().getsigma0Squared()) + "</sigma0Squared>\n";
            txt += "</qualityData>\n";
        }
        if(treeItems.at(17)->checkState(0)==2)
        {
            txt += "<valuesOfEachIteration></valuesOfEachIteration>\n";
        }
        txt += "</SR>\n";
    }
    txt += "</spatialRessection>\n";
    return txt;
}

string ReportManager::eprPhotogrammetricBlock(QList<QTreeWidgetItem*> treeItems)
{
    string txt;
    if (project->photoTri() == NULL)
        return "";

    deque<Image*> usedImages = project->photoTri()->getImages();
    deque<Point*> usedPoints = project->photoTri()->getPoints();
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
        deque<double> RMSEs = bundle->getListRMSE();
        txt += "<rmseOfEachIteration>\n";
        for(unsigned int j=0;j<RMSEs.size();j++)
        {
            txt += "<rmse"+Conversion::intToString(j)+">" + Conversion::doubleToString(RMSEs.at(j)) + "</rmse"+Conversion::intToString(j)+">\n";
        }
        txt += "</rmseOfEachIteration>\n";

    }

    txt += "</photogrammetricBlock>\n";
    return txt;
}

string ReportManager::eprInteriorOrientation(QList<QTreeWidgetItem*> treeItems)
{
    string txt;
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
                txt +=  "<a0>" + Conversion::doubleToString(io->getXaa0()) + "</a0>\n";
                txt +=  "<a1>" + Conversion::doubleToString(io->getXaa1()) + "</a1>\n";
                txt +=  "<a2>" + Conversion::doubleToString(io->getXaa2()) + "</a2>\n";
                txt +=  "<b0>" + Conversion::doubleToString(io->getXab0()) + "</b0>\n";
                txt +=  "<b1>" + Conversion::doubleToString(io->getXab1()) + "</b1>\n";
                txt +=  "<b2>" + Conversion::doubleToString(io->getXab2()) + "</b2>\n";
                txt += "</Xa>\n";
            }
            if(treeItems.at(23)->checkState(0)==2)
            {
                txt += "<V>\n";
                txt += io->getQuality().getV().xmlGetData();
                txt += "</V>\n";
            }
            if(treeItems.at(24)->checkState(0)==2)
            {
                txt += "<sigmaXa>\n";
                txt += io->getQuality().getSigmaXa().xmlGetData();
                txt += "</sigmaXa>\n";
            }
            txt += "</IO>\n";
        }
    }
    txt += "</interiorOrientation>\n";
    return txt;
}

string ReportManager::eprExteriorOrientation(QList<QTreeWidgetItem*> treeItems)
{
    string txt;
    Image *img;
    SpatialRessection* eo;
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
                // Pensar! Pegar o EO Type
                txt += "<eoType>Spatial Ressection</eoType>\n";
            }
            if(treeItems.at(27)->checkState(0)==2)
            {
                // Pensar! Pegar o Total Iterations
                //txt += "<noIterations>" + Conversion::intToString(eo->getTotalIterations()) + "</noIterations>\n";
                txt += "<noIterations>0</noIterations>\n";
            }
            if(treeItems.at(28)->checkState(0)==2)
            {
                txt +=  "<Xa>\n";
                txt +=  "<X0>" + Conversion::doubleToString(eo->getXaX0()) + "</X0>\n";
                txt +=  "<Y0>" + Conversion::doubleToString(eo->getXaY0()) + "</Y0>\n";
                txt +=  "<Z0>" + Conversion::doubleToString(eo->getXaZ0()) + "</Z0>\n";
                txt +=  "<phi>" + Conversion::doubleToString(eo->getXaphi()) + "</phi>\n";
                txt +=  "<omega>" + Conversion::doubleToString(eo->getXaomega()) + "</omega>\n";
                txt +=  "<kappa>" + Conversion::doubleToString(eo->getXakappa()) + "</kappa>\n";
                txt +=  "</Xa>\n";
            }
            if(treeItems.at(29)->checkState(0)==2)
            {
                txt +=  "<V>\n";
                txt +=  eo->getQuality().getV().xmlGetData();
                txt +=  "</V>\n";
            }
            if(treeItems.at(30)->checkState(0)==2)
            {
                txt +=  "<sigmaXa>\n";
                txt +=  eo->getQuality().getSigmaXa().xmlGetData();
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

string ReportManager::eprStereoPairs()
{
    string txt;
    txt = "<stereoPairs>\n";

    //
    // Derivado do código nas interfaces implementadas pelo Marcelo.
    //
    if (project->allImages().size() < 2)
        return "<stereoPairs></stereoPairs>\n";

    Image *img;
    double X1, Y1, X2, Y2, R, dist, best_dist, bX2, bY2, overlap, align_ang, kappa;
    int p1, p2, best_img;
    Matrix Xa;

    // Calculate Image Radius
    ObjectSpaceCoordinate osc;
    img = project->allImages().at(0);
    ProjectiveRay pr(img);
    Xa = img->getEO()->getXa();
    X1 = Xa.get(1,1);
    Y1 = Xa.get(2,1);
    p1 = img->getWidth();
    p2 = img->getHeight()/2;
    osc = pr.imageToObject(p1,p2,img->getHeight(),false);
    X2 = osc.getX();
    Y2 = osc.getY();
    R = sqrt(pow(X1-X2,2) + pow(Y1-Y2,2));

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
            if (i==j)
                continue;

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

string ReportManager::eprStereoPlotting()
{
    string txt;
    txt = "<stereoPlotting>Teste 13</stereoPlotting>\n";
    return txt;
}

string ReportManager::eprDSM()
{
    string txt;
    txt = "<dsm>Teste 14</dsm>\n";
    return txt;
}

string ReportManager::eprOrthorectification()
{
    string txt;
    txt = "<orthoRectification>Teste 15</orthoRectification>\n";
    return txt;
}

bool ReportManager::makeFile(string filename, int idExt, QList<QTreeWidgetItem*> treeItems)
{
    stringstream out;
    ofstream myFile (filename.c_str());
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

bool ReportManager::makeXslt(int idExt, string path)
{
    string xsltransformation;
    stringstream outxsl;
    xsltransformation = path.c_str();    
    outxsl << "<xsl:stylesheet version=\"1.0\" xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" xmlns:mml=\"http://www.w3.org/1998/Math/MathML\">\n";

    if(idExt == 1)
    {
        xsltransformation += "/epr_txt.xsl";
        outxsl << "<xsl:output method=\"xml\" indent=\"yes\" encoding=\"UTF-8\" omit-xml-declaration=\"yes\"/>\n";
        outxsl << "\t<xsl:template match=\"efotoPhotogrammetricReport\">\n";
        outxsl << "\tBegin of E-Foto Photogrammetric Report\n";
        outxsl << "\n";
        outxsl << "<xsl:if test=\"projectHeader\">\n";
        outxsl << "\t====================================================================\n";
        outxsl << "\t\tProject Header:\n";
        outxsl << "\n";
        outxsl << "\t\tName: <xsl:value-of select=\"projectHeader/name\"/>\n";
        outxsl << "\t\tDescription: <xsl:value-of select=\"projectHeader/description\"/>\n";
        outxsl << "\t\tCreation Date: <xsl:value-of select=\"projectHeader/creation\"/>\n";
        outxsl << "\t\tModification Date: <xsl:value-of select=\"projectHeader/modification\"/>\n";
        outxsl << "\t\tOwner: <xsl:value-of select=\"projectHeader/owner\"/>\n";
        outxsl << "====================================================================\n";
        outxsl << "\t</xsl:if>\n";
        outxsl << "\n\n";
        outxsl << "\t<xsl:if test=\"terrain\">\n";
        outxsl << "\t====================================================================\n";
        outxsl << "\t\tTerrain:\n";
        outxsl << "\n";
        outxsl << "\t\tMean Altitude: <xsl:value-of select=\"terrain/meanAltitude\"/>\n";
        outxsl << "\t\tMinimum Altitude: <xsl:value-of select=\"terrain/minAltitude\"/>\n";
        outxsl << "\t\tMaximum Altitude: <xsl:value-of select=\"terrain/maxAltitude\"/>\n";
        outxsl << "\t\tGRS: <xsl:value-of select=\"terrain/GRS\"/>\n";
        outxsl << "\t\tCPS: <xsl:value-of select=\"terrain/CPS\"/>\n";
        outxsl << "\t\tWork Area Center Coordinates:";
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
        outxsl << "\t====================================================================\n";
        outxsl << "\t</xsl:if>\n";
        outxsl << "\n\n";
        outxsl << "\t<xsl:if test=\"sensors\">\n";
        outxsl << "\t====================================================================\n";
        outxsl << "\t\tSensors:\n";
        outxsl << "\n";
        outxsl << "\t\tSensor Id: <xsl:value-of select=\"sensors/sensorId\"/>\n";
        outxsl << "\t\tDescription: <xsl:value-of select=\"sensors/description\"/>\n";
        outxsl << "\t\tFocal Distance: <xsl:value-of select=\"sensors/focalDistance/value\"/>\n";
        outxsl << "====================================================================\n";
        outxsl << "\t</xsl:if>\n";
        outxsl << "\n\n";
        outxsl << "\t<xsl:if test=\"flights\">\n";
        outxsl << "====================================================================\n";
        outxsl << "\t\tFlights:\n";
        outxsl << "\n";
        outxsl << "\t\t<xsl:for-each select=\"flights\">\n";
        outxsl << "\t\tFlight Id: <xsl:value-of select=\"flightId\"/>\n";
        outxsl << "\t\tExecution Date: <xsl:value-of select=\"execution\"/>\n";
        outxsl << "\t\tNominal Scale: <xsl:value-of select=\"nominalScale/mml:mfrac/*[1]\"/>:\n";
        outxsl << "\t\t<xsl:value-of select=\"nominalScale/mml:mfrac/*[2]\"/>\n";
        outxsl << "\t\tLongitudinal Overlap: <xsl:value-of select=\"overlap/longitudinal\"/>\n";
        outxsl << "\t\tTransversal Overlap: <xsl:value-of select=\"overlap/transversal\"/>\n";
        outxsl << "\t\t</xsl:for-each>\n";
        outxsl << "\t====================================================================\n";
        outxsl << "\t</xsl:if>\n";
        outxsl << "\n\n";
        outxsl << "\t<xsl:if test=\"images\">\n";
        outxsl << "\t====================================================================\n";
        outxsl << "\t\tImages:\n";
        outxsl << "\n";
        outxsl << "\t\t-----------------------------------------------------------------\n";
        outxsl << "\t\t<xsl:for-each select=\"images/image\">\n";
        outxsl << "\n";
        outxsl << "\t\tImage Id: <xsl:value-of select=\"imageId\"/>\n";
        outxsl << "\t\tDimension: <xsl:value-of select=\"width\"/>x<xsl:value-of select=\"heigth\"/> px\n";
        outxsl << "\t\tFilename: <xsl:value-of select=\"fileName\"/>\n";
        outxsl << "\t\tFilepath: <xsl:value-of select=\"filePath\"/>\n";
        outxsl << "\t\tResolution: <xsl:value-of select=\"resolution\"/> dpi\n";
        outxsl << "\n";
        outxsl << "\t\t\t<xsl:if test=\"interiorOrientation\">\n";
        outxsl << "\t\t\tInterior Orientation - <xsl:value-of select=\"interiorOrientation/ioType\"/>:\n";
        outxsl << "\n";
        outxsl << "\t\t\t\t\t\t\t\t\t\tXa\n";
        outxsl << "\t\t\t\t|\ta0\t|\t<xsl:value-of select=\"interiorOrientation/Xa/a0\"/>\n";
        outxsl << "\t\t\t\t|\ta1\t|\t<xsl:value-of select=\"interiorOrientation/Xa/a1\"/>\n";
        outxsl << "\t\t\t\t|\ta2\t|\t<xsl:value-of select=\"interiorOrientation/Xa/a2\"/>\n";
        outxsl << "\t\t\t\t|\tb0\t|\t<xsl:value-of select=\"interiorOrientation/Xa/b0\"/>\n";
        outxsl << "\t\t\t\t|\tb1\t|\t<xsl:value-of select=\"interiorOrientation/Xa/b1\"/>\n";
        outxsl << "\t\t\t\t|\tb2\t|\t<xsl:value-of select=\"interiorOrientation/Xa/b2\"/>\n";
        outxsl << "\t\t\t</xsl:if>\n";
        outxsl << "\n";
        outxsl << "\t\t\t<xsl:if test=\"exteriorOrientation\">\n";
        outxsl << "\n";
        outxsl << "\t\t\tExterior Orientation - <xsl:value-of select=\"exteriorOrientation/eoType\"/>:\n";
        outxsl << "\n";
        outxsl << "\t\t\t\t\t\t\t\t\t\tXa\n";
        outxsl << "\t\t\t\t|\tX0\t\t|\t<xsl:value-of select=\"exteriorOrientation/Xa/X0\"/>\n";
        outxsl << "\t\t\t\t|\tY0\t\t|\t<xsl:value-of select=\"exteriorOrientation/Xa/Y0\"/>\n";
        outxsl << "\t\t\t\t|\tZ0\t\t|\t<xsl:value-of select=\"exteriorOrientation/Xa/Z0\"/>\n";
        outxsl << "\t\t\t\t|\tPhi\t\t|\t<xsl:value-of select=\"exteriorOrientation/Xa/phi\"/>\n";
        outxsl << "\t\t\t\t|\tOmega\t|\t<xsl:value-of select=\"exteriorOrientation/Xa/omega\"/>\n";
        outxsl << "\t\t\t\t|\tKappa\t|\t<xsl:value-of select=\"exteriorOrientation/Xa/kappa\"/>\n";
        outxsl << "\t\t\t</xsl:if>\n";
        outxsl << "\t\t-----------------------------------------------------------------\n";
        outxsl << "\t\t</xsl:for-each>\n";
        outxsl << "\t====================================================================\n";
        outxsl << "\t</xsl:if>\n";
        outxsl << "\n\n";
        outxsl << "\t<xsl:if test=\"blockPoints\">\n";
        outxsl << "\t====================================================================\n";
        outxsl << "\t\tBlock Of Points:\n";
        outxsl << "\t\t-----------------------------------------------------------------\n";
        outxsl << "\t\t<xsl:for-each select=\"blockPoints/point\">\n";
        outxsl << "\n";
        outxsl << "\t\tPoint Id: <xsl:value-of select=\"pointId\"/>\n";
        outxsl << "\t\tType: <xsl:value-of select=\"pointType\"/>\n";
        outxsl << "\t\t\t<xsl:if test=\"spatialCoordinates\">\n";
        outxsl << "\n";
        outxsl << "\t\t\t\t\tSpatial Coordinates\n";
        outxsl << "\t\t|\tX\t|\t<xsl:value-of select=\"spatialCoordinates/x\"/>\n";
        outxsl << "\t\t|\tY\t|\t<xsl:value-of select=\"spatialCoordinates/y\"/>\n";
        outxsl << "\t\t|\tZ\t|\t<xsl:value-of select=\"spatialCoordinates/z\"/>\n";
        outxsl << "\t\t|\tUnit\t|\t<xsl:if test=\"spatialCoordinates/unit = 'm'\">Meters</xsl:if>\n";
        outxsl << "\t\t\t</xsl:if>\n";
        outxsl << "\t\t\t<xsl:if test=\"imagesMeasurements\">\n";
        outxsl << "\n";
        outxsl << "\t\t\t\t\tImage Coordinates\n";
        outxsl << "\t\t\t<xsl:for-each select=\"imagesMeasurements/imageCoordinates\">\n";
        outxsl << "\n";
        outxsl << "\t\tImage Id: <xsl:value-of select=\"imageId\"/>\n";
        outxsl << "\t\tLine: <xsl:value-of select=\"position/lin\"/>\n";
        outxsl << "\t\tColumn: <xsl:value-of select=\"position/col\"/>\n";
        outxsl << "\t\tUnit:	<xsl:if test=\"position/unit = 'px'\">pixels</xsl:if>\n";
        outxsl << "\t\t\t</xsl:for-each>\n";
        outxsl << "\t\t\t</xsl:if>\n";
        outxsl << "\n";
        outxsl << "\t\t-----------------------------------------------------------------\n";
        outxsl << "\t\t</xsl:for-each>\n";
        outxsl << "\t====================================================================\n";
        outxsl << "\t</xsl:if>\n";
        outxsl << "\n\n";
        outxsl << "\tEnd of E-Foto Photogrammetric Report\n";
        outxsl << "\t</xsl:template>\n";
        outxsl << "</xsl:stylesheet>\n";
        ofstream myXsltFile(xsltransformation.c_str());
        myXsltFile << outxsl.str();
        myXsltFile.close();
    } else {
        xsltransformation += "/epr_html.xsl";
        outxsl << "<xsl:template match=\"efotoPhotogrammetricReport\">\n";
        outxsl << "<html>\n";
        outxsl << "<body>\n";
        outxsl << "<h2>E-Foto Photogrammetric Report</h2>\n";
        outxsl << "<xsl:if test=\"projectHeader\">\n";
        outxsl << "<h3>Project Header</h3>\n";
        outxsl << "Name: <xsl:value-of select=\"projectHeader/name\"/><br/>\n";
        outxsl << "Description: <xsl:value-of select=\"projectHeader/description\"/><br/>\n";
        outxsl << "Creation Date: <xsl:value-of select=\"projectHeader/creation\"/><br/>\n";
        outxsl << "Modification Date: <xsl:value-of select=\"projectHeader/modification\"/><br/>\n";
        outxsl << "Owner: <xsl:value-of select=\"projectHeader/owner\"/><br/>\n";
        outxsl << "</xsl:if>\n";
        outxsl << "<xsl:if test=\"terrain\">\n";
        outxsl << "<h3>Terrain</h3>\n";
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
        outxsl << "</xsl:if>\n";
        outxsl << "<xsl:if test=\"sensors\">\n";
        outxsl << "<h3>Sensors</h3>\n";
        outxsl << "Sensor Id: <xsl:value-of select=\"sensors/sensorId\"/><br/>\n";
        outxsl << "Description: <xsl:value-of select=\"sensors/description\"/><br/>\n";
        outxsl << "Focal Distance: <xsl:value-of select=\"sensors/focalDistance\"/><br/>\n";
        outxsl << "</xsl:if>\n";
        outxsl << "<xsl:if test=\"flights\">\n";
        outxsl << "<h3>Flights</h3>\n";
        outxsl << "<xsl:for-each select=\"flights\">\n";
        outxsl << "Flight Id: <xsl:value-of select=\"flightId\"/><br/>\n";
        outxsl << "Execution Date: <xsl:value-of select=\"execution\"/><br/>\n";
        outxsl << "Nominal Scale: <xsl:value-of select=\"nominalScale/mml:mfrac/*[1]\"/>:\n";
        outxsl << "<xsl:value-of select=\"nominalScale/mml:mfrac/*[2]\"/><br/><br/>\n";
        outxsl << "Longitudinal Overlap: <xsl:value-of select=\"overlap/longitudinal\"/><br/>\n";
        outxsl << "Transversal Overlap: <xsl:value-of select=\"overlap/transversal\"/><br/>\n";
        outxsl << "</xsl:for-each>\n";
        outxsl << "</xsl:if>\n";
        outxsl << "<xsl:if test=\"images\">\n";
        outxsl << "<h3>Images</h3>\n";
        outxsl << "<xsl:for-each select=\"images/image\">\n";
        outxsl << "Image Id: <xsl:value-of select=\"imageId\"/><br/>\n";
        outxsl << "Dimension: <xsl:value-of select=\"width\"/>x<xsl:value-of select=\"heigth\"/> px<br/>\n";
        outxsl << "Filename: <xsl:value-of select=\"fileName\"/><br/>\n";
        outxsl << "Filepath: <xsl:value-of select=\"filePath\"/><br/>\n";
        outxsl << "Resolution: <xsl:value-of select=\"resolution\"/> dpi<br/>\n";
        outxsl << "<xsl:if test=\"interiorOrientation\">\n";
        outxsl << "<br/><i>Interior Orientation</i><br/><br/>\n";
        outxsl << "<table border=\"1\">\n";
        outxsl << "<tr>\n";
        outxsl << "<td align=\"center\"><xsl:value-of select=\"interiorOrientation/ioType\"/></td>\n";
        outxsl << "<td align=\"center\"><b>Xa</b></td>\n";
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
        outxsl << "</table><br/>\n";
        outxsl << "</xsl:if>\n";
        outxsl << "<xsl:if test=\"exteriorOrientation\">\n";
        outxsl << "<br/><i>Exterior Orientation</i><br/><br/>\n";
        outxsl << "<table border=\"1\">\n";
        outxsl << "<tr>\n";
        outxsl << "<td align=\"center\"><xsl:value-of select=\"exteriorOrientation/eoType\"/></td>\n";
        outxsl << "<td align=\"center\"><b>Xa</b></td>\n";
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
        outxsl << "<td align=\"center\">phi</td>\n";
        outxsl << "<td align=\"center\"><xsl:value-of select=\"exteriorOrientation/Xa/phi\"/></td>\n";
        outxsl << "</tr>\n";
        outxsl << "<tr>\n";
        outxsl << "<td align=\"center\">omega</td>\n";
        outxsl << "<td align=\"center\"><xsl:value-of select=\"exteriorOrientation/Xa/omega\"/></td>\n";
        outxsl << "</tr>\n";
        outxsl << "<tr>\n";
        outxsl << "<td align=\"center\">kappa</td>\n";
        outxsl << "<td align=\"center\"><xsl:value-of select=\"exteriorOrientation/Xa/kappa\"/></td>\n";
        outxsl << "</tr>\n";
        outxsl << "</table><br/>\n";
        outxsl << "</xsl:if>\n";
        outxsl << "</xsl:for-each>\n";
        outxsl << "</xsl:if>\n";
        outxsl << "<xsl:if test=\"blockPoints\">\n";
        outxsl << "<h3>Block Of Points</h3>\n";
        outxsl << "<xsl:for-each select=\"blockPoints/point\">\n";
        outxsl << "Point Id: <xsl:value-of select=\"pointId\"/><br/>\n";
        outxsl << "Type: <xsl:value-of select=\"pointType\"/><br/>\n";
        outxsl << "<xsl:if test=\"spatialCoordinates\">\n";
        outxsl << "<table border=\"1\">\n";
        outxsl << "<tr>\n";
        outxsl << "<td colspan=\"2\" align=\"center\">Spatial Coordinates</td>\n";
        outxsl << "</tr>\n";
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
        outxsl << "<td align=\"center\">Unit</td>\n";
        outxsl << "<td align=\"center\"><xsl:if test=\"spatialCoordinates/unit = 'm'\">Meters</xsl:if></td>\n";
        outxsl << "</tr>\n";
        outxsl << "</table><br/>\n";
        outxsl << "</xsl:if>\n";
        outxsl << "<xsl:if test=\"imagesMeasurements\">\n";
        outxsl << "<table border=\"1\">\n";
        outxsl << "<tr>\n";
        outxsl << "<td colspan=\"4\" align=\"center\">Image Coordinates</td>\n";
        outxsl << "</tr>\n";
        outxsl << "<tr>\n";
        outxsl << "<td align=\"center\">Image Id</td>\n";
        outxsl << "<td align=\"center\">Line</td>\n";
        outxsl << "<td align=\"center\">Column</td>\n";
        outxsl << "<td align=\"center\">Unit</td>\n";
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
        outxsl << "</table><br/>\n";
        outxsl << "</xsl:if>\n";
        outxsl << "</xsl:for-each>\n";
        outxsl << "</xsl:if>\n";
        outxsl << "</body>\n";
        outxsl << "</html>\n";
        outxsl << "</xsl:template>\n";
        outxsl << "</xsl:stylesheet>\n";
        ofstream myXsltFile(xsltransformation.c_str());
        EDomElement xsl(outxsl.str());
        myXsltFile << xsl.removeBlankLines(true).indent('\t').getContent() ;
        myXsltFile.close();
    }

    return true;
}

/*string ReportManager::getImageIds(int )
{

}*/


} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
