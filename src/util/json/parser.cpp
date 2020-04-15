//
// Created by root on 4/12/20.
//

#include <iostream>
#include "parser.h"

Parser::Parser(std::string inputString) : _rawString(inputString) {}

void Parser::Parse() {
    Json::Reader jsonReader;

    if (jsonReader.parse(_rawString, _jsonValue)) {
        //std::cout << "Debug: Successfully parsed json data" << std::endl;
    } else {
        std::cout << "Error: Could not Parse http data as json data" << std::endl;
    }
}

bool Parser::IsMember(const std::string &field) {
    return _jsonValue.isMember(field);
}

std::string Parser::GetField(const std::string& field) {
    return _jsonValue.get(field, "").toStyledString();
}