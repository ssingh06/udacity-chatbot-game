//
// Created by root on 4/11/20.
//

#ifndef CHATBOT_HTTP_CLIENT_H
#define CHATBOT_HTTP_CLIENT_H

#include <string>
#include <curl/curl.h>
#include <vector>
#include <thread>
#include <future>

#include "restapi/rest_api_request.h"
#include "restapi/rest_api_response.h"
#include "../util/message_queue.h"

enum HTTP_CLIENT_TYPE {
    SYNC, ASYNC
};

class HttpClient {
public:
    HttpClient() : _clientType(ASYNC), _isRunning(false) {}
    HttpClient(HTTP_CLIENT_TYPE type) : _clientType(type), _isRunning(false) {}
    void execute(RestApiRequest& request);
    void asyncFetchResponse();
    void Start();
    void Stop();
private:
    HTTP_CLIENT_TYPE _clientType;
    bool _isRunning;
    std::thread _executorThread;
    MessageQueue<RestApiRequest> _httpRequestsQueue;

    void fetchResponse(RestApiRequest& request);
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
};


#endif //CHATBOT_HTTP_CLIENT_H
