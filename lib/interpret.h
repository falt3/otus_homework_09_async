#pragma once

#include <string>
#include <fstream>
#include <functional>

#include "command.h"
#include "observer.h"


/**
 * @brief Класс перевода входных данных в блоки команд
 * 
 */
class Interpretator : public Publisher {
public:
    Interpretator(int maxSize_) : maxSize(maxSize_) {
        block = std::make_shared<BlockCommands>();
    }
    virtual void input(std::string& line);

private:
    int dinamicBlock;       ///< уровень вложенности 
    std::shared_ptr<BlockCommands> block;   ///< блок команд
    int maxSize;            ///< максимальный размер блока команд
};

