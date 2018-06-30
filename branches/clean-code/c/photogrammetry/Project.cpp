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

#include "Project.h"

#include <QApplication>

#include "ProjectHeader.h"
#include "SensorWithFiducialMarks.h"
#include "SensorWithKnowDimensions.h"
#include "SensorWithKnowParameters.h"
#include "Terrain.h"
#include "Flight.h"
#include "Image.h"
#include "Point.h"
#include "InteriorOrientation.h"
#include "ExteriorOrientation.h"
#include "SpatialRessection.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

// Constructors and destructor
//

/**
 *
 */
Project::Project():
    xmlData_ {""},
    theHeader_ {nullptr},
        theTerrain_ {nullptr},
    processStates_ {""},
    thePhotoTri_ {nullptr}
{
}

/**
 *
 */
Project::~Project()
{

}


// Instance objects accessor methods
//

/**
 *
 */
ProjectHeader *Project::instanceHeader()
{
    if (theHeader_ != NULL)
        return theHeader_;
    EDomElement root(xmlData_);
    EDomElement xmlHeader = root.elementByTagName("projectHeader");
    if (xmlHeader.getContent().compare("") == 0)
        return NULL;
    theHeader_ = new ProjectHeader;
    theHeader_->xmlSetData(xmlHeader.getContent());
    return theHeader_;

}

Terrain* Project::instanceTerrain()
{
    if (theTerrain_ != NULL)
        return theTerrain_;
    EDomElement root(xmlData_);
    EDomElement xmlTerrain = root.elementByTagName("terrain");
    if (xmlTerrain.getContent().compare("") == 0)
        return NULL;
    theTerrain_ = new Terrain();
    theTerrain_->xmlSetData(xmlTerrain.getContent());
    return theTerrain_;
}

/**
 *
 */
Sensor* Project::instanceSensor(int id)
{
    for (unsigned int i = 0; i < sensors_.size(); i++)
        if (sensors_.at(i)->getId() == id)
            return sensors_.at(i);
    EDomElement root(xmlData_);
    EDomElement xmlSensor = root.elementByTagAtt("sensor", "key", Conversion::intToString(id));
    if (xmlSensor.getContent().compare("") == 0)
        return NULL;
    if (xmlSensor.elementByTagName("geometry").toString().compare("frame") == 0 &&
            xmlSensor.elementByTagName("calculationMode").toString().compare("With Fiducial Marks") == 0)
    {
        SensorWithFiducialMarks* newSensorWithFiducialMarks = new SensorWithFiducialMarks();
        newSensorWithFiducialMarks->xmlSetData(xmlSensor.getContent());
        sensors_.push_back(newSensorWithFiducialMarks);
        return dynamic_cast<Sensor*>(newSensorWithFiducialMarks);
    }
    if (xmlSensor.elementByTagName("geometry").toString().compare("frame") == 0 &&
            xmlSensor.elementByTagName("calculationMode").toString().compare("With Sensor Dimensions") == 0)
    {
        SensorWithKnowDimensions* newSensorWithKnowDimensions = new SensorWithKnowDimensions();
        newSensorWithKnowDimensions->xmlSetData(xmlSensor.getContent());
        sensors_.push_back(newSensorWithKnowDimensions);
        return dynamic_cast<Sensor*>(newSensorWithKnowDimensions);
    }
    if (xmlSensor.elementByTagName("geometry").toString().compare("frame") == 0 &&
            xmlSensor.elementByTagName("calculationMode").toString().compare("Fixed Parameters") == 0)
    {
        SensorWithKnowParameters* newSensorWithKnowParameters = new SensorWithKnowParameters();
        newSensorWithKnowParameters->xmlSetData(xmlSensor.getContent());
        sensors_.push_back(newSensorWithKnowParameters);
        return dynamic_cast<Sensor*>(newSensorWithKnowParameters);
    }
    return nullptr;
}

/**
 *
 */
Flight* Project::instanceFlight(int id)
{
    for (unsigned int i = 0; i < flights_.size(); i++)
        if (flights_.at(i)->getId() == id)
            return flights_.at(i);
    EDomElement root(xmlData_);
    EDomElement xmlFlight = root.elementByTagAtt("flight", "key", Conversion::intToString(id));
    if (xmlFlight.getContent().compare("") == 0)
        return NULL;
    Flight* newFlight = new Flight();
    newFlight->xmlSetData(xmlFlight.getContent());
    flights_.push_back(newFlight);
    return newFlight;
}

