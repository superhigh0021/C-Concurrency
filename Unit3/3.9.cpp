#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <vector>
using namespace std;

int* resource_ptr = nullptr;
std::once_flag resource_flag{};
vector<std::thread> vec{};

void init_resource(int i) {
    resource_ptr = new int(i);
}

void foo(int i) {
    std::call_once(resource_flag, init_resource, i);
    cout << *resource_ptr << endl;
}

int main(void) {
    for (int i = 1; i < 8; ++i) {
        vec.emplace_back(std::thread(foo, i));
    }
    for (auto& i : vec) {
        i.join();
    }
    return 0;
}