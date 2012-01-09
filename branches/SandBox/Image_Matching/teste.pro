TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

HEADERS += Matrix.h \
	RectSupport.h \
	PositionMatrix.h \
	EDom.h \
	EObject.h \
	RadiometricTransformation.h \
	NormalizedCrossCorrelation.h \
	LeastSquaresMatching.h \
	ImageMatching.h \
	MatchingPoints.h \
	MatchingPoints.h \
	AutoSeeds.h \
	DemGrid.h

SOURCES	+= teste_image_matching.cpp \
	Matrix.cpp \
	RectSupport.cpp \
	PositionMatrix.cpp \
	EDom.cpp \
	EObject.cpp \
	RadiometricTransformation.cpp \
	NormalizedCrossCorrelation.cpp \
	LeastSquaresMatching.cpp \
	ImageMatching.cpp \
	MatchingPoints.cpp \
	MatchingPointsGrid.cpp \
	AutoSeeds.cpp \
	DemGrid.cpp

# teste_autoseeds.cpp \
# teste_image_matching.cpp \
# teste_lsm.cpp \
# teste_ncc.cpp \
# teste_radiometric_images.cpp \
# teste_inter_grid.cpp \

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}



