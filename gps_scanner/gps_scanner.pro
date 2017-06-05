TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    gpsscanner.cpp \
    utils/tinyxml/tinystr.cpp \
    utils/tinyxml/tinyxml.cpp \
    utils/tinyxml/tinyxmlerror.cpp \
    utils/tinyxml/tinyxmlparser.cpp

HEADERS += \
    gpsscanner.h \
    utils/tinyxml/tinystr.h \
    utils/tinyxml/tinyxml.h
