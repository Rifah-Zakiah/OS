#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>

using namespace std;

enum State { THINKING, HUNGRY, EATING };

class DiningPhilosophers {
private:
    int N; // Number of philosophers
    vector<State> state; // State of each philosopher
    vector<bool> forks; // Availability of forks
    mutex mtx; // Monitor lock
    vector<condition_variable> cv; // Condition variables for each philosopher
    vector<bool> has_thought; // Track whether each philosopher has thought at least once
    vector<bool> has_eaten; // Track whether each philosopher has eaten at least once

    void test(int philosopher) {
        if (state[philosopher] == HUNGRY && 
            forks[philosopher] && 
            forks[(philosopher + 1) % N]) {

            // Allow the philosopher to eat
            state[philosopher] = EATING;
            forks[philosopher] = false;
            forks[(philosopher + 1) % N] = false;
            cv[philosopher].notify_one();
        }
    }

public:
    DiningPhilosophers(int n) : N(n), state(n, THINKING), forks(n, true), cv(n), has_thought(n, false), has_eaten(n, false) {}

    void pickup_forks(int philosopher) {
        unique_lock<mutex> lock(mtx);
        state[philosopher] = HUNGRY;
        cout << "Philosopher " << philosopher << " is HUNGRY" << endl;
        test(philosopher);
        while (state[philosopher] != EATING) {
            cv[philosopher].wait(lock);
        }
        cout << "Philosopher " << philosopher << " picked up forks " << philosopher << " and " << (philosopher + 1) % N << endl;
        cout << "Philosopher " << philosopher << " is EATING" << endl;
    }

    void putdown_forks(int philosopher) {
        unique_lock<mutex> lock(mtx);
        state[philosopher] = THINKING;
        forks[philosopher] = true;
        forks[(philosopher + 1) % N] = true;
        has_eaten[philosopher] = true;
        cout << "Philosopher " << philosopher << " put down forks " << philosopher << " and " << (philosopher + 1) % N << endl;
        cout << "Philosopher " << philosopher << " is THINKING" << endl;
        has_thought[philosopher] = true;

        // Check if all philosophers have eaten and thought
        if (all_of(has_eaten.begin(), has_eaten.end(), [](bool eaten) { return eaten; }) &&
            all_of(has_thought.begin(), has_thought.end(), [](bool thought) { return thought; })) {
            //cout << "All philosophers have gone through THINKING, HUNGRY, EATING, and back to THINKING at least once. Exiting..." << endl;
            exit(0);
        }

        test((philosopher + N - 1) % N); // Check left neighbor
        test((philosopher + 1) % N); // Check right neighbor
    }

    void philosopher(int philosopher) {
        while (true) {
            cout << "Philosopher " << philosopher << " is THINKING" << endl;
            this_thread::sleep_for(chrono::milliseconds(1000)); // Simulate THINKING
            pickup_forks(philosopher);
            this_thread::sleep_for(chrono::milliseconds(1000)); // Simulate EATING
            putdown_forks(philosopher);
        }
    }
};

int main() {
    const int N = 5; // Number of philosophers
    DiningPhilosophers dp(N);
    vector<thread> philosophers;

    // Start philosopher threads
    for (int i = 0; i < N; ++i) {
        philosophers.emplace_back(&DiningPhilosophers::philosopher, &dp, i);
    }

    // Join threads (will exit when all philosophers are done)
    for (auto &t : philosophers) {
        t.join();
    }

    return 0;
}
