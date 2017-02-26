#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "RichBoy.h"
#include "Utils/new.h"

static void *RichBoy_ctor(void *_self, va_list *app) {
    struct RichBoy *self = ((const struct Class *)User)->ctor(_self, app);

    self->money = va_arg(*app, int);

    return self;
}

static void RichBoy_type_msg(const void * _self)
{

    const struct RichBoy * self = _self;
    char *username = ((const struct User *)(self))->username;
    MsgList *msgs = ((const struct User *)(self))->msgs;

    MsgList *excepted = find_msg(msgs, "I want a dress. Will somebody buy it for me?");

    if(excepted == NULL){
        if(rand() % 2 == 1){
            ((const struct Class *)User)->type_msg(_self);
        }else{
            char *text =
                    build_request("I have a supercar and %d money",
                                  self->money);
            if(text != NULL){
                insert_msg(msgs, username, text);
                free(text);
            }else{
                insert_msg(msgs, username, "I have a supercar");
            }
        }
    }else{
        char *text =
                build_request("%s Of course, baby. I'm the richest boy in the world! My money: %d",
                              excepted->username, self->money);
        if(text != NULL){
            insert_msg(msgs, username, text);
            free(text);
        }else{
            insert_msg(msgs, username, "Of course, baby. I'm the richest boy in the world!");
        }
    }

}

static const struct Class _RichBoy = {
    sizeof(struct RichBoy),
            RichBoy_ctor,
            0,
            RichBoy_type_msg
};

const void *RichBoy = &_RichBoy;
