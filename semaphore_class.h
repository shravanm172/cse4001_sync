// semaphore_class.h
#ifndef SEMAPHORE_CLASS_H
#define SEMAPHORE_CLASS_H

#include <semaphore.h>

class Semaphore {
public:
    // Constructor
    Semaphore(int initialValue) {
        sem_init(&mSemaphore, 0, initialValue);
    }

    // Destructor
    ~Semaphore() {
        sem_destroy(&mSemaphore);
    }

    // wait
    void wait() {
        sem_wait(&mSemaphore);
    }

    // signal
    void signal() {
        sem_post(&mSemaphore);
    }

private:
    sem_t mSemaphore;
};

#endif
