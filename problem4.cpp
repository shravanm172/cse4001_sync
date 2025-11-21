// Problem 4: Dining Philosophers Solution #2 (asymmetric)

#include <iostream>
#include <pthread.h>
#include <unistd.h>      
#include "semaphore_class.h"

using namespace std;

const int NUM_PHILOSOPHERS_4 = 5;
const int NUM_MEALS_4        = 5;

// Fork semaphores for Problem 4
Semaphore* forks_4[NUM_PHILOSOPHERS_4];

// Helper to compute left/right fork indices.
int left_fork4(int i) {
    return (i + 1) % NUM_PHILOSOPHERS_4;
}

int right_fork4(int i) {
    return i;
}

// Asymmetric fork acquisition:
//   - even philosophers: right then left
//   - odd philosophers:  left then right
void get_forks4(int i) {
    int r = right_fork4(i);
    int l = left_fork4(i);

    if (i % 2 == 0) {
        // Even philosopher: rightie
        forks_4[r]->wait();
        forks_4[l]->wait();
    } else {
        // Odd philosopher: leftie
        forks_4[l]->wait();
        forks_4[r]->wait();
    }
}

// Put down both forks (order doesn't matter for correctness here).
void put_forks4(int i) {
    int r = right_fork4(i);
    int l = left_fork4(i);

    forks_4[r]->signal();
    forks_4[l]->signal();
}

void* philosopher4_thread(void* arg) {
    long id = (long)arg;  

    for (int meal = 0; meal < NUM_MEALS_4; meal++) {
        // Thinking
        cout << "[P4] Philosopher " << (id + 1)
             << ": thinking (meal " << (meal + 1) << ")" << endl;
        sleep(1);

        // Get forks using asymmetric ordering
        get_forks4((int)id);

        // Eating
        cout << "[P4] Philosopher " << (id + 1)
             << ": eating (meal " << (meal + 1) << ")" << endl;
        sleep(1);

        // Put forks down
        put_forks4((int)id);

        cout << "[P4] Philosopher " << (id + 1)
             << ": finished eating (meal " << (meal + 1) << ")" << endl;
    }

    return nullptr;
}

void run_problem4() {
    cout << "=== Problem 4: Dining Philosophers Solution #2 (asymmetric) ===" << endl;
    cout << NUM_PHILOSOPHERS_4 << " philosophers, " << NUM_MEALS_4
         << " meals each." << endl << endl;

    // Allocate fork semaphores, initialized to 1 (available)
    for (int i = 0; i < NUM_PHILOSOPHERS_4; i++) {
        forks_4[i] = new Semaphore(1);
    }

    pthread_t philosophers[NUM_PHILOSOPHERS_4];

    // Create philosopher threads
    for (long i = 0; i < NUM_PHILOSOPHERS_4; i++) {
        pthread_create(&philosophers[i], nullptr, philosopher4_thread, (void*)i);
    }

    // Wait for all philosophers to finish
    for (int i = 0; i < NUM_PHILOSOPHERS_4; i++) {
        pthread_join(philosophers[i], nullptr);
    }

    // Clean up fork semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS_4; i++) {
        delete forks_4[i];
        forks_4[i] = nullptr;
    }

    cout << endl << "=== Problem 4 complete ===" << endl;
}
