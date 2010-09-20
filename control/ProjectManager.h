/**************************************************************************
                            ProjectManager.h
**************************************************************************/


#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "EObject.h"
#include "ETreeModel.h"
#include "PositionMatrix.h"
#include "ProjectUserInterface.h"

class ProjectUserInterface;
class EFotoManager;

/**
  * class ProjectManager
  *
  * @author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 25/06/2009
  * @version 1.0 - Rafael Alves de Aguiar & Irving da Silva Badolato
  *
  */

class ProjectManager
{

    EFotoManager* manager;
    ProjectUserInterface* myInterface;
    FILE* xmlFile;
    ETreeModel* treeModel;

public:

    // Constructors and Destructor
    //
    ProjectManager();
    ProjectManager(EFotoManager* manager);
    ~ProjectManager();

    // Other Methods
    //
    bool connectDatabase();
    bool disconnectDatabase();
    bool newProject();
    bool loadProject();
    bool saveProject();
    bool loadFile(string filename);
    bool saveFile(string filename);
    bool addComponent(string data, string parent);
    bool removeComponent(string type, int id);
    bool editComponent(string type, string data);
    bool editComponent(string type, int id, string data);
    EObject* viewComponent(string type, int id);
    ETreeModel* getTreeModel();
	deque<int> listImageKeys();
	deque<string> listImages();
	int getImageId(string imageName);
	int getFreeImageId();
	int getFreePointId();
    bool startModule(string module, int image);
    bool exec();
    bool reload();

    string getXml(string tagname);
    string getXml(string tagname, string att, string value);
    bool makeSPFile(string filename, int image1, int image2);

};

#endif //PROJECTMANAGER_H
