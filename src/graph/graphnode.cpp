#include <iostream>

#include "graphedge.h"
#include "graphnode.h"
#include "util/graph_constants.h"
#include "../game/trivia/trivia.h"
#include "../manage/chatbot_manager.h"


GraphNode::GraphNode(int id) {
    _id = id;
}

GraphNode::~GraphNode() {}

void GraphNode::AddToken(std::string token) {
    _answers.push_back(token);
}

void GraphNode::AddEdgeToParentNode(GraphEdge *edge) {
    _parentEdges.push_back(edge);
}

void GraphNode::AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge) {
    _childEdges.push_back(std::move(edge));
}

void GraphNode::MoveChatbotHere(ChatBot chatbot) {
    _chatBot = std::move(chatbot);
    _chatBot.SetCurrentNode(this);
}

void GraphNode::MoveChatbotHere(ChatBot chatbot,
                                ChatMetadata chatMetadata) {
    _chatBot = std::move(chatbot);
    _chatBot.SetCurrentNode(this, chatMetadata);
}

void GraphNode::MoveChatbotToNewNode(GraphNode *newNode) {
    newNode->MoveChatbotHere(std::move(_chatBot));
    //_chatBot = nullptr; // invalidate pointer at source
}

void GraphNode::MoveChatbotToNewNode(GraphNode *newNode,
                                    ChatMetadata chatMetadata) {
    newNode->MoveChatbotHere(std::move(_chatBot), chatMetadata);
    //_chatBot = nullptr; // invalidate pointer at source
}

ChatMetadata GraphNode::Run() {
    ChatMetadata ret;
    /* check the node type */
    switch (GetID()) {
        case MANAGE_START_NODE:
            /*
             * C++ switch-case syntax specific:
             * this case declares new variables, so we have to enclose it
             * inside a block of code to avoid compilation errors
             */
            {
                /*
                 * The user will be prompted to enter the password for the manage mode.
                 * We must get the password from ChatbotManager and populate the password
                 * for the correct password edge.
                 */
                for (size_t i = 0; i < GetNumberOfChildEdges(); ++i) {
                    GraphEdge *edge = GetChildEdgeAtIndex(i);
                    if (edge->GetID() == MANAGE_CORRECT_PASSWORD_EDGE) {
                        edge->AddKeyword(ChatbotManager::GetInstance().GetPassword());
                        break;
                    }
                }
            }
            break;
        case TRIVIA_START_NODE:
            Trivia::GetInstance().Launch();
            Trivia::GetInstance().Reset();
            ret.SetChatMode(TRIVIA_MODE);
            ret.AddExtraMessage("Current Score: " + std::to_string(Trivia::GetInstance().GetScore()));
            break;
        case TRIVIA_NEW_QUESTION_NODE:
            {
                /*
                 * C++ switch-case syntax specific:
                 * this case declares new variables, so we have to enclose it
                 * inside a block of code to avoid compilation errors
                 */
                ret.SetChatMode(TRIVIA_MODE);
                const bool fetchDefaultResult = Trivia::GetInstance().FetchDefaultResource();
                std::string nextAnswer = fetchDefaultResult ?
                        Trivia::GetInstance().GetNextQuestion() :
                        "Sorry! " + Trivia::GetInstance().GetErrorReason() + "\nEnter 'Restart' to restart the game.";

                std::string nextKeyword = fetchDefaultResult ?
                        Trivia::GetInstance().GetNextAnswer() : "Restart";

                /* Add the trivia question to the current node */
                AddAnswer(nextAnswer);

                /* Add category to the return vector so it could be shown as a separate message to the user */
                if (fetchDefaultResult) {
                    ret.AddExtraMessage(Trivia::GetInstance().GetNextCategory());
                    ret.AddExtraMessage("Value: " + std::to_string(Trivia::GetInstance().GetNextValue()));
                }

                /*
                 * Next task is to set the keyword on the next edge that the graph will move upon
                 * If the last question was fetched correctly, then the user must be allowed to
                 * enter the answer and transition accordingly. To enable this, we set the answer
                 * from the rest api call on the correct answer edge.
                 * If there was an error in fetching the last question, then the user must be allowed
                 * to restart the game entering 'Restart'. That transition is captured by the ERROR_EDGE.
                 */
                const GRAPH_EDGE_ID nextEdge = fetchDefaultResult ? CORRECT_ANSWER_EDGE : ERROR_EDGE;

                /* add the trivia answer to the correct answer edge */
                for (size_t i = 0; i < GetNumberOfChildEdges(); ++i) {
                    GraphEdge *edge = GetChildEdgeAtIndex(i);
                    if (edge->GetID() == nextEdge) {
                        edge->AddKeyword(nextKeyword);
                        break;
                    }
                }

                /* We're done with this question. Reset the game */
                Trivia::GetInstance().Reset();
            }
            break;
        case TRIVIA_CORRECT_ANSWER_NODE:
            ret.SetChatMode(TRIVIA_MODE);
            Trivia::GetInstance().IncrementScore(Trivia::GetInstance().GetNextValue());
            break;
        case TRIVIA_END_NODE:
            Trivia::GetInstance().Stop();
            ret.SetQuit(true);
            break;
        case MANAGE_TRIVIA_STATS_NODE:
            ret.AddExtraMessage("Total Trivia Questions: " +
            std::to_string(ChatbotManager::GetInstance().GetTriviaQuestionCount()));
            ret.AddExtraMessage("Trivia High Score: " +
            std::to_string(ChatbotManager::GetInstance().GetGameHighScore("Trivia")));
            break;
        case MANAGE_TRIVIA_HIGHSCORE_NODE:
            ret.AddExtraMessage("Trivia High Score: " +
            std::to_string(ChatbotManager::GetInstance().GetGameHighScore("Trivia")));
            break;
        case MANAGE_END_NODE:
            ret.SetQuit(true);
            break;
        default: ;
    }

    return std::move(ret);
}

GraphEdge *GraphNode::GetChildEdgeAtIndex(int index)
{
    return _childEdges[index].get();
}

void GraphNode::AddAnswer(std::string answer) {
    _answers.pop_back();
    _answers.push_back(answer);
}