#ifndef OBJECT_H
#define OBJECT_H

#include <pthread.h>

class Object
{
public:
    Object();
    virtual ~Object();

#define synchronized Mutex synch(&pthread_mutex);
    class Mutex
    {
    public:
        Mutex(pthread_mutex_t* mutex);
        ~Mutex();
    private:
        pthread_mutex_t *mutex;
    };

    void notify();
    void wait();

protected:
    pthread_mutex_t pthread_mutex;
    pthread_cond_t pthread_cond;
};

#endif // OBJECT_H
