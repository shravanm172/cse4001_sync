// Problem 3: Dining Philosophers Solution #1


#include <iostream>
#include <pthread.h>
#include <unistd.h>         
#include "semaphore_class.h"

using namespace std;

const int NUM_PHILOSOPHERS_3 = 5;
const int NUM_MEALS_3        = 5;

// "Footman" multiplex: only 4 philosophers are allowed to try to eat at once.
Semaphore footman(4);

// We use an array of pointers and allocate them once in run_problem3().
Semaphore* forks_3[NUM_PHILOSOPHERS_3];

int left_fork(int i) {
    return (i + 1) % NUM_PHILOSOPHERS_3;
}

int right_fork(int i) {
    return i;
}

void get_forks(int i) {
    // At most 4 philosophers may be "in the room" trying to eat.
    footman.wait();

    int r = right_fork(i);
    int l = left_fork(i);

    // Order: right then left (deadlock is prevented by footman).
    forks_3[r]->wait();
    forks_3[l]->wait();
}

// Put forks back and release seat at table.
void put_forks(int i) {
    int r = right_fork(i);
    int l = left_fork(i);

    forks_3[r]->signal();
    forks_3[l]->signal();

    // Free one spot at the table.
    footman.signal();
}

void* philosopher3_thread(void* arg) {
    long id = (long)arg;  

    for (int meal = 0; meal < NUM_MEALS_3; meal++) {
        // Thinking
        cout << "[P3] Philosopher " << (id + 1)
             << ": thinking (meal " << (meal + 1) << ")" << endl;
        sleep(1);

        // Get forks via footman + fork semaphores
        get_forks((int)id);

        // Eating
        cout << "[P3] Philosopher " << (id + 1)
             << ": eating (meal " << (meal + 1) << ")" << endl;
        sleep(1);

        // Put forks back
        put_forks((int)id);

        cout << "[P3] Philosopher " << (id + 1)
             << ": finished eating (meal " << (meal + 1) << ")" << endl;
    }

    return nullptr;
}

void run_problem3() {
    cout << "=== Problem 3: Dining Philosophers Solution #1 (footman) ===" << endl;
    cout << NUM_PHILOSOPHERS_3 << " philosophers, " << NUM_MEALS_3
         << " meals each." << endl << endl;

    // Allocate the 5 fork semaphores, each initialized to 1 (available).
    for (int i = 0; i < NUM_PHILOSOPHERS_3; i++) {
        forks_3[i] = new Semaphore(1);
    }

    pthread_t philosophers[NUM_PHILOSOPHERS_3];

    // Create philosopher threads
    for (long i = 0; i < NUM_PHILOSOPHERS_3; i++) {
        pthread_create(&philosophers[i], nullptr, philosopher3_thread, (void*)i);
    }

    // Wait for all philosophers to finish
    for (int i = 0; i < NUM_PHILOSOPHERS_3; i++) {
        pthread_join(philosophers[i], nullptr);
    }

    // Clean up fork semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS_3; i++) {
        delete forks_3[i];
        forks_3[i] = nullptr;
    }

    cout << endl << "=== Problem 3 complete ===" << endl;
}
