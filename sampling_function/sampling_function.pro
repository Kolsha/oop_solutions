TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    sampler.cpp \
    basefunction.cpp \
    sqrtfunc.cpp \
    sampler_tests.cpp

HEADERS += \
    sampler.h \
    basefunction.h \
    sqrtfunc.h \
    sampler_tests.h

