#ifndef __spammer_h
#define __spammer_h

#include "BaseUser/user.h"

struct Spammer {
    const struct User _;
    int count;
};

extern const void *Spammer;

#endif
