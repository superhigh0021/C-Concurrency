#include <iostream>
#include <mutex>
#include <thread>
#include <unistd.h>

std::mutex m;
int i = 1;
std::unique_lock<std::mutex> get_lock() {
    std::unique_lock<std::mutex> lk(m);
    ++i;
    return lk;
}

void process_data() {
    std::unique_lock<std::mutex> lk(get_lock());
    --i;
}

int main(void) {
    std::thread t1(get_lock);
    sleep(1);
    std::cout << i << "\n";
    std::thread t2(process_data);
    sleep(1);
    std::cout << i << "\n";
    t1.join();
    t2.join();
}