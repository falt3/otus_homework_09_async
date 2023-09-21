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

    // std::string ss;
    std::stringstream sout;
    sout << "cmd_1\n" << "cmd_2\n";// << "cmd_3\n";
    // std::cout << sout.str() ;
    // ss.str
    receive((char *)sout.str().data(), sout.str().length(), mas[0]);
    sout.clear();
    sout << "cmd_3";
    receive((char *)sout.str().data(), sout.str().length(), mas[0]);


    std::stringstream sout2;
    sout2 << "cmd_11\n" << "cmd_12\n" << "cmd_13\n";
    receive((char *)sout2.str().data(), sout2.str().length(), mas[0]);


    disconnect(mas[0]);

    std::this_thread::sleep_for(1000ms);
    // std::chrono::
    // receive()
    // receive(nullptr, 0, 1);
    // receive(nullptr, 0, 2);
    // receive(nullptr, 0, 4);

    std::cout << "finish\n";

    return 0;
}