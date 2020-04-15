//
// Created by root on 4/12/20.
//

#include "game.h"
#include "../manage/chatbot_manager.h"

void Game::Launch() {
    if (_isRunning) {
        return;
    }

    _isRunning = true;
    /* Initialize the game high score by querying the ChatbotManager */
    _highScore = ChatbotManager::GetInstance().GetGameHighScore(_name);
}

void Game::Reset() {
    _isError = false;
    _errorReason = std::string();
}

void Game::Stop() {
    _isRunning = false;
    _score = 0;

    /* store the game high score info in the ChatbotManager */
    ChatbotManager::GetInstance().SetGameHighScore(_name, _highScore);
}

void Game::SetScore(long score) {
    _score = score;

    /* update the high score */
    if (_score > _highScore) {
        _highScore = _score;
    }
}

void Game::IncrementScore(long increment) {
    SetScore(_score + increment);
}
