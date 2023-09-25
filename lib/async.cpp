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
    /**
     * @brief Конструктор
     * @param [in] maxSize  размер блока команд
     * @param [in] id       идентификтаор соединения
     */
    Context(int maxSize, int id) : Interpretator(maxSize), m_id(id) {}
    /**
     * @brief Функция возврата идентификатора соединения
     * @return int          идентификатор соединения
     */
    int id() { return m_id; }
    /**
     * @brief Функция обработки входной строки в команды
     * @param [in] line     строка команд
     */
    void input(std::string& line) override {
        std::lock_guard<std::mutex> lock(m_mutex);
        Interpretator::input(line);
    }
private:    
    int m_id;               ///< идентификатор соединения
    std::mutex m_mutex;     ///< мьютек для синхронизации доступа к формированию блока команд
};

//----------------------------------------------------------------------------

//! список контекстов 
std::vector<std::unique_ptr<Context>> g_context;
//! пул потоков для записи в файл
std::shared_ptr<PoolThread> g_poolThreadFile;
//! пул потоков для вывода в консоль
std::shared_ptr<PoolThread> g_poolThreadConsole;

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
    // std::cout << "libInitilize\n";
    // создается пул потоков для записи в консоль
    g_poolThreadConsole = std::shared_ptr<PoolThread>(new PoolThread(1, ff_console));
    // создается пул потоков для записи в файл
    g_poolThreadFile = std::shared_ptr<PoolThread>(new PoolThread(2, ff_file));
}


void libRelease() 
{
    // std::cout << "libRelease\n";
    g_poolThreadConsole->exit();
    g_poolThreadConsole.reset();
    
    g_poolThreadFile->exit();
    g_poolThreadFile.reset();
}

//----------------------------------------------------------------------------

/**
 * @brief Функция нового подключения
 * 
 * @param [in] size     размер блока команд
 * @return [out] int    идентификатор подключения
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
 * @brief Функция приема данных по ранее созданному подключению
 * 
 * @param [in] data     указатель на начало строки
 * @param [in] len      длина строки
 * @param [in] id       идентификатор подключения
 */
void receive(const char* data, std::size_t len, int id) 
{
    auto it = std::find_if(g_context.begin(), g_context.end(), [id](auto& el) {
        return id == el->id();        
    });

    if (it != g_context.end()) {
        std::istringstream sin({data, len});
        for (std::string line; std::getline(sin, line) && !line.empty(); ) {
            (*it)->input(line);        
        }
    }
}


/**
 * @brief Функция разрыва подключения
 * 
 * @param [in] id   идентификато подключения
 */
void disconnect(int id) 
{
    // std::cout << "disconnect: " << id << std::endl;
     auto it = std::find_if(g_context.begin(), g_context.end(), [id](auto& el) {
        return id == el->id();        
    });

    //! удаление контекста соединения
    if (it != g_context.end()) {
        // (*it)->
        g_context.erase(it);
    }

    //! нет соединений -> удалить пулы потоков
    if (g_context.empty())
        libRelease();
}