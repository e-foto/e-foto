/*******************************************************************************
                                  ProjectHeader.h
*******************************************************************************/


#ifndef PROJECTHEADER_H
#define PROJECTHEADER_H

#include "EObject.h"

class ProjectHeader : public EObject
{
    //EOBJECT

    //Private attributes
    //
    string name;
    string description;
    string filePath;
    string fileName;
    string creation;
    string modification;
    string owner;
    string aims;
    string context;

public:

    // Constructors and Destructor
    //
    virtual ~ProjectHeader();

    // Private attribute accessor methods
    //
    void setName(string newName);
    void setDescription(string newDescription);
    void setFilePath(string newFilePath);
    void setFileName(string newFileName);
    void setCreation(string newCreation);
    void setModification(string newModification);
    void setOwner(string newOwner);
    void setAims(string newAims);
    void setContext(string newContext);
    string getName();
    string getDescription();
    string getFilePath();
    string getFileName();
    string getCreation();
    string getModification();
    string getOwner();
    string getAims();
    string getContext();

    //friend class Project;

    // EObject methods
    //
    string objectType(void);
    string objectAssociations(void);
    bool is(string s);

    // XML methods
    void xmlSetData(string xml);
    string xmlGetData();

};

#endif //PROJECTHEADER_H
