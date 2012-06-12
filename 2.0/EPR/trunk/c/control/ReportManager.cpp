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
    return EDomElement (project->sensor(1)->xmlGetData());
}


string ReportManager::eprHeader()
{
    string txt;
    txt = "\t<projectHeader>\n";
    txt += "\t\t<name>";            
    txt += ReportManager::getHeaderXml().elementByTagName("name").toString().c_str();
    txt += "</name>\n";
    txt += "\t\t<description>";
    txt += ReportManager::getHeaderXml().elementByTagName("description").toString().c_str();
    txt += "</description>\n";
    txt += "\t\t<creation>";
    txt += ReportManager::getHeaderXml().elementByTagName("creation").toString().c_str();
    txt += "</creation>\n";
    txt += "\t\t<modification>";
    txt += ReportManager::getHeaderXml().elementByTagName("modification").toString().c_str();
    txt += "</modification>\n";
    txt += "\t\t<owner>";
    txt += ReportManager::getHeaderXml().elementByTagName("owner").toString().c_str();
    txt += "</owner>\n";
    txt += "\t</projectHeader>\n";
    //qDebug(txt);
    return txt;
}

string ReportManager::eprTerrain()
{
    string txt;
    txt = "\t<terrain>\n";
    txt += "\t\t<meanAltitude>";
    txt += ReportManager::getTerrainXml().elementByTagAtt("meanAltitude","uom","#m").toString().c_str();
    txt += "</meanAltitude>\n";
    txt += "\t\t<minAltitude>";
    txt += ReportManager::getTerrainXml().elementByTagAtt("minAltitude","uom","#m").toString().c_str();
    txt += "</minAltitude>\n";
    txt += "\t\t<maxAltitude>";
    txt += ReportManager::getTerrainXml().elementByTagAtt("maxAltitude","uom","#m").toString().c_str();
    txt += "</maxAltitude>\n";
    txt += "\t\t<GRS>";
    txt += ReportManager::getTerrainXml().elementByTagName("GRS").toString().c_str();
    txt += "</GRS>\n";
    txt += "\t\t<CPS>";
    txt += ReportManager::getTerrainXml().elementByTagName("CPS").toString().c_str();
    txt += "</CPS>\n";
    txt += "\t\t<workAreaCenterCoordinates>\n";
    txt += "\t\t\t<Lat direction=\"S\">\n";
    txt += "\t\t\t\t<degrees>";
    txt += ReportManager::getTerrainXml().elementByTagName("Lat").elementByTagName("degrees").toString();
    txt += "</degrees>\n";
    txt += "\t\t\t\t<minutes>";
    txt += ReportManager::getTerrainXml().elementByTagName("Lat").elementByTagName("minutes").toString();
    txt += "</minutes>\n";
    txt += "\t\t\t\t<seconds>";
    txt += ReportManager::getTerrainXml().elementByTagName("Lat").elementByTagName("seconds").toString();
    txt += "</seconds>\n";
    txt += "\t\t\t</Lat>\n";
    txt += "\t\t\t<Long direction=\"W\">\n";
    txt += "\t\t\t\t<degrees>";
    txt += ReportManager::getTerrainXml().elementByTagName("Long").elementByTagName("degrees").toString();
    txt += "</degrees>\n";
    txt += "\t\t\t\t<minutes>";
    txt += ReportManager::getTerrainXml().elementByTagName("Long").elementByTagName("minutes").toString();
    txt += "</minutes>\n";
    txt += "\t\t\t\t<seconds>";
    txt += ReportManager::getTerrainXml().elementByTagName("Long").elementByTagName("seconds").toString();
    txt += "</seconds>\n";
    txt += "\t\t\t</Long>\n";
    txt += "\t\t\t<utmFuse>";
    txt += ReportManager::getTerrainXml().elementByTagName("utmFuse").toString().c_str();
    txt += "</utmFuse>\n";
    txt += "\t\t</workAreaCenterCoordinates>\n";
    txt += "\t</terrain>\n";
    return txt;
}

string ReportManager::eprSensors()
{
    string txt;

    txt = "\t<sensors>\n";
    txt += "\t\t<sensorId>";
    txt += ReportManager::getSensorXml().elementByTagName("sensorId").toString().c_str();
    txt += "</sensorId>\n";
    txt += "\t\t<description>";
    txt += ReportManager::getSensorXml().elementByTagName("description").toString().c_str();
    txt += "</description>\n";
    txt += "\t\t<focalDistance>";
    txt += ReportManager::getSensorXml().elementByTagName("focalDistance").elementByTagName("value").toString().c_str();
    txt += "</focalDistance>\n";
    txt += "\t</sensors>\n";
    return txt;
}

