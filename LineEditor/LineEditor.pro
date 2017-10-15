TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    cmd_parser.cpp

HEADERS += \
    line_editor.h \
    cmd_parser.h
