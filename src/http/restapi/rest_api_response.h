//
// Created by root on 4/11/20.
//

#ifndef CHATBOT_REST_API_RESPONSE_H
#define CHATBOT_REST_API_RESPONSE_H

#include <iostream>
#include <string>
#include <utility>

#include "../util/http_constants.h"


class RestApiResponse {
public:
    RestApiResponse() : _response("Error"), _statusCode(STATUS_CODE::INTERNAL_ERROR) {}
    RestApiResponse(std::string raw_response,
            STATUS_CODE statusCode) : _response(std::move(raw_response)), _statusCode(statusCode) {}


    const STATUS_CODE &getStatusCode() const {
        return _statusCode;
    }

    const std::string &getResponse() const {
        return _response;
    }

private:
    STATUS_CODE _statusCode;
    std::string _response;
};


#endif //CHATBOT_REST_API_RESPONSE_H
