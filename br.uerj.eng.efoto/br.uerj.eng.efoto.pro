
#------------------------------------------------
# General
#------------------------------------------------
SYNAPSE_ID = br.uerj.eng.efoto
SYNAPSE_VERSION = 1.0.0
QT +=
RESOURCES += qrc/*.qrc
include(../synapse.pri)
FORMS = ui/*.ui
#------------------------------------------------
# Synapses
#------------------------------------------------
INCLUDEPATH += $$synapseIncludePath(br.org.sigdesktop) \
		 $$synapseIncludePath(br.org.te.geo)

QT += opengl
unix {
    INCLUDEPATH += /usr/include/opencv \
        /usr/include/SDL
    LIBS += -lGL \
        -lGLU \
        -lcv \
        -lhighgui \
        -lcvaux \
        -lSDL \
        -lSDL_image
}
win32 {
    INCLUDEPATH += C:/OpenCV2.0/include/opencv
    LIBS += "C:\OpenCV2.0\bin\libcv200.dll" \
        "C:\OpenCV2.0\bin\libcvaux200.dll" \
        "C:\OpenCV2.0\bin\libcxcore200.dll" \
        "C:\OpenCV2.0\bin\libcxts200.dll" \
        "C:\OpenCV2.0\bin\libhighgui200.dll"
}


#------------------------------------------------
# Extensions
#------------------------------------------------

DEPENDPATH += $${INCLUDEPATH}

