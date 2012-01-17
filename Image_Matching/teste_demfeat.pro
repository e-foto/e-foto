TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

HEADERS += Matrix.h \
	RectSupport.h \
	PositionMatrix.h \
	EDom.h \
	EObject.h \
	MatchingPoints.h \
	DemFeatures.h

SOURCES	+= test_demfeat.cpp \
	Matrix.cpp \
	RectSupport.cpp \
	PositionMatrix.cpp \
	EDom.cpp \
	EObject.cpp \
	MatchingPoints.cpp \
	DemFeatures.cpp

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}



