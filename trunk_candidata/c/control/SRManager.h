/**************************************************************************
							   SRManager.h
**************************************************************************/


#ifndef SRMANAGER_H
#define SRMANAGER_H

#include "EObject.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Terrain;
class Sensor;
class Flight;
class Image;
class InteriorOrientation;
class SpatialRessection;
class Point;
class SRUserInterface;
class EFotoManager;

/**
  * class SRManager
  *
  * @author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 10/06/2009
  * @version 1.0 - Rafael Alves de Aguiar & Irving da Silva Badolato
  *
  */

class SRManager
{
	// Private Attributes
	//
	bool started;
	bool status;
	EFotoManager* manager;
	Terrain* myTerrain;
	Sensor* mySensor;
	Flight* myFlight;
	Image* myImage;
	InteriorOrientation* myIO;
	SpatialRessection* mySR;
	deque<Point*> myPoints;
	SRUserInterface* myInterface;

public:

	// Constructors and Destructors
	//
	SRManager();
	SRManager(EFotoManager* manager, Terrain* myTerrain, Sensor* mySensor, Flight* myFlight, Image* myImage, InteriorOrientation* myIO, SpatialRessection* mySR);
	~SRManager();

	// Association Methods
	//
	void setInterface(SRUserInterface* newInterface);
	SRUserInterface* getInterface();

	// Other Methods
	//
	bool measurePoint(int id, int col, int lin);
	void selectPoint(int id);
	void unselectPoint(int id);
	deque<double> pointToAnalog(int col, int lin);
	bool removePoint(int id);
	bool insertPointOnImage(int id);
	bool removePointFromImage(int id);
	deque<string> listSelectedPoints();
	deque<string> listImagePoints();
	deque<string> listAllPoints();
	deque<string> pointData(int index);
	unsigned int countSelectedPoints();
	bool connectImagePoints();
	bool updatePointsTable();
	bool flightDirection(int markId);
	bool flightDirection(int col, int lin);
	bool calculateSR(int iterations, double gnssPrecision, double insPrecision);
	bool exteriorDone();
	deque<string> makeReport();
	bool exec();
	int getId();
	void returnProject();
	bool save(string path);
	bool load(string path);
	string getImageFile();
	void acceptSR();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //SRMANAGER_H
