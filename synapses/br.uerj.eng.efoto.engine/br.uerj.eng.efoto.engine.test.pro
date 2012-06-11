#------------------------------------------------
# General
#------------------------------------------------
include(br.uerj.eng.efoto.engine.pro)
include(../../runtime/test.pri)

#Test specific files
HEADERS = tests/TestProxy.h
SOURCES = tests/TestProxy.cpp

DEPENDPATH += tests
