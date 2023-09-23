#include "pool.h"


PoolThread_::PoolThread_(int countThreads, FuncThread f)
{
    m_func = f;
    // m_threads.reserve(countThreads);
    for (int i = 0; i < countThreads; ++i) {
        std::thread th(&PoolThread_::worker, this, i);
        th.detach();
    }
}


void PoolThread_::update(std::shared_ptr<BlockCommands> &block)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_blocks.push(block);
    m_cv.notify_one();
}


void PoolThread_::stop()
{
    m_flagStop = true;
    m_cv.notify_all();
}


void PoolThread_::worker(int id)
{
    while(!m_flagStop) {        
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock, [&]() { return !m_blocks.empty() || m_flagStop; } );

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
