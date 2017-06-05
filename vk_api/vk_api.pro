TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    vk_api.cpp \
    vk_messages.cpp \
    vk_attachment.cpp

HEADERS += \
    vk_api.h \
    json.hpp \
    singleton.hpp \
    vk_messages.h \
    vk_attachment.h

LIBS += -lcurl
