TEMPLATE = app
TARGET = e-foto

# Build settings
DESTDIR = ../bin
OBJECTS_DIR = ../temp/obj
MOC_DIR = ../temp/moc
UI_DIR = ../temp/ui
RCC_DIR = ../temp/rcc
DEPENDPATH += . \
    control \
    infrastructure \
    photogrammetry \
    implementation/main \
    interface/io \
    interface/project \
    interface/sr \
    interface/view \
    interface/io/resource \
    interface/io/translation \
    interface/project/resource \
    interface/view/mmv
INCLUDEPATH += . \
    control \
    photogrammetry \
    infrastructure \
    interface/project \
    interface/io \
    interface/view \
    interface/view/mmv \
    interface/sr

# Input
HEADERS += control/EFotoManager.h \
    control/IOManager.h \
    control/ProjectManager.h \
    control/SRManager.h \
    infrastructure/AbstractAnalogCoordinate.h \
    infrastructure/AbstractDigitalCoordinate.h \
    infrastructure/Coordinate.h \
    infrastructure/EDom.h \
    infrastructure/EDomValidator.h \
    infrastructure/EObject.h \
    infrastructure/EParser.h \
    infrastructure/ETreeModel.h \
    infrastructure/Matrix.h \
    infrastructure/PositionMatrix.h \
    infrastructure/RectSupport.h \
    photogrammetry/Aerial.h \
    photogrammetry/AnalogFiductialMark.h \
    photogrammetry/AnalogImageSpaceCoordinate.h \
    photogrammetry/CheckingPoint.h \
    photogrammetry/ControlPoint.h \
    photogrammetry/DigitalFiductialMark.h \
    photogrammetry/DigitalImageSpaceCoordinate.h \
    photogrammetry/EOQuality.h \
    photogrammetry/ExteriorOrientation.h \
    photogrammetry/Flight.h \
    photogrammetry/Image.h \
    photogrammetry/InteriorOrientation.h \
    photogrammetry/IOQuality.h \
    photogrammetry/ObjectSpaceCoordinate.h \
    photogrammetry/PhotogrammetricPoint.h \
    photogrammetry/Point.h \
    photogrammetry/ProjectHeader.h \
    photogrammetry/Sensor.h \
    photogrammetry/SpatialRessection.h \
    photogrammetry/Terrain.h \
    interface/io/IOUserInterface.h \
    interface/io/IOUserInterface_Qt.h \
    interface/project/AbstractForm.h \
    interface/project/ControlButtons.h \
    interface/project/FlightForm.h \
    interface/project/HeaderForm.h \
    interface/project/ImageForm.h \
    interface/project/ImagesForm.h \
    interface/project/PointForm.h \
    interface/project/PointsForm.h \
    interface/project/ProjectUserInterface.h \
    interface/project/ProjectUserInterface_Qt.h \
    interface/project/SensorForm.h \
    interface/project/TerrainForm.h \
    interface/sr/SRUserInterface.h \
    interface/sr/SRUserInterface_Qt.h \
    interface/view/ImageView.h \
    interface/view/mmv/AbstractEModel.h \
    interface/view/mmv/blockview.h \
    interface/view/mmv/demview.h \
    interface/view/mmv/digitalimageview.h \
    interface/view/mmv/exteriororientationview.h \
    interface/view/mmv/fiductialmarksview.h \
    interface/view/mmv/flightsview.h \
    interface/view/mmv/flightview.h \
    interface/view/mmv/interiororientationview.h \
    interface/view/mmv/matrixview.h \
    interface/view/mmv/normalizationview.h \
    interface/view/mmv/orthorectificationview.h \
    interface/view/mmv/pointsview.h \
    interface/view/mmv/pointview.h \
    interface/view/mmv/projectheaderview.h \
    interface/view/mmv/sensorview.h \
    interface/view/mmv/terrainview.h \
    interface/view/AboutForm.h
