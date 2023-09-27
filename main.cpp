#include <iostream>
#include "async.h"
#include <vector>
#include <queue>
#include <sstream>
#include <chrono>
#include <thread>
#include <string>
#include <thread>
#include <mutex>

using namespace std::chrono_literals;


namespace ns_test_thread
{
    struct CommandsContext {
        std::queue<std::string> cmds;
        int id_context;
    };     
    struct CommandThread {
        std::string cmd;
        int id_context;
    };

    /**
     * @brief Функция тестирования работы библиотеки
     * 
     * @param countBlocks   количество блоков, которые надо отправить
     * @param sizeBlock     размер блока
     * @param countContext  количество соединений
     * @param countThreads  количество потоков
     */
    void run(int countBlocks, int sizeBlock, int countContext, int countThreads) {
        /// Список соединений со своими очередями команд для выдачи
        std::vector<CommandsContext> commandsContext(countContext);
        /// Список идентификаторов соединений, храним для дисконекта
        std::vector<int> id_contexts(countContext);
        /// общая очередь команд, сформированная из commandsContext
        std::queue<CommandThread> commandsThread;

        srand(time(0));

        // Формирование команд и привязка их к соединению
        int counterCmd = 0;
        for (int i = 0; i < countBlocks; ++i) {
            int n_context = rand() % countContext;
            for (int k = 0; k < sizeBlock; ++k)
                commandsContext[n_context].cmds.push(std::to_string(++counterCmd));
        }
        
        // Установка соединений в основном потоке и получение идентификаторов соединений
        for (int i = 0; i < countContext; ++i) {
            id_contexts[i] = connect(sizeBlock);
            commandsContext[i].id_context = id_contexts[i];
        }
        
        // Формировнаие общей очереди команд для выдачи из очередей, привязанных к соединению
        while (!commandsContext.empty()) {
            int i = rand() % commandsContext.size();
            if (!commandsContext[i].cmds.empty()) {
                commandsThread.push({commandsContext[i].cmds.front(), commandsContext[i].id_context});
                // std::cout << commandsContext[i].cmds.front() << " " << commandsContext[i].id_context << std::endl;
                commandsContext[i].cmds.pop();                
            }
            if (commandsContext[i].cmds.empty())
                commandsContext.erase(commandsContext.begin() + i);
        }

        
        // функция потока
        std::mutex  mutex;
        auto ff_worker = [&mutex, &commandsThread](int /*id*/) {
            while (true) {
                CommandThread el;
                {
                    std::lock_guard<std::mutex> lock(mutex);
                    if (commandsThread.empty())     // выход из потока если очередь пустая
                        break;

                    el = commandsThread.front();
                    commandsThread.pop();
                    // std::cout << /*id << */": receive: " << " " << el.cmd << " " << el.id_context << std::endl;
                }                
                receive(el.cmd.data(), el.cmd.size(), el.id_context);
            }
        };

        // создание потоков
        std::vector<std::thread> threads;
        for (int i = 0; i < countThreads; ++i) {
            threads.emplace_back(ff_worker, i);
        }

        // ожидание окончания выполнения потоков
        for (int i = 0; i < countThreads; ++i)
            threads[i].join();


        // Завершение соединений в основном потоке
        for (std::size_t i = 0; i < id_contexts.size(); ++i)
            disconnect(id_contexts[i]);

    }
} // namespace ns_test_thread

//----------------------------------------------------------------------------

namespace ns_test1 {
    void run() {
        std::vector<int> mas;
        for (int i = 0; i < 1; ++i) {
            mas.push_back(connect(3));
        }

        int com = 0;
        for (int i = 0; i < 1; ++i) {
            std::stringstream sout;
            sout << "cmd" << ++com << "\n";
            sout << "cmd" << ++com << "\n";
            sout << "{\n"; 
            sout << "cmd" << ++com << "\n";
            sout << "}\n";
            sout << "cmd" << ++com << "\n";
            sout << "{\n";
            sout << "cmd" << ++com << "\n";
            sout << "cmd" << ++com << "\n";

            receive((char *)sout.str().data(), sout.str().length(), mas[0]);
        }
        
        disconnect(mas[0]);

        std::cout << "finish\n";
    }
}

//----------------------------------------------------------------------------

int main() {
    // ns_test1::run();
    ns_test_thread::run(30, 5, 4, 10);

    return 0;
}