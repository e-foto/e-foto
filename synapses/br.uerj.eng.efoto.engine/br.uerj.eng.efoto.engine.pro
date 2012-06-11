#------------------------------------------------
# General
#------------------------------------------------
SYNAPSE_ID = br.uerj.eng.efoto.engine
SYNAPSE_VERSION = 1.0
QT +=
RESOURCES +=
include(../../runtime/synapse.pri)
#------------------------------------------------
# Synapses
#------------------------------------------------


#------------------------------------------------
# Extensions
#------------------------------------------------

INCLUDEPATH += src \
			   src/photogrammetry \
			   src/imageProcessing \
			   src/infrastructure \
			   src/xml_definitions

DEPENDPATH += $${INCLUDEPATH}

HEADERS =  include/IPhotogrammetricEngine.h \
		   #
		   src/PhotogrammetricEngine.h \
		   #
		   src/photogrammetry/DetectorFiducialMark.h \
		   src/photogrammetry/DetectorSpaceCoordinate.h \
		   src/photogrammetry/BundleAdjustment.h \
		   src/photogrammetry/DemGrid.h \
		   src/photogrammetry/ImageFiducialMark.h \
		   src/photogrammetry/ImageSpaceCoordinate.h \
		   src/photogrammetry/EOQuality.h \
		   src/photogrammetry/ExteriorOrientation.h \
		   src/photogrammetry/Flight.h \
		   src/photogrammetry/FrameSensor.h \
		   src/photogrammetry/Image.h \
		   src/photogrammetry/InteriorOrientation.h \
		   src/photogrammetry/IOQuality.h \
		   src/photogrammetry/MatchingPoints.h \
		   src/photogrammetry/MatchingPointsGrid.h \
		   src/photogrammetry/ObjectSpaceCoordinate.h \
		   src/photogrammetry/Orthorectification.h \
		   src/photogrammetry/Point.h \
		   src/photogrammetry/ProjectHeader.h \
		   src/photogrammetry/ProjectiveRay.h \
		   src/photogrammetry/RayTester.h \
		   src/photogrammetry/Sensor.h \
		   src/photogrammetry/SensorWithFiducialMarks.h \
		   src/photogrammetry/SensorWithKnowDimensions.h \
		   src/photogrammetry/SensorWithKnowParameters.h \
		   src/photogrammetry/SpatialIntersection.h \
		   src/photogrammetry/SpatialRessection.h \
		   src/photogrammetry/StereoPair.h \
		   src/photogrammetry/Terrain.h \
		   src/photogrammetry/Dummies.h \
		   src/photogrammetry/Project.h \
		   #
		   src/imageProcessing/ImageMatching.h \
		   src/imageProcessing/Interpolation.h \
		   src/imageProcessing/LeastSquaresMatching.h \
		   src/imageProcessing/NormalizedCrossCorrelation.h \
		   src/imageProcessing/RadiometricTransformation.h \
		   #
		   src/infrastructure/AbstractDetectorCoordinate.h \
		   src/infrastructure/AbstractImageCoordinate.h \
		   src/infrastructure/ConvertionsSystems.h \
		   src/infrastructure/Coordinate.h \
		   src/infrastructure/Dms.h \
		   src/infrastructure/EDom.h \
		   src/infrastructure/EDomValidator.h \
		   src/infrastructure/EObject.h \
		   src/infrastructure/ETreeModel.h \
		   src/infrastructure/GeoSystem.h \
		   src/infrastructure/Matrix.h \
		   src/infrastructure/PositionMatrix.h \
		   src/infrastructure/RectSupport.h \
		   src/infrastructure/SparseMatrix.h \
		   src/infrastructure/TreeFeatures.h \
		   src/infrastructure/Features.h \
		   src/infrastructure/ProgressPublisher.h \
		   #
		   src/xml_definitions/XmlUpdater.h

SOURCES =  src/PhotogrammetricEngine.cpp \
		   #
		   src/photogrammetry/DetectorFiducialMark.cpp \
		   src/photogrammetry/DetectorSpaceCoordinate.cpp \
		   src/photogrammetry/BundleAdjustment.cpp \
		   src/photogrammetry/DemGrid.cpp \
		   src/photogrammetry/ImageFiducialMark.cpp \
		   src/photogrammetry/ImageSpaceCoordinate.cpp \
		   src/photogrammetry/EOQuality.cpp \
		   src/photogrammetry/ExteriorOrientation.cpp \
		   src/photogrammetry/Flight.cpp \
		   src/photogrammetry/FrameSensor.cpp \
		   src/photogrammetry/Image.cpp \
		   src/photogrammetry/InteriorOrientation.cpp \
		   src/photogrammetry/IOQuality.cpp \
		   src/photogrammetry/MatchingPoints.cpp \
		   src/photogrammetry/MatchingPointsGrid.cpp \
		   src/photogrammetry/ObjectSpaceCoordinate.cpp \
		   src/photogrammetry/Orthorectification.cpp \
		   src/photogrammetry/Point.cpp \
		   src/photogrammetry/ProjectHeader.cpp \
		   src/photogrammetry/ProjectiveRay.cpp \
		   src/photogrammetry/RayTester.cpp \
		   src/photogrammetry/Sensor.cpp \
		   src/photogrammetry/SensorWithFiducialMarks.cpp \
		   src/photogrammetry/SensorWithKnowDimensions.cpp \
		   src/photogrammetry/SensorWithKnowParameters.cpp \
		   src/photogrammetry/SpatialIntersection.cpp \
		   src/photogrammetry/SpatialRessection.cpp \
		   src/photogrammetry/StereoPair.cpp \
		   src/photogrammetry/Terrain.cpp \
		   src/photogrammetry/Dummies.cpp \
		   src/photogrammetry/Project.cpp \
		   #
		   src/imageProcessing/ImageMatching.cpp \
		   src/imageProcessing/Interpolation.cpp \
		   src/imageProcessing/LeastSquaresMatching.cpp \
		   src/imageProcessing/NormalizedCrossCorrelation.cpp \
		   src/imageProcessing/RadiometricTransformation.cpp \
		   #
		   src/infrastructure/AbstractDetectorCoordinate.cpp \
		   src/infrastructure/AbstractImageCoordinate.cpp \
		   src/infrastructure/ConvertionsSystems.cpp \
		   src/infrastructure/Coordinate.cpp \
		   src/infrastructure/Dms.cpp \
		   src/infrastructure/EDom.cpp \
		   src/infrastructure/EDomValidator.cpp \
		   src/infrastructure/ETreeModel.cpp \
		   src/infrastructure/GeoSystem.cpp \
		   src/infrastructure/Matrix.cpp \
		   src/infrastructure/PositionMatrix.cpp \
		   src/infrastructure/RectSupport.cpp \
		   src/infrastructure/SparseMatrix.cpp \
		   src/infrastructure/TreeFeatures.cpp \
		   src/infrastructure/Features.cpp \
		   src/infrastructure/ProgressPublisher.cpp \
		   #
		   src/xml_definitions/XmlUpdater.cpp

