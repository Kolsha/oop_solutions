TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    BaseUser/user.c \
    Girls/girl.c \
    Girls/CowardGirl.c \
    Utils/new.c \
    Utils/msg_list.c \
    Boys/boy.c \
    Boys/RichBoy.c \
    Bots/spammer.c \
    Utils/parser.c


HEADERS += \
    BaseUser/user.h \
    Girls/girl.h \
    Girls/CowardGirl.h \
    Utils/new.h \
    Utils/msg_list.h \
    Boys/boy.h \
    Boys/RichBoy.h \
    Bots/spammer.h \
    Utils/parser.h

QMAKE_CFLAGS += -std=c99
