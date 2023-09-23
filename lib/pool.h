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
    using FuncThread = std::function<void(std::shared_ptr<BlockCommands>&, int)>;

    PoolThread_(int countThreads, FuncThread f);

    void update(std::shared_ptr<BlockCommands>& block) override;    
    void stop();

private:
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::queue<std::shared_ptr<BlockCommands>> m_blocks;
    int m_flagStop  = 0;
    FuncThread m_func;
    
    void worker(int id);    
};
