//
// Created by root on 4/14/20.
//

#ifndef CHATBOT_CHATBOT_MANAGER_H
#define CHATBOT_CHATBOT_MANAGER_H


#include <string>
#include <map>

class ChatbotManager {
public:
    static ChatbotManager& GetInstance();
    void Start();
    void Stop();
    std::string GetUsername() const;
    void SetUsername(const std::string& username);
    std::string GetPassword() const;
    void SetPassword(const std::string& password);
    long GetGameHighScore(std::string gameName) const;
    void SetGameHighScore(std::string gameName, long highScore);

    /* Trivia specific methods */
    long GetTriviaQuestionCount() const;
    void SetTriviaQuestionCount(long questionCount);

    ChatbotManager(const ChatbotManager&) = delete;
    ChatbotManager& operator=(const ChatbotManager&) = delete;

private:
    bool isRunning_{false};
    std::map<std::string, std::string> _runningConfig;

    ChatbotManager() = default;
    void LoadConfig();
    void StoreConfig();
    std::string to_string();
};


#endif //CHATBOT_CHATBOT_MANAGER_H
