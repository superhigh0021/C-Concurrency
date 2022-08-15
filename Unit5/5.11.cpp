#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

std::vector<int> queue_data;
std::atomic<int> count;

void populate_queue() {
    unsigned const number_of_items = 20;
    queue_data.clear();
    for (unsigned i = 0; i < number_of_items; ++i)
        queue_data.push_back(i);
    count.store(number_of_items, std::memory_order_release);//1.最初的存储操作
}

void consume_queue_items() {
    while (true) {
        int item_index;
        //2.一项“读-改-写”操作
        if ((item_index = count.fetch_sub(1, std::memory_order_acquire)) <= 0) {
            ;   //3.等待队列容器装入新数据
            continue;
        }
        //4.从内部容器读取数据项是安全的
        queue_data[item_index - 1];
    }
}

auto main() -> int {
    std::thread a(populate_queue);
    std::thread b(consume_queue_items);
    std::thread c(consume_queue_items);
    a.join();
    b.join();
    c.join();
}
