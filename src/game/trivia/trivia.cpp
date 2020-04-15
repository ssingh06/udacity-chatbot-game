//
// Created by root on 4/12/20.
//

#include "trivia.h"
#include "util/game_constants.h"
#include "../../util/json/parser.h"
#include "../../manage/chatbot_manager.h"

Trivia & Trivia::GetInstance() {
    static Trivia instance;
    return instance;
}

void Trivia::Launch() {
    GetHttpClient().Start();

    /* invoke the base class method to initialize Game class members */
    Game::Launch();
}

std::string Trivia::GetNextQuestion() {
    if (!_isError) {
        /* increment the question count in ChatbotManager */
        ChatbotManager::GetInstance().
        SetTriviaQuestionCount(ChatbotManager::GetInstance().
        GetTriviaQuestionCount()+1);

        return _question;
    } else {
        return _errorReason;
    }
}

std::string Trivia::GetNextCategory() {
    if (!_isError) {
        return _category;
    } else {
        return _errorReason;
    }
}

long Trivia::GetNextValue() {
    if (!_isError) {
        return _value;
    } else {
        return 0;
    }
}

std::string Trivia::GetNextAnswer() {
    if (!_isError) {
        return _answer;
    } else {
        return _errorReason;
    }
}

bool Trivia::FetchDefaultResource() {
    std::promise<RestApiResponse> testPromise;
    std::future<RestApiResponse> testResponse = testPromise.get_future();
    RestApiRequest request(TRIVIA_BASE_REST_URL + TRIVIA_RANDOM_QUESTION_REST_SUFFIX, std::move(testPromise));
    GetHttpClient().execute(request);
    RestApiResponse response = testResponse.get();
    if (response.getStatusCode() == OK) {
        /*
         * http request succeeded
         * Parse json response
         */
        Parser jsonParser(response.getResponse());
        jsonParser.Parse();

        /* Response of random api from this url is an array */
        jsonParser.SetJsonValue(jsonParser.GetJsonValue()[0]);

        for (std::string& requiredField : TRIVIA_REQUIRED_FIELDS) {
            if (!jsonParser.IsMember(requiredField)) {
                std::cout << "Error: Required field: [" + requiredField + "] missing from server response.";
                _isError = true;
                _errorReason = "Invalid Response from server";
                break;
            }
        }

        if (!_isError) {
            _question = "Question: " + jsonParser.GetJsonValue()[TRIVIA_QUESTION_TAG].asString();
            _category = "Category: " + jsonParser.GetJsonValue()[TRIVIA_CATEGORY_TAG][TRIVIA_TITLE_TAG].asString();
            _value = jsonParser.GetJsonValue()[TRIVIA_VALUE_TAG].asInt64();
            _answer = jsonParser.GetField(TRIVIA_ANSWER_TAG);

            /* Show error if question or answer fields are empty */
            if (_question.empty() || _answer.empty()) {
                _isError = true;
                _errorReason = "Invalid Response from server";
            }
        }
    } else {
        _isError = true;
        _errorReason = "Http request failed. Status Code: " + std::to_string(response.getStatusCode());
    }

    return !_isError;
}

void Trivia::StartGameLoop() {
    while (IsRunning()) {
        if (GetContinueMessageQueue().receive()) {
            std::promise<RestApiResponse> testPromise;
            std::future<RestApiResponse> testResponse = testPromise.get_future();
            RestApiRequest request(TRIVIA_BASE_REST_URL + TRIVIA_RANDOM_QUESTION_REST_SUFFIX, std::move(testPromise));
            GetHttpClient().execute(request);
            RestApiResponse response = testResponse.get();
        }
    }
}

void Trivia::Reset() {
    _question = std::string();
    _answer = std::string();
    Game::Reset();
}

void Trivia::Stop() {
    /* Stop the http async client to stop any outgoing rest api requests */
    GetHttpClient().Stop();

    /* send false to continue message queue to allow the executor thread to end */
    //GetContinueMessageQueue().send(false);

    /* Stop the game to finish the game loop */
    Game::Stop();
}
