#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;

int SIZE = 5;
vector<int> buffer(SIZE);
int m = 1;  // Mutex
int e = SIZE;  // Empty slots in buffer
int f = 0;  // Filled slots
int in = 0;  // Producer index
int out = 0; // Consumer index
int item = 0;
bool turn = true; // True = Producer's turn, False = Consumer's turn

void wait(int &semaphore) {
    while (semaphore <= 0) {
        this_thread::yield(); // Busy wait
    }
    --semaphore;
}

void signal(int &semaphore) {
    ++semaphore;
}

void prod() {
    for (int i = 0; i < SIZE; i++) {
        while (!turn) this_thread::yield();  // Ensure producer runs first

        wait(e);
        wait(m);
        buffer[in] = ++item;
        cout << "Producer produced item " << item << " at index " << in << ".\n";
        in = (in + 1) % SIZE;
        signal(m);
        signal(f);

        turn = false;  // Hand over control to consumer
        this_thread::sleep_for(chrono::milliseconds(50)); // Small delay for synchronization
    }
}

void cons() {
    for (int i = 0; i < SIZE; i++) {
        while (turn) this_thread::yield();  // Ensure consumer waits for producer

        wait(f);
        wait(m);
        int cons_item = buffer[out];
        cout << "Consumer consumed item " << cons_item << " at index " << out << ".\n";
        out = (out + 1) % SIZE;
        signal(m);
        signal(e);

        turn = true;  // Hand over control back to producer
        this_thread::sleep_for(chrono::milliseconds(50)); // Small delay for synchronization
    }
}

int main() {
    thread producer(prod);
    thread consumer(cons);

    producer.join();
    consumer.join();

    return 0;
}
