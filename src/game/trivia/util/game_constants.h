//
// Created by root on 4/12/20.
//

#ifndef CHATBOT_GAME_CONSTANTS_H
#define CHATBOT_GAME_CONSTANTS_H

#include <string>

std::string TRIVIA_BASE_REST_URL = "http://jservice.io";
std::string TRIVIA_RANDOM_QUESTION_REST_SUFFIX = "/api/random";

std::string TRIVIA_QUESTION_TAG = "question";
std::string TRIVIA_CATEGORY_TAG = "category";
std::string TRIVIA_TITLE_TAG = "title";
std::string TRIVIA_ANSWER_TAG = "answer";
std::string TRIVIA_VALUE_TAG = "value";

std::string TRIVIA_REQUIRED_FIELDS[] = {
        TRIVIA_QUESTION_TAG,
        TRIVIA_CATEGORY_TAG,
        TRIVIA_ANSWER_TAG,
        TRIVIA_VALUE_TAG
};

#endif //CHATBOT_GAME_CONSTANTS_H
