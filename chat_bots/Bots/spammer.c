#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "spammer.h"
#include "Utils/new.h"

static void *Spammer_ctor(void *_self, va_list *app) {

    struct Spammer *self = ((const struct Class *)User)->ctor(_self, app);

    self->count = va_arg(*app, int);

    return self;
}

static void Spammer_type_msg(const void * _self)
{
    const struct Spammer * self = _self;

    char *username = ((const struct User *)(self))->username;
    MsgList *msgs = ((const struct User *)(self))->msgs;

    for(int i = 0; i < self->count; i++){
        if(rand() % 2 == 1){

            insert_msg(msgs, username, "Visit our WebSite http://Kolsha.ru");

        }
    }


}

static const struct Class _Spammer = {
    sizeof(struct Spammer),
            Spammer_ctor,
            0,
            Spammer_type_msg
};

    const void *Spammer = &_Spammer;
