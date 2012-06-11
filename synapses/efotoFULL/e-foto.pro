TEMPLATE = app
TARGET = e-foto

# Build settings
DESTDIR = bin
OBJECTS_DIR = temp/obj
MOC_DIR = temp/moc
UI_DIR = temp/ui
RCC_DIR = temp/rcc

QT += opengl

unix {
LIBS += -lGL -lGLU
}

win32 {
}

DEFINES += INTEGRATED_EFOTO

INCLUDEPATH += .\
			  ../br.uerj.eng.efoto.engine/src/photogrammetry \
			  ../br.uerj.eng.efoto.engine/src/imageProcessing \
			  ../br.uerj.eng.efoto.engine/src/infrastructure \
			  ../br.uerj.eng.efoto.engine/src/xml_definitions \
			  ../br.uerj.eng.efoto.viewer/src/imageDisplay \
			  ../br.uerj.eng.efoto.app/src/control \
			  ../br.uerj.eng.efoto.app/src/interface \
			  ../br.uerj.eng.efoto.app/src/formDisplay \
			  ../br.uerj.eng.efoto.app/src/infrastructure \
			  ../br.uerj.eng.efoto.app/src/resource

DEPENDPATH += $${INCLUDEPATH}

# Input
HEADERS += \
		   #../br.uerj.eng.efoto.engine/src/photogrammetry/
		   DetectorFiducialMark.h \
		   DetectorSpaceCoordinate.h \
		   BundleAdjustment.h \
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
		   Dummies.h \
		   Project.h \
		   #../br.uerj.eng.efoto.engine/src/imageProcessing/
		   ImageMatching.h \
		   Interpolation.h \
		   LeastSquaresMatching.h \
		   NormalizedCrossCorrelation.h \
		   RadiometricTransformation.h \
		   #../br.uerj.eng.efoto.engine/src/infrastructure/
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
		   Features.h \
		   ProgressPublisher.h \
		   #../br.uerj.eng.efoto.engine/src/xml_definitions/
		   XmlUpdater.h \
		   #
		   #
		   #
		   #../br.uerj.eng.efoto.viewer/src/imageDisplay/
		   GeometryResource.h \
		   ImageViewers.h \
		   RasterResource.h \
		   SingleDisplay.h \
		   SingleScene.h \
		   SingleTools.h \
		   StereoDisplay.h \
		   StereoScene.h \
		   StereoTools.h \
		   #
		   #
		   #
		   #c/control/
		   DEMManager.h \
		   EFotoManager.h \
		   IOManager.h \
		   OrthoManager.h \
		   ProjectManager.h \
		   PTManager.h \
		   SPManager.h \
		   SRManager.h \
		   #c/interface/
		   DEMUserInterface.h \
		   IOUserInterface.h \
		   OrthoUserInterface.h \
		   ProjectUserInterface.h \
		   PTUserInterface.h \
		   SPUserInterface.h \
		   SRUserInterface.h \
		   #qt/formDisplay/
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
		   #qt/infrastructure/
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
		   #qt/interface/
		   DEMUserInterface_Qt.h \
		   IOUserInterface_Qt.h \
		   OrthoUserInterface_Qt.h \
		   ProgressWindow_Qt.h \
		   ProjectUserInterface_Qt.h \
		   PTUserInterface_Qt.h \
		   SPUserInterface_Qt.h \
		   SRUserInterface_Qt.h

SOURCES += \
		   #../br.uerj.eng.efoto.engine/src/photogrammetry/
		   DetectorFiducialMark.cpp \
		   DetectorSpaceCoordinate.cpp \
		   BundleAdjustment.cpp \
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
		   Dummies.cpp \
		   Project.cpp \
		   #../br.uerj.eng.efoto.engine/src/imageProcessing/
		   ImageMatching.cpp \
		   Interpolation.cpp \
		   LeastSquaresMatching.cpp \
		   NormalizedCrossCorrelation.cpp \
		   RadiometricTransformation.cpp \
		   #../br.uerj.eng.efoto.engine/src/infrastructure/
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
		   Features.cpp \
		   ProgressPublisher.cpp \
		   #../br.uerj.eng.efoto.engine/src/xml_definitions/
		   XmlUpdater.cpp \
		   #
		   #
		   #
		   #../br.uerj.eng.efoto.viewer/src/imageDisplay/
		   GeometryResource.cpp \
		   ImageViewers.cpp \
		   RasterResource.cpp \
		   SingleDisplay.cpp \
		   SingleScene.cpp \
		   SingleTools.cpp \
		   StereoDisplay.cpp \
		   StereoScene.cpp \
		   StereoTools.cpp \
		   #
		   #
		   #
		   #c/control/
		   DEMManager.cpp \
		   EFotoManager.cpp \
		   IOManager.cpp \
		   OrthoManager.cpp \
		   ProjectManager.cpp \
		   PTManager.cpp \
		   SPManager.cpp \
		   SRManager.cpp \
		   #c/interface/
		   DEMUserInterface.cpp \
		   IOUserInterface.cpp \
		   OrthoUserInterface.cpp \
		   ProjectUserInterface.cpp \
		   PTUserInterface.cpp \
		   SPUserInterface.cpp \
		   SRUserInterface.cpp \
		   #qt/formDisplay/
		   ControlButtons.cpp \
		   FlightForm.cpp \
		   HeaderForm.cpp \
		   ImageForm.cpp \
		   ImagesForm.cpp \
		   PointForm.cpp \
		   PointsForm.cpp \
		   SensorForm.cpp \
		   TerrainForm.cpp \
		   #qt/infrastructure/
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
		   #qt/interface/
		   DEMUserInterface_Qt.cpp \
		   IOUserInterface_Qt.cpp \
		   main.cpp \
		   OrthoUserInterface_Qt.cpp \
		   ProgressWindow_Qt.cpp \
		   ProjectUserInterface_Qt.cpp \
		   PTUserInterface_Qt.cpp \
		   SPUserInterface_Qt.cpp \
		   SRUserInterface_Qt.cpp

FORMS += \
		  #qt/formDisplay/
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
		  #qt/infrastructure/
		  AboutLayout.ui \
		  FlightDirectionForm.ui \
		  LoadingScreen.ui \
		  #qt/interface/
		  DemExtraction.ui \
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