FORMS += interface/io/IOForm.ui \
    interface/project/FormProject.ui \
    interface/project/HeaderLayout.ui \
    interface/project/ImageLayout.ui \
    interface/project/ImagesLayout.ui \
    interface/project/PointLayout.ui \
    interface/project/TerrainLayout.ui \
    interface/sr/SRForm.ui \
    interface/view/AboutLayout.ui \
    interface/project/SensorLayout.ui
SOURCES += control/EFotoManager.cpp \
    control/IOManager.cpp \
    control/ProjectManager.cpp \
    control/SRManager.cpp \
    infrastructure/AbstractAnalogCoordinate.cpp \
    infrastructure/AbstractDigitalCoordinate.cpp \
    infrastructure/Coordinate.cpp \
    infrastructure/EDom.cpp \
    infrastructure/EDomValidator.cpp \
    infrastructure/EObject.cpp \
    infrastructure/EParser.cpp \
    infrastructure/ETreeModel.cpp \
    infrastructure/Matrix.cpp \
    infrastructure/PositionMatrix.cpp \
    infrastructure/RectSupport.cpp \
    photogrammetry/Aerial.cpp \
    photogrammetry/AnalogFiductialMark.cpp \
    photogrammetry/AnalogImageSpaceCoordinate.cpp \
    photogrammetry/CheckingPoint.cpp \
    photogrammetry/ControlPoint.cpp \
    photogrammetry/DigitalFiductialMark.cpp \
    photogrammetry/DigitalImageSpaceCoordinate.cpp \
    photogrammetry/EOQuality.cpp \
    photogrammetry/ExteriorOrientation.cpp \
    photogrammetry/Flight.cpp \
    photogrammetry/Image.cpp \
    photogrammetry/InteriorOrientation.cpp \
    photogrammetry/IOQuality.cpp \
    photogrammetry/ObjectSpaceCoordinate.cpp \
    photogrammetry/PhotogrammetricPoint.cpp \
    photogrammetry/Point.cpp \
    photogrammetry/ProjectHeader.cpp \
    photogrammetry/Sensor.cpp \
    photogrammetry/SpatialRessection.cpp \
    photogrammetry/Terrain.cpp \
    implementation/main/main.cpp \
    interface/io/IOUserInterface.cpp \
    interface/io/IOUserInterface_Qt.cpp \
    interface/project/ControlButtons.cpp \
    interface/project/FlightForm.cpp \
    interface/project/HeaderForm.cpp \
    interface/project/ImageForm.cpp \
    interface/project/ImagesForm.cpp \
    interface/project/PointForm.cpp \
    interface/project/PointsForm.cpp \
    interface/project/ProjectUserInterface.cpp \
    interface/project/ProjectUserInterface_Qt.cpp \
    interface/project/SensorForm.cpp \
    interface/project/TerrainForm.cpp \
    interface/sr/SRUserInterface.cpp \
    interface/sr/SRUserInterface_Qt.cpp \
    interface/view/ImageView.cpp \
    interface/view/mmv/blockview.cpp \
    interface/view/mmv/demview.cpp \
    interface/view/mmv/digitalimageview.cpp \
    interface/view/mmv/exteriororientationview.cpp \
    interface/view/mmv/fiductialmarksview.cpp \
    interface/view/mmv/flightsview.cpp \
    interface/view/mmv/flightview.cpp \
    interface/view/mmv/interiororientationview.cpp \
    interface/view/mmv/matrixview.cpp \
    interface/view/mmv/normalizationview.cpp \
    interface/view/mmv/orthorectificationview.cpp \
    interface/view/mmv/pointsview.cpp \
    interface/view/mmv/pointview.cpp \
    interface/view/mmv/projectheaderview.cpp \
    interface/view/mmv/sensorview.cpp \
    interface/view/mmv/terrainview.cpp \
    interface/view/AboutForm.cpp
RESOURCES += interface/sr/sr_resource.qrc \
    interface/io/resource/io_icons.qrc \
    interface/project/resource/project_resource.qrc \
    interface/view/resource/view_resource.qrc
TRANSLATIONS += interface/io/translation/IOForm_pt_br.ts
