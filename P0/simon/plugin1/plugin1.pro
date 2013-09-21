CONFIG      += designer plugin release
TARGET      = $$qtLibraryTarget(bluelabelplugin)
TEMPLATE    = lib

HEADERS     = bluelabelplugin.h
SOURCES     = bluelabelplugin.cpp
RESOURCES   = icons.qrc
LIBS        += -L. 

target.path = ~/.designer/plugins/designer
INSTALLS    += target

include(bluelabel.pri)
