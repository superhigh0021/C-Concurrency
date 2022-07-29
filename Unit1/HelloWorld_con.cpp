#include <iostream>
#include <thread>

void hello() {
    std::cout << "Hello World" << std::endl;
}

int main(int argc, char** argv) {
    std::thread t(hello);
    t.join();
}