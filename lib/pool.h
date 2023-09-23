#pragma once

#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <functional>

#include "interpret.h"
#include "observer.h"


class PoolThread_ : public Subscriber {
public:
    using FuncThread = std::function<void(std::shared_ptr<BlockCommand>&, int)>;
    // using FuncThread = void(std::shared_ptr<BlockCommand>& block, int);
    PoolThread_(int countThreads, FuncThread f);
    // ~PoolThread_() override {};

    // void addDataToQueue(std::shared_ptr<BlockCommand>& block);
    // std::shared_ptr<BlockCommand> getDataFromQueue();

    void update(std::shared_ptr<BlockCommand>& block) override;    
    void stop();

    // void update(std::shared_ptr<BlockCommand>& block) override;    
    // void setNotifyDataAdd(std::function<void(PoolThread&)> f, PoolThread* pool) { 
    //     notifyDataAdd = f; m_pool = pool; }
   
    // int size() { return m_blocks.size(); }
    // bool empty() {return m_blocks.empty(); }
    // friend void worker_file(QueueTasks& tasks);
    // friend class PoolThread;
private:
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::queue<std::shared_ptr<BlockCommand>> m_blocks;
    // std::vector<std::unique_ptr<std::thread>> m_threads;
    int m_flagStop  = 0;
    FuncThread m_func;
    
    void worker(int id);    
};
