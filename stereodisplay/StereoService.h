#ifndef STEREOSERVICE_H
#define STEREOSERVICE_H

#include "StereoDisplay.h"

class StereoService
{
public:
	StereoService();
	~StereoService();

        MonoDisplay* getMonoDisplay(QWidget* parent, MonoView* currentView = NULL);
        DetailDisplay* getDetailDisplay(MonoDisplay* parent);
        OverviewDisplay* getOverviewDisplay(MonoDisplay* parent);
	StereoDisplay* getStereoDisplay(QWidget* parent, StereoView* currentView = NULL);

        MonoView* getMonoView(QObject* parent, QString filepath);
	StereoView* getStereoView(QObject* parent, QString leftImageFilepath, QString rightImageFilepath);
};

#endif // STEREOSERVICE_H



/* IGeoService interface
class IGeoService: public cortex::ISynapse
{
	public:
		virtual ~IGeoService(){}
		virtual IDataSource* getDBSource(QString dbms, QString dbName, QString host,
					 QString user, QString passwd, int portNumber)=0;
		virtual IMapViewer* getMapViewer(QWidget* parent) = 0;
		virtual QTreeView* getLayersTreeView(QWidget* parent=0) = 0;
		virtual QTreeView* getViewsTreeView(QWidget* parent=0) = 0;
		virtual QWidget* getRasterTransform(br::org::te::geo::ITheme* theme) = 0;
		virtual QGLWidget* getStereoDisplay() = 0;
		virtual void addModelListener(IDSrcModelListener* mdListeners);
		virtual void removeModelListener(IDSrcModelListener* mdListeners);
	protected:
		std::vector<IDSrcModelListener*> mdListeners_;
};
typedef QSharedPointer<IGeoService> IGeoServicePtr;
*/
/* inline IGeoService methods
inline void IGeoService::addModelListener(IDSrcModelListener* mdListener)
{
	mdListeners_.push_back(mdListener);
}
inline void IGeoService::removeModelListener(IDSrcModelListener* mdListener)
{
	std::vector<IDSrcModelListener*>::iterator it = std::find(mdListeners_.begin(), mdListeners_.end(), mdListener);

	if(it == mdListeners_.end())
		throw;

	mdListeners_.erase(it);
}
*/
/* GeoService class
class Q_DECL_EXPORT GeoService: public QObject, public IGeoService
{
	Q_OBJECT
	Q_INTERFACES(cortex::ISynapse br::org::te::geo::IGeoService)
	public:
		GeoService();
		virtual ~GeoService();
		IDataSource* getDBSource(QString dbms, QString dbName, QString host,
								 QString user, QString passwd, int portNumber);
		IMapViewer* getMapViewer(QWidget* parent);
		QTreeView* getLayersTreeView(QWidget* parent=0);
		QTreeView* getViewsTreeView(QWidget* parent=0);
		QWidget* getRasterTransform(br::org::te::geo::ITheme* theme);
		QGLWidget* getStereoDisplay();
		virtual bool start();
		virtual bool stop();
	protected slots:
		void viewChanged(br::org::te::geo::IView* view);
	protected:
		virtual cortex::ISynapse* getImplementation();
		TdkApplicationController* appControl_;
		TdkPersister* persister_;
		DSItemModel* model_;
		DataSrcLayerFilter* layerModel_;
		DataSrcLayerFilter* viewModel_;
		DSViewSelectionModel* lselModel_;
		DSViewSelectionModel* vselModel_;
};
*/
