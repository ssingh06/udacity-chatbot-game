//
// Created by root on 4/13/20.
//

#ifndef CHATBOT_CHAT_METADATA_H
#define CHATBOT_CHAT_METADATA_H


#include <string>
#include <vector>

#include "util/chat_constants.h"

class ChatMetadata {
public:
    ChatMetadata() : _quit(false), _chatMode(DEFAULT_MODE) {}
    void SetQuit(bool quit) {
        _quit = quit;
    }

    bool GetQuit() const {
        return _quit;
    };

    void AddExtraMessage(std::string &&message);
    std::vector<std::string>&& GetExtraMessages() {
        return std::move(_extraMessages);
    }

    void SetChatMode(CHAT_MODE chatMode) {
        _chatMode = chatMode;
    }

    CHAT_MODE GetChatMode() const {
        return _chatMode;
    }

private:
    bool _quit;
    std::vector<std::string> _extraMessages;
    CHAT_MODE _chatMode;
};


#endif //CHATBOT_CHAT_METADATA_H
