//
// Created by root on 4/12/20.
//

#ifndef CHATBOT_TRIVIA_H
#define CHATBOT_TRIVIA_H


#include "../rest_api_game.h"

class Trivia : public RestApiGame {
public:
    static Trivia& GetInstance();

    /* Game methods */
    void Launch() override;
    void Reset() override ;
    void Stop() override;

    /* RestApiGame methods */
    bool FetchDefaultResource() override;

    /* Trivia specific methods */
    std::string GetNextQuestion();
    std::string GetNextCategory();
    long GetNextValue();
    std::string GetNextAnswer();

    Trivia(const Trivia&) = delete;
    Trivia& operator=(const Trivia&) = delete;

private:
    std::string _question;
    std::string _category;
    std::string _answer;
    long _value;

    Trivia() : RestApiGame("Trivia") {}
    void StartGameLoop();
};


#endif //CHATBOT_TRIVIA_H
