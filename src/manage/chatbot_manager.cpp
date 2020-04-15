//
// Created by root on 4/14/20.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>

#include "chatbot_manager.h"
#include "util/chatbot_manager_constants.h"

ChatbotManager & ChatbotManager::GetInstance() {
    static ChatbotManager instance;
    return instance;
}

void ChatbotManager::Start() {
    if (_isRunning) {
        return;
    }

    /* load the running config */
    LoadConfig();

    /* mark _isRunning as true */
    _isRunning = true;
}

void ChatbotManager::Stop() {
    if (!_isRunning) {
        return;
    }
    _isRunning = false;

    /* store the running config */
    StoreConfig();
}

std::string ChatbotManager::GetUsername() const {
    if (!_isRunning) {
        std::cout << "Error: ChatbotManager not running. Returning error value." << std::endl;
        return std::string();
    }

    return _runningConfig.find(CONFIG_USERNAME_KEY)->second;
}

void ChatbotManager::SetUsername(const std::string& username) {
    if (!_isRunning) {
        std::cout << "Error: ChatbotManager not running. Ignoring update." << std::endl;
        return;
    }

    if (username.empty()) {
        std::cout << "Error: Empty username. Won't update." << std::endl;
        return;
    }

    _runningConfig.at(CONFIG_USERNAME_KEY) = username;
}

std::string ChatbotManager::GetPassword() const {
    if (!_isRunning) {
        std::cout << "Error: ChatbotManager not running. Returning error value." << std::endl;
        return std::string();
    }

    return _runningConfig.find(CONFIG_PASSWORD_KEY)->second;
}

void ChatbotManager::SetPassword(const std::string& password) {
    if (!_isRunning) {
        std::cout << "Error: ChatbotManager not running. Ignoring update." << std::endl;
        return;
    }

    if (password.empty()) {
        std::cout << "Error: Empty password. Won't update." << std::endl;
        return;
    }

    _runningConfig.at(CONFIG_PASSWORD_KEY) = password;
}

long ChatbotManager::GetGameHighScore(std::string gameName) const {
    if (!_isRunning) {
        std::cout << "Error: ChatbotManager not running. Returning error value." << std::endl;
        return -1;
    }

    return std::stol(_runningConfig.find(gameName.append(CONFIG_GAME_HIGHSCORE_SUFFIX))->second);
}

void ChatbotManager::SetGameHighScore(std::string gameName, long highScore) {
    if (!_isRunning) {
        std::cout << "Error: ChatbotManager not running. Ignoring update." << std::endl;
        return;
    }

    if (highScore <= 0) {
        std::cout << "Error: High score can't be 0 or negative. Ignoring update" << std:: endl;
        return;
    }

    _runningConfig.at(gameName.append(CONFIG_GAME_HIGHSCORE_SUFFIX)) = std::to_string(highScore);
}

long ChatbotManager::GetTriviaQuestionCount() const {
    if (!_isRunning) {
        std::cout << "Error: ChatbotManager not running. Returning error value." << std::endl;
        return -1;
    }

    return std::stol(_runningConfig.find(CONFIG_TRIVIA_QUESTION_COUNT_KEY)->second);
}

void ChatbotManager::SetTriviaQuestionCount(long questionCount) {
    if (!_isRunning) {
        std::cout << "Error: ChatbotManager not running. Ignoring update." << std::endl;
        return;
    }

    if (questionCount <= 0) {
        std::cout << "Error: Trivia question count can't be 0 or negative. Ignoring update" << std:: endl;
        return;
    }

    _runningConfig.at(CONFIG_TRIVIA_QUESTION_COUNT_KEY) = std::to_string(questionCount);
}

void ChatbotManager::LoadConfig() {
    std::ifstream configFileStream(RUNNING_CONFIG_BASE_DIRECTORY + RUNNING_CONFIG_FILENAME);
    if (configFileStream.is_open()) {
        /*
         * running config present
         * Load it into the running config map
         */
        std::string line;
        while (std::getline(configFileStream, line)) {
            std::istringstream linestream(line);
            int count = 0;
            std::string tempToken;
            std::string key, value;

            /* split the line from the file on ": */
            while(std::getline(linestream, tempToken, ':')) {
                if (count == 0) {
                    key = tempToken;
                } else if (count == 1) {
                    value = tempToken;
                } else {
                    std::cout << "Error: Invalid config line. Ignoring" << std::endl;
                }

                count++;
            }

            _runningConfig.insert(std::pair<std::string, std::string>(key, value));
        }

        /* close the file stream */
        configFileStream.close();

        /* Done reading the config from the file. We can delete this file now */
        std::remove((RUNNING_CONFIG_BASE_DIRECTORY + RUNNING_CONFIG_FILENAME).c_str());
    } else {
        /* running config not present. Load default config values */
        _runningConfig.insert(std::pair<std::string, std::string>(CONFIG_USERNAME_KEY, DEFAULT_CONFIG_USERNAME));
        _runningConfig.insert(std::pair<std::string, std::string>(CONFIG_PASSWORD_KEY, DEFAULT_CONFIG_PASSWORD));
        _runningConfig.insert(std::pair<std::string, std::string>("Trivia" + CONFIG_GAME_HIGHSCORE_SUFFIX, std::to_string(DEFAULT_CONFIG_GAME_HIGHSCORE)));
        _runningConfig.insert(std::pair<std::string, std::string>(CONFIG_TRIVIA_QUESTION_COUNT_KEY, std::to_string(DEFAULT_CONFIG_TRIVIA_QUESTION_COUNT)));
    }
}

void ChatbotManager::StoreConfig() {
    std::ofstream configFileStream(RUNNING_CONFIG_BASE_DIRECTORY + RUNNING_CONFIG_FILENAME);
    configFileStream << to_string();
    configFileStream.close();
}

std::string ChatbotManager::to_string() {
    std::string ret;

    for (std::pair<std::string, std::string> p : _runningConfig) {
        ret.append(p.first).append(":").append(p.second).append("\n");
    }

    return ret;
}
