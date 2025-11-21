// Problem 2: Writer-priority Readers–Writers

#include <iostream>
#include <pthread.h>
#include <unistd.h>        
#include "semaphore_class.h"

using namespace std;

const int NUM_READERS_2    = 5;
const int NUM_WRITERS_2    = 5;
const int NUM_ITERATIONS_2 = 5;


// noReaders: if 1, readers are allowed to try to enter. writers grab it (via writeSwitch) to block new readers.
Semaphore noReaders(1);

// noWriters: if 1, writers are allowed to enter / resource is freereaders grab it (via readSwitch) to block writers while readers are inside; 
// writers wait directly on it.
Semaphore noWriters(1);

// Lightswitch for readers: controls noWriters
int readSwitchCount = 0;
Semaphore readSwitchMutex(1);

// Lightswitch for writers: controls noReaders
int writeSwitchCount = 0;
Semaphore writeSwitchMutex(1);


// readSwitch.lock(noWriters)
void readSwitch_lock() {
    readSwitchMutex.wait();
    readSwitchCount++;
    if (readSwitchCount == 1) {
        // first reader locks out writers
        noWriters.wait();
    }
    readSwitchMutex.signal();
}

// readSwitch.unlock(noWriters)
void readSwitch_unlock() {
    readSwitchMutex.wait();
    readSwitchCount--;
    if (readSwitchCount == 0) {
        // last reader allows writers again
        noWriters.signal();
    }
    readSwitchMutex.signal();
}

// writeSwitch.lock(noReaders)
void writeSwitch_lock() {
    writeSwitchMutex.wait();
    writeSwitchCount++;
    if (writeSwitchCount == 1) {
        // first writer blocks new readers
        noReaders.wait();
    }
    writeSwitchMutex.signal();
}

// writeSwitch.unlock(noReaders)
void writeSwitch_unlock() {
    writeSwitchMutex.wait();
    writeSwitchCount--;
    if (writeSwitchCount == 0) {
        // last writer allows readers again
        noReaders.signal();
    }
    writeSwitchMutex.signal();
}

void* reader2_thread(void* arg) {
    long id = (long)arg;
    for (int i = 0; i < NUM_ITERATIONS_2; i++) {
        sleep(1);
        // If a writer has grabbed noReaders, we block here.
        noReaders.wait();

        // Lightswitch on noWriters: first reader blocks writers.
        readSwitch_lock();

        // Allow other readers/writers to proceed to their respective logic.
        noReaders.signal();

        // ---- critical section ----
        cout << "[P2] Reader " << id << ": reading (iteration "
             << (i + 1) << ")" << endl;
        sleep(1);
        // ---- exit ----
        readSwitch_unlock();

        cout << "[P2] Reader " << id << ": finished reading (iteration "
             << (i + 1) << ")" << endl;
    }

    return nullptr;
}


void* writer2_thread(void* arg) {
    long id = (long)arg;

    for (int i = 0; i < NUM_ITERATIONS_2; i++) {
        sleep(2);
        // Lightswitch on noReaders: first writer blocks new readers.
        writeSwitch_lock();

        // Only one writer at a time in the critical section.
        noWriters.wait();

        // ---- critical section ----
        cout << "[P2] Writer " << id << ": writing (iteration "
             << (i + 1) << ")" << endl;
        sleep(1);
        // ---- exit ----
        noWriters.signal();
        writeSwitch_unlock();

        cout << "[P2] Writer " << id << ": finished writing (iteration "
             << (i + 1) << ")" << endl;
    }

    return nullptr;
}


void run_problem2() {
    cout << "=== Problem 2: Writer-priority Readers–Writers ===" << endl;
    cout << NUM_READERS_2 << " readers, " << NUM_WRITERS_2
         << " writers, " << NUM_ITERATIONS_2
         << " iterations each." << endl << endl;

    pthread_t readers[NUM_READERS_2];
    pthread_t writers[NUM_WRITERS_2];

    // Create reader threads
    for (long i = 0; i < NUM_READERS_2; i++) {
        pthread_create(&readers[i], nullptr, reader2_thread, (void*)(i + 1));
    }

    // Create writer threads
    for (long i = 0; i < NUM_WRITERS_2; i++) {
        pthread_create(&writers[i], nullptr, writer2_thread, (void*)(i + 1));
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_READERS_2; i++) {
        pthread_join(readers[i], nullptr);
    }
    for (int i = 0; i < NUM_WRITERS_2; i++) {
        pthread_join(writers[i], nullptr);
    }

    cout << endl << "=== Problem 2 complete ===" << endl;
}
