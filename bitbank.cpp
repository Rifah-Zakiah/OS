#include <iostream>
#include <thread>
#include <chrono>

int balance = 70; // Initial account balance
int mutex = 1;     // Binary semaphore acting as a mutex
int full = 0;      // Semaphore for BotBeta
int empty = 1;     // Semaphore for BotAlpha

void wait(int &semaphore) {
    while (semaphore <= 0) {
        std::this_thread::yield(); // Busy-wait
    }
    --semaphore;
}

void signal(int &semaphore) {
    ++semaphore;
}

// Function for BotAlpha to deposit money
void BotAlphaDeposit(int amount) {
    for (int i = 0; i < 5; ++i) {
        wait(empty); // Wait for BotAlpha's turn
        wait(mutex); // Lock the balance for modification
        balance += amount;
        std::cout << "BotAlpha deposited " << amount << "\n";
        //std::cout << "Current Balance: " << balance << "\n";
        signal(mutex); // Unlock the balance
        signal(full);  // Notify BotBeta
    }
}

// Function for BotBeta to withdraw money
void BotBetaWithdraw(int amount) {
    for (int i = 0; i < 5; ++i) {
        wait(full);  // Wait for BotBeta's turn
        wait(mutex); // Lock the balance for modification
        balance -= amount;
        std::cout << "BotBeta withdrew " << amount << "\n";
        //std::cout << "Current Balance: " << balance << "\n";
        signal(mutex); // Unlock the balance
        signal(empty); // Notify BotAlpha
    }
}

int main() {
    std::thread botAlpha(BotAlphaDeposit, 50); // BotAlpha deposits 50
    std::thread botBeta(BotBetaWithdraw, 30);  // BotBeta withdraws 30

    botAlpha.join();
    botBeta.join();

    std::cout << "Final Account Balance: " << balance << "\n";
    return 0;
}
