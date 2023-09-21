#pragma once

#include <string>
#include <fstream>
#include <functional>

#include "command.h"
#include "observer.h"


class Interpret : public Publisher {
public:
    using TFunc = std::function<void(ICommand& cmd)>;

    Interpret(int count, int id) : m_count(count), m_id(id) {}
    void execute(std::istream& in);
    int id() {return m_id;}
protected:
    int m_count;
    std::string m_fileName;
    // TFunc m_funcCallBack; 
    int m_id;
private:    
    void run(std::istream &in);
    int block(BlockCommand &cmd, std::istream &in);
    int dinamicBlock(BlockCommand &cmd, std::istream &in);
};

