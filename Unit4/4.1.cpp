#include<iostream>
#include<thread>
#include<mutex>
#include<queue>
#include<condition_variable>
using namespace std;

std::mutex mut;
std::queue<int>data_queue;
std::condition_variable data_cond;

void data_preparation_thread(){
    static int i = 0;
    while(1){
        const int data = i++;
        {
            std::lock_guard<std::mutex>lk(mut);
            data_queue.push(data);
        }
        data_cond.notify_one();
        if(i == 5)
            break;
    }
}

void data_processing_thread(){
    while(true){
        std::unique_lock<std::mutex>lk(mut);
        data_cond.wait(lk, []{return !data_queue.empty();});
        int data = data_queue.front();
        data_queue.pop();
        lk.unlock();
        if(data == 5)
            break;
    }
}

auto main()->int{
    std::thread t1(data_processing_thread);
    std::thread t2(data_preparation_thread);

    t1.join();
    t2.join();

    return 0;
}