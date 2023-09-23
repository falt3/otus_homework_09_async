#pragma once

#include <ctime>
#include <string>
#include <fstream>
#include <sstream>

class Log {
    Log() {};
public:
    static void msg(std::time_t time, const std::string &msg);
};

void Log::msg(std::time_t time, const std::string &msg) 
{
    std::ostringstream fileName;
    fileName << "./bulk" << time << ".log";
    std::fstream fs(fileName.str(), std::fstream::app);
    if (fs.is_open()) {
        fs << msg << "\n";
        fs.close();
    }
}
