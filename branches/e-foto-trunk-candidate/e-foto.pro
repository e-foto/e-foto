######################################################################
# e-foto.pro
######################################################################

TEMPLATE = app
TARGET = e-foto

QT += core widgets opengl

INCLUDEPATH += . \
               c/control \
               c/photogrammetry \
               c/infrastructure \
               c/xml_definitions \
               c/interface \
               c/imageProcessing \
               c/shapelib \
               qt/interface \
               qt/imageDisplay \
               qt/infrastructure \
               qt/formDisplay

# Input
HEADERS += c/control/DEMManager.h \
           c/control/EFotoManager.h \
           c/control/IOManager.h \
           c/control/OrthoManager.h \
           c/control/ProjectManager.h \
           c/control/PTManager.h \
           c/control/ReportManager.h \
           c/control/SPManager.h \
           c/control/SRManager.h \
           c/imageProcessing/ImageMatching.h \
           c/imageProcessing/Interpolation.h \
           c/imageProcessing/LeastSquaresMatching.h \
           c/imageProcessing/NormalizedCrossCorrelation.h \
           c/imageProcessing/RadiometricTransformation.h \
           c/infrastructure/AbstractDetectorCoordinate.h \
           c/infrastructure/AbstractImageCoordinate.h \
           c/infrastructure/ConvertionsSystems.h \
           c/infrastructure/Coordinate.h \
           c/infrastructure/Dms.h \
           c/infrastructure/EDom.h \
           c/infrastructure/EDomValidator.h \
           c/infrastructure/EObject.h \
           c/infrastructure/ETreeModel.h \
           c/infrastructure/GeoSystem.h \
           c/infrastructure/Matrix.h \
           c/infrastructure/PositionMatrix.h \
           c/infrastructure/ProgressPublisher.h \
           c/infrastructure/RectSupport.h \
           c/infrastructure/SparseMatrix.h \
           c/infrastructure/TreeFeatures.h \
           c/interface/DEMUserInterface.h \
           c/interface/IOUserInterface.h \
           c/interface/OrthoUserInterface.h \
           c/interface/ProjectUserInterface.h \
           c/interface/PTUserInterface.h \
           c/interface/ReportUserInterface.h \
           c/interface/SPUserInterface.h \
           c/interface/SRUserInterface.h \
           c/photogrammetry/BundleAdjustment.h \
           c/photogrammetry/DemFeatures.h \
           c/photogrammetry/DemGrid.h \
           c/photogrammetry/DetectorFiducialMark.h \
           c/photogrammetry/DetectorSpaceCoordinate.h \
           c/photogrammetry/Dummies.h \
           c/photogrammetry/EOQuality.h \
           c/photogrammetry/ExteriorOrientation.h \
           c/photogrammetry/Flight.h \
           c/photogrammetry/FrameSensor.h \
           c/photogrammetry/Image.h \
           c/photogrammetry/ImageFiducialMark.h \
           c/photogrammetry/ImageSpaceCoordinate.h \
           c/photogrammetry/InteriorOrientation.h \
           c/photogrammetry/IOQuality.h \
           c/photogrammetry/MatchingPoints.h \
           c/photogrammetry/MatchingPointsGrid.h \
           c/photogrammetry/ObjectSpaceCoordinate.h \
           c/photogrammetry/Orthorectification.h \
           c/photogrammetry/PhotoTriReport.h \
           c/photogrammetry/Point.h \
           c/photogrammetry/Project.h \
           c/photogrammetry/PhotoTri.h \
           c/photogrammetry/ProjectHeader.h \
           c/photogrammetry/ProjectiveRay.h \
           c/photogrammetry/RayTester.h \
           c/photogrammetry/Sensor.h \
           c/photogrammetry/SensorWithFiducialMarks.h \
           c/photogrammetry/SensorWithKnowDimensions.h \
           c/photogrammetry/SensorWithKnowParameters.h \
           c/photogrammetry/SpatialIntersection.h \
           c/photogrammetry/SpatialRessection.h \
           c/photogrammetry/StereoPair.h \
           c/photogrammetry/Terrain.h \
           c/xml_definitions/XMLAerial.h \
           c/xml_definitions/XMLFlight.h \
           c/xml_definitions/XMLProjectHeader.h \
           c/xml_definitions/XMLSensor.h \
           c/xml_definitions/XMLSensorWithFiducialMarks.h \
           c/xml_definitions/XMLTerrain.h \
           c/xml_definitions/XmlUpdater.h \
           qt/formDisplay/AbstractForm.h \
           qt/formDisplay/ControlButtons.h \
           qt/formDisplay/FlightForm.h \
           qt/formDisplay/HeaderForm.h \
           qt/formDisplay/ImageForm.h \
           qt/formDisplay/ImagesForm.h \
           qt/formDisplay/PointForm.h \
           qt/formDisplay/PointsForm.h \
           qt/formDisplay/SensorForm.h \
           qt/formDisplay/TerrainForm.h \
           qt/imageDisplay/GeometryResource.h \
           qt/imageDisplay/GLDisplay.h \
           qt/imageDisplay/ImageViewers.h \
           qt/imageDisplay/RasterResource.h \
           qt/imageDisplay/SingleDisplay.h \
           qt/imageDisplay/SingleScene.h \
           qt/imageDisplay/SingleTools.h \
           qt/imageDisplay/StereoDisplay.h \
           qt/imageDisplay/StereoScene.h \
           qt/imageDisplay/StereoTools.h \
           qt/infrastructure/AboutForm.h \
           qt/infrastructure/DmsEdit.h \
           qt/infrastructure/EfotoDoubleSpinBox.h \
           qt/infrastructure/EPolygon.h \
           qt/infrastructure/ETableWidget.h \
           qt/infrastructure/FlightDirectionForm.h \
           qt/infrastructure/LoadingScreen.h \
           qt/infrastructure/ScienceSpinBox.h \
           qt/infrastructure/SigmaForm.h \
           qt/infrastructure/WindowsSelectPage.h \
           qt/interface/DEMUserInterface_Qt.h \
           qt/interface/IOUserInterface_Qt.h \
           qt/interface/OrthoUserInterface_Qt.h \
           qt/interface/ProgressWindow_Qt.h \
           qt/interface/ProjectUserInterface_Qt.h \
           qt/interface/PTUserInterface_Qt.h \
           qt/interface/ReportUserInterface_Qt.h \
           qt/interface/SPUserInterface_Qt.h \
           qt/interface/SRUserInterface_Qt.h