/**
 *
 */
Image* Project::instanceImage(int id)
{
    for (unsigned int i = 0; i < images_.size(); i++)
        if (images_.at(i)->getId() == id)
            return images_.at(i);
    EDomElement root(xmlData_);
    EDomElement xmlImage = root.elementByTagAtt("image", "key", Conversion::intToString(id));
    if (xmlImage.getContent().compare("") == 0)
        return NULL;
    Image* newImage = new Image();
    newImage->xmlSetData(xmlImage.getContent());
    images_.push_back(newImage);
    return newImage;
}

void Project::instanceAllImages()
{
    EDomElement root(xmlData_);
    std::deque<EDomElement> xmlAllImages = root.elementsByTagName("image");
    for (unsigned int i = 0 ;i < xmlAllImages.size();i++)
    {
        qApp->processEvents();
        if (xmlAllImages.at(i).getContent().compare("") == 0)
            continue;
        bool notAvailable = true;
        for (unsigned int j = 0; j < images_.size() && notAvailable; j++)
            if (images_.at(j)->getId() == Conversion::stringToInt(xmlAllImages.at(i).attribute("key")))
                notAvailable = false;
        if (notAvailable)
        {
            Image* newImage = new Image();
            newImage->xmlSetData(xmlAllImages.at(i).getContent());
            images_.push_back(newImage);
        }
    }
}

/**
 *
 */
void Project::instanceAllPoints()
{
    EDomElement root(xmlData_);
    std::deque<EDomElement> xmlAllPoint = root.elementsByTagName("point");

    for (unsigned int i = 0; i < xmlAllPoint.size(); i++)
    {
        qApp->processEvents();
        if (xmlAllPoint.at(i).getContent().compare("") == 0)
            continue;

        // Evita reinstaciar pontos que ja estejam disponiveis
        bool notAvailable = true;
        for (unsigned int j = 0; j < points_.size() && notAvailable; j++)
            if (points_.at(j)->getId() == Conversion::stringToInt(xmlAllPoint.at(i).attribute("key")))
                notAvailable = false;
        if (notAvailable)
        {
            Point* newPoint = new Point();
            newPoint->xmlSetData(xmlAllPoint.at(i).getContent());
            points_.push_back(newPoint);
        }
    }
}


Point* Project::instancePoint(int id)
{
    for (unsigned int i = 0; i < points_.size(); i++)
        if (points_.at(i)->getId() == id)
            return points_.at(i);
    EDomElement root(xmlData_);
    EDomElement xmlPoint = root.elementByTagAtt("point", "key", Conversion::intToString(id));
    if (xmlPoint.getContent().compare("") == 0)
        return NULL;

    Point* newPoint = new Point();
    newPoint->xmlSetData(xmlPoint.getContent());
    points_.push_back(newPoint);
    return dynamic_cast<Point*>(newPoint);
}

/**
 *
 */
InteriorOrientation* Project::instanceIO(int imageId)
{
    for (unsigned int i = 0; i < IOs_.size(); i++)
        if (IOs_.at(i)->getImageId() == imageId)
            return IOs_.at(i);
    EDomElement root(xmlData_);
    EDomElement xmlIO = root.elementByTagAtt("imageIO", "image_key", Conversion::intToString(imageId));
    if (xmlIO.getContent().compare("") == 0)
        return NULL;
    InteriorOrientation* newIO = new InteriorOrientation();
    newIO->setImage(image(imageId));
    newIO->xmlSetData(xmlIO.getContent());
    newIO->setImage(NULL);
    IOs_.push_back(newIO);
    return newIO;
}

