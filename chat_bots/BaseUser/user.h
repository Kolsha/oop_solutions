#ifndef __user_h
#define __user_h

#include "Utils/msg_list.h"

struct User {
    const void *class;
    MsgList *msgs;
    char *username;
};

void set_msgs_history(void *_self, MsgList *msgs);

extern const void *User;

#endif
