#include <iostream>
#include "async.h"
#include <vector>
#include <sstream>
#include <chrono>
#include <thread>
// #include <future>

using namespace std::chrono_literals;

int main() {

    std::vector<int> mas;
    for (int i = 0; i < 1; ++i) {
        mas.push_back(connect(3));
    }

    int com = 0;
    for (int i = 0; i < 10; ++i) {
        std::stringstream sout;
        sout << "cmd" << ++com << "\n";
        sout << "cmd" << ++com << "\n";
        sout << "cmd" << ++com << "\n";

        receive((char *)sout.str().data(), sout.str().length(), mas[0]);
    }
    
    std::this_thread::sleep_for(500ms);
    disconnect(mas[0]);

    std::this_thread::sleep_for(500ms);
    // std::chrono::
    // receive()
    // receive(nullptr, 0, 1);
    // receive(nullptr, 0, 2);
    // receive(nullptr, 0, 4);

    std::cout << "finish\n";

    return 0;
}