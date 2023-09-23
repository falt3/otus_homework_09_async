#pragma once

#include <ctime>
#include <sstream>
#include <memory>
#include <vector>


struct BlockCommands {
public:
    std::string print() {
        std::string str;
        for (auto it = commands.begin(); it < commands.end(); ++it) {
            str += *it;

            if (it != commands.end() - 1)
                str += ", ";
        } 
        return str;       
    }
    void addCommand(std::string& str) {
        commands.push_back(std::move(str));
        if (commands.size() == 1)
            m_time = std::time(nullptr); 
    }
    int size() {return commands.size();}
    std::time_t time() { return m_time; };

private:
    std::vector<std::string> commands;
    std::time_t m_time;
};