void Project::instanceAllIOs()
{
    EDomElement root(xmlData_);
    std::deque<EDomElement> xmlAllIOs = root.elementsByTagName("imageIO");
    for (unsigned int i = 0 ;i < xmlAllIOs.size();i++)
    {
        qApp->processEvents();
        if (xmlAllIOs.at(i).getContent().compare("") == 0)
            continue;
        bool notAvailable = true;
        for (unsigned int j = 0; j < IOs_.size() && notAvailable; j++)
            if (IOs_.at(j)->getImageId() == Conversion::stringToInt(xmlAllIOs.at(i).attribute("image_key")))
                notAvailable = false;
        //InteriorOrientation *pkj;
        //pkj->getImageId()
        if (notAvailable)
        {
            InteriorOrientation* newIO = new InteriorOrientation();
            newIO->setImage(image(Conversion::stringToInt(xmlAllIOs.at(i).attribute("image_key"))));
            newIO->xmlSetData(xmlAllIOs.at(i).getContent());
            newIO->setImage(NULL);
            IOs_.push_back(newIO);
        }
    }
}

/**
 *
 */
ExteriorOrientation* Project::instanceEO(int imageId)
{
    for (unsigned int i = 0; i < EOs_.size(); i++)
        if (EOs_.at(i)->getImageId() == imageId)
            return EOs_.at(i);
    EDomElement root(xmlData_);
    EDomElement xmlEO = root.elementByTagAtt("imageEO", "image_key", Conversion::intToString(imageId));
    EDomElement xmlSR = root.elementByTagAtt("imageSR", "image_key", Conversion::intToString(imageId));
    if (xmlEO.getContent().compare("") == 0 || xmlSR.getContent().compare("") == 0)
        return NULL;
    //if (xmlEO.attribute("type").compare("spatialRessection") == 0)
    {
        SpatialRessection* newEO = new SpatialRessection();
        newEO->xmlSetData(xmlEO.getContent().append(xmlSR.getContent()));
        EOs_.push_back(newEO);
        return dynamic_cast<ExteriorOrientation*>(newEO);
    }
    return NULL;
}

void Project::instanceAllEOs()
{
    EDomElement root(xmlData_);
    std::deque<EDomElement> xmlAllEOs = root.elementsByTagName("imageEO");
    std::deque<EDomElement> xmlAllSRs = root.elementsByTagName("imageSR");
    for (unsigned int i = 0 ;i < xmlAllEOs.size();i++)
    {
        qApp->processEvents();
        if (xmlAllEOs.at(i).getContent().compare("") == 0)
            continue;
        bool notAvailable = true;
        for (unsigned int j = 0; j < EOs_.size() && notAvailable; j++)
            if (EOs_.at(j)->getImageId() == Conversion::stringToInt(xmlAllEOs.at(i).attribute("image_key")))
                notAvailable = false;
        //InteriorOrientation *pkj;
        //pkj->getImageId()
        if (notAvailable)
        {
            SpatialRessection* newEO = new SpatialRessection();
            newEO->setImage(image(Conversion::stringToInt(xmlAllEOs.at(i).attribute("image_key"))));

            bool hasSR = false;
            for (unsigned int k = 0; k < xmlAllSRs.size() && !hasSR; k++)
                if (xmlAllEOs.at(i).attribute("image_key") == xmlAllSRs.at(k).attribute("image_key"))
                {
                    hasSR = true;
                    newEO->xmlSetData(xmlAllEOs.at(i).getContent()+xmlAllSRs.at(k).getContent());
                    xmlAllSRs.erase(xmlAllSRs.begin()+k);
                }
            if (!hasSR)
                newEO->xmlSetData(xmlAllEOs.at(i).getContent());
            newEO->setImage(NULL);
            EOs_.push_back(newEO);
        }
    }
}

PhotoTri *Project::instancePhotoTri()
{
    if (thePhotoTri_ != NULL)
        return thePhotoTri_;
    EDomElement root(xmlData_);
    EDomElement xmlPhoto = root.elementByTagName("phototriangulation");
    if (xmlPhoto.getContent().compare("") == 0)
        return NULL;
    thePhotoTri_ = new PhotoTri;
    thePhotoTri_->xmlSetData(xmlPhoto.getContent());
    return thePhotoTri_;
}

/**
 *
 */
void Project::deleteTerrain(bool makeReconnections)
{
    if (theTerrain_ != NULL)
    {
        delete(theTerrain_);
        theTerrain_ = NULL;
    }
    if (makeReconnections)
        linkAll();
}

