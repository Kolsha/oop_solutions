#include <stdio.h>
#include <stdarg.h>
#include "girl.h"
#include "new.h"

static void *Girl_ctor(void *_self, va_list *app) {
    struct Girl *self = ((const struct Class *)User)->ctor(_self, app);
    self->rating = va_arg(*app, int);
    return self;
}

static void Girl_type_msg(const void * _self)
{

    const struct Circle * self = _self;
    char *username = ((const struct User *)(self))->username;
    MsgList *msgs = ((const struct User *)(self))->msgs;

    if(rand() % 2 == 1){
        //insert_msg(msgs, username, "Hey");
        ((const struct Class *)User)->type_msg(_self);
    }else{
        insert_msg(msgs, username, "I want a dress. Will somebody buy it for me?");
    }

}

static const struct Class _Girl = {
    sizeof(struct Girl),      // size
            Girl_ctor,                // ctor
            0,                          // dtor
            Girl_type_msg                 // draw
};

    const void *Girl = &_Girl;
