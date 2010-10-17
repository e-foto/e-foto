/**************************************************************************
                           ProjectManager.cpp
**************************************************************************/

#include "ProjectManager.h"
#include "EFotoManager.h"
#include "ProjectUserInterface_Qt.h"

// Constructors and Destructor
//

ProjectManager::ProjectManager()
{
    this->manager = NULL;
    this->xmlFile = NULL;
    this->treeModel = NULL;
}

ProjectManager::ProjectManager(EFotoManager* manager)
{
    this->manager = manager;
    this->xmlFile = NULL;
    this->treeModel = NULL;
}

ProjectManager::~ProjectManager()
{
    if (treeModel != NULL)
        delete treeModel;
}

// Other Methods
//

bool ProjectManager::connectDatabase()
{
    if (manager != NULL)
    {

    }
    return false;
}

bool ProjectManager::disconnectDatabase()
{
    if (manager != NULL)
    {

    }
    return false;
}

bool ProjectManager::newProject()
{
    if (manager != NULL)
    {
        string xmlData = "";
        xmlData += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        xmlData += "<?xml-stylesheet type=\"text/xsl\" href=\"xsl/epp.xsl\"?>\n\n";
        xmlData += "<efotoPhotogrammetricProject version=\"1.0.20\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n";
        xmlData += "xsi:noNamespaceSchemaLocation=\"EPPSchema/epp_structure.xsd\"\n";
        xmlData += "xmlns:gml=\"http://www.opengis.net/gml\"\n";
		xmlData += "xmlns:mml=\"http://www.w3.org/1998/Math/MathML\">\n";
		xmlData += "\n";
		xmlData += "\t<projectHeader>\n";
		xmlData += "\t\t<name></name>\n";
		xmlData += "\t\t<description></description>\n";
		xmlData += "\t\t<filePath></filePath>\n";
		xmlData += "\t\t<fileName></fileName>\n";
		xmlData += "\t\t<creation></creation>\n";
		xmlData += "\t\t<modification></modification>\n";
		xmlData += "\t\t<owner></owner>\n";
		xmlData += "\t\t<aims></aims>\n";
		xmlData += "\t\t<context></context>\n";
		xmlData += "\t</projectHeader>\n";
		xmlData += "\n";
		xmlData += "\t<terrain>\n";
		xmlData += "\t\t<meanAltitude uom=\"#m\"></meanAltitude>\n";
		xmlData += "\t\t<minAltitude uom=\"#m\"></minAltitude>\n";
		xmlData += "\t\t<maxAltitude uom=\"#m\"></maxAltitude>\n";
		xmlData += "\t\t<GRS>WGS84</GRS>\n";
		xmlData += "\t\t<CPS>UTM</CPS>\n";
		xmlData += "\t\t<workAreaCenterCoordinates>\n";
		xmlData += "\t\t\t<Lat direction=\"S\">\n";
		xmlData += "\t\t\t\t<degrees></degrees>\n";
		xmlData += "\t\t\t\t<minutes></minutes>\n";
		xmlData += "\t\t\t\t<seconds></seconds>\n";
		xmlData += "\t\t\t</Lat>\n";
		xmlData += "\t\t\t<Long direction=\"W\">\n";
		xmlData += "\t\t\t\t<degrees></degrees>\n";
		xmlData += "\t\t\t\t<minutes></minutes>\n";
		xmlData += "\t\t\t\t<seconds></seconds>\n";
		xmlData += "\t\t\t</Long>\n";
		xmlData += "\t\t\t<utmFuse></utmFuse>\n";
		xmlData += "\t\t</workAreaCenterCoordinates>\n";
		xmlData += "\t</terrain>\n";
		xmlData += "\n";
		xmlData += "\t<sensors>\n";
		xmlData += "\t</sensors>\n";
		xmlData += "\n";
		xmlData += "\t<flights>\n";
		xmlData += "\t</flights>\n";
		xmlData += "\n";
		xmlData += "\t<points>\n";
		xmlData += "\t</points>\n";
		xmlData += "\n";
		xmlData += "\t<images>\n";
		xmlData += "\t</images>\n";
		xmlData += "\n";
		xmlData += "\t<interiorOrientation>\n";
		xmlData += "\t</interiorOrientation>\n";
		xmlData += "\n";
		xmlData += "\t<exteriorOrientation>\n";
		xmlData += "\t</exteriorOrientation>\n";
		xmlData += "\n";
        xmlData += "</efotoPhotogrammetricProject>";

        manager->xmlSetData(xmlData);

        if (treeModel != NULL)
            delete treeModel;

        treeModel = new ETreeModel(EDomElement(xmlData).elementByTagName("efotoPhotogrammetricProject").getContent());

        return true;
    }
    return false;
}

