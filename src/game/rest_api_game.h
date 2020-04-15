//
// Created by root on 4/12/20.
//

#ifndef CHATBOT_REST_API_GAME_H
#define CHATBOT_REST_API_GAME_H

#include "game.h"
#include "../util/message_queue.h"

class RestApiGame : public Game {
public:
    RestApiGame(std::string name) : Game(name) {}
    virtual bool FetchDefaultResource() = 0;

protected:
    HttpClient& GetHttpClient() {
        return _httpClient;
    }

    MessageQueue<bool>& GetContinueMessageQueue() {
        return _continueMessagesQueue;
    }

private:
    /* default async http client to issue rest api requests */
    HttpClient _httpClient;

    /* bool message queue to receive user inputs indicating if the user wants continue */
    MessageQueue<bool> _continueMessagesQueue;
};


#endif //CHATBOT_REST_API_GAME_H
