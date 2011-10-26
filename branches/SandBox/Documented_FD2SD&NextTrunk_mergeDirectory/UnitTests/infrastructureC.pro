TEMPLATE = app
TARGET =
DESTDIR = bin
OBJECTS_DIR = temp/obj
MOC_DIR = temp/moc
UI_DIR = temp/ui
RCC_DIR = temp/rcc
DEPENDPATH += . ../c/infrastructure
INCLUDEPATH += . ../c/infrastructure

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
		   ../c/infrastructure/SparseMatrix.h
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
		   infrastructureMain.cpp
