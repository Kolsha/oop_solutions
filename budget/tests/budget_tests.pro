TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    ../src/spendrow.cpp \
    ../src/budget.cpp \
    ../src/utils.cpp

QMAKE_CXXFLAGS += -std=c++0x -pthread
LIBS += -pthread


INCLUDEPATH =\
../../../gtest/googletest/include\
../../../gtest/googletest

SOURCES += \
    ../../../gtest/googletest/src/gtest-all.cc

HEADERS += \
    ../src/spendrow.h \
    ../src/budget.h \
    ../src/utils.h
