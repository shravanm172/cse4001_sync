// main.cpp
#include <iostream>
#include <cstdlib>

using namespace std;

// These will be defined in problem1.cpp, problem2.cpp, etc.
void run_problem1();
void run_problem2();
void run_problem3();
void run_problem4();

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: cse4001_sync <problem#>" << endl;
        cout << "  1 = No-starve readers–writers" << endl;
        cout << "  2 = Writer-priority readers–writers" << endl;
        cout << "  3 = Dining philosophers solution #1" << endl;
        cout << "  4 = Dining philosophers solution #2" << endl;
        return 1;
    }

    int problem = atoi(argv[1]);

    switch (problem) {
        case 1:
            run_problem1();
            break;
        case 2:
            run_problem2();
            break;
        case 3:
            run_problem3();
            break;
        case 4:
            run_problem4();
            break;
        default:
            cout << "Invalid problem number: " << problem << endl;
            return 1;
    }

    return 0;
}
