#include "Timestamp.h"

Timestamp::Timestamp() :
    valid(false)
{
    t.tv_sec = 0;
    t.tv_nsec = 0;
}

Timestamp::Timestamp(const Timestamp &that) :
    valid(that.valid)
{
    t.tv_sec = that.t.tv_sec;
    t.tv_nsec = that.t.tv_nsec;
}

Timestamp Timestamp::now()
{
    Timestamp t;
    clock_gettime(CLOCK_MONOTONIC_RAW, &t.t);
    t.valid = true;
    return t;
}

float operator -(const Timestamp & a, const Timestamp & b) {
    return (float)(a.t.tv_sec - b.t.tv_sec) +
            ((float)(a.t.tv_nsec - b.t.tv_nsec)) / 1.e9;
}
