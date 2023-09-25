#pragma once

#include <vector>
#include <memory>

#include "command.h"

/**
 * @brief Класс подписчика сообщений
 * 
 */
class Subscriber {
public:
    // virtual ~Subscriber() = default;
    virtual void update(std::shared_ptr<BlockCommands>& block) = 0;    
};


/**
 * @brief Класс издателя сообщений
 * 
 */
class Publisher {
public:
    void addSubscriber(std::shared_ptr<Subscriber> el) {
        m_subscribers.push_back(std::move(el));
    }
protected:
    void notify(std::shared_ptr<BlockCommands>& block) {
        for (auto& el : m_subscribers)
            el->update(block);
    }
private:
    std::vector<std::shared_ptr<Subscriber>> m_subscribers;
};

