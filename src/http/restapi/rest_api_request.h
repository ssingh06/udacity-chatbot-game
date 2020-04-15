//
// Created by root on 4/11/20.
//

#ifndef CHATBOT_REST_API_REQUEST_H
#define CHATBOT_REST_API_REQUEST_H

#include <string>
#include <future>

#include "rest_api_response.h"

class RestApiRequest {
public:
    RestApiRequest(std::string url): _url(url) {}
    RestApiRequest(std::string url, std::promise<RestApiResponse> promise) : _url(url), _promise(std::move(promise)) {}

    const std::string &getUrl() const {
        return _url;
    }

    std::promise<RestApiResponse> &&getPromise() {
        return std::move(_promise);
    }

private:
    std::string _url;
    std::promise<RestApiResponse> _promise;
};


#endif //CHATBOT_REST_API_REQUEST_H
