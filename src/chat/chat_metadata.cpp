//
// Created by root on 4/13/20.
//

#include "chat_metadata.h"

void ChatMetadata::AddExtraMessage(std::string &&message) {
    if (!message.empty()) {
        _extraMessages.push_back(std::move(message));
    }
}