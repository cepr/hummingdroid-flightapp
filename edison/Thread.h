#ifndef THREAD_H
#define THREAD_H

class Thread
{
public:
    void start();
    virtual void run() = 0;
};

#endif // THREAD_H
