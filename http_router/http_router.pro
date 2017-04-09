TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    my_router.cpp \
    route_tests.cpp

HEADERS += \
    my_router.h \
    route_tests.h


QMAKE_CXXFLAGS += -std=c++0x -pthread
LIBS += -pthread


INCLUDEPATH =\
../../gtest/googletest/include\
../../gtest/googletest

SOURCES += \
    ../../gtest/googletest/src/gtest-all.cc
