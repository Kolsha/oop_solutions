#include <stdio.h>
#include <stdarg.h>
#include "boy.h"
#include "Utils/new.h"

static void *Boy_ctor(void *_self, va_list *app) {
    struct Boy *self = ((const struct Class *)User)->ctor(_self, app);
    self->money = va_arg(*app, int);
    return self;
}

static void Boy_type_msg(const void * _self)
{

    const struct Boy * self = _self;
    char *username = ((const struct User *)(self))->username;
    MsgList *msgs = ((const struct User *)(self))->msgs;

    MsgList *excepted = find_msg(msgs, "I want a dress");

    if(excepted == NULL){
        ((const struct Class *)User)->type_msg(_self);
    }else{
        char *text = build_request("U r such a girl! R u afraid of rats, %s?", excepted->username);
        if(text != NULL){
            insert_msg(msgs, username, text);
            free(text);
        }else{
            insert_msg(msgs, username, "U r such a girl! R u afraid of rats?");
        }
    }

}

static const struct Class _Boy = {
    sizeof(struct Boy),
            Boy_ctor,
            0,
            Boy_type_msg
};

    const void *Boy = &_Boy;
