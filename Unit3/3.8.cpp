#include <iostream>
#include <mutex>
#include <thread>

std::mutex m;
std::unique_lock<std::mutex> x;
int main(void) {
    std::cout << x.owns_lock() << std::endl;
}