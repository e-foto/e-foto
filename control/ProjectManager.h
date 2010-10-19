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
  * This class controls the Project Manager UI.
  * @author The e-foto team
  * @date 14/10/2010
  * @version 1.0.21
  *
  */

class ProjectManager
{

    EFotoManager* manager;
    ProjectUserInterface* myInterface;
    FILE* xmlFile;
    ETreeModel* treeModel;

public:

    /**
      * Default constructor.
      */
    ProjectManager();

    /**
      * Overloaded constructor for convenience.
      */
    ProjectManager(EFotoManager* manager);

    /**
      * Default destructor.
      */
    ~ProjectManager();

    /**
      * This method connects the application to the current database.
      */
    bool connectDatabase();

    /**
      * This method disconnects the application from the current database.
      */
    bool disconnectDatabase();

    /**
      * This method starts a new project.
      */
    bool newProject();

    /**
      * This method loads an existing project from the current database.
      */
    bool loadProject();

    /**
      * This method saves the current project to the current database.
      */
    bool saveProject();

    /**
      * This method loads a project from a *.epp file.
      * @param filename Name of the file to be loaded.
      */
    bool loadFile(string filename);

    /**
      * This method saves a project to a *.epp file.
      * @param filename Name of the file to be saved.
      */
    bool saveFile(string filename);

    /**
      * This method tests the file version.
      */
    bool testFileVersion();

    /**
      * This method adds a new component to the XML project.
      * @param data Data of the component.
      * @param parent Parent node of the component in the XML.
      */
    bool addComponent(string data, string parent);

    /**
      * This method removes a component from the XML project.
      * @param type Type of the component.
      * @param id Identifier of the component in the XML.
      */
    bool removeComponent(string type, int id);

    /**
      * This is an overloaded method, added for convenience.
      * @param type Type of the component.
      * @param data Data of the component.
      */
    bool editComponent(string type, string data);

    /**
      * This method edits a component from the XML project.
      * @param type Type of the component.
      * @param id Identifier of the component in the XML.
      * @param data Data of the component.
      */
    bool editComponent(string type, int id, string data);

    /**
      * This method creates an instance of the view data.
      * @param type Type of the component.
      * @param id Identifier of the component in the XML.
      */
    EObject* viewComponent(string type, int id);

    /**
      * This method returns the current project's tree model.
      *
      */
    ETreeModel* getTreeModel();

    /**
      * This method returns a list of the image keys.
      *
      */
    deque<int> listImageKeys();

    /**
      * This method returns a list of the images.
      *
      */
    deque<string> listImages();

    /**
      * This method returns an image identifier.
      *
      */
    int getImageId(string imageName);

    /**
      * This method returns a free image identifier.
      *
      */
    int getFreeImageId();

    /**
      * This method returns a free point identifier.
      *
      */
    int getFreePointId();

    /**
      * This method starts a module.
      * @param module Name of the module.
      * @param image Image identifier.
      *
      */
    bool startModule(string module, int image);

    /**
     * This method starts the main execution loop.
     */
    bool exec();

    /**
     * This method reloads the module.
     */
    bool reload();

    /**
     * This is an overloaded method, added for convenience.
     * @param tagname The name of the intended XML tag.
     */
    string getXml(string tagname);

    /**
     * This method returns data from a specific node in a XML string..
     * @param tagname The name of the intended XML tag.
     * @param att The value of the attribute of the intended XML tag.
     * @param value The value of the node of the intended XML tag.
     */
    string getXml(string tagname, string att, string value);

    /**
      * This method makes a file to be used with the Stereoplotter module.
      * @param filename Name of the file to be saved on disk.
      * @param image1 Identifier of the first image.
      * @param image2 Identifier of the second image.
      */
    bool makeSPFile(string filename, int image1, int image2);

};

#endif //PROJECTMANAGER_H
