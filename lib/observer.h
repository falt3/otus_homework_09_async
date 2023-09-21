#pragma once

#include <vector>
#include <memory>

#include "command.h"


class Subscriber {
public:
    virtual ~Subscriber() = default;
    virtual void update(std::shared_ptr<BlockCommand>& block) = 0;    
};


class Publisher {
public:
    void addSubscriber(std::shared_ptr<Subscriber> el) {
        m_subscribers.push_back(std::move(el));
    }
protected:
    void notify(std::shared_ptr<BlockCommand>& block) {
        for (auto& el : m_subscribers)
            el->update(block);
    }
private:
    std::vector<std::shared_ptr<Subscriber>> m_subscribers;
};

