TEMPLATE = app
TARGET = StereoDisplayConcept

DESTDIR = bin
OBJECTS_DIR = temp/obj
MOC_DIR = temp/moc
UI_DIR = temp/ui
RCC_DIR = temp/rcc
QT += opengl

HEADERS += 	AbstractDisplay.h \
			StereoService.h \
			StereoDisplay.h \
			RasterResource.h \
			Geometry.h \
			View.h \
			TestWindow.h

SOURCES += 	AbstractDisplay.cpp \
			StereoService.cpp \
			StereoDisplay.cpp \
			RasterResource.cpp \
			Geometry.cpp \
			View.cpp \
			TestWindow.cpp \
			main.cpp
