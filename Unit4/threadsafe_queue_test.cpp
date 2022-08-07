#include"threadsafe_queue.h"
#include<vector>
using namespace std;

threadsafe_queue<int>tq{};
vector<int>vec{};

void pushIntoQueue(){
    for(int i = 0; i < 50; ++i){
        tq.push(i);
    }
}

void popFromQueue(){
    int i = 50;
    while(i--){
        vec.push_back(*tq.wait_and_pop());
    }
}

auto main()->int{
    std::thread t1(popFromQueue);
    std::thread t2(pushIntoQueue);

    t1.join();
    t2.join();
    for(auto i : vec){
        cout<<i<<" ";
    }

    return 0;
}