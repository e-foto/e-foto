#------------------------------------------------
# General
#------------------------------------------------
include(br.uerj.eng.efoto.app.pro)
include(../../runtime/test.pri)

#Test specific files
HEADERS = tests/TestProxy.h
SOURCES = tests/TestProxy.cpp

DEPENDPATH += tests
