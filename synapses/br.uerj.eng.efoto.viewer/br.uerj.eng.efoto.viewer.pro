# ------------------------------------------------
# General
# ------------------------------------------------
SYNAPSE_ID = br.uerj.eng.efoto.viewer
SYNAPSE_VERSION = 1.0
include(../../runtime/synapse.pri)

# ------------------------------------------------
# Synapses
# ------------------------------------------------
INCLUDEPATH += $$synapseIncludePath(br.uerj.eng.efoto.engine)

# ------------------------------------------------
# Extensions
# ------------------------------------------------

DEFINES += SYNAPSE_EFOTO
DEFINES += EFOTO_VIEWER

INCLUDEPATH += src \
	src/imageDisplay \
	src/resource \
	../br.uerj.eng.efoto.engine/src/infrastructure

DEPENDPATH += $${INCLUDEPATH}

HEADERS = include/IViewerService.h \
        include/ISingleViewer.h \
        include/IDoubleViewer.h \
        include/IStereoViewer.h \
	src/ViewerService.h \
	src/imageDisplay/GeometryResource.h \
	src/imageDisplay/RasterResource.h \
	src/imageDisplay/SingleDisplay.h \
	src/imageDisplay/SingleScene.h \
        src/imageDisplay/SingleTools.h \
        src/imageDisplay/SingleViewer.h \
        src/imageDisplay/StereoDisplay.h \
        src/imageDisplay/StereoScene.h \
        src/imageDisplay/StereoTools.h \
        src/imageDisplay/DoubleViewer.h \
        src/imageDisplay/StereoViewer.h \
	../br.uerj.eng.efoto.engine/src/infrastructure/AbstractDetectorCoordinate.h \
	../br.uerj.eng.efoto.engine/src/infrastructure/AbstractImageCoordinate.h \
	../br.uerj.eng.efoto.engine/src/infrastructure/ConvertionsSystems.h \
	../br.uerj.eng.efoto.engine/src/infrastructure/Coordinate.h \
	../br.uerj.eng.efoto.engine/src/infrastructure/Dms.h \
	../br.uerj.eng.efoto.engine/src/infrastructure/EDom.h \
	../br.uerj.eng.efoto.engine/src/infrastructure/EDomValidator.h \
	../br.uerj.eng.efoto.engine/src/infrastructure/EObject.h \
	../br.uerj.eng.efoto.engine/src/infrastructure/ETreeModel.h \
	../br.uerj.eng.efoto.engine/src/infrastructure/GeoSystem.h \
	../br.uerj.eng.efoto.engine/src/infrastructure/Matrix.h \
	../br.uerj.eng.efoto.engine/src/infrastructure/PositionMatrix.h \
	../br.uerj.eng.efoto.engine/src/infrastructure/RectSupport.h \
	../br.uerj.eng.efoto.engine/src/infrastructure/SparseMatrix.h \
	../br.uerj.eng.efoto.engine/src/infrastructure/TreeFeatures.h \
	../br.uerj.eng.efoto.engine/src/infrastructure/Features.h \
	../br.uerj.eng.efoto.engine/src/infrastructure/ProgressPublisher.h

SOURCES = src/ViewerService.cpp \
	src/imageDisplay/GeometryResource.cpp \
        src/imageDisplay/RasterResource.cpp \
        src/imageDisplay/SingleDisplay.cpp \
        src/imageDisplay/SingleScene.cpp \
        src/imageDisplay/SingleTools.cpp \
        src/imageDisplay/SingleViewer.cpp \
        src/imageDisplay/StereoDisplay.cpp \
        src/imageDisplay/StereoScene.cpp \
        src/imageDisplay/StereoTools.cpp \
        src/imageDisplay/DoubleViewer.cpp \
        src/imageDisplay/StereoViewer.cpp \
	../br.uerj.eng.efoto.engine/src/infrastructure/AbstractDetectorCoordinate.cpp \
	../br.uerj.eng.efoto.engine/src/infrastructure/AbstractImageCoordinate.cpp \
	../br.uerj.eng.efoto.engine/src/infrastructure/ConvertionsSystems.cpp \
	../br.uerj.eng.efoto.engine/src/infrastructure/Coordinate.cpp \
	../br.uerj.eng.efoto.engine/src/infrastructure/Dms.cpp \
	../br.uerj.eng.efoto.engine/src/infrastructure/EDom.cpp \
	../br.uerj.eng.efoto.engine/src/infrastructure/EDomValidator.cpp \
	../br.uerj.eng.efoto.engine/src/infrastructure/ETreeModel.cpp \
	../br.uerj.eng.efoto.engine/src/infrastructure/GeoSystem.cpp \
	../br.uerj.eng.efoto.engine/src/infrastructure/Matrix.cpp \
	../br.uerj.eng.efoto.engine/src/infrastructure/PositionMatrix.cpp \
	../br.uerj.eng.efoto.engine/src/infrastructure/RectSupport.cpp \
	../br.uerj.eng.efoto.engine/src/infrastructure/SparseMatrix.cpp \
	../br.uerj.eng.efoto.engine/src/infrastructure/TreeFeatures.cpp \
	../br.uerj.eng.efoto.engine/src/infrastructure/Features.cpp \
	../br.uerj.eng.efoto.engine/src/infrastructure/ProgressPublisher.cpp

RESOURCES += resource.qrc

QT += opengl

unix {LIBS += -lGL -lGLU}
