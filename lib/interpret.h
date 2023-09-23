#pragma once

#include <string>
#include <fstream>
#include <functional>

#include "command.h"
#include "observer.h"


class Interpretator : public Publisher {
public:
    Interpretator(int maxSize_) : maxSize(maxSize_) {
        block = std::make_shared<BlockCommands>();
    }
    virtual void input(std::string& line);

private:
    int dinamicBlock;
    std::shared_ptr<BlockCommands> block;
    int maxSize;
};

