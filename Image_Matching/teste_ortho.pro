TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

HEADERS += Orthorectification.h \
	Matrix.h \
	RectSupport.h \
	PositionMatrix.h \
	EDom.h \
	EObject.h

SOURCES	+= Orthorectification.cpp \
	Matrix.cpp \
	RectSupport.cpp \
	PositionMatrix.cpp \
	EDom.cpp \
	EObject.cpp \
	teste_ortho.cpp

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}



