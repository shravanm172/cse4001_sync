// No-starve Readers–Writers Problem
#include <iostream>
#include <pthread.h>
#include <unistd.h>       
#include "semaphore_class.h"

using namespace std;

const int NUM_READERS    = 5;
const int NUM_WRITERS    = 5;
const int NUM_ITERATIONS = 5;

// Protects readCount (the "lightswitch" for readers)
Semaphore mutex_rw(1);

// Indicates whether the room is empty; writers need exclusive access
Semaphore roomEmpty(1);

// Turnstile: forces readers and writers to line up fairly, so that readers cannot starve writers.
Semaphore turnstile(1);

// Number of active readers currently in the critical section
int readCount = 0;


void* reader_thread(void* arg) {
    long id = (long)arg;  

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        sleep(1);

        // First, pass through the turnstile, this prevents new readers from cutting in front of a waiting writer.
        turnstile.wait();
        turnstile.signal();   // Immediately reopen it for others.

        // Now use the "lightswitch" on roomEmpty.
        mutex_rw.wait();
        readCount++;
        if (readCount == 1) {
            // First reader locks the room for readers, blocking writers on roomEmpty.
            roomEmpty.wait();
        }
        mutex_rw.signal();

        // ---- Critical section (reading) ----
        cout << "Reader " << id << ": reading (iteration "
             << (i + 1) << ")" << endl;
        sleep(1); 
        // ----- end of critical section -----

        mutex_rw.wait();
        readCount--;
        if (readCount == 0) {
            // Last reader leaves; room is now empty again, allow writers to enter
            roomEmpty.signal();
        }
        mutex_rw.signal();

        cout << "Reader " << id << ": finished reading (iteration "
             << (i + 1) << ")" << endl;
    }

    return nullptr;
}



void* writer_thread(void* arg) {
    long id = (long)arg;  

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        sleep(2);

        // Writer closes the turnstile so that no new readers can start. 
        // Existing readers that already passed the turnstile are allowed to finish.
        turnstile.wait();

        // Now wait until the room is empty (no readers/writers).
        roomEmpty.wait();

        // ---- Critical section (writing) ----
        cout << "Writer " << id << ": writing (iteration "
             << (i + 1) << ")" << endl;
        sleep(1); 
        // ----- end of critical section -----

        roomEmpty.signal();

        // Re-open the turnstile so readers or writers can proceed
        turnstile.signal();

        cout << "Writer " << id << ": finished writing (iteration "
             << (i + 1) << ")" << endl;
    }

    return nullptr;
}


void run_problem1() {
    cout << "=== Problem 1: No-starve Readers–Writers ===" << endl;
    cout << NUM_READERS << " readers, " << NUM_WRITERS
         << " writers, " << NUM_ITERATIONS
         << " iterations each." << endl << endl;

    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];

    // Create reader threads
    for (long i = 0; i < NUM_READERS; i++) {
        pthread_create(&readers[i], nullptr, reader_thread, (void*)(i + 1));
    }

    // Create writer threads
    for (long i = 0; i < NUM_WRITERS; i++) {
        pthread_create(&writers[i], nullptr, writer_thread, (void*)(i + 1));
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], nullptr);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], nullptr);
    }

    cout << endl << "=== Problem 1 complete ===" << endl;
}
