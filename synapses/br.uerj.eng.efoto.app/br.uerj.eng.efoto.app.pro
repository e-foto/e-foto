#------------------------------------------------
# General
#------------------------------------------------
SYNAPSE_ID = br.uerj.eng.efoto.app
SYNAPSE_VERSION = 1.0
include(../../runtime/synapse.pri)

#------------------------------------------------
# Synapses
#------------------------------------------------
INCLUDEPATH += $$synapseIncludePath(br.uerj.eng.efoto.viewer)
INCLUDEPATH += $$synapseIncludePath(br.uerj.eng.efoto.engine)


#------------------------------------------------
# Extensions
#------------------------------------------------

DEFINES += SYNAPSE_EFOTO

INCLUDEPATH += src \
			   src/control \
			   src/interface \
			   src/formDisplay \
			   src/infrastructure \
			   src/resource \
			   ../br.uerj.eng.efoto.engine/src/infrastructure \
			   ../br.uerj.eng.efoto.engine/src/photogrammetry \
			   ../br.uerj.eng.efoto.viewer/src/imageDisplay

DEPENDPATH += $${INCLUDEPATH}

HEADERS =  include/IPhotogrammetry.h \
		   #
		   src/Photogrammetry.h \
		   #
		   src/control/DEMManager.h \
		   src/control/EFotoManager.h \
		   src/control/IOManager.h \
		   src/control/OrthoManager.h \
		   src/control/ProjectManager.h \
		   src/control/PTManager.h \
		   src/control/SPManager.h \
		   src/control/SRManager.h \
		   #
		   src/interface/DEMUserInterface.h \
		   src/interface/IOUserInterface.h \
		   src/interface/OrthoUserInterface.h \
		   src/interface/ProjectUserInterface.h \
		   src/interface/PTUserInterface.h \
		   src/interface/SPUserInterface.h \
		   src/interface/SRUserInterface.h \
		   src/interface/DEMUserInterface_Qt.h \
		   src/interface/IOUserInterface_Qt.h \
		   src/interface/OrthoUserInterface_Qt.h \
		   src/interface/ProgressWindow_Qt.h \
		   src/interface/ProjectUserInterface_Qt.h \
		   src/interface/PTUserInterface_Qt.h \
		   src/interface/SPUserInterface_Qt.h \
		   src/interface/SRUserInterface_Qt.h \
		   #
		   src/formDisplay/AbstractForm.h \
		   src/formDisplay/ControlButtons.h \
		   src/formDisplay/FlightForm.h \
		   src/formDisplay/HeaderForm.h \
		   src/formDisplay/ImageForm.h \
		   src/formDisplay/ImagesForm.h \
		   src/formDisplay/PointForm.h \
		   src/formDisplay/PointsForm.h \
		   src/formDisplay/SensorForm.h \
		   src/formDisplay/TerrainForm.h \
		   #
		   src/infrastructure/AboutForm.h \
		   src/infrastructure/DmsEdit.h \
		   src/infrastructure/EfotoDoubleSpinBox.h \
		   src/infrastructure/EPolygon.h \
		   src/infrastructure/ETableWidget.h \
		   src/infrastructure/FlightDirectionForm.h \
		   src/infrastructure/LoadingScreen.h \
		   src/infrastructure/ScienceSpinBox.h \
		   src/infrastructure/SigmaForm.h \
		   src/infrastructure/WindowsSelectPage.h \
		   #
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
		   ../br.uerj.eng.efoto.engine/src/infrastructure/ProgressPublisher.h \
		   #
		   ../br.uerj.eng.efoto.engine/src/photogrammetry/ProjectHeader.h \
		   ../br.uerj.eng.efoto.engine/src/photogrammetry/Terrain.h \
		   ../br.uerj.eng.efoto.engine/src/photogrammetry/Sensor.h \
		   ../br.uerj.eng.efoto.engine/src/photogrammetry/Flight.h \
		   ../br.uerj.eng.efoto.engine/src/photogrammetry/Image.h \
		   ../br.uerj.eng.efoto.engine/src/photogrammetry/Point.h \
		   ../br.uerj.eng.efoto.engine/src/photogrammetry/InteriorOrientation.h \
		   ../br.uerj.eng.efoto.engine/src/photogrammetry/ExteriorOrientation.h \
		   ../br.uerj.eng.efoto.engine/src/photogrammetry/SensorWithFiducialMarks.h \
		   ../br.uerj.eng.efoto.engine/src/photogrammetry/SensorWithKnowDimensions.h \
		   ../br.uerj.eng.efoto.engine/src/photogrammetry/ImageFiducialMark.h \
		   ../br.uerj.eng.efoto.engine/src/photogrammetry/SensorWithKnowParameters.h \
                   #
                   ../br.uerj.eng.efoto.viewer/src/imageDisplay/SingleViewer.h \
                   ../br.uerj.eng.efoto.viewer/src/imageDisplay/DoubleViewer.h \
                   ../br.uerj.eng.efoto.viewer/src/imageDisplay/StereoViewer.h \
                   ../br.uerj.eng.efoto.viewer/src/imageDisplay/GeometryResource.h


