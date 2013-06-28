#ifndef BR_UERJ_ENG_EFOTO_ENGINE_IPROJECT_H
#define BR_UERJ_ENG_EFOTO_ENGINE_IPROJECT_H

#include <QSharedPointer>
#include <deque>
using namespace std;

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class ProjectHeader;
class Terrain;
class Sensor;
class Flight;
class Image;
class Point;
class InteriorOrientation;
class ExteriorOrientation;
class DatabaseConnection;
//class Te3DPersistenceService;


namespace engine {


class IProject
{
public:
    virtual DatabaseConnection* getDatabase()=0;
    virtual bool connectDatabase()=0;
    virtual bool disconnectDatabase()=0;
    virtual bool newProject(string filename)=0;
    virtual bool loadProject()=0;
    virtual bool saveProject()=0;
    virtual bool loadFile(string filename)=0;
    virtual bool saveFile(string filename)=0;
    virtual bool getSaveState()=0;
    virtual bool closeProject()=0;
    virtual int getError()=0;

    virtual ProjectHeader* header()=0;
    virtual Terrain* terrain()=0;
    virtual Sensor* sensor(int id)=0;
    virtual Flight* flight(int id)=0;
    virtual Image* image(int id)=0;
    virtual Point* point(int id)=0;
    virtual InteriorOrientation* IO(int id)=0;
    virtual ExteriorOrientation* EO(int id)=0;
    virtual deque<Image*> allImages()=0;
    virtual deque<Point*> allPoints()=0;
    virtual deque<InteriorOrientation*> allIOs()=0;
    virtual deque<ExteriorOrientation*> allEOs()=0;

    virtual bool allImageHasIO()=0;
    virtual bool allImageHasEO()=0;
    virtual int getFreeSensorId()=0;
    virtual int getFreeFlightId()=0;
    virtual int getFreeImageId()=0;
    virtual int getFreePointId()=0;

    virtual void deleteSensor(int id, bool makeReconnections = true)=0;
    virtual void deleteFlight(int id, bool makeReconnections = true)=0;
    virtual void deleteImage(int id, bool makeReconnections = true)=0;
    virtual void deletePoint(int id, bool makeReconnections = true)=0;
    virtual void deleteIO(int id, bool makeReconnections = true)=0;
    virtual void deleteEO(int id, bool makeReconnections = true)=0;

    virtual void addSensor(string data = "", bool makeReconnections = true)=0;
    virtual void addFlight(string data = "", bool makeReconnections = true)=0;
    virtual void addImage(string data = "", bool makeReconnections = true)=0;
    virtual void addPoint(string data = "", bool makeReconnections = true)=0;
    virtual void addIO(string data, bool makeReconnections = true)=0;
    virtual void addEO(string data, bool makeReconnections = true)=0;

    virtual void setXml(string xml)=0;
    virtual string getXml(string tagname)=0;
    virtual string getXml(string tagname, string att, string value)=0;
    virtual string getXml()=0;

    virtual string getProcessStates()=0;
    virtual void setProcessStates(string state)=0;
};

typedef QSharedPointer<IProject> IProjectPtr;

}
}
}
}
} //namespace

#endif // BR_UERJ_ENG_EFOTO_ENGINE_IPROJECT_H
