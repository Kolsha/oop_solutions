#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "CowardGirl.h"
#include "Utils/new.h"

static void *CowardGirl_ctor(void *_self, va_list *app) {

    struct CowardGirl *self = ((const struct Class *)Girl)->ctor(_self, app);

    return self;
}

static void CowardGirl_type_msg(const void * _self)
{
    const struct CowardGirl * self = _self;

    char *username = ((const struct User *)(self))->username;
    MsgList *msgs = ((const struct User *)(self))->msgs;

    MsgList *excepted = find_msg(msgs, "rat");

    if(excepted == NULL){

        ((const struct Class *)Girl)->type_msg(_self);

    }else{

        char *text = build_request("AAAAaaa! No! No rats here, pls %s", excepted->username);
        if(text != NULL){

            insert_msg(msgs, username, text);
            free(text);

        }else{

            insert_msg(msgs, username, "AAAAaaa! No! No rats here, pls");

        }

    }

}

static const struct Class _CowardGirl = {
    sizeof(struct CowardGirl),
            CowardGirl_ctor,
            0,
            CowardGirl_type_msg
};

const void *CowardGirl = &_CowardGirl;
