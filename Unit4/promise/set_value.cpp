#include <algorithm>
#include <cctype>
#include <future>
#include <iostream>
#include <iterator>
#include <sstream>
#include <thread>
#include <vector>

int main() {
    std::istringstream iss_numbers{"3 4 1 42 23 -23 93 2 -289 93"};
    std::istringstream iss_letters{" a 23 b,e a2 k k?a;si,ksa c"};

    std::vector<int> numbers;
    std::vector<char> letters;
    std::promise<void> numbers_promise, letters_promise;

    auto numbers_ready = numbers_promise.get_future();
    auto letter_ready = letters_promise.get_future();

    std::thread value_reader([&] {
        // I/O 操作。
        std::copy(std::istream_iterator<int>{iss_numbers},
                  std::istream_iterator<int>{},
                  std::back_inserter(numbers));

        // 为数字提醒。
        numbers_promise.set_value();

        std::copy_if(std::istream_iterator<char>{iss_letters},
                     std::istream_iterator<char>{},
                     std::back_inserter(letters),
                     ::isalpha);

        // 为字母提醒。
        letters_promise.set_value();
    });

    numbers_ready.wait();

    std::sort(numbers.begin(), numbers.end());

    if (letter_ready.wait_for(std::chrono::seconds(1)) ==
        std::future_status::timeout) {
        // 在获得字母的同时输出数
        for (int num : numbers)
            std::cout << num << ' ';
        numbers.clear(); // Numbers were already printed.
    }

    letter_ready.wait();
    std::sort(letters.begin(), letters.end());

    // 若已打印数，则不做任何事。
    for (int num : numbers)
        std::cout << num << ' ';
    std::cout << '\n';

    for (char let : letters)
        std::cout << let << ' ';
    std::cout << '\n';

    value_reader.join();

    return 0;
}