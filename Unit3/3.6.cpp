#include <iostream>
#include <mutex>
#include <thread>

class A {
private:
    int x;
    std::mutex m;

public:
    A(const int& x_) : x(x_) {}

    friend void swap(A& lhs, A& rhs) {
        if (&lhs == &rhs)
            return;
        std::lock(lhs.m, rhs.m);
        std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);
        std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);
        std::swap(lhs.x, rhs.x);
    }
};
void swap(A& lhs, A& rhs);

int main(void) {
    A x1(1);
    A x2(2);
    std::thread t1(swap, std::ref(x1), std::ref(x2));
    std::thread t2(swap, std::ref(x1), std::ref(x2));
    t1.join();
    t2.join();
}