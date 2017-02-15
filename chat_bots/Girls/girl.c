#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "girl.h"
#include "Utils/new.h"

static void *Girl_ctor(void *_self, va_list *app) {

    struct Girl *self = ((const struct Class *)User)->ctor(_self, app);

    self->rating = va_arg(*app, int);

    return self;
}

static void Girl_type_msg(const void * _self)
{
    const struct Girl * self = _self;

    char *username = ((const struct User *)(self))->username;
    MsgList *msgs = ((const struct User *)(self))->msgs;

    if(rand() % 2 == 1){

        ((const struct Class *)User)->type_msg(_self);

    }else{

        char *text =
                build_request("I want a dress. Will somebody buy it for me? My stat: %d",
                              self->rating);

        if(text != NULL){

            insert_msg(msgs, username, text);
            free(text);

        }else{

            insert_msg(msgs, username, "I want a dress. Will somebody buy it for me?");

        }

    }

}

static const struct Class _Girl = {
    sizeof(struct Girl),
            Girl_ctor,
            0,
            Girl_type_msg
};

const void *Girl = &_Girl;
