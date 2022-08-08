#include<iostream>
#include<future>
#include<thread>
using namespace std;

int main(void){
    std::promise<void> p{};
    std::future<void> f = p.get_future();
    std::cout<<std::boolalpha;

    std::cout<<f.valid()<<std::endl;
    p.set_value();
    std::cout<<f.valid()<<std::endl;
    f.get();
    std::cout<<f.valid()<<std::endl;
}