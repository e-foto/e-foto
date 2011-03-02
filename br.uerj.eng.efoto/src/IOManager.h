/**************************************************************************
							   IOManager.h
**************************************************************************/


#ifndef IOMANAGER_H
#define IOMANAGER_H

#include "EObject.h"
#include "PositionMatrix.h"

class Sensor;
class Image;
class InteriorOrientation;
class IOUserInterface;
class EFotoManager;

/**
  * class IOManager
  *
  * @author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 10/06/2009
  * @version 1.0 - Rafael Alves de Aguiar & Irving da Silva Badolato
  *
  */

class IOManager
{
	// Private Attributes
	//
	bool started;
	bool status;
	EFotoManager* manager;
	Sensor* mySensor;
	Image* myImage;
	InteriorOrientation* myIO;
	IOUserInterface* myInterface;

public:

	// Constructors and Destructors
	//
	IOManager();
	IOManager(EFotoManager* manager, Sensor* mySensor, Image* myImage, InteriorOrientation* myIO);
	~IOManager();

	// Association Methods
	//
	void setInterface(IOUserInterface* newInterface);
	IOUserInterface* getInterface();

	// Other Methods
	//
	PositionMatrix getAnalogMarks();
	bool measureMark(int id, int col, int lin);
	unsigned int countMarks();
	deque<string> markData(int index);
	unsigned int getTotalMarks();
	int getCalculationMode();
	bool calculateIO();
	bool interiorDone();
	deque<string> makeReport();
	bool exec();
	void stop();
	int getId();
	void returnProject();
	bool save(string path);
	bool load(string path);
	string getImageFile();
	int getFrameRows();
	int getFrameColumns();
	void acceptIO();

};

#endif //IOMANAGER_H
