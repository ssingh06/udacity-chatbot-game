//
// Created by root on 4/11/20.
//

#include "http_client.h"
#include "util/http_constants.h"

void HttpClient::execute(RestApiRequest &request) {
    if (_clientType == ASYNC) {
        _httpRequestsQueue.send(std::move(request));
    } else {
        fetchResponse(request);
    }
}

void HttpClient::asyncFetchResponse() {
    while (_isRunning) {
        RestApiRequest request = _httpRequestsQueue.receive();
        if (!request.getUrl().empty()) {
            /*
             * only fetch response for valid requests
             * Requests with empty URLs could be sent to break out of the while
             * loop here when the client is to be stopped.
             */
            fetchResponse(request);
        }
    }
}

void HttpClient::Start() {
    if (_isRunning) {
        /* http client is already running. Ignore the Start request */
        return;
    }

    _isRunning = true;

    if (_clientType == ASYNC) {
        /* start the async executor thread */
        _executorThread = std::thread(&HttpClient::asyncFetchResponse, this);
    }
}

void HttpClient::Stop() {
    _isRunning = false;

    if (_clientType == ASYNC) {
        /*
         * send an invalid rest api request to the http message queue so that
         * the call to receive() AsyncFetchResponse can return and the while
         * loop condition can be evaluated again.
         */
        _httpRequestsQueue.send(RestApiRequest(""));
    }
    _executorThread.join();
}

void HttpClient::fetchResponse(RestApiRequest& request) {
    bool isError = false;
    CURL *curl = curl_easy_init();
    RestApiResponse response;
    if(curl) {
        std::string responseBuffer;
        curl_easy_setopt(curl, CURLOPT_URL, request.getUrl().c_str());

        /* Hook up data handling function. */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        /* Hook up data container (will be passed as the last parameter to the
         * callback handling function).  Can be any pointer type, since it will
         * internally be passed as a void pointer.
         */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);
        if (curl_easy_perform(curl)) {
            isError = true;
        } else {
            long responseCode(0);
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
            response = RestApiResponse(responseBuffer, (STATUS_CODE)responseCode);
        }
        curl_easy_cleanup(curl);
    } else {
        isError = true;
    }

    /* construct a generic error response object */
    if (isError) {
        response = RestApiResponse();
    }
    request.getPromise().set_value(response);
}

size_t HttpClient::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}