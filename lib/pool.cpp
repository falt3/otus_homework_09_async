#include "pool.h"


/**
 * @brief Construct a new PoolThread::PoolThread object
 * 
 * @param [in] countThreads количество потоков, которые нужну создать
 * @param [in] f            функция для обрабоки задачи
 */
PoolThread::PoolThread(int countThreads, FuncThread f)
{
    m_func = f;
    // m_threads.reserve(countThreads);
    for (int i = 0; i < countThreads; ++i) {
        std::thread th(&PoolThread::worker, this, i);
        th.detach();
    }
}


/**
 * @brief Функция добавления нового блока команд в очерель
 * 
 * @param [in] block    новый блок команд
 */
void PoolThread::update(std::shared_ptr<BlockCommands> &block)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_blocks.push(block);
    m_cv.notify_one();
}


/**
 * @brief Функция устнаовки сигнала дл язавершения потоков
 * 
 */
void PoolThread::exit()
{
    m_flagExit = 1;
    m_cv.notify_all();
}


/**
 * @brief Рабочая функция потоков
 * 
 * @param [in] id   идентификатор потока
 */
void PoolThread::worker(int id)
{
    while(!m_flagExit) {        
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock, [&]() { return !m_blocks.empty() || m_flagExit; } );

        if (!m_blocks.empty()) 
        {
            auto block = m_blocks.front();
            m_blocks.pop();
            lock.unlock();

            m_func(block, id);

            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1ms);
        }
    }
}
