TEMPLATE = app
TARGET =
DESTDIR = bin
OBJECTS_DIR = temp/obj
MOC_DIR = temp/moc
UI_DIR = temp/ui
RCC_DIR = temp/rcc
DEPENDPATH += . ../c/infrastructure ../c/photogrammetry ../c/control ../c/xml_definitions
INCLUDEPATH += . ../c/infrastructure ../c/photogrammetry ../c/control ../c/xml_definitions

# Input
HEADERS += ../c/infrastructure/AbstractAnalogCoordinate.h \
		   ../c/infrastructure/AbstractDigitalCoordinate.h \
		   ../c/infrastructure/ConvertionsSystems.h \
		   ../c/infrastructure/Coordinate.h \
		   ../c/infrastructure/Dms.h \
		   ../c/infrastructure/EDom.h \
		   ../c/infrastructure/EDomValidator.h \
		   ../c/infrastructure/EObject.h \
		   ../c/infrastructure/ETreeModel.h \
		   ../c/infrastructure/GeoSystem.h \
		   ../c/infrastructure/Matrix.h \
		   ../c/infrastructure/PositionMatrix.h \
		   ../c/infrastructure/RectSupport.h \
		   ../c/infrastructure/SparseMatrix.h \
		   ../c/photogrammetry/AnalogFiductialMark.h \
		   ../c/photogrammetry/AnalogImageSpaceCoordinate.h \
		   ../c/photogrammetry/BundleAdjustment.h \
		   ../c/photogrammetry/CheckingPoint.h \
		   ../c/photogrammetry/ControlPoint.h \
		   ../c/photogrammetry/DigitalFiductialMark.h \
		   ../c/photogrammetry/DigitalImageSpaceCoordinate.h \
		   ../c/photogrammetry/EOQuality.h \
		   ../c/photogrammetry/ExteriorOrientation.h \
		   ../c/photogrammetry/Flight.h \
		   ../c/photogrammetry/FrameSensor.h \
		   ../c/photogrammetry/Image.h \
		   ../c/photogrammetry/InteriorOrientation.h \
		   ../c/photogrammetry/IOQuality.h \
		   ../c/photogrammetry/ObjectSpaceCoordinate.h \
		   ../c/photogrammetry/PhotogrammetricPoint.h \
		   ../c/photogrammetry/Point.h \
		   ../c/photogrammetry/ProjectHeader.h \
		   ../c/photogrammetry/ProjectiveRay.h \
		   ../c/photogrammetry/Sensor.h \
		   ../c/photogrammetry/SensorWithFiducialMarks.h \
		   ../c/photogrammetry/SensorWithKnowDimensions.h \
		   ../c/photogrammetry/SensorWithKnowParameters.h \
		   ../c/photogrammetry/SpatialRessection.h \
		   ../c/photogrammetry/Terrain.h \
		   ../c/control/IOManager.h \
		   ../c/control/SRManager.h \
		   ../c/control/EFotoManager.h \
		   ../c/control/ProjectManager.h \
		   ../c/control/PTManager.h \
		   ../c/xml_definitions/XmlUpdater.h
SOURCES += ../c/infrastructure/AbstractAnalogCoordinate.cpp \
		   ../c/infrastructure/AbstractDigitalCoordinate.cpp \
		   ../c/infrastructure/ConvertionsSystems.cpp \
		   ../c/infrastructure/Coordinate.cpp \
		   ../c/infrastructure/Dms.cpp \
		   ../c/infrastructure/EDom.cpp \
		   ../c/infrastructure/EDomValidator.cpp \
		   ../c/infrastructure/ETreeModel.cpp \
		   ../c/infrastructure/GeoSystem.cpp \
		   ../c/infrastructure/Matrix.cpp \
		   ../c/infrastructure/PositionMatrix.cpp \
		   ../c/infrastructure/RectSupport.cpp \
		   ../c/infrastructure/SparseMatrix.cpp \
		   ../c/photogrammetry/AnalogFiductialMark.cpp \
		   ../c/photogrammetry/AnalogImageSpaceCoordinate.cpp \
		   ../c/photogrammetry/BundleAdjustment.cpp \
		   ../c/photogrammetry/CheckingPoint.cpp \
		   ../c/photogrammetry/ControlPoint.cpp \
		   ../c/photogrammetry/DigitalFiductialMark.cpp \
		   ../c/photogrammetry/DigitalImageSpaceCoordinate.cpp \
		   ../c/photogrammetry/EOQuality.cpp \
		   ../c/photogrammetry/ExteriorOrientation.cpp \
		   ../c/photogrammetry/Flight.cpp \
		   ../c/photogrammetry/FrameSensor.cpp \
		   ../c/photogrammetry/Image.cpp \
		   ../c/photogrammetry/InteriorOrientation.cpp \
		   ../c/photogrammetry/IOQuality.cpp \
		   ../c/photogrammetry/ObjectSpaceCoordinate.cpp \
		   ../c/photogrammetry/PhotogrammetricPoint.cpp \
		   ../c/photogrammetry/Point.cpp \
		   ../c/photogrammetry/ProjectHeader.cpp \
		   ../c/photogrammetry/ProjectiveRay.cpp \
		   ../c/photogrammetry/Sensor.cpp \
		   ../c/photogrammetry/SensorWithFiducialMarks.cpp \
		   ../c/photogrammetry/SensorWithKnowDimensions.cpp \
		   ../c/photogrammetry/SensorWithKnowParameters.cpp \
		   ../c/photogrammetry/SpatialRessection.cpp \
		   ../c/photogrammetry/Terrain.cpp \
		   ../c/control/IOManager.cpp \
		   ../c/control/SRManager.cpp \
		   ../c/control/EFotoManager.cpp \
		   ../c/control/ProjectManager.cpp \
		   ../c/control/PTManager.cpp \
		   ../c/xml_definitions/XmlUpdater.cpp \
		   controlMain.cpp
