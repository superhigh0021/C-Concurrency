#include <functional>
#include <iostream>
#include <type_traits>
#include <utility>

struct Default {
    int foo() const { return 1; }
};

struct NonDefault {
    NonDefault() = delete;
    int foo() const { return 1; }
};

int main() {
    decltype(Default().foo()) n1 = 1; // n1 的类型是 int
    //  decltype(NonDefault().foo()) n2 = n1;               // 错误：无默认构造函数
    decltype(std::declval<NonDefault>().foo()) n2 = n1; // n2 的类型是 int
    std::cout << "n1 = " << n1 << '\n'
              << "n2 = " << n2 << '\n';
    Default d;
    std::__invoke(&Default::foo, &d);
}