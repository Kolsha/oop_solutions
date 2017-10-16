TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    cache.cpp

HEADERS += \
    cache.h \
    tests.h

QMAKE_CXXFLAGS += -std=c++0x -pthread
LIBS += -pthread


INCLUDEPATH =\
../../gtest/googletest/include\
../../gtest/googletest

SOURCES += \
    ../../gtest/googletest/src/gtest-all.cc
