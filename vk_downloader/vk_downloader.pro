TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    ../vk/src/api.cpp \
    ../vk/src/attachment.cpp \
    ../vk/src/messages.cpp \
    ../downloader/curl_downloader.cpp \
    ../oop_solutions/readConfiguration/read_conf.cpp \
    app_logic.cpp


LIBS += -lcurl -pthread

HEADERS += \
    ../vk/src/api.h \
    ../vk/src/attachment.h \
    ../vk/src/json.hpp \
    ../vk/src/messages.h \
    ../downloader/curl_downloader.h \
    ../oop_solutions/readConfiguration/read_conf.h \
    app_logic.h