bool ProjectManager::loadProject()
{
    if (manager != NULL)
    {

    }
    return false;
}

bool ProjectManager::saveProject()
{
    if (manager != NULL)
    {

    }
    return false;
}

bool ProjectManager::loadFile(string filename)
{
    if (manager != NULL)
    {
        stringstream myData;
        ifstream myFile(filename.c_str());
        if (myFile.is_open())
        {
            string line;
            while (!myFile.eof())
            {
                getline (myFile,line);
                myData << line << endl;
            }
            myFile.close();

            string xmlData = myData.str();
			// Aqui deve entrar um codigo para validar o XML.
            manager->xmlSetData(xmlData);

            if (treeModel != NULL)
                delete treeModel;

            treeModel = new ETreeModel(EDomElement(xmlData).elementByTagName("efotoPhotogrammetricProject").getContent());
            return true;
        }
        else cout << "Unable to open file";
        return false;
    }

    return false;
}

bool ProjectManager::saveFile(string filename)
{
    if (manager != NULL)
    {
        ofstream myFile (filename.c_str());
        if (myFile.is_open())
		{
            myFile << manager->xmlGetData();
            myFile.close();
			return true;
        }
        else cout << "Unable to open file";
        return false;
    }
    return false;
}

bool ProjectManager::testFileVersion()
{
	if (manager != NULL)
	{
		EDomElement ede(manager->xmlGetData());
		if ("1.0.20" == ede.elementByTagName("efotoPhotogrammetricProject").attribute("version"))
			return true;
	}
	return false;
}

bool ProjectManager::addComponent(string data, string parent)
{
    if (manager != NULL)
    {
        EDomElement newXml(manager->xmlGetData());
        newXml.addChildAtTagName(parent, data);
        manager->xmlSetData(newXml.getContent());

        if (treeModel != NULL)
            delete treeModel;
        treeModel = new ETreeModel(newXml.elementByTagName("efotoPhotogrammetricProject").getContent());
        return true;
    }
    return false;
}

bool ProjectManager::removeComponent(string type, int id)
{
    if (manager != NULL)
    {
        EDomElement newXml(manager->xmlGetData());
        if (type == "Sensor")
            newXml.replaceChildByTagAtt("sensor", "key", intToString(id), "");
        else if (type == "Flight")
            newXml.replaceChildByTagAtt("flight", "key", intToString(id), "");
        else if (type == "Image")
            newXml.replaceChildByTagAtt("image", "key", intToString(id), "");
        else if (type == "Point")
            newXml.replaceChildByTagAtt("point", "key", intToString(id), "");
        else if (type == "IO")
            newXml.replaceChildByTagAtt("imageIO", "image_key", intToString(id), "");
        else if (type == "EO")
            newXml.replaceChildByTagAtt("imageEO", "image_key", intToString(id), "");
        manager->xmlSetData(newXml.getContent());

        if (treeModel != NULL)
            delete treeModel;
        treeModel = new ETreeModel(newXml.elementByTagName("efotoPhotogrammetricProject").getContent());
        return true;
    }
    return false;
}

bool ProjectManager::editComponent(string type, string data)
{
    if (manager != NULL)
    {
        EDomElement newXml(manager->xmlGetData());
        if (type == "Header")
			newXml.replaceChildByTagName("projectHeader", data);
        else if (type == "Terrain")
            newXml.replaceChildByTagName("terrain", data);
        manager->xmlSetData(newXml.getContent());

        if (treeModel != NULL)
            delete treeModel;
        treeModel = new ETreeModel(newXml.elementByTagName("efotoPhotogrammetricProject").getContent());
        return true;
    }
    return false;
}

