TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    vk_api.cpp

HEADERS += \
    vk_api.h \
    json.hpp \
    singleton.hpp

LIBS += -lcurl
