//
// Created by root on 4/11/20.
//

#ifndef CHATBOT_MESSAGE_QUEUE_H
#define CHATBOT_MESSAGE_QUEUE_H


#include <deque>
#include <mutex>
#include <condition_variable>

template <class T>
class MessageQueue {
public:
    T receive() {
        std::unique_lock<std::mutex> uniqueLock(_mutex);
        _condition.wait(uniqueLock,
            [this]() { return !_messages.empty(); });
        T ret = std::move(_messages.back());
        _messages.pop_back();
        return ret;
    }

    void send(T &&message) {
        std::unique_lock<std::mutex> uniqueLock(_mutex);
        _messages.push_back(std::move(message));
        _condition.notify_one();
    }

private:
    std::deque<T> _messages;
    std::mutex _mutex;
    std::condition_variable _condition;
};


#endif //CHATBOT_MESSAGE_QUEUE_H
