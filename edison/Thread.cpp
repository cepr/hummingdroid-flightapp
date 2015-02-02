#include "Thread.h"
#include "pthread.h"

static void *start_routine (void *obj)
{
    static_cast<Thread*>(obj)->run();
    return 0;
}

void Thread::start()
{
    pthread_t thread;
    pthread_create(&thread, NULL, start_routine, this);
}
