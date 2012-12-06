#------------------------------------------------
# General
#------------------------------------------------
SYNAPSE_ID = br.uerj.eng.efoto.teste.app
SYNAPSE_VERSION = 1.0
QT +=
RESOURCES +=
include(../../runtime/synapse.pri)
#------------------------------------------------
# Synapses
#------------------------------------------------


#------------------------------------------------
# Extensions
#------------------------------------------------

INCLUDEPATH += . \
						   src/c/infrastructure \
						   src/c/photogrammetry \
						   src/c/control \
						   src/c/interface \
						   src/qt/interface \
						   src/c/imageProcessing \
						   src/qt/imageDisplay \
						   src/qt/infrastructure \
						   src/c/imageDisplay \
						   src/qt/formDisplay \
						   src/qt/resource \
                                                   src/c/xml_definitions \


DEPENDPATH += $${INCLUDEPATH}

HEADERS = include/IPhotogrammetry.h \
		  src/Photogrammetry.h \
				   DEMManager.h \
				   EFotoManager.h \
				   IOManager.h \
				   OrthoManager.h \
				   ProjectManager.h \
				   PTManager.h \
				   SPManager.h \
				   SRManager.h \
				   ImageMatching.h \
				   Interpolation.h \
				   LeastSquaresMatching.h \
				   NormalizedCrossCorrelation.h \
				   RadiometricTransformation.h \
				   AbstractDetectorCoordinate.h \
				   AbstractImageCoordinate.h \
				   ConvertionsSystems.h \
				   Coordinate.h \
				   Dms.h \
				   EDom.h \
				   EDomValidator.h \
				   EObject.h \
				   ETreeModel.h \
				   GeoSystem.h \
				   Matrix.h \
				   PositionMatrix.h \
				   RectSupport.h \
				   SparseMatrix.h \
				   TreeFeatures.h \
				   DEMUserInterface.h \
				   IOUserInterface.h \
				   OrthoUserInterface.h \
				   ProjectUserInterface.h \
				   PTUserInterface.h \
				   SPUserInterface.h \
				   SRUserInterface.h \
				   DetectorFiducialMark.h \
				   DetectorSpaceCoordinate.h \
				   BundleAdjustment.h \
				   DemFeatures.h \
				   DemGrid.h \
				   ImageFiducialMark.h \
				   ImageSpaceCoordinate.h \
				   EOQuality.h \
				   ExteriorOrientation.h \
				   Flight.h \
				   FrameSensor.h \
				   Image.h \
				   InteriorOrientation.h \
				   IOQuality.h \
				   MatchingPoints.h \
				   MatchingPointsGrid.h \
				   ObjectSpaceCoordinate.h \
				   Orthorectification.h \
				   Point.h \
				   ProjectHeader.h \
				   ProjectiveRay.h \
				   RayTester.h \
				   Sensor.h \
				   SensorWithFiducialMarks.h \
				   SensorWithKnowDimensions.h \
				   SensorWithKnowParameters.h \
				   SpatialIntersection.h \
				   SpatialRessection.h \
				   StereoPair.h \
				   Terrain.h \
				   XmlUpdater.h \
				   AbstractForm.h \
				   ControlButtons.h \
				   FlightForm.h \
				   HeaderForm.h \
				   ImageForm.h \
				   ImagesForm.h \
				   PointForm.h \
				   PointsForm.h \
				   SensorForm.h \
				   TerrainForm.h \
				   GeometryResource.h \
				   ImageViewers.h \
				   RasterResource.h \
				   SingleDisplay.h \
				   SingleScene.h \
				   SingleTools.h \
				   StereoDisplay.h \
				   StereoScene.h \
				   StereoTools.h \
				   AboutForm.h \
				   DmsEdit.h \
				   EfotoDoubleSpinBox.h \
				   EPolygon.h \
				   ETableWidget.h \
				   FlightDirectionForm.h \
				   LoadingScreen.h \
				   ScienceSpinBox.h \
				   SigmaForm.h \
				   WindowsSelectPage.h \
				   DEMUserInterface_Qt.h \
				   IOUserInterface_Qt.h \
				   OrthoUserInterface_Qt.h \
				   ProgressWindow_Qt.h \
				   ProjectUserInterface_Qt.h \
				   PTUserInterface_Qt.h \
				   SPUserInterface_Qt.h \
				   SRUserInterface_Qt.h


