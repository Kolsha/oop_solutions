TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    curl_downloader.cpp


LIBS += -lcurl -pthread

HEADERS += \
    curl_downloader.h
