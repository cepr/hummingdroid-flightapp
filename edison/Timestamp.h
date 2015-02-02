#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <time.h>

class Timestamp {
public:
    // Default constructor
    Timestamp();

    // Copy constructor
    Timestamp(const Timestamp & that);

    // Operators
    void operator=(const Timestamp & that) {
        this->t.tv_sec = that.t.tv_sec;
        this->t.tv_nsec = that.t.tv_nsec;
        this->valid = that.valid;
    }

    static Timestamp now();

    bool valid;
    struct timespec t;
};

float operator -(const Timestamp & a, const Timestamp & b);

#endif // TIMESTAMP_H
