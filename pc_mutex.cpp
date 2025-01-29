#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>

using namespace std;

const int SIZE = 5; // Size of the buffer
vector<int> buffer(SIZE); // Shared buffer
int in = 0; // Index for producer
int out = 0; // Index for consumer
int item = 0; // Item to be produced/consumed

mutex mtx;// Mutex for synchronization

// Flags to control the order of execution
bool produced = false; // Indicates if an item has been produced
bool consumed = true; // Indicates if the last item has been consumed

void producer() {
    for (int i = 0; i < SIZE; i++) {
        unique_lock<mutex> lock(mtx);

        // Wait until the consumer has consumed the previous item
        while (!consumed) {
            lock.unlock();
            this_thread::yield(); // Yield to avoid busy waiting
            lock.lock();
        }

        // Produce an item
        buffer[in] = ++item;
        cout << "Producer produced item " << item << " at index " << in << ".\n";
        in = (in + 1) % SIZE;

        // Update flags
        produced = true;
        consumed = false;

        // Simulate production time
        this_thread::sleep_for(chrono::milliseconds(20));
    }
}

void consumer() {
    for (int i = 0; i < SIZE; i++) {
        unique_lock<mutex> lock(mtx);

        // Wait until the producer has produced an item
        while (!produced) {
            lock.unlock();
            this_thread::yield(); // Yield to avoid busy waiting
            lock.lock();
        }

        // Consume the item
        int consumedItem = buffer[out];
        cout << "Consumer consumed item = " << consumedItem << " at index " << out << ".\n";
        out = (out + 1) % SIZE;

        // Update flags
        consumed = true;
        produced = false;

        // Simulate consumption time
        this_thread::sleep_for(chrono::milliseconds(200));
    }
}

int main() {
    // Create producer and consumer threads
    thread producerThread(producer);
    thread consumerThread(consumer);

    // Wait for threads to finish
    producerThread.join();
    consumerThread.join();

    return 0;
}