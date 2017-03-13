TEMPLATE = app
CONFIG += console c++11 -pthread
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++0x -pthread
LIBS += -pthread


INCLUDEPATH =\
../../gtest/googletest/include\
../../gtest/googletest

SOURCES += main.cpp \
    ../../gtest/googletest/src/gtest-all.cc
