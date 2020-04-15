//
// Created by root on 4/12/20.
//

#ifndef CHATBOT_PARSER_H
#define CHATBOT_PARSER_H


#include <string>
#include <jsoncpp/json/json.h>

class Parser {
public:
    Parser(std::string inputString);
    void Parse();
    bool IsMember(const std::string& field);
    std::string GetField(const std::string& field);

    Json::Value GetJsonValue() const {
        return _jsonValue;
    }
    void SetJsonValue(Json::Value jsonValue) {
        _jsonValue = jsonValue;
    }

private:
    std::string _rawString;
    Json::Value _jsonValue;
};


#endif //CHATBOT_PARSER_H
