TEMPLATE = app
TARGET = 
DEPENDPATH += . \
    ../../interface/io/view \
    ../../interface/io \
    ../../interface/project \
    ../../interface/io/resource \
    ../../interface/io/translation \
    INCLUDEPATH \
    += \
    .

# Input
HEADERS += ../../interface/project/ProjectUserInterface.h \
    ../../interface/project/ProjectUserInterface_Qt.h \
    ../../interface/project/ControlButtons.h \
    ../../interface/io/IOUserInterface.h \
    ../../interface/io/IOUserInterface_Qt.h \
    ../../interface/sr/SRUserInterface.h \
	../../interface/sr/SRUserInterface_Qt.h \
	../../interface/view/ImageView.h \
	../../interface/view/mmv/AbstractEModel.h \
	../../interface/view/mmv/matrixview.h \
        ../../interface/view/mmv/flightview.h \
        ../../interface/view/mmv/sensorview.h \
        ../../interface/view/mmv/pointsview.h \
	../../interface/project/HeaderForm.h \
	../../interface/project/ImageForm.h \
	../../interface/project/ImagesForm.h \
	../../interface/project/AbstractForm.h \
	../../interface/project/TerrainForm.h \
        ../../interface/project/FlightForm.h \
        ../../interface/project/PointForm.h \
        ../../interface/project/PointsForm.h \
        ../../interface/project/SensorForm.h \
    ../../photogrammetry/EObject.h \
    ../../photogrammetry/EDom.h \
    ../../photogrammetry/EDomValidator.h \
    ../../photogrammetry/ETreeModel.h \
    ../../photogrammetry/EParser.h \
    ../../photogrammetry/RectSupport.h \
    ../../photogrammetry/PositionMatrix.h \
    ../../photogrammetry/Matrix.h \
    ../../photogrammetry/Coordinate.h \
    ../../photogrammetry/AbstractAnalogCoordinate.h \
    ../../photogrammetry/AbstractDigitalCoordinate.h \
    ../../photogrammetry/AnalogImageSpaceCoordinate.h \
    ../../photogrammetry/DigitalImageSpaceCoordinate.h \
    ../../photogrammetry/ObjectSpaceCoordinate.h \
    ../../photogrammetry/AnalogFiductialMark.h \
    ../../photogrammetry/DigitalFiductialMark.h \
    ../../photogrammetry/Point.h \
    ../../photogrammetry/CheckingPoint.h \
    ../../photogrammetry/ControlPoint.h \
    ../../photogrammetry/PhotogrammetricPoint.h \
    ../../photogrammetry/Image.h \
    ../../photogrammetry/Sensor.h \
    ../../photogrammetry/Aerial.h \
    ../../photogrammetry/Flight.h \
    ../../photogrammetry/InteriorOrientation.h \
    ../../photogrammetry/IOQuality.h \
    ../../photogrammetry/ExteriorOrientation.h \
    ../../photogrammetry/SpatialRessection.h \
    ../../photogrammetry/EOQuality.h \
    ../../photogrammetry/ProjectHeader.h \
    ../../photogrammetry/Terrain.h \
    ../../control/EFotoManager.h \
    ../../control/ProjectManager.h \
    ../../control/IOManager.h \
	../../control/SRManager.h
FORMS += ../../interface/project/FormProject.ui \
    ../../interface/io/IOForm.ui \
	../../interface/sr/SRForm.ui \
	../../interface/project/headerLayout.ui \
	../../interface/project/imageLayout.ui \
	../../interface/project/imagesLayout.ui \
	../../interface/project/terrainLayout.ui \
	../../interface/project/PointLayout.ui
SOURCES += ../../interface/project/ProjectUserInterface.cpp \
    ../../interface/project/ProjectUserInterface_Qt.cpp \
    ../../interface/project/ControlButtons.cpp \
    ../../interface/io/IOUserInterface.cpp \
    ../../interface/io/IOUserInterface_Qt.cpp \
    ../../interface/sr/SRUserInterface.cpp \
	../../interface/sr/SRUserInterface_Qt.cpp \
	../../interface/view/ImageView.cpp \
	../../interface/view/mmv/matrixview.cpp \
        ../../interface/view/mmv/flightview.cpp \
        ../../interface/view/mmv/sensorview.cpp \
        ../../interface/view/mmv/pointsview.cpp \
	../../interface/project/HeaderForm.cpp \
	../../interface/project/ImageForm.cpp \
	../../interface/project/ImagesForm.cpp \
	../../interface/project/TerrainForm.cpp \
        ../../interface/project/FlightForm.cpp \
        ../../interface/project/PointForm.cpp \
        ../../interface/project/PointsForm.cpp \
        ../../interface/project/SensorForm.cpp \
    ../../photogrammetry/EDom.cpp \
    ../../photogrammetry/EDomValidator.cpp \
    ../../photogrammetry/ETreeModel.cpp \
    ../../photogrammetry/EParser.cpp \
    ../../photogrammetry/RectSupport.cpp \
    ../../photogrammetry/PositionMatrix.cpp \
    ../../photogrammetry/Matrix.cpp \
    ../../photogrammetry/Coordinate.cpp \
    ../../photogrammetry/AbstractAnalogCoordinate.cpp \
    ../../photogrammetry/AbstractDigitalCoordinate.cpp \
    ../../photogrammetry/AnalogImageSpaceCoordinate.cpp \
    ../../photogrammetry/DigitalImageSpaceCoordinate.cpp \
    ../../photogrammetry/ObjectSpaceCoordinate.cpp \
    ../../photogrammetry/AnalogFiductialMark.cpp \
    ../../photogrammetry/DigitalFiductialMark.cpp \
    ../../photogrammetry/Point.cpp \
    ../../photogrammetry/CheckingPoint.cpp \
    ../../photogrammetry/ControlPoint.cpp \
    ../../photogrammetry/PhotogrammetricPoint.cpp \
    ../../photogrammetry/Image.cpp \
    ../../photogrammetry/Sensor.cpp \
    ../../photogrammetry/Aerial.cpp \
    ../../photogrammetry/Flight.cpp \
    ../../photogrammetry/InteriorOrientation.cpp \
    ../../photogrammetry/IOQuality.cpp \
    ../../photogrammetry/ExteriorOrientation.cpp \
    ../../photogrammetry/SpatialRessection.cpp \
    ../../photogrammetry/EOQuality.cpp \
    ../../photogrammetry/ProjectHeader.cpp \
    ../../photogrammetry/Terrain.cpp \
    ../../control/EFotoManager.cpp \
    ../../control/ProjectManager.cpp \
    ../../control/IOManager.cpp \
    ../../control/SRManager.cpp \
	main.cpp
RESOURCES += ../../interface/io/resource/io_icons.qrc \
    ../../interface/sr/sr_resource.qrc \
    ../../interface/project/resource/project_resource.qrc
TRANSLATIONS += ../../interface/io/translation/IOForm_pt_br.ts
