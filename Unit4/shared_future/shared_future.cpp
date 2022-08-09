#include <chrono>
#include <future>
#include <iostream>
#include <thread>

int main(void) {
    std::promise<void> ready_promise, t1_ready_promise, t2_ready_promise;
    std::shared_future<void> ready_future(ready_promise.get_future());

    std::chrono::time_point<std::chrono::high_resolution_clock> start;

    auto fun1 = [&, ready_future]() -> std::chrono::duration<double, std::milli> {
        t1_ready_promise.set_value();
        ready_future.wait(); // 等待来自 main() 的信号
        return std::chrono::high_resolution_clock::now() - start;
    };

    auto fun2 = [&, ready_future]() -> std::chrono::duration<double, std::milli> {
        t2_ready_promise.set_value();
        ready_future.wait(); // 等待来自 main() 的信号
        return std::chrono::high_resolution_clock::now() - start;
    };

    auto result1 = std::async(std::launch::async, fun1);
    auto result2 = std::async(std::launch::async, fun2);

    //等待线程变为就绪
    t1_ready_promise.get_future().wait();
    t2_ready_promise.get_future().wait();

    //线程已就绪，开始时钟
    start = std::chrono::high_resolution_clock::now();
    ready_promise.set_value();

    std::cout << "Thread 1 received the signal "
              << result1.get().count() << " ms after start\n"
              << "Thread 2 received the signal "
              << result2.get().count() << " ms after start\n";
}