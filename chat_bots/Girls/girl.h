#ifndef __girl_h
#define __girl_h

#include "BaseUser/user.h"

struct Girl {
    const struct User _;
    int rating;
};

extern const void *Girl;

#endif
