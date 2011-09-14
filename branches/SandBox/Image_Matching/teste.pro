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
	AutoSeeds.h

SOURCES	+= teste_autoseeds.cpp \
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
	AutoSeeds.cpp

# teste_autoseeds.cpp \
# teste_image_matching.cpp \
# teste_lsm.cpp \
# teste_ncc.cpp \
# teste_radiometric_images.cpp \

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}



