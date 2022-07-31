#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
using namespace std;

class X {
private:
    int a;
    std::once_flag connection_init_flag;

    void open_connection() {
        cout << "open_connection\n";
    }

public:
    X() {}
    X(int const& int_) : a(int_) {}
    void send_data(X const& data) // 1
    {
        std::call_once(connection_init_flag, &X::open_connection, this); // 2
    }
    void receive_data() // 3
    {
        std::call_once(connection_init_flag, &X::open_connection, this); // 2
    }
    void m_invoke() {
        std::thread t(&X::receive_data, this);
    }
};

int main(void) {
    X x;
    std::thread t1(&X::receive_data, &x);
    t1.join(); 
}