TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    lib_fill_img.cpp

HEADERS += \
    lib_fill_img.h


LIBS += `pkg-config --cflags --libs opencv`
