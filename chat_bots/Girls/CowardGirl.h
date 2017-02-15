#ifndef __CowardGirl_h
#define __CowardGirl_h

#include "girl.h"

struct CowardGirl {
    const struct Girl _;
    int rating;
};

extern const void *CowardGirl;

#endif
