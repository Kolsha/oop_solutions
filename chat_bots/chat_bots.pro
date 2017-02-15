TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    new.c \
    msg_list.c \
    user.c \
    girl.c


HEADERS += \
    new.h \
    msg_list.h \
    user.h \
    girl.h

QMAKE_CFLAGS += -std=c99
