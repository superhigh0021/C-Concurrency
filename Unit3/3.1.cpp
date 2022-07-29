#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include <unistd.h>
using namespace std;

std::mutex mut1;
std::mutex mut2;

int i = 1;

void func1() {
    mut1.lock();
    sleep(1);
    mut2.lock();
    ++i;
    mut2.unlock();
    mut1.unlock();
}
void func2() {
    mut2.lock();
    sleep(1);
    mut1.lock();
    --i;
    mut1.unlock();
    mut2.unlock();
}

int main(void) {
    std::thread thread1(func1);
    std::thread thread2(func2);
    thread1.join();
    thread2.join();
}
