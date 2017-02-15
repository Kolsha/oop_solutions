#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "user.h"
#include "Utils/new.h"
#include "Utils/msg_list.h"

void set_msgs_history(void *_self, MsgList *msgs)
{
    struct User * self = _self;
    self->msgs = msgs;
}

static void *User_ctor(void *_self, va_list *app)
{
    struct User *self = _self;
    self->username = strdup(va_arg(*app, char*));
    assert(self->username != NULL);
    self->msgs = va_arg(*app, void*);
    /*
 */
    assert(self->msgs->username != NULL);
    /*
     */
    return self;
}

static void *User_dtor(void *_self)
{
    struct User *self = _self;
    free(self->username);
}

static void User_type_msg(const void *_self)
{
    const struct User *self = _self;

    char *text = build_request("Hey! I'm %s )", self->username);

    if(text != NULL){

        insert_msg(self->msgs, self->username, text);
        free(text);

    }else{

        insert_msg(self->msgs, self->username, "Hey!");

    }
}

static const struct Class _User = {
    sizeof(struct User),    // size
            User_ctor,      // ctor
            User_dtor,      // dtor
            User_type_msg   // draw
};

const void *User = &_User;
