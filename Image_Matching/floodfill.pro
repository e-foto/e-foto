TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

HEADERS += Matrix.h \
	RectSupport.h \
	PositionMatrix.h \
	EDom.h \
	EObject.h \

SOURCES	+= flood_fill_stack_vector.cpp \
	Matrix.cpp \
	RectSupport.cpp \
	PositionMatrix.cpp \
	EDom.cpp \
	EObject.cpp

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}



