#ifndef __RichBoy_h
#define __RichBoy_h

#include "BaseUser/user.h"

struct RichBoy {
    const struct User _;
    int money;
};

extern const void *RichBoy;

#endif