SOURCES =  src/Photogrammetry.cpp \
		   #
		   src/control/DEMManager.cpp \
		   src/control/EFotoManager.cpp \
		   src/control/IOManager.cpp \
		   src/control/OrthoManager.cpp \
		   src/control/ProjectManager.cpp \
		   src/control/PTManager.cpp \
		   src/control/SPManager.cpp \
		   src/control/SRManager.cpp \
		   #
		   src/interface/DEMUserInterface.cpp \
		   src/interface/IOUserInterface.cpp \
		   src/interface/OrthoUserInterface.cpp \
		   src/interface/ProjectUserInterface.cpp \
		   src/interface/PTUserInterface.cpp \
		   src/interface/SPUserInterface.cpp \
		   src/interface/SRUserInterface.cpp \
		   src/interface/DEMUserInterface_Qt.cpp \
		   src/interface/IOUserInterface_Qt.cpp \
		   src/interface/OrthoUserInterface_Qt.cpp \
		   src/interface/ProgressWindow_Qt.cpp \
		   src/interface/ProjectUserInterface_Qt.cpp \
		   src/interface/PTUserInterface_Qt.cpp \
		   src/interface/SPUserInterface_Qt.cpp \
		   src/interface/SRUserInterface_Qt.cpp \
		   #
		   src/formDisplay/ControlButtons.cpp \
		   src/formDisplay/FlightForm.cpp \
		   src/formDisplay/HeaderForm.cpp \
		   src/formDisplay/ImageForm.cpp \
		   src/formDisplay/ImagesForm.cpp \
		   src/formDisplay/PointForm.cpp \
		   src/formDisplay/PointsForm.cpp \
		   src/formDisplay/SensorForm.cpp \
		   src/formDisplay/TerrainForm.cpp \
		   #
		   src/infrastructure/AboutForm.cpp \
		   src/infrastructure/DmsEdit.cpp \
		   src/infrastructure/EfotoDoubleSpinBox.cpp \
		   src/infrastructure/EPolygon.cpp \
		   src/infrastructure/ETableWidget.cpp \
		   src/infrastructure/FlightDirectionForm.cpp \
		   src/infrastructure/LoadingScreen.cpp \
		   src/infrastructure/ScienceSpinBox.cpp \
		   src/infrastructure/SigmaForm.cpp \
		   src/infrastructure/WindowsSelectPage.cpp \
		   #
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
		   ../br.uerj.eng.efoto.engine/src/infrastructure/ProgressPublisher.cpp \
                   #
                   ../br.uerj.eng.efoto.viewer/src/imageDisplay/GeometryResource.cpp

FORMS =   src/formDisplay/FeatEditor.ui \
		  src/formDisplay/FeatsEditor.ui \
		  src/formDisplay/FlightLayout.ui \
		  src/formDisplay/HeaderLayout.ui \
		  src/formDisplay/ImageLayout.ui \
		  src/formDisplay/ImagesLayout.ui \
		  src/formDisplay/PointLayout.ui \
		  src/formDisplay/PointsLayout.ui \
		  src/formDisplay/SensorLayout.ui \
		  src/formDisplay/SpEditor.ui \
		  src/formDisplay/TerrainLayout.ui \
		  #
		  src/infrastructure/AboutLayout.ui \
		  src/infrastructure/FlightDirectionForm.ui \
		  src/infrastructure/LoadingScreen.ui \
		  #
		  src/interface/DemExtraction.ui \
		  src/interface/FormProject.ui \
		  src/interface/FotoTriForm.ui \
		  src/interface/Imageview.ui \
		  src/interface/IOForm.ui \
		  src/interface/OrthoForm.ui \
		  src/interface/ProgressWindow.ui \
		  src/interface/SeedEditor.ui \
		  src/interface/SRForm.ui \
		  src/interface/Stereoplotter.ui

RESOURCES += resource.qrc

QT += opengl

unix {LIBS += -lGL -lGLU}