FORMS += qt/formDisplay/FeatEditor.ui \
         qt/formDisplay/FeatsEditor.ui \
         qt/formDisplay/FlightLayout.ui \
         qt/formDisplay/HeaderLayout.ui \
         qt/formDisplay/ImageLayout.ui \
         qt/formDisplay/ImagesLayout.ui \
         qt/formDisplay/PointLayout.ui \
         qt/formDisplay/PointsLayout.ui \
         qt/formDisplay/SensorLayout.ui \
         qt/formDisplay/SpEditor.ui \
         qt/formDisplay/TerrainLayout.ui \
         qt/infrastructure/AboutLayout.ui \
         qt/infrastructure/FlightDirectionForm.ui \
         qt/infrastructure/LoadingScreen.ui \
         qt/interface/DemExtraction.ui \
         qt/interface/EfotoViewer.ui \
         qt/interface/FormProject.ui \
         qt/interface/FotoTriForm.ui \
         qt/interface/Imageview.ui \
         qt/interface/IOForm.ui \
         qt/interface/OrthoForm.ui \
         qt/interface/OrthoQualityEditor.ui \
         qt/interface/ProgressWindow.ui \
         qt/interface/ReportForm.ui \
         qt/interface/SeedEditor.ui \
         qt/interface/SRForm.ui \
         qt/interface/Stereoplotter.ui

SOURCES += c/control/DEMManager.cpp \
           c/control/EFotoManager.cpp \
           c/control/IOManager.cpp \
           c/control/OrthoManager.cpp \
           c/control/ProjectManager.cpp \
           c/control/PTManager.cpp \
           c/control/ReportManager.cpp \
           c/control/SPManager.cpp \
           c/control/SRManager.cpp \
           c/imageProcessing/ImageMatching.cpp \
           c/imageProcessing/Interpolation.cpp \
           c/imageProcessing/LeastSquaresMatching.cpp \
           c/imageProcessing/NormalizedCrossCorrelation.cpp \
           c/imageProcessing/RadiometricTransformation.cpp \
           c/infrastructure/AbstractDetectorCoordinate.cpp \
           c/infrastructure/AbstractImageCoordinate.cpp \
           c/infrastructure/ConvertionsSystems.cpp \
           c/infrastructure/Coordinate.cpp \
           c/infrastructure/Dms.cpp \
           c/infrastructure/EDom.cpp \
           c/infrastructure/EDomValidator.cpp \
           c/infrastructure/ETreeModel.cpp \
           c/infrastructure/GeoSystem.cpp \
           c/infrastructure/Matrix.cpp \
           c/infrastructure/PositionMatrix.cpp \
           c/infrastructure/ProgressPublisher.cpp \
           c/infrastructure/RectSupport.cpp \
           c/infrastructure/SparseMatrix.cpp \
           c/infrastructure/TreeFeatures.cpp \
           c/interface/DEMUserInterface.cpp \
           c/interface/IOUserInterface.cpp \
           c/interface/OrthoUserInterface.cpp \
           c/interface/ProjectUserInterface.cpp \
           c/interface/PTUserInterface.cpp \
           c/interface/ReportUserInterface.cpp \
           c/interface/SPUserInterface.cpp \
           c/interface/SRUserInterface.cpp \
           c/photogrammetry/BundleAdjustment.cpp \
           c/photogrammetry/DemFeatures.cpp \
           c/photogrammetry/DemGrid.cpp \
           c/photogrammetry/DetectorFiducialMark.cpp \
           c/photogrammetry/DetectorSpaceCoordinate.cpp \
           c/photogrammetry/Dummies.cpp \
           c/photogrammetry/EOQuality.cpp \
           c/photogrammetry/ExteriorOrientation.cpp \
           c/photogrammetry/Flight.cpp \
           c/photogrammetry/FrameSensor.cpp \
           c/photogrammetry/Image.cpp \
           c/photogrammetry/ImageFiducialMark.cpp \
           c/photogrammetry/ImageSpaceCoordinate.cpp \
           c/photogrammetry/InteriorOrientation.cpp \
           c/photogrammetry/IOQuality.cpp \
           c/photogrammetry/MatchingPoints.cpp \
           c/photogrammetry/MatchingPointsGrid.cpp \
           c/photogrammetry/ObjectSpaceCoordinate.cpp \
           c/photogrammetry/Orthorectification.cpp \
           c/photogrammetry/PhotoTriReport.cpp \
           c/photogrammetry/Point.cpp \
           c/photogrammetry/Project.cpp \
           c/photogrammetry/PhotoTri.cpp \
           c/photogrammetry/ProjectHeader.cpp \
           c/photogrammetry/ProjectiveRay.cpp \
           c/photogrammetry/RayTester.cpp \
           c/photogrammetry/Sensor.cpp \
           c/photogrammetry/SensorWithFiducialMarks.cpp \
           c/photogrammetry/SensorWithKnowDimensions.cpp \
           c/photogrammetry/SensorWithKnowParameters.cpp \
           c/photogrammetry/SpatialIntersection.cpp \
           c/photogrammetry/SpatialRessection.cpp \
           c/photogrammetry/StereoPair.cpp \
           c/photogrammetry/Terrain.cpp \
           c/xml_definitions/XMLAerial.cpp \
           c/xml_definitions/XMLFlight.cpp \
           c/xml_definitions/XMLProjectHeader.cpp \
           c/xml_definitions/XMLSensor.cpp \
           c/xml_definitions/XMLSensorWithFiducialMarks.cpp \
           c/xml_definitions/XMLTerrain.cpp \
           c/xml_definitions/XmlUpdater.cpp \
           qt/formDisplay/ControlButtons.cpp \
           qt/formDisplay/FlightForm.cpp \
           qt/formDisplay/HeaderForm.cpp \
           qt/formDisplay/ImageForm.cpp \
           qt/formDisplay/ImagesForm.cpp \
           qt/formDisplay/PointForm.cpp \
           qt/formDisplay/PointsForm.cpp \
           qt/formDisplay/SensorForm.cpp \
           qt/formDisplay/TerrainForm.cpp \
           qt/imageDisplay/GeometryResource.cpp \
           qt/imageDisplay/GLDisplay.cpp \
           qt/imageDisplay/ImageViewers.cpp \
           qt/imageDisplay/RasterResource.cpp \
           qt/imageDisplay/SingleDisplay.cpp \
           qt/imageDisplay/SingleScene.cpp \
           qt/imageDisplay/SingleTools.cpp \
           qt/imageDisplay/StereoDisplay.cpp \
           qt/imageDisplay/StereoScene.cpp \
           qt/imageDisplay/StereoTools.cpp \
           qt/infrastructure/AboutForm.cpp \
           qt/infrastructure/DmsEdit.cpp \
           qt/infrastructure/EfotoDoubleSpinBox.cpp \
           qt/infrastructure/EPolygon.cpp \
           qt/infrastructure/ETableWidget.cpp \
           qt/infrastructure/FlightDirectionForm.cpp \
           qt/infrastructure/LoadingScreen.cpp \
           qt/infrastructure/ScienceSpinBox.cpp \
           qt/infrastructure/SigmaForm.cpp \
           qt/infrastructure/WindowsSelectPage.cpp \
           qt/interface/DEMUserInterface_Qt.cpp \
           qt/interface/IOUserInterface_Qt.cpp \
           qt/interface/main.cpp \
           qt/interface/OrthoUserInterface_Qt.cpp \
           qt/interface/ProgressWindow_Qt.cpp \
           qt/interface/ProjectUserInterface_Qt.cpp \
           qt/interface/PTUserInterface_Qt.cpp \
           qt/interface/ReportUserInterface_Qt.cpp \
           qt/interface/SPUserInterface_Qt.cpp \
           qt/interface/SRUserInterface_Qt.cpp

