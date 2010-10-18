TEMPLATE = app
TARGET = fotogrametria_digital

# Build settings
DESTDIR = ../bin
OBJECTS_DIR = ../temp/FD2SD/obj
MOC_DIR = ../temp/FD2SD/moc
UI_DIR = ../temp/FD2SD/ui
RCC_DIR = ../temp/FD2SD/rcc
DEPENDPATH += . \
    control \
    infrastructure \
    photogrammetry \
	implementation/main \
	interface/project \
	interface/view \
	interface/project/resource
INCLUDEPATH += . \
    control \
    photogrammetry \
    infrastructure \
	interface/project \
	interface/view

# Input
HEADERS += control/EFotoManager.h \
    control/ProjectManager.h \
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
    photogrammetry/Flight.h \
	photogrammetry/Image.h \
    photogrammetry/ObjectSpaceCoordinate.h \
    photogrammetry/PhotogrammetricPoint.h \
    photogrammetry/Point.h \
    photogrammetry/ProjectHeader.h \
	photogrammetry/Sensor.h \
    photogrammetry/Terrain.h \
    interface/project/AbstractForm.h \
    interface/project/ControlButtons.h \
    interface/project/HeaderForm.h \
    interface/project/ImageForm.h \
    interface/project/ImagesForm.h \
    interface/project/PointForm.h \
	interface/project/PointsForm.h \
    interface/project/ProjectUserInterface.h \
    interface/project/ProjectUserInterface_Qt.h \
    interface/project/SensorForm.h \
    interface/project/FlightForm.h \
    interface/project/TerrainForm.h \
    interface/view/SigmaForm.h \
	interface/view/AboutForm.h
FORMS += interface/project/FormProject.ui \
    interface/project/HeaderLayout.ui \
    interface/project/ImageLayout.ui \
    interface/project/ImagesLayout.ui \
    interface/project/PointLayout.ui \
    interface/project/SensorLayout.ui \
    interface/project/FlightLayout.ui \
    interface/project/TerrainLayout.ui \
    interface/view/AboutLayout.ui \
    interface/project/PointsLayout.ui
SOURCES += control/EFotoManager.cpp \
	control/ProjectManager.cpp \
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
    photogrammetry/Flight.cpp \
	photogrammetry/Image.cpp \
    photogrammetry/ObjectSpaceCoordinate.cpp \
    photogrammetry/PhotogrammetricPoint.cpp \
    photogrammetry/Point.cpp \
    photogrammetry/ProjectHeader.cpp \
	photogrammetry/Sensor.cpp \
    photogrammetry/Terrain.cpp \
	implementation/main/main.cpp \
    interface/project/ControlButtons.cpp \
    interface/project/HeaderForm.cpp \
    interface/project/ImageForm.cpp \
    interface/project/ImagesForm.cpp \
    interface/project/PointForm.cpp \
	interface/project/PointsForm.cpp \
    interface/project/ProjectUserInterface.cpp \
    interface/project/ProjectUserInterface_Qt.cpp \
    interface/project/SensorForm.cpp \
    interface/project/FlightForm.cpp \
	interface/project/TerrainForm.cpp \
    interface/view/SigmaForm.cpp \
	interface/view/AboutForm.cpp
RESOURCES += \
    interface/project/resource/project_resource.qrc \
	interface/view/resource/view_resource.qrc