bool ProjectManager::editComponent(string type, int id, string data)
{
    if (manager != NULL)
    {
        EDomElement newXml(manager->xmlGetData());
        if (type == "Sensor")
            newXml.replaceChildByTagAtt("sensor", "key", intToString(id), data);
        else if (type == "Flight")
            newXml.replaceChildByTagAtt("flight", "key", intToString(id), data);
        else if (type == "Image")
            newXml.replaceChildByTagAtt("image", "key", intToString(id), data);
        else if (type == "Point")
            newXml.replaceChildByTagAtt("point", "key", intToString(id), data);
        else if (type == "IO")
            newXml.replaceChildByTagAtt("imageIO", "image_key", intToString(id), data);
        else if (type == "EO")
            newXml.replaceChildByTagAtt("imageEO", "image_key", intToString(id), data);
        manager->xmlSetData(newXml.getContent());

        if (treeModel != NULL)
            delete treeModel;
        treeModel = new ETreeModel(newXml.elementByTagName("efotoPhotogrammetricProject").getContent());
        return true;
    }
    return false;
}

EObject* ProjectManager::viewComponent(string type, int id)
{
    if (manager != NULL)
    {
        if (type == "Sensor")
            return (EObject*) manager->instanceSensor(id);
        else if (type == "Flight")
            return (EObject*) manager->instanceFlight(id);
        else if (type == "Image")
            return (EObject*) manager->instanceImage(id);
        else if (type == "Point")
			return (EObject*) manager->instancePoint(id);
    }
    return NULL;
}

ETreeModel* ProjectManager::getTreeModel()
{
    return treeModel;
}

deque<int> ProjectManager::listImageKeys()
{
	deque<int> result;
	for (unsigned int i = 0; i < treeModel->getChild(4).countChildren(); i++)
		result.push_back(treeModel->idAt(4, i));
	return result;
}

deque<string> ProjectManager::listImages()
{
    deque<string> result;
    for (unsigned int i = 0; i < treeModel->getChild(4).countChildren(); i++)
        result.push_back(treeModel->dataAt(4, i));
    return result;
}

int ProjectManager::getImageId(string imageName)
{
    for (unsigned int i = 0; i < treeModel->getChild(4).countChildren(); i++)
        if (treeModel->dataAt(4, i) == imageName)
            return treeModel->idAt(4, i);
    return -1;
}

int ProjectManager::getFreeImageId()
{
	int result = 0;
	if (manager != NULL)
	{
		deque<EDomElement> images = EDomElement(manager->getXml("images")).children();
		result = 1;
		for (int i = images.size()-1; i >= 0; i--)
		{
			if (stringToInt(images.at(i).attribute("key")) >= result)
				result = stringToInt(images.at(i).attribute("key"))+1;
		}
		return result;
	}
	return result;
}

int ProjectManager::getFreePointId()
{
	int result = 0;
	if (manager != NULL)
	{
		deque<EDomElement> points = EDomElement(manager->getXml("points")).children();
		result = 1;
		for (int i = points.size()-1; i >= 0; i--)
		{
			if (stringToInt(points.at(i).attribute("key")) >= result)
				result = stringToInt(points.at(i).attribute("key"))+1;
		}
		return result;
	}
	return result;
}

bool ProjectManager::exec()
{
    if (manager != NULL)
    {
        if (manager->getInterfaceType().compare("Qt") == 0)
        {
            myInterface = new ProjectUserInterface_Qt(this);
        }
        if (myInterface != NULL)
        {
            return myInterface->exec();
        }
        return false;
    }
    return false;
}

bool ProjectManager::reload()
{
    if (manager != NULL)
    {
        if (myInterface != NULL)
        {
            return myInterface->exec();
        }
        return false;
    }
    return false;
}

string ProjectManager::getXml(string tagname)
{
    return manager->getXml(tagname);
}

string ProjectManager::getXml(string tagname, string att, string value)
{
    return manager->getXml(tagname, att, value);
}
