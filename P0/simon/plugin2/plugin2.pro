CONFIG      += designer plugin debug_and_release
TARGET      = $$qtLibraryTarget(coloraobuttonplugin)
TEMPLATE    = lib

HEADERS     = coloraobuttonplugin.h
SOURCES     = coloraobuttonplugin.cpp
RESOURCES   = icons.qrc
LIBS        += -L. 

target.path = ~/.designer/plugins/designer
INSTALLS    += target

include(coloraobutton.pri)
