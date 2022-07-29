#include <iostream>
#include <thread>
#include <vector>
using namespace std;

void func(unsigned id) {
    cout << id << " ";
}

int main(void) {
    std::cout << std::thread::hardware_concurrency() << std::endl;

    return 0;
}