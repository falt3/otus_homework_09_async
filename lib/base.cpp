#include "base.h"
#include <algorithm>

//---------------------------------------------------------------------

// Base::Base()
// {
//     m_queueFile.reset(new DataCommand());
// }

// int Base::connect(int size)
// {
//     std::unique_ptr<Interpret> obj(new Interpret(size, ++m_incId));
//     obj->addSubscriber(m_queueFile);
//     m_interprets.emplace_back(std::move(obj));
//     return m_incId;
// }


// void Base::receive(const char* data, int len, int id)
// {
//     auto it = std::find_if(m_interprets.begin(), m_interprets.end(), [id](auto& el) {
//         return id == el->id();        
//     });
//     if (it != m_interprets.end()) {
//         std::cout << "Base: receive: " << it->get()->id() << std::endl;
//         // (*it)->execute();
//         // std::mem
//         // std::stringstream 
//         std::string ss(data, len);
//         std::istringstream sin(ss);
//         // std::cout << "12: " << sin.str();
//         (*it)->execute(sin);        
//     }
// }


// void Base::disconnect(int /*id*/)
// {
// }

//---------------------------------------------------------------------

void QueueTasks::addBlockCommands(std::shared_ptr<BlockCommand>& block)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_blocks.push(block);

    if (notifyDataAdd)
        notifyDataAdd(*m_pool);
}

std::shared_ptr<BlockCommand> QueueTasks::getBlockCommands() 
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::shared_ptr<BlockCommand> data = m_blocks.front();
    m_blocks.pop();
    return data;
}

void QueueTasks::update(std::shared_ptr<BlockCommand> &block)
{
    std::cout << "DataCommand: update: " << block->getCountCommands() << "\n";
    addBlockCommands(block);
}

//---------------------------------------------------------------------

std::condition_variable cv;

PoolThread::PoolThread(int countThreads, QueueTasks& tasks)
{
    m_threads.reserve(countThreads);
    for (int i = 0; i < countThreads; ++i) {
        std::thread th(&PoolThread::worker, this, i+1, std::ref(tasks));
        th.detach();
    }
}

void PoolThread::notify()
{
    std::cerr << "PoolThread: notify\n";
    cv.notify_one();
}

void PoolThread::stop()
{
    m_flagStop = true;
    cv.notify_all();
}

void PoolThread::worker(int id, QueueTasks &tasks)
{
    std::cerr << "worker_"<< id <<": start\n";
    while(!m_flagStop) {        
        bool flagStop = m_flagStop;
        std::cerr << "worker_"<< id <<": pause\n";
        std::unique_lock<std::mutex> lock(tasks.m_mutex);
        cv.wait(lock, [&tasks, flagStop]() { return !tasks.m_blocks.empty() || flagStop; } );

        std::cerr << "worker_"<< id <<": do: "<< tasks.m_blocks.size() << "\n";
        if (!tasks.m_blocks.empty()) 
        {
            auto block = tasks.m_blocks.front();
            tasks.m_blocks.pop();
            lock.unlock();

            block->execute(std::cout); 
            std::cout << std::endl;
        }
    }
    std::cerr << "worker_"<< id <<": exit\n";
}
