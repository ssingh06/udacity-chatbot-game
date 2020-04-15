//
// Created by root on 4/12/20.
//

#ifndef CHATBOT_GAME_H
#define CHATBOT_GAME_H


#include <string>

#include "../http/http_client.h"

class Game {
public:
    Game(std::string name) : _name(name) {}

    virtual void Launch() = 0;
    virtual void Reset() = 0;
    virtual void Stop() = 0;

    bool IsRunning() {
        return _isRunning;
    }

    bool IsError() {
        return _isError;
    }

    std::string GetErrorReason() const {
        return _errorReason;
    }

    long GetScore() const {
        return _score;
    }

    void SetScore(long score);
    void IncrementScore(long increment);

    long GetHighScore() const {
        return _highScore;
    }

    void SetHighScore(long highScore) {
        _highScore = highScore;
    }

protected:
    std::string _name;
    bool _isError;
    std::string _errorReason;

private:
    bool _isRunning{false};
    long _score{0};
    long _highScore{0};
};


#endif //CHATBOT_GAME_H