SOURCES = src/Photogrammetry.cpp \
				   DEMManager.cpp \
				   EFotoManager.cpp \
				   IOManager.cpp \
				   OrthoManager.cpp \
				   ProjectManager.cpp \
				   PTManager.cpp \
				   SPManager.cpp \
				   SRManager.cpp \
				   ImageMatching.cpp \
				   Interpolation.cpp \
				   LeastSquaresMatching.cpp \
				   NormalizedCrossCorrelation.cpp \
				   RadiometricTransformation.cpp \
				   AbstractDetectorCoordinate.cpp \
				   AbstractImageCoordinate.cpp \
				   ConvertionsSystems.cpp \
				   Coordinate.cpp \
				   Dms.cpp \
				   EDom.cpp \
				   EDomValidator.cpp \
				   ETreeModel.cpp \
				   GeoSystem.cpp \
				   Matrix.cpp \
				   PositionMatrix.cpp \
				   RectSupport.cpp \
				   SparseMatrix.cpp \
				   TreeFeatures.cpp \
				   DEMUserInterface.cpp \
				   IOUserInterface.cpp \
				   OrthoUserInterface.cpp \
				   ProjectUserInterface.cpp \
				   PTUserInterface.cpp \
				   SPUserInterface.cpp \
				   SRUserInterface.cpp \
				   DetectorFiducialMark.cpp \
				   DetectorSpaceCoordinate.cpp \
				   BundleAdjustment.cpp \
				   DemFeatures.cpp \
				   DemGrid.cpp \
				   ImageFiducialMark.cpp \
				   ImageSpaceCoordinate.cpp \
				   EOQuality.cpp \
				   ExteriorOrientation.cpp \
				   Flight.cpp \
				   FrameSensor.cpp \
				   Image.cpp \
				   InteriorOrientation.cpp \
				   IOQuality.cpp \
				   MatchingPoints.cpp \
				   MatchingPointsGrid.cpp \
				   ObjectSpaceCoordinate.cpp \
				   Orthorectification.cpp \
				   Point.cpp \
				   ProjectHeader.cpp \
				   ProjectiveRay.cpp \
				   RayTester.cpp \
				   Sensor.cpp \
				   SensorWithFiducialMarks.cpp \
				   SensorWithKnowDimensions.cpp \
				   SensorWithKnowParameters.cpp \
				   SpatialIntersection.cpp \
				   SpatialRessection.cpp \
				   StereoPair.cpp \
				   Terrain.cpp \
				   XmlUpdater.cpp \
				   ControlButtons.cpp \
				   FlightForm.cpp \
				   HeaderForm.cpp \
				   ImageForm.cpp \
				   ImagesForm.cpp \
				   PointForm.cpp \
				   PointsForm.cpp \
				   SensorForm.cpp \
				   TerrainForm.cpp \
				   GeometryResource.cpp \
				   ImageViewers.cpp \
				   RasterResource.cpp \
				   SingleDisplay.cpp \
				   SingleScene.cpp \
				   SingleTools.cpp \
				   StereoDisplay.cpp \
				   StereoScene.cpp \
				   StereoTools.cpp \
				   AboutForm.cpp \
				   DmsEdit.cpp \
				   EfotoDoubleSpinBox.cpp \
				   EPolygon.cpp \
				   ETableWidget.cpp \
				   FlightDirectionForm.cpp \
				   LoadingScreen.cpp \
				   ScienceSpinBox.cpp \
				   SigmaForm.cpp \
				   WindowsSelectPage.cpp \
				   DEMUserInterface_Qt.cpp \
				   IOUserInterface_Qt.cpp \
				   OrthoUserInterface_Qt.cpp \
				   ProgressWindow_Qt.cpp \
				   ProjectUserInterface_Qt.cpp \
				   PTUserInterface_Qt.cpp \
				   SPUserInterface_Qt.cpp \
				   SRUserInterface_Qt.cpp

FORMS += \
				  FeatEditor.ui \
				  FeatsEditor.ui \
				  FlightLayout.ui \
				  HeaderLayout.ui \
				  ImageLayout.ui \
				  ImagesLayout.ui \
				  PointLayout.ui \
				  PointsLayout.ui \
				  SensorLayout.ui \
				  SpEditor.ui \
				  TerrainLayout.ui \
				  AboutLayout.ui \
				  FlightDirectionForm.ui \
				  LoadingScreen.ui \
				  DemExtraction.ui \
				  EfotoViewer.ui \
				  FormProject.ui \
				  FotoTriForm.ui \
				  Imageview.ui \
				  IOForm.ui \
				  OrthoForm.ui \
				  ProgressWindow.ui \
				  SeedEditor.ui \
				  SRForm.ui \
				  Stereoplotter.ui

RESOURCES += resource.qrc

QT += opengl

#LIBS += -L/usr/local/lib/ -lterralib
#INCLUDEPATH += $$PWD/../../../../../../../usr/local/lib
unix {
LIBS += -lGL -lGLU
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../usr/local/lib/release/ -lterralib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../usr/local/lib/debug/ -lterralib
else:symbian: LIBS += -lterralib
else:unix: LIBS += -L$$PWD/../../../../../../../usr/local/lib/ -lterralib

INCLUDEPATH += $$PWD/../../../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../../../usr/local/include
