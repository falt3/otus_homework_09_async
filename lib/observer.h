#pragma once

#include <vector>
#include <memory>
#include <iostream>

#include "command.h"

/**
 * @brief Класс подписчика сообщений
 * 
 */
template <typename T>
class Subscriber {
public:
    // virtual ~Subscriber() = default;
    virtual void update(std::shared_ptr<T>& block) = 0;    
};


/**
 * @brief Класс издателя сообщений
 * 
 */
template <typename T>
class Publisher {
public:
    void addSubscriber(std::shared_ptr<Subscriber<BlockCommands>> el) {
        m_subscribers.push_back(std::move(el));
    }
protected:
    void notify(std::shared_ptr<T>& block) {
        for (auto& el : m_subscribers)
            el->update(block);
    }
private:
    std::vector<std::shared_ptr<Subscriber<BlockCommands>>> m_subscribers;
};

