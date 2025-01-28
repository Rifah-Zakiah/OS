#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>

std::atomic<int> semaphores[5] = {1, 1, 1, 1, 1}; // One semaphore for each fork, initialized to 1

void wait(std::atomic<int> &semaphore, int philosopher_id, int fork_id) {
    while (semaphore <= 0) {
        std::this_thread::yield(); // Busy wait
    }
    --semaphore;
}

void signal(std::atomic<int> &semaphore) {
    ++semaphore;
}

void think(int id) {
    std::cout << "Philosopher " << id << " is thinking.\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
}

void eat(int id) {
    std::cout << "Philosopher " << id << " is eating.\n" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
}
void pick_up_forks(int i) {
    if (i % 2 == 0) {
        wait(semaphores[i], i, i); // Pick up the left fork first
        std::cout << "Philosopher " << i << " picked up fork " << i << "." << std::endl;
        if (semaphores[(i + 1) % 5] <= 0) {
            std::cout << "Philosopher " << i << " is waiting for fork " << (i + 1) % 5 << "." << std::endl << std::endl;
        }
        wait(semaphores[(i + 1) % 5], i, (i + 1) % 5); // Pick up the right fork
        std::cout << "Philosopher " << i << " picked up fork " << (i + 1) % 5 << "." << std::endl;
    } else {
        wait(semaphores[(i + 1) % 5], i, (i + 1) % 5); // Pick up the right fork first
        std::cout << "Philosopher " << i << " picked up fork " << (i + 1) % 5 << "." << std::endl;
        if (semaphores[i] <= 0) {
            std::cout << "Philosopher " << i << " is waiting for fork " << i << "." << std::endl << std::endl;
        }
        wait(semaphores[i], i, i); // Pick up the left fork
        std::cout << "Philosopher " << i << " picked up fork " << i << "." << std::endl;
    }
}

void put_down_forks(int i) {
    signal(semaphores[i]); // Release the left fork
    std::cout << "Philosopher " << i << " put down fork " << i << ".\n";
    signal(semaphores[(i + 1) % 5]); // Release the right fork
    std::cout << "Philosopher " << i << " put down fork " << (i + 1) % 5 << ".\n";
}

void philosopher(int id) {
    for (int j = 0; j < 3; ++j) { // Each philosopher alternates between thinking and eating 3 times
        think(id);
        pick_up_forks(id);
        eat(id);
        put_down_forks(id);
    }
}

int main() {
    std::vector<std::thread> philosophers;
    for (int i = 0; i < 5; ++i) {
        philosophers.emplace_back(philosopher, i);
    }

    for (auto &p : philosophers) {
        p.join();
    }

    std::cout << "All philosophers have finished their meals.\n";
    return 0;
}
