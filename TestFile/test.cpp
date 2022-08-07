#include<future>
#include<iostream>
#include<thread>
using namespace std;

int find_answer_to_ltuae(){
    return 100;
}
void do_other_stuff(){
    std::cout<<"do_other_stuff ing......"<<std::endl;
}

class Widget{
public:
    static int print(){
        cout<<"Widget::print()!"<<std::endl;
        return 10;
    }
    int a;
};


auto main()->int{
    Widget w;
    std::future f = std::async(&Widget::print);
}