string ReportManager::eprFlights()
{
    string txt;
    txt = "\t<flights>Teste 04</flights>\n";
    return txt;
}

string ReportManager::eprImages()
{
    string txt;
    txt = "\t<images>Teste 05</images>\n";
    return txt;
}

string ReportManager::eprBlockPoints()
{
    string txt;
    txt = "\t<block_points>Teste 06</block_points>\n";
    return txt;
}

string ReportManager::eprAffineTransformation()
{
    string txt;
    txt = "\t<affine_transformation>Teste 07</affine_transformation>\n";
    return txt;
}

string ReportManager::eprSpatialRessection()
{
    string txt;
    txt = "\t<spatial_ressection>Teste 08</spatial_ressection>\n";
    return txt;
}

string ReportManager::eprPhotogrammetricBlock()
{
    string txt;
    txt = "\t<photogrammetric_block>Teste 09</photogrammetric_block>\n";
    return txt;
}

string ReportManager::eprInteriorOrientation()
{
    string txt;
    txt = "\t<interior_orientation>Teste 10</interior_orientation>\n";
    return txt;
}

string ReportManager::eprExteriorOrientation()
{
    string txt;
    txt = "\t<exterior_orientation>Teste 11</exterior_orientation>\n";
    return txt;
}

string ReportManager::eprStereoPairs()
{
    string txt;
    txt = "\t<stereo_pairs>Teste 12</stereo_pairs>\n";
    return txt;
}

string ReportManager::eprStereoPlotting()
{
    string txt;
    txt = "\t<stereo_plotting>Teste 13</stereo_plotting>\n";
    return txt;
}

string ReportManager::eprDSM()
{
    string txt;
    txt = "\t<dsm>Teste 14</dsm>\n";
    return txt;
}

string ReportManager::eprOrthorectification()
{
    string txt;
    txt = "\t<orthorectification>Teste 15</orthorectification>\n";
    return txt;
}

bool ReportManager::makeFile(string filename, int idExt, QList<QTreeWidgetItem*> treeItems)
{
    ofstream myFile (filename.c_str());
    if (myFile.is_open())
    {
        int numChild = treeItems.count();

        myFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

        if(idExt == 1){
            myFile << "<?xml-stylesheet type=\"text/xsl\" href=\"epr_txt.xsl\"?>\n";
        } else {
            if(idExt ==2){
                myFile << "<?xml-stylesheet type=\"text/xsl\" href=\"epr_html.xsl\"?>\n";
            }
        }

        myFile << "<efotoPhotogrammetricReport>\n";

        for (int i=0;i<numChild;i++)
        {
            QTreeWidgetItem *aux= treeItems.at(i);
            if(aux->checkState(0) == 2 || aux->checkState(0) == 1)
            {
                switch(i)
                {
                    case 0:
                        myFile << eprHeader();
                    break;
                    case 1:
                       myFile << eprTerrain();
                    break;
                    case 2:
                        myFile << eprSensors();
                    break;
                    case 3:
                       myFile << eprFlights();
                    break;
                    case 4:
                        myFile << eprImages();
                    break;
                    case 7:
                       myFile << eprBlockPoints();
                    break;
                    case 10:
                       myFile << eprAffineTransformation();
                    break;
                    case 13:
                       myFile << eprSpatialRessection();
                    break;
                    case 17:
                       myFile << eprPhotogrammetricBlock();
                    break;
                    case 21:
                       myFile << eprInteriorOrientation();
                    break;
                    case 25:
                       myFile << eprExteriorOrientation();
                    break;
                    case 31:
                       myFile << eprStereoPairs();
                    break;
                    case 32:
                       myFile << eprStereoPlotting();
                    break;
                    case 33:
                       myFile << eprDSM();
                    break;
                    case 34:
                       myFile << eprOrthorectification();
                    break;
                }
            }
        }

        myFile << "</efotoPhotogrammetricReport>";

        myFile.close();

        return true;
    }
    else
    {
        return false;
    }
}

/*string ReportManager::getImageIds(int )
{

}*/


} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
