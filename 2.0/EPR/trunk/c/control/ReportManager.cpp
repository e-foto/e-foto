/**************************************************************************
		  ReportManager.cpp
**************************************************************************/

#include "ReportManager.h"
#include "EFotoManager.h"
#include "ReportUserInterface.h"
#include "ReportUserInterface_Qt.h"



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
    if(lat.hasSignal())
    {
        direction = "N";
    } else {
        direction = "S";
    }
    txt += "<Lat direction=\"" + direction + "\">\n";    
    txt += "<degrees>" + Conversion::intToString(lat.getDegree()) + "</degrees>\n";
    txt += "<minutes>" + Conversion::intToString(lat.getMinute()) + "</minutes>\n";
    txt += "<seconds>" + Conversion::doubleToString(lat.getSeconds(),2) + "</seconds>\n";
    txt += "</Lat>\n";
    if(lon.hasSignal())
    {
        direction = "W";
    } else {
        direction = "E";
    }
    txt += "<Long direction=\"" + direction + "\">\n";    
    txt += "<degrees>" + Conversion::intToString(lon.getDegree()) + "</degrees>\n";
    txt += "<minutes>" + Conversion::intToString(lon.getMinute()) + "</minutes>\n";
    txt += "<seconds>" + Conversion::doubleToString(lon.getSeconds(),2) + "</seconds>\n";
    txt += "</Long>\n";    
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
    txt = "<images>\n";    
    for(int i=0;i<project->allImages().size();i++)
    {
        txt += "<image>\n";
        Image *img = project->allImages().at(i);
        if(treeItems.at(5)->checkState(0)==2)
        {
            txt += "<imageId>" + img->getImageId() + "</imageId>\n";            
            txt += "<width>" + Conversion::intToString(img->getWidth()) + "</width>\n";
            txt += "<heigth>" + Conversion::intToString(img->getHeight()) + "</heigth>\n";
            txt += "<fileName>" + img->getFilename() + "</fileName>\n";
            txt += "<filePath>" + img->getFilepath() + "</filePath>\n";
            // Pensar! adicionar o caminho absoluto            
            txt += "<resolution>" + Conversion::intToString(img->getResolution()) + "</resolution>\n";
        }
        if(treeItems.at(6)->checkState(0)==2)
        {
            InteriorOrientation* io = img->getIO();
            if (NULL != io)
            {
                txt += "<InteriorOrientation>\n";
                txt += "<Type>Affine</Type>\n";
                txt +=  "<Xa>\n";
                txt +=  "<a0>" + Conversion::doubleToString(io->getXaa0()) + "</a0>\n";
                txt +=  "<a1>" + Conversion::doubleToString(io->getXaa1()) + "</a1>\n";
                txt +=  "<a2>" + Conversion::doubleToString(io->getXaa2()) + "</a2>\n";
                txt +=  "<b0>" + Conversion::doubleToString(io->getXab0()) + "</b0>\n";
                txt +=  "<b1>" + Conversion::doubleToString(io->getXab1()) + "</b1>\n";
                txt +=  "<b2>" + Conversion::doubleToString(io->getXab2()) + "</b2>\n";
                txt +=  "</Xa>\n";
                txt += "</InteriorOrientation>\n";
            }
            SpatialRessection* eo = img->getEO();
            if (NULL != eo)
            {
                txt += "<ExteriorOrientation>\n";               
                txt += "<Type>Spatial Ressection</Type>\n";
                txt +=  "<Xa>\n";
                txt +=  "<X0>" + Conversion::doubleToString(eo->getXaX0()) + "</X0>\n";
                txt +=  "<Y0>" + Conversion::doubleToString(eo->getXaY0()) + "</Y0>\n";
                txt +=  "<Z0>" + Conversion::doubleToString(eo->getXaZ0()) + "</Z0>\n";
                txt +=  "<phi>" + Conversion::doubleToString(eo->getXaphi()) + "</phi>\n";
                txt +=  "<omega>" + Conversion::doubleToString(eo->getXaomega()) + "</omega>\n";
                txt +=  "<kappa>" + Conversion::doubleToString(eo->getXakappa()) + "</kappa>\n";
                txt +=  "</Xa>\n";
                txt += "</ExteriorOrientation>\n";
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
    txt = "<block_points>\n";
    for(int i=0;i<project->allPoints().size();i++)
    {
        Point *pnt = project->allPoints().at(i);
        txt += "<point>\n";
        txt += "<pointId>" + pnt->getPointId() + "</pointId>\n";
        /*
        if(pnt->getType() == PointType::)
        txt += "<pointType></pointType>\n";*/
        if(treeItems.at(8)->checkState(0)==2)
        {
            txt += "spatialCoordinates\n";
        }
        if(treeItems.at(9)->checkState(0)==2)
        {
            txt += "imagesMeasurements\n";
        }
        txt += "</point>\n";
    }
    txt += "</block_points>\n";
    return txt;
}

string ReportManager::eprAffineTransformation()
{
string txt;
txt = "<affine_transformation>Teste 07</affine_transformation>\n";
return txt;
}

string ReportManager::eprSpatialRessection()
{
string txt;
txt = "<spatial_ressection>Teste 08</spatial_ressection>\n";
return txt;
}

string ReportManager::eprPhotogrammetricBlock()
{
string txt;
txt = "<photogrammetric_block>Teste 09</photogrammetric_block>\n";
return txt;
}

string ReportManager::eprInteriorOrientation()
{
string txt;
txt = "<interior_orientation>Teste 10</interior_orientation>\n";
return txt;
}

string ReportManager::eprExteriorOrientation()
{
string txt;
txt = "<exterior_orientation>Teste 11</exterior_orientation>\n";
return txt;
}

string ReportManager::eprStereoPairs()
{
string txt;
txt = "<stereo_pairs>Teste 12</stereo_pairs>\n";
return txt;
}

string ReportManager::eprStereoPlotting()
{
string txt;
txt = "<stereo_plotting>Teste 13</stereo_plotting>\n";
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
txt = "<orthorectification>Teste 15</orthorectification>\n";
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
                case 0:
                    out << eprHeader();
                break;
                case 1:
                   out << eprTerrain();
                break;
                case 2:
                    out << eprSensors();
                break;
                case 3:
                   out << eprFlights();
                break;
                case 4:
                    out << eprImages(treeItems);
                break;
                case 7:
                   out << eprBlockPoints(treeItems);
                break;
                case 10:
                   out << eprAffineTransformation();
                break;
                case 13:
                   out << eprSpatialRessection();
                break;
                case 17:
                   out << eprPhotogrammetricBlock();
                break;
                case 21:
                   out << eprInteriorOrientation();
                break;
                case 25:
                   out << eprExteriorOrientation();
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

outxsl << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
outxsl << "<xsl:stylesheet version=\"1.0\" xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">";
outxsl << "<xsl:template match=\"efotoPhotogrammetricReport\">";

if(idExt == 1)
{
    xsltransformation += "/epr_txt.xsl";
} else {
    xsltransformation += "/epr_html.xsl";
    outxsl << "<html>";
    outxsl << "<body>";
    outxsl << "<h2>E-Foto Photogrammetric Report</h2>";
    outxsl << "<xsl:if test=\"projectHeader\">";
    outxsl << "<h3>Project Header</h3>";
    outxsl << "Name: <xsl:value-of select=\"projectHeader/name\"/><br/>";
    outxsl << "Description: <xsl:value-of select=\"projectHeader/description\"/><br/>";
    outxsl << "Creation Date: <xsl:value-of select=\"projectHeader/creation\"/><br/>";
    outxsl << "Modification Date: <xsl:value-of select=\"projectHeader/modification\"/><br/>";
    outxsl << "Owner: <xsl:value-of select=\"projectHeader/owner\"/><br/>";
    outxsl << "</xsl:if>";
    outxsl << "<xsl:if test=\"terrain\">";
    outxsl << "<h3>Terrain</h3>";
    outxsl << "Mean Altitude: <xsl:value-of select=\"terrain/meanAltitude\"/><br/>";
    outxsl << "Minimum Altitude: <xsl:value-of select=\"terrain/minAltitude\"/><br/>";
    outxsl << "Maximum Altitude: <xsl:value-of select=\"terrain/maxAltitude\"/><br/>";
    outxsl << "GRS: <xsl:value-of select=\"terrain/GRS\"/><br/>";
    outxsl << "CPS: <xsl:value-of select=\"terrain/CPS\"/><br/>";
    outxsl << "Work Area Center Coordinates: ";
    outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/Lat/degrees\"/>º";
    outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/Lat/minutes\"/>'";
    outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/Lat/seconds\"/>''";
    outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/Lat/@direction\"/>";
    outxsl << "<xsl:text> | </xsl:text>";
    outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/Long/degrees\"/>º";
    outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/Long/minutes\"/>'";
    outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/Long/seconds\"/>''";
    outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/Long/@direction\"/>";
    outxsl << "<xsl:text> | </xsl:text>";
    outxsl << "<xsl:value-of select=\"terrain/workAreaCenterCoordinates/utmFuse\"/> UTM";
    outxsl << "</xsl:if>";
    outxsl << "<xsl:if test=\"sensors\">";
    outxsl << "<h3>Sensors</h3>";
    outxsl << "Sensor Id: <xsl:value-of select=\"sensors/sensorId\"/><br/>";
    outxsl << "Description: <xsl:value-of select=\"sensors/description\"/><br/>";
    outxsl << "Focal Distance: <xsl:value-of select=\"sensors/focalDistance\"/><br/>";
    outxsl << "</xsl:if>";
    outxsl << "</body>";
    outxsl << "</html>";
    outxsl << "</xsl:template>";
    outxsl << "</xsl:stylesheet>";
}
ofstream myXsltFile(xsltransformation.c_str());
// EDomElement xsl(outxsl.str());
// myXsltFile << xsl.removeBlankLines(true).indent('\t').getContent() ;
myXsltFile << outxsl.str();
myXsltFile.close();

return true;
}

/*string ReportManager::getImageIds(int )
{

}*/


} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
