#include<future>
#include<iostream>
using namespace std;

int find_answer_to_ltuae(){
    return 100;
}
void do_other_stuff(){
    std::cout<<"do_other_stuff ing......"<<std::endl;
}


auto main()->int{
    std::future<int> the_answer = std::async(find_answer_to_ltuae);
    do_other_stuff();
    std::cout<<"The answer is: "<<the_answer.get()<<std::endl;
}