/**
 *
 */
void Project::deleteSensor(int id, bool makeReconnections)
{
    unsigned int i;
    Sensor* mySensor = NULL;
    for (i = 0; i < sensors_.size(); i++)
        if (sensors_.at(i)->getId() == id)
        {
            mySensor = sensors_.at(i);
            break;
        }
    if (mySensor != NULL)
    {
        EDomElement xmlSensor(mySensor->xmlGetData());
        if (xmlSensor.elementByTagName("geometry").toString().compare("frame") == 0 &&
                xmlSensor.elementByTagName("platform").toString().compare("aerial") == 0 &&
                xmlSensor.elementByTagName("detector").toString().compare("film") == 0)
        {
            SensorWithFiducialMarks* mySensorWithFiducialMarks =
                    dynamic_cast<SensorWithFiducialMarks*>(mySensor);
            delete(mySensorWithFiducialMarks);
        }
        sensors_.erase(sensors_.begin() + i);
    }
    if (makeReconnections)
        linkAll();
}

/**
 *
 */
void Project::deleteFlight(int id, bool makeReconnections)
{
    unsigned int i;
    Flight* myFlight = NULL;
    for (i = 0; i < flights_.size(); i++)
        if (flights_.at(i)->getId() == id)
        {
            myFlight = flights_.at(i);
            break;
        }
    if (myFlight != NULL)
    {
        delete(myFlight);
        flights_.erase(flights_.begin() + i);
    }
    if (makeReconnections)
        linkAll();
}

/**
 *
 */
void Project::deleteImage(int id, bool makeReconnections)
{
    unsigned int i;
    Image* myImage = NULL;
    for (i = 0; i < images_.size(); i++)
        if (images_.at(i)->getId() == id)
        {
            myImage = images_.at(i);
            break;
        }
    if (myImage != NULL)
    {
        delete(myImage);
        images_.erase(images_.begin() + i);
    }
    if (makeReconnections)
        linkAll();
}

/**
 *
 */
void Project::deletePoint(int id, bool makeReconnections)
{
    unsigned int i;
    Point* myPoint = NULL;
    for (i = 0; i < points_.size(); i++)
        if (points_.at(i)->getId() == id)
        {
            myPoint = points_.at(i);
            break;
        }
    if (myPoint != NULL)
    {
        Point* point = myPoint;
            delete(point);
        points_.erase(points_.begin() + i);
    }
    if (makeReconnections)
        linkAll();
}

/**
 *
 */
void Project::deleteIO(int id, bool makeReconnections)
{
    unsigned int i;
    InteriorOrientation* myIO = NULL;
    for (i = 0; i < IOs_.size(); i++)
        if (IOs_.at(i)->getImageId() == id)
        {
            myIO = IOs_.at(i);
            break;
        }
    if (myIO != NULL)
    {
        delete(myIO);
        IOs_.erase(IOs_.begin() + i);
    }
    if (makeReconnections)
        linkAll();
}

/**
 *
 */
void Project::deleteEO(int id, bool makeReconnections)
{
    unsigned int i;
    ExteriorOrientation* myEO = NULL;
    for (i = 0; i < EOs_.size(); i++)
        if (EOs_.at(i)->getImageId() == id)
        {
            myEO = EOs_.at(i);
            break;
        }
    if (myEO != NULL)
    {
        //EDomElement xmlEO(myEO->xmlGetData());
        //if (xmlEO.attribute("type").compare("spatialRessection") == 0)
        if (myEO->is("SpatialResection"))
        {
            SpatialRessection* mySR = dynamic_cast<SpatialRessection*>(myEO);
            delete(mySR);
        }
        EOs_.erase(EOs_.begin() + i);
    }
    if (makeReconnections)
        linkAll();
}

void Project::addPoint(std::string data, bool makeReconnections)
{
    if (data != "")
    {
        EDomElement xmlPoint(data);
        int id = Conversion::stringToInt(xmlPoint.elementByTagName("point").attribute("key"));
        for (int i = points_.size()-1; i >= 0; i--)
            if (points_.at(i)->getId() == id)
            {
                delete points_.at(i);
                points_.erase(points_.begin()+i);
            }
        Point* point = new Point();
        point->xmlSetData(xmlPoint.getContent());
        points_.push_back(point);
    }
    else
    {
        points_.push_back(new Point(getFreePointId()));
    }

    if (makeReconnections)
        linkAll();
}

ProjectHeader *Project::header()
{
    if (theHeader_ != NULL)
        return theHeader_;
    return NULL;
}

/**
 *
 */
Terrain* Project::terrain()
{
    if (theTerrain_ != NULL)
        return theTerrain_;
    return NULL;
}

/**
 *
 */
Sensor* Project::sensor(int id)
{
    for (unsigned int i = 0; i < sensors_.size(); i++)
        if (sensors_.at(i)->getId() == id)
            return sensors_.at(i);
    return NULL;
}

/**
 *
 */
Flight* Project::flight(int id)
{
    for (unsigned int i = 0; i < flights_.size(); i++)
        if (flights_.at(i)->getId() == id)
            return flights_.at(i);
    return NULL;
}

/**
 *
 */
Image* Project::image(int id)
{
    for (unsigned int i = 0; i < images_.size(); i++)
        if (images_.at(i)->getId() == id)
            return images_.at(i);
    return NULL;
}

/**
 *
 */
Point* Project::point(int id)
{
    for (unsigned int i = 0; i < points_.size(); i++)
        if (points_.at(i)->getId() == id)
            return points_.at(i);
    return NULL;
}

PhotoTri *Project::photoTri()
{
    if (thePhotoTri_ != NULL)
        return thePhotoTri_;
    return NULL;
}

/**
 *
 */
std::string Project::getXml(std::string tagname)
{
    EDomElement root(getXml());
    return root.elementByTagName(tagname).getContent();
}

/**
 *
 */
std::string Project::getXml(std::string tagname, std::string att, std::string value)
{
    EDomElement root(getXml());
    return root.elementByTagAtt(tagname, att, value).getContent();
}


// EObject methods
/**
 *
 */
bool Project::is(std::string s)
{
    return (s == "Project" ? true : false);
}


// XML methods
//
void Project::setXml(std::string xml)
{
    xmlData_ = xml;

    EDomElement ede(xmlData_);
    //EDomElement dem = ede.elementByTagName("DEMs");
    //EDomElement eoi = ede.elementByTagName("orthoImages");
    //EDomElement feat = ede.elementByTagName("features");
    EDomElement sr = ede.elementByTagName("spatialResections");

    // Rever a parte de estados de processos... armazenados e acessados na string processStates
    processStates_ = /*dem.getContent() + eoi.getContent() + feat.getContent() +*/ sr.getContent();
    xmlData_ = ede.indent('\t').getContent();

    instanceAllImages();
    instanceAllPoints();
    instanceAllIOs();
    instanceAllEOs();

    instanceHeader();
    instanceTerrain();
    instanceSensor(1);
    instanceFlight(1);
    instancePhotoTri();

    //Rever a criação dos instance DEMs, EOIs e FEATs.

    linkAll();
}

void Project::linkAll()
{
    Sensor *sns = sensor(1);
    Flight *flt = flight(1);

    if (flt)
    {
        flt->setTerrain(theTerrain_);
        flt->setSensor(sns);
        flt->clearImages();
    }
    if (sns)
    {
        sns->clearImages();
        sns->clearFlights();
        sns->putFlight(flt);
    }

    for (unsigned int i = 0; i < points_.size(); i++)
    {
        Point* point = points_.at(i);
        point->clearImages();
    }

    for (unsigned int j = 0; j < images_.size(); j++)
    {
        Image *img = images_.at(j);
        img->clearPoints();

        for (unsigned int i = 0; i < points_.size(); i++)
        {
            Point* pointToInsert = points_.at(i);

            if (pointToInsert->hasImageCoordinate(img->getId()))
            {
                img->putPoint(pointToInsert);
                pointToInsert->putImage(img);
            }
        }

        img->setSensor(sns);
        img->setFlight(flt);
        if (sns) sns->putImage(img);
        if (flt) flt->putImage(img);

    }

    for (unsigned int i=0;i<IOs_.size();i++)
    {
        InteriorOrientation *io=IOs_.at(i);
        Image *img = image(io->getImageId());
        io->setImage(img);
        if (img) img->setIO(io);
    }

    for (unsigned int i=0;i<EOs_.size();i++)
    {
        ExteriorOrientation *eo=EOs_.at(i);
        Image *img = image(eo->getImageId());
        eo->setImage(img);
        if (img) img->setEO(dynamic_cast<SpatialRessection*>(eo));
    }

    if (thePhotoTri_)
    {
        thePhotoTri_->clearImages();
        for (unsigned int i=0; i<thePhotoTri_->getImageKeys().size(); i++)
        {
            Image *img = image(thePhotoTri_->getImageKeys().at(i));
            if (img) thePhotoTri_->putImage(img);
        }

        thePhotoTri_->clearPoints();
        for (unsigned int i=0; i<thePhotoTri_->getPointKeys().size(); i++)
        {
            Point *pt = point(thePhotoTri_->getPointKeys().at(i));
            if (pt) thePhotoTri_->putPoint(pt);
        }
    }
}

std::string Project::getXml()
{
    if (xmlData_.empty())
        return "";

    std::string xmlData = "";
    xmlData += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    xmlData += "<?xml-stylesheet type=\"text/xsl\" href=\"xsl/epp.xsl\"?>\n\n";
    xmlData += "<efotoPhotogrammetricProject version=\"1.0.42\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n";
    xmlData += "xsi:noNamespaceSchemaLocation=\"EPPSchema/epp_structure.xsd\"\n";
    xmlData += "xmlns:gml=\"http://www.opengis.net/gml\"\n";
    xmlData += "xmlns:mml=\"http://www.w3.org/1998/Math/MathML\">\n";
    xmlData += header() ? header()->xmlGetData() : "";
    xmlData += terrain() ? terrain()->xmlGetData() : "";
    xmlData += "<sensors>\n";
    xmlData += sensor(1) ? sensor(1)->xmlGetData() : "";
    xmlData += "</sensors>\n";
    xmlData += "<flights>\n";
    xmlData += flight(1) ? flight(1)->xmlGetData() : "";
    xmlData += "</flights>\n";
    xmlData += "<points>\n";
    for(unsigned int i=0; i<points_.size(); i++)
    {
        xmlData += points_.at(i)->xmlGetData();
    }
    xmlData += "</points>\n";
    xmlData += "<images>\n";
    for(unsigned int i=0; i<images_.size(); i++)
    {
        xmlData += images_.at(i)->xmlGetData();
    }
    xmlData += "</images>\n";
    xmlData += "<interiorOrientation>\n";
    for(unsigned int i=0; i<IOs_.size(); i++)
    {
        xmlData += IOs_.at(i)->xmlGetData();
    }
    xmlData += "</interiorOrientation>\n";
    xmlData += "<exteriorOrientation>\n";
    for(unsigned int i=0; i<EOs_.size(); i++)
    {
        SpatialRessection* sp = dynamic_cast<SpatialRessection*>(EOs_.at(i));
        xmlData += sp->xmlGetDataEO();
    }
    xmlData += "</exteriorOrientation>\n";

    // Rever aqui para adicionar os DEMs, EOIs e FEATs ao xml de saida.

    xmlData += "<spatialResections>\n";
    for(unsigned int i=0; i<EOs_.size(); i++)
    {
        SpatialRessection* sp = dynamic_cast<SpatialRessection*>(EOs_.at(i));
        xmlData += sp->xmlGetDataEO();
    }
    xmlData += "</spatialResections>\n";

    if (photoTri())
        xmlData += photoTri()->xmlGetData();

    xmlData += "</efotoPhotogrammetricProject>";

    EDomElement xml(xmlData);
    xmlData = xml.indent('\t').getContent();

    return xmlData;
}

int Project::getFreeImageId()
{
    int result = 1;
    for (int i = images_.size()-1; i >= 0; i--)
    {
        if (images_.at(i)->getId() >= result)
            result = images_.at(i)->getId()+1;
    }
    return result;
}

int Project::getFreePointId()
{
    int result = 1;
    for (int i = points_.size()-1; i >= 0; i--)
    {
        if (points_.at(i)->getId() >= result)
            result = points_.at(i)->getId()+1;
    }
    return result;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

