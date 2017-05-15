TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    trader.cpp \
    broker.cpp \
    stockmarket.cpp

HEADERS += \
    trader.h \
    broker.h \
    stockmarket.h
