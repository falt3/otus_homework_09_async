#include <iostream>
#include <algorithm>
#include <functional>

#include "async.h"
#include "pool.h"


/**
 * @brief Класс контекста подключения к библиотеке
 * 
 */
class Context : public Interpretator {
public:
    Context(int maxSize, int id) : Interpretator(maxSize), m_id(id) {}
    int id() { return m_id; }
    void input(std::string& line) override {
        std::lock_guard<std::mutex> lock(m_mutex);
        Interpretator::input(line);
    }
private:    
    int m_id;               ///< идентификатор контекста
    std::mutex m_mutex;     ///< мьютек для синхронизации доступа к формированию блока команд
};


std::vector<std::unique_ptr<Context>> g_context;
std::shared_ptr<PoolThread_> g_poolThreadFile;
std::shared_ptr<PoolThread_> g_poolThreadConsole;

//----------------------------------------------------------------------------
/**
 * @brief Функция вывода в консоль
 * 
 */
void ff_console(std::shared_ptr<BlockCommands>& block, int /*id*/) 
{
    std::cout << "bulk: " << block->print() << "\n";    
};


/**
 * @brief Функция вывода в файл
 * 
 */
void ff_file(std::shared_ptr<BlockCommands>& block, int id) 
{
    std::ostringstream fileName;
    fileName << "./bulk" << block->time() << "_" << id << ".log";
    std::fstream fs(fileName.str(), std::fstream::app);
    if (fs.is_open()) {
        fs << "bulk: " << block->print() << "\n";
        fs.close();
    }
};

//----------------------------------------------------------------------------

void libInitilize() 
{
    // создается пул потоков для записи в консоль
    g_poolThreadConsole = std::shared_ptr<PoolThread_>(new PoolThread_(1, ff_console));
    // создается пул потоков для записи в файл
    g_poolThreadFile = std::shared_ptr<PoolThread_>(new PoolThread_(2, ff_file));
}


void libRelease() 
{
    g_poolThreadConsole->stop();
    g_poolThreadFile->stop();
}

//----------------------------------------------------------------------------

/**
 * @brief 
 * 
 * @param [in] size     размер блока команд
 * @return [out] int    идентификатор контекста
 */
int connect(int size) 
{
    static int incId = 0;
    if (g_context.empty())
        libInitilize();            ///< создаются пулы потоков с очередями заданий

    std::unique_ptr<Context> context(new Context(size, ++incId));   ///< Создаем новый контекс входа

    context->addSubscriber(g_poolThreadConsole);    ///< подписываем пул на получение заданий из контекста
    context->addSubscriber(g_poolThreadFile);
    g_context.emplace_back(std::move(context));
    
    return incId;
}


/**
 * @brief 
 * 
 * @param data 
 * @param len 
 * @param id 
 */
void receive(char* data, std::size_t len, int id) 
{
    auto it = std::find_if(g_context.begin(), g_context.end(), [id](auto& el) {
        return id == el->id();        
    });

    if (it != g_context.end()) {
        std::istringstream sin({data, len});
        std::string line;
        while (std::getline(sin, line) && !line.empty()) {
            (*it)->input(line);        
        }
    }
}


/**
 * @brief 
 * 
 * @param id 
 */
void disconnect(int id) 
{
    // std::cout << "disconnect: " << id << std::endl;
     auto it = std::find_if(g_context.begin(), g_context.end(), [id](auto& el) {
        return id == el->id();        
    });

    if (it != g_context.end()) {
        g_context.erase(it);
    }   

    if (g_context.empty())
        libRelease();
}