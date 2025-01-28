#include <iostream>
#include <thread>
#include<windows.h>
#include <vector>
using namespace std;

int BUFFER_SIZE = 5;
vector<int> buffer(BUFFER_SIZE);
int in = 0;
int out = 0;
int item = 0;

void producerFunc() {
    for (int i = 0; i < 5; i++) {
        buffer[in] = ++item; // item is produced
        cout << "Produced item = " << item << " at index " << in << endl;
        in = (in + 1) % BUFFER_SIZE;
        this_thread::sleep_for(chrono::milliseconds(50));
       // Sleep(2);
    }
}

void consumerFunc() {
    for (int i = 0; i < 5; i++) {
        cout << "Consumed item = " << item << " at index " << out << endl; // consumer consumes the item
        out = (out + 1) % BUFFER_SIZE;
        this_thread::sleep_for(chrono::milliseconds(80));
        //Sleep(10);
    }
}

int main() {
    thread producer(producerFunc);
    thread consumer(consumerFunc);
    producer.join();
    consumer.join();
    return 0;
}