//
// Created by root on 4/12/20.
//

#ifndef CHATBOT_GRAPH_CONSTANTS_H
#define CHATBOT_GRAPH_CONSTANTS_H

#include <string>

enum GRAPH_NODE_ID {
    CHATBOT_START_NODE,
    MANAGE_START_NODE = 1,
    MEM_NODE,
    GAME_NODE,
    TRIVIA_START_NODE = 11,
    TRIVIA_NEW_QUESTION_NODE,
    TRIVIA_END_NODE,
    TRIVIA_CORRECT_ANSWER_NODE,
    MANAGE_TRIVIA_STATS_NODE = 17,
    MANAGE_TRIVIA_HIGHSCORE_NODE,
    MANAGE_RESET_PASSWORD_NODE,
    MANAGE_END_NODE = 21,

};

enum GRAPH_EDGE_ID {
    CORRECT_ANSWER_EDGE = 13,
    ERROR_EDGE = 19,
    MANAGE_CORRECT_PASSWORD_EDGE = 21
};

#endif //CHATBOT_GRAPH_CONSTANTS_H
