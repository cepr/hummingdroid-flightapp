#include "Object.h"

Object::Object()
{
    pthread_mutex_init(&pthread_mutex, NULL);
    pthread_cond_init(&pthread_cond, NULL);
}

Object::~Object()
{
    pthread_mutex_destroy(&pthread_mutex);
    pthread_cond_destroy(&pthread_cond);
}

void Object::notify()
{
    pthread_cond_signal(&pthread_cond);
}

void Object::wait()
{
    pthread_cond_wait(&pthread_cond, &pthread_mutex);
}

Object::Mutex::Mutex(pthread_mutex_t *mutex) : mutex(mutex)
{
    pthread_mutex_lock(mutex);
}

Object::Mutex::~Mutex()
{
    pthread_mutex_unlock(mutex);
}
