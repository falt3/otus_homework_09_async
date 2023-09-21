#include <iostream>
#include <algorithm>

#include "async.h"
#include "pool.h"



std::vector<std::unique_ptr<Interpret>> g_interprets;
std::shared_ptr<PoolThread_> g_poolThreadFile;
// auto g_pool = std::shared_ptr<PoolThread_>(new PoolThread_(2));


void start() 
{
    g_poolThreadFile = std::shared_ptr<PoolThread_>(new PoolThread_(2));
}


void finish() 
{

}


int connect(int size) 
{
    static int incId = 0;
    // std::cout << "f.connect: " << size << std::endl;
    if (g_interprets.empty())
        start();

    std::unique_ptr<Interpret> interpret(new Interpret(size, ++incId));
    interpret->addSubscriber(g_poolThreadFile);
    g_interprets.emplace_back(std::move(interpret));
    
    return incId;
}


void receive(char* data, std::size_t len, int id) 
{
    std::cout << "f.receive: " << id << " " << len << std::endl;
    // base.receive(data, len, id);

    auto it = std::find_if(g_interprets.begin(), g_interprets.end(), [id](auto& el) {
        return id == el->id();        
    });
    if (it != g_interprets.end()) {
        std::cout << "Base: receive: " << it->get()->id() << std::endl;
        // (*it)->execute();
        // std::mem
        // std::stringstream 
        std::string ss(data, len);
        std::istringstream sin(ss);
        (*it)->execute(sin);        
    }
}


void disconnect(int id) 
{
    std::cout << "disconnect: " << id << std::endl;
    // g_thread->stop();
    g_poolThreadFile->stop();
}