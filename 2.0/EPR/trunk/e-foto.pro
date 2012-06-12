TEMPLATE = app
TARGET = e-foto_Report

# Build settings
DESTDIR = ../bin
OBJECTS_DIR = ../temp/obj
MOC_DIR = ../temp/moc
UI_DIR = ../temp/ui
RCC_DIR = ../temp/rcc

QT += opengl

unix:
{
LIBS += -lGL -lGLU
}

DEPENDPATH += c/control \
    c/imageDisplay \
    c/imageProcessing \
    c/infrastructure \
    c/interface \
    c/photogrammetry \
    c/xml_definitions \
    qt/formDisplay \
    qt/imageDisplay \
    qt/infrastructure \
    qt/interface \
    qt/resource
INCLUDEPATH += . \
    c/infrastructure \
    c/photogrammetry \
    c/control \
    c/interface \
    qt/interface \
    c/imageProcessing \
    qt/imageDisplay \
    qt/infrastructure \
    c/imageDisplay \
    c/xml_definitions \
    qt/formDisplay \
    qt/resource

# Input
HEADERS += ReportManager.h \
    EFotoManager.h \
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
    ReportUserInterface.h \
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
    GeometryResource.h \
    ImageViewers.h \
    RasterResource.h \
    SingleDisplay.h \
    SingleScene.h \
    SingleTools.h \
    StereoDisplay.h \
    StereoScene.h \
    StereoTools.h \
    DmsEdit.h \
    EfotoDoubleSpinBox.h \
    EPolygon.h \
    ETableWidget.h \
    LoadingScreen.h \
    ScienceSpinBox.h \
    SigmaForm.h \
    WindowsSelectPage.h \
    ReportUserInterface_Qt.h \
    ProgressPublisher.h \
    Project.h

SOURCES += ReportManager.cpp \
    EFotoManager.cpp \
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
    ReportUserInterface.cpp \
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
    GeometryResource.cpp \
    ImageViewers.cpp \
    RasterResource.cpp \
    SingleDisplay.cpp \
    SingleScene.cpp \
    SingleTools.cpp \
    StereoDisplay.cpp \
    StereoScene.cpp \
    StereoTools.cpp \
    DmsEdit.cpp \
    EfotoDoubleSpinBox.cpp \
    EPolygon.cpp \
    ETableWidget.cpp \
    LoadingScreen.cpp \
    ScienceSpinBox.cpp \
    SigmaForm.cpp \
    WindowsSelectPage.cpp \
    main.cpp \
    ReportUserInterface_Qt.cpp \
    ProgressPublisher.cpp \
    Project.cpp

FORMS += LoadingScreen.ui \
    ReportForm.ui
RESOURCES += resource.qrc

RC_FILE = efotoIcon.rc
