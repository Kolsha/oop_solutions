TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += tests_main.cpp \
    lib_fill_img.cpp \
    tests.cpp

HEADERS += \
    lib_fill_img.h \
    tests.h


LIBS += `pkg-config --cflags --libs opencv`


QMAKE_CXXFLAGS += -std=c++0x -pthread
LIBS += -pthread


INCLUDEPATH =\
../../gtest/googletest/include\
../../gtest/googletest

SOURCES += \
    ../../gtest/googletest/src/gtest-all.cc