RESOURCES += qt/resource/resource.qrc qt/resource/shaders.qrc

# e-foto icon
RC_FILE = efotoIcon.rc

# Check the Qt version
QT_VERSION = $$[QT_VERSION]
!build_pass:message($$QT_VERSION)
QT_VERSION = $$split(QT_VERSION, ".")
QT_VER_MAJ = $$member(QT_VERSION, 0)
QT_VER_MIN = $$member(QT_VERSION, 1)

message("QT_VERSION: $$QT_VER_MAJ.$$QT_VER_MIN")
message("QT_ARCH: $$QT_ARCH")
message("QMAKE_HOST.arch: $$QMAKE_HOST.arch")
	
# System unix configuration
unix {
	# Set gdal usage
    LIBS += -L/usr/include/gdal -lgdal
    INCLUDEPATH += /usr/include/gdal
    DEPENDPATH += /usr/include/gdal
	
	# install settings
    DEFINES += unix
    target.path = /usr/bin
    desk.path = /usr/share/applications
    desk.files += efoto.desktop
    icon.path = /usr/share/applications/pixmaps
    icon.files += efoto-icon.png
    INSTALLS += target desk icon
    #INCLUDEPATH += /usr/include/  Commented to avoid the fatal error during compilation on Ubuntu 18.04 with g++7, as answered by n.m on stackoverflow:
    #DEPENDPATH += /usr/include/   https://stackoverflow.com/questions/51350998/7515-fatal-error-stdlib-h-no-such-file-or-directory-include-next-stdlib-h/51351206

    # Build Settings
    DESTDIR = build/bin
    OBJECTS_DIR = build/temp/obj
    MOC_DIR = build/temp/moc
    UI_DIR = build/temp/ui
    RCC_DIR = build/temp/rcc
    QMAKE_CXXFLAGS += -std=c++17 -Wall
}

# System Windows Configuration
win32{
    # config&release mode off
    CONFIG-=debug_and_release
    CONFIG-=debug_and_release_target
	
    # Common parammeters for windows
    QMAKE_CXXFLAGS += -std=c++11 -Wall
	
	# Windows 64 bits version only
    win32-g++:contains(QT_ARCH, x86_64):{
        # Set gdal from msys64 default path
        INCLUDEPATH += "C:/msys64/mingw64/include"
        DEPENDPATH += "C:/msys64/mingw64/include"
        LIBS += -L"C:/msys64/mingw64/lib" -lgdal

        #Debug settings
        !build_pass:CONFIG(debug, debug|release) {
            !build_pass:message($$find(CONFIG, "debug"))
            !build_pass:message("Windows de 64 bits")
            #Build Debug
            debug: DESTDIR = "C:/msys64/mingw64/bin"
            debug: OBJECTS_DIR = build_debug_64bits/temp/obj
            debug: MOC_DIR = build_debug_64bits/temp/moc
            debug: UI_DIR = build_debug_64bits/temp/ui
            debug: RCC_DIR = build_debug_64bits/temp/rcc
        }

        #Release settings
        !build_pass:CONFIG(release, debug|release) {
            !build_pass:message($$find(CONFIG, "release"))
            !build_pass:message("Windows de 64 bits")
            #Release settings
            release: DESTDIR = "C:/msys64/mingw64/bin"
            release: OBJECTS_DIR = build_release_64bits/temp/obj
            release: MOC_DIR = build_release_64bits/temp/moc
            release: UI_DIR = build_release_64bits/temp/ui
            release: RCC_DIR = build_release_64bits/temp/rcc
        }

        # Grant deployment of Qt libraries
        QMAKE_POST_LINK = windeployqt $${DESTDIR}/e-foto.exe
    }
}

