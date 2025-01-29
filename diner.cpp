#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

using namespace std;

mutex forks[5];  // Mutex array to represent the forks

void eating(int i) {
    cout << "Philosopher " << i << " is eating.\n";
    this_thread::sleep_for(chrono::milliseconds(100));  // Simulating eating time
}

void thinking(int i) {
    cout << "Philosopher " << i << " is thinking.\n";
    this_thread::sleep_for(chrono::milliseconds(100));  // Simulating thinking time
}

void pf(int i){
    // Philosophers pick up forks in different orders to avoid deadlock
     if (i % 2 == 0) {  
            forks[i].lock();
            cout << "Philosopher " << i << " picks up fork " << i << ".\n";
            forks[(i + 1) % 5].lock();
            cout << "Philosopher " << i << " picks up fork " << (i + 1) % 5 << ".\n";
        } else {  
            forks[(i + 1) % 5].lock();
            cout << "Philosopher " << i << " picks up fork " << (i + 1) % 5 << ".\n";
            forks[i].lock();
            cout << "Philosopher " << i << " picks up fork " << i << ".\n";
        }
        //  while (true) {
        //     if (i % 2 == 0) {  
        //         forks[i].lock();
        //         cout << "Philosopher " << i << " picks up fork " << i << ".\n";

        //         if (forks[(i + 1) % 5].try_lock()) {  
        //             cout << "Philosopher " << i << " picks up fork " << (i + 1) % 5 << ".\n";
        //             break;
        //         } else {  
        //             cout << "Philosopher " << i << " is waiting for fork " << (i + 1) % 5 << ".\n";
        //             forks[i].unlock();
        //             this_thread::sleep_for(chrono::milliseconds(50));  // Wait before retrying
        //         }
        //     } else {  
        //         forks[(i + 1) % 5].lock();
        //         cout << "Philosopher " << i << " picks up fork " << (i + 1) % 5 << ".\n";

        //         if (forks[i].try_lock()) {  
        //             cout << "Philosopher " << i << " picks up fork " << i << ".\n";
        //             break;
        //         } else {  
        //             cout << "Philosopher " << i << " is waiting for fork " << i << ".\n";
        //             forks[(i + 1) % 5].unlock();
        //             this_thread::sleep_for(chrono::milliseconds(50));  // Wait before retrying
        //         }
        //     }
        // }
}
void pd(int i){
    // Releasing forks
    forks[i].unlock();
    cout << "Philosopher " << i << " puts down fork " << i << ".\n";
    forks[(i + 1) % 5].unlock();
    cout << "Philosopher " << i << " puts down fork " << (i + 1) % 5 << ".\n";
}

void philosopher(int i) {
    //for (int j = 0; j < 1; j++) {
        thinking(i);
        pf(i);
        eating(i);
        pd(i);        
   // }
}

int main() {
    vector<thread> philosophers;
    for (int i = 0; i < 5; i++) {
        philosophers.emplace_back(philosopher, i);
    }
    for (auto &p : philosophers) {
        p.join();
    }

    cout << "All philosophers have eaten." << endl;
    return 0;
}
