#pragma once

#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <functional>

#include "interpret.h"
#include "observer.h"


class PoolThread;

//---------------------------------------------------------------------
/**
 * @brief Класс очереди блоков команд для доступа потоками files
 * 
 */
class QueueTasks : public Subscriber {
public:
    QueueTasks() {}
    ~QueueTasks() override {};
    void addBlockCommands(std::shared_ptr<BlockCommand>& block);
    std::shared_ptr<BlockCommand> getBlockCommands();

    void update(std::shared_ptr<BlockCommand>& block) override;    
    void setNotifyDataAdd(std::function<void(PoolThread&)> f, PoolThread* pool) { 
        notifyDataAdd = f; m_pool = pool; }
   
    // int size() { return m_blocks.size(); }
    // bool empty() {return m_blocks.empty(); }
    // friend void worker_file(QueueTasks& tasks);
    friend class PoolThread;
private:
    std::queue<std::shared_ptr<BlockCommand>> m_blocks;
    std::mutex m_mutex;
    std::function<void(PoolThread&)> notifyDataAdd;
    PoolThread* m_pool;
};


//---------------------------------------------------------------------

// class Base {
// public:
//     Base();
//     int connect(int size);
//     void receive(const char* data, int len, int id);
//     void disconnect(int id);

//     friend std::ostream& operator<<(std::ostream& out, Base& b) {
//         out << "Base: " << b.m_interprets.size(); 
//         return out;
//     }
//     friend std::ostream& operator/(std::ostream& out, Base& b) {
//         out << "Base/: " << b.m_interprets.size(); 
//         return out;
//     }

// private:
//     int m_incId = 0;
// };

class PoolThread {
public:
    PoolThread(int countThreads, QueueTasks& tasks);
    void notify();
    void stop();

private:
    int m_countThreads;
    std::vector<std::unique_ptr<std::thread>> m_threads;
    int m_flagStop  = false;
    
    void worker(int id, QueueTasks& tasks);
};