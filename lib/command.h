#pragma once

#include <ctime>
#include <sstream>
#include <memory>
#include <vector>


//*****************************************************************
/**
 * @brief Интерфейс команд
 * 
 */
class ICommand {
public:
    virtual void execute(std::ostream &out) = 0;
    virtual ~ICommand() = default;
    virtual std::time_t getTime() = 0;
};


//*****************************************************************
// Класс команды
class Command : public ICommand {
public:
    Command(std::string &name) { 
        m_name = name; 
        m_time = std::time(nullptr); 
    }
    ~Command() = default;
    void execute(std::ostream &out) override { out << " " << m_name; }
    std::time_t getTime() { return m_time; }
private:
    std::string m_name;
    std::time_t m_time;
};


//*****************************************************************
// Класс блока команд
class BlockCommand: public ICommand {
public:
    BlockCommand() {}
    ~BlockCommand() = default;
    void execute(std::ostream &out) override {
        for (auto it = m_commands.begin(); it < m_commands.end(); ++it) {
            (*it)->execute(out);
            if (it != --m_commands.end())
                out << ",";
        }
    }
    int getCountCommands() { return m_commands.size(); }
    void addCommand(std::unique_ptr<ICommand> cmd) {
        m_commands.emplace_back(std::move(cmd));
    }
    std::time_t getTime() { 
        if (m_commands.size() > 0) 
            return m_commands[0]->getTime();
        else return 0;
    };
private:
    std::vector<std::unique_ptr<ICommand>> m_commands;
};