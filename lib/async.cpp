#include <iostream>
#include <algorithm>
#include <functional>

#include "async.h"
#include "pool.h"



std::vector<std::unique_ptr<Interpret>> g_interprets;
std::shared_ptr<PoolThread_> g_poolThreadFile;
std::shared_ptr<PoolThread_> g_poolThreadConsole;
// auto g_pool = std::shared_ptr<PoolThread_>(new PoolThread_(2));


// auto ff_file = [](std::shared_ptr<BlockCommand>& block, int id) {

// };

auto ff_console = [](std::shared_ptr<BlockCommand>& block, int) 
{
    std::cout << "bulk:";
    block->execute(std::cout);
    std::cout << "\n";    
};

void f1(std::shared_ptr<BlockCommand>& /*block*/, int id) 
{
    std::cout << "func1: " << id << "\n";
}

void start() 
{
    // using namespace std::placeholders;
    // g_poolThreadFile = std::shared_ptr<PoolThread_>(new PoolThread_(2, LogConsole::log));
    // g_poolThreadConsole = std::shared_ptr<PoolThread_>(new PoolThread_(4, LogConsole::log));
    g_poolThreadConsole = std::shared_ptr<PoolThread_>(new PoolThread_(1, ff_console));
    // g_poolThreadFile = std::shared_ptr<PoolThread_>(new PoolThread_(2, std::bind(&LogFile::f2, &foo2, _1, _2)));
}


void finish() 
{

}


int connect(int size) 
{
    static int incId = 0;
    if (g_interprets.empty())
        start();

    std::unique_ptr<Interpret> interpret(new Interpret(size, ++incId));
    // interpret->addSubscriber(g_poolThreadFile);
    interpret->addSubscriber(g_poolThreadConsole);
    g_interprets.emplace_back(std::move(interpret));
    
    return incId;
}


void receive(char* data, std::size_t len, int id) 
{
    // std::cerr << "f.receive: " << id << " " << len << std::endl;

    auto it = std::find_if(g_interprets.begin(), g_interprets.end(), [id](auto& el) {
        return id == el->id();        
    });
    if (it != g_interprets.end()) {
        std::string ss(data, len);
        // std::cout << "receive: " << ss << std::endl;
        std::istringstream sin(ss);
        (*it)->execute(sin);        
    }
}


void disconnect(int id) 
{
    std::cout << "disconnect: " << id << std::endl;
    // g_thread->stop();
    // g_poolThreadFile->stop();
    g_poolThreadConsole->stop();
}