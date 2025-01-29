#include <iostream>
#include <thread>
#include <vector>
#include <semaphore>
#include <chrono>

using namespace std;

const int BUFFER_SIZE = 5; // Size of the buffer
vector<int> buffer(BUFFER_SIZE); // Shared buffer
int in = 0; // Index for producer
int out = 0; // Index for consumer
int item = 0; // Item to be produced/consumed
// Semaphores for synchronization
counting_semaphore<1> emptySlots(BUFFER_SIZE); // Tracks empty slots in the buffer
counting_semaphore<1> fullSlots(0); // Tracks filled slots in the buffer
mutex mtx; // Mutex to protect critical sections

void producerFunc() {
    for (int i = 0; i < 5; i++) {
        emptySlots.acquire();// Wait for an empty slot in the buffer
        mtx.lock();// Critical section: Add item to the buffer

        buffer[in] = ++item; // Produce an item
        cout << "Produced item = " << item << " at index " << in << endl;
        in = (in + 1) % BUFFER_SIZE; // Update the index

        mtx.unlock();
        fullSlots.release();// Signal that a slot is now full

        this_thread::sleep_for(chrono::milliseconds(50)); // Simulate production time
    }
}

void consumerFunc() {
    for (int i = 0; i < 5; i++) {
        fullSlots.acquire(); // Wait for a filled slot in the buffer
        mtx.lock();// Critical section: Consume item from the buffer

        int consumedItem = buffer[out]; // Consume an item
        cout << "Consumed item = " << consumedItem << " at index " << out << endl;
        out = (out + 1) % BUFFER_SIZE; // Update the index
        
        mtx.unlock();
        emptySlots.release(); // Signal that a slot is now empty

        this_thread::sleep_for(chrono::milliseconds(80));// Simulate consumption time
    }
}

int main() {
    // Create producer and consumer threads
    thread producer(producerFunc);
    thread consumer(consumerFunc);

    // Wait for threads to finish
    producer.join();
    consumer.join();

    return 0;
}