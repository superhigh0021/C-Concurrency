#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>
using namespace std;

vector<std::thread> vec{};

void func(int i) {
    if (i == 0) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        vec[1].join();
    } else {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        vec[0].join();
    }
}

int main(void) {
    for (int i = 0; i < 2; ++i) {
        vec.emplace_back(std::thread(func, i));
    }
    
}