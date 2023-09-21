#include "interpret.h"
#include <iostream>



void Interpret::execute(std::istream& in) 
{
    run(in);
}


/**
 * @brief func run
 * 
 * @param in 
 * 
 * 
 */
void Interpret::run(std::istream &in)
{
    int res = 0;
    while (true) {
        auto cmd = std::shared_ptr<BlockCommand>(new BlockCommand());
        if (res == 0) 
            res = block(*cmd, in);
        else if (res == 1) // обычный блок закончился началом динамического "{"
            res = dinamicBlock(*cmd, in);

        if (res >= 0) {
            // std::cout << "Interpret: callback\n";
            notify(cmd);
            //m_funcCallBack(cmd);
            if (res == 2)
                break;
        }
        else {
            break;
        }
    }
}


/**
 * @brief 
 * 
 * @param cmd 
 * @param in 
 * @return int 
 */
int Interpret::block(BlockCommand &cmd, std::istream &in) 
{
    std::string line;   
    while (std::getline(in, line) && !line.empty()) {
        if (line == "{") {
            if (cmd.getCountCommands() == 0) //
                return dinamicBlock(cmd, in);
            else {
                return 1;   // обработка блока и начало нового динамического
            }
        }
        else if (line == "}") { // ошибка ввода
            if (cmd.getCountCommands() == 0) return -1;    
            else return 2;  // если уже есть команды, обрабатываем их и выходим
        }
        else {
            cmd.addCommand(std::unique_ptr<ICommand>(new Command(line)));
            if (m_count == cmd.getCountCommands())
                return 0;
        }   
    }
    if (cmd.getCountCommands() == 0) return -1;
    else return 0;
}

int Interpret::dinamicBlock(BlockCommand &cmd, std::istream &in) 
{
    std::string line;    
    while (std::getline(in, line) && !line.empty()) {
        if (line == "{") {
            BlockCommand *cmd2 = new BlockCommand();
            dinamicBlock(*cmd2, in);
            if (cmd2->getCountCommands() > 0)
                cmd.addCommand(std::unique_ptr<ICommand>(cmd2));
        }
        else if (line == "}") {
            return 0;
        }
        else {
            cmd.addCommand(std::unique_ptr<ICommand>(new Command(line)));
        }
    }
    return -1;
}