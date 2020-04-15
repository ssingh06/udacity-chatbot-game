//
// Created by root on 4/14/20.
//

#ifndef CHATBOT_CHATBOT_MANAGER_CONSTANTS_H
#define CHATBOT_CHATBOT_MANAGER_CONSTANTS_H

#include <string>

/* common default config values */
std::string DEFAULT_CONFIG_USERNAME = "admin";
std::string DEFAULT_CONFIG_PASSWORD = "password";
long DEFAULT_CONFIG_GAME_HIGHSCORE = 0;

/* common running config values */
std::string RUNNING_CONFIG_BASE_DIRECTORY = "../";
std::string RUNNING_CONFIG_FILENAME = "running_config.bin";

std::string CONFIG_USERNAME_KEY = "username";
std::string CONFIG_PASSWORD_KEY = "password";

std::string CONFIG_GAME_HIGHSCORE_SUFFIX = "_highscore";

/* Trivia config keys */
std::string CONFIG_TRIVIA_QUESTION_COUNT_KEY = "trivia_question_count";
long DEFAULT_CONFIG_TRIVIA_QUESTION_COUNT = 0;

#endif //CHATBOT_CHATBOT_MANAGER_CONSTANTS_H
