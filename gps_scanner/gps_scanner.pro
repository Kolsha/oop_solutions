TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    gpsscanner.cpp \
    utils/tinyxml/tinystr.cpp \
    utils/tinyxml/tinyxml.cpp \
    utils/tinyxml/tinyxmlerror.cpp \
    utils/tinyxml/tinyxmlparser.cpp \
    utils/geopoint.cxx

HEADERS += \
    gpsscanner.h \
    utils/tinyxml/tinystr.h \
    utils/tinyxml/tinyxml.h \
    utils/geopoint.h
