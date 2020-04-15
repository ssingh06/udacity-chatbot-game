#include <iostream>
#include <random>
#include <algorithm>
#include <ctime>

#include "chatlogic.h"
#include "../graph/graphnode.h"
#include "../graph/graphedge.h"
#include "../graph/util/graph_constants.h"
#include "../manage/chatbot_manager.h"
#include "chatbot.h"

// constructor WITHOUT memory allocation
ChatBot::ChatBot()
{
    // invalidate data handles
    _image = nullptr;
    _chatLogic = nullptr;
    _rootNode = nullptr;
}

// constructor WITH memory allocation
ChatBot::ChatBot(std::string filename) {
    // invalidate data handles
    _chatLogic = nullptr;
    _rootNode = nullptr;

    // load image into heap memory
    _image = new wxBitmap(filename, wxBITMAP_TYPE_PNG);
}

ChatBot::~ChatBot() {
    // deallocate heap memory
    // Attention: wxWidgets used NULL and not nullptr
    if(_image != NULL) {
        delete _image;
        _image = NULL;
    }
}

/* copy constructor */
ChatBot::ChatBot(const ChatBot& source) {
    /* copy non-heap member data */
    _chatLogic = source._chatLogic;
    _rootNode = source._rootNode;

    /*
     * Deep Copy: if source object contains an image, allocate memory and
     * create a new image object.
     */
    if (source._image != NULL) {
        _image = new wxBitmap(*source._image);
    } else {
        _image = nullptr;
    }
}

/* copy assignment operator */
ChatBot& ChatBot::operator=(const ChatBot& source) {
    /* Prevent copy of an object into itself */
    if (this == &source) {
        return *this;
    }

    /* deallocate any previous heap members */
    if (_image != NULL) {
        delete _image;
        _image= nullptr;
    }

    /* copy non-heap member data */
    _chatLogic = source._chatLogic;
    _rootNode = source._rootNode;

    /*
     * Deep Copy: if source object contains an image, allocate memory and
     * create a new image object.
     */
    if (source._image != NULL) {
        _image = new wxBitmap(*source._image);
    } else {
        _image = nullptr;
    }
}

/* move constructor */
ChatBot::ChatBot(ChatBot&& source) {
    /* Shallow copy: copy all member data from source */
    _chatLogic = source._chatLogic;
    _rootNode = source._rootNode;
    _image = source._image;

    /* invalidate all source member data */
    source._image = nullptr;
    source._chatLogic = nullptr;
    source._rootNode = nullptr;
}

/* move assignment operator */
ChatBot& ChatBot::operator=(ChatBot&& source) {
    /* Prevent copy of an object into itself */
    if (this == &source) {
        return *this;
    }

    /* deallocate any previous heap members */
    if (_image != NULL) {
        delete _image;
        _image= nullptr;
    }

    /* Shallow copy: copy all member data from source */
    _chatLogic = source._chatLogic;
    _rootNode = source._rootNode;
    _image = source._image;

    /* invalidate all source member data */
    source._image = nullptr;
    source._chatLogic = nullptr;
    source._rootNode = nullptr;
}

void ChatBot::ReceiveMessageFromUser(std::string message)
{
    // loop over all edges and keywords and compute Levenshtein distance to query
    typedef std::pair<GraphEdge *, int> EdgeDist;
    std::vector<EdgeDist> levDists; // format is <ptr,levDist>

    // select best fitting edge to proceed along
    GraphNode *newNode;

    /*
     * If the user is resetting the password, we use ChatbotManager to store
     * the new password
     */
    if (_currentNode->GetID() == MANAGE_RESET_PASSWORD_NODE) {
        ChatbotManager::GetInstance().SetPassword(message);
        /* TODO: set the new node to be the password reset node
         * 1. Write a new method: findNeighborById(currentNode, ID)
         * 2. newNode = findNeighborById(currentNode, ID)
         */
    } else {
        /* TODO: move the levenshtein logic here */
    }

    for (size_t i = 0; i < _currentNode->GetNumberOfChildEdges(); ++i) {
        GraphEdge *edge = _currentNode->GetChildEdgeAtIndex(i);
        for (auto keyword : edge->GetKeywords()) {
            EdgeDist ed{edge, ComputeLevenshteinDistance(keyword, message)};
            levDists.push_back(ed);
        }
    }

    if (levDists.size() > 0) {
        // sort in ascending order of Levenshtein distance (best fit is at the top)
        std::sort(levDists.begin(), levDists.end(), [](const EdgeDist &a, const EdgeDist &b) { return a.second < b.second; });
        newNode = levDists.at(0).first->GetChildNode(); // after sorting the best edge is at first position
    } else {
        // go back to root node
        newNode = _rootNode;
    }

    /* run any node specific logic here */
    ChatMetadata chatMetadata = newNode->Run();

    /* check if any node wants the graph to start over */

    if (chatMetadata.GetQuit()) {
        /* If the user wants to quit, restart the chatbot state machine */
        newNode = _rootNode;
    }

    // tell current node to move chatbot to new node
    _currentNode->MoveChatbotToNewNode(newNode, chatMetadata);
}

void ChatBot::SetCurrentNode(GraphNode *node) {
    SetCurrentNode(node, ChatMetadata{});
}

void ChatBot::SetCurrentNode(GraphNode *node,
                            ChatMetadata chatMetadata) {
    // update pointer to current node
    _currentNode = node;

    // select a random node answer (if several answers should exist)
    std::vector<std::string> answers = _currentNode->GetAnswers();
    std::mt19937 generator(int(std::time(0)));
    std::uniform_int_distribution<int> dis(0, answers.size() - 1);
    std::string answer = answers.at(dis(generator));

    // send selected node answer to user
    _chatLogic->SetChatbotHandle(this);
    _chatLogic->SendMessageToUser(answer);

    /* Set the chat mode */
    _chatLogic->SetChatMode(chatMetadata.GetChatMode());

    /* Add extra messages */
    std::vector<std::string> extraMessages = chatMetadata.GetExtraMessages();
    for (std::string& extraMessage : extraMessages) {
        _chatLogic->SendMessageToUser(extraMessage);
    }
}

int ChatBot::ComputeLevenshteinDistance(std::string s1, std::string s2) {
    // convert both strings to upper-case before comparing
    std::transform(s1.begin(), s1.end(), s1.begin(), ::toupper);
    std::transform(s2.begin(), s2.end(), s2.begin(), ::toupper);

    // compute Levenshtein distance measure between both strings
    const size_t m(s1.size());
    const size_t n(s2.size());

    if (m == 0)
        return n;
    if (n == 0)
        return m;

    size_t *costs = new size_t[n + 1];

    for (size_t k = 0; k <= n; k++)
        costs[k] = k;

    size_t i = 0;
    for (std::string::const_iterator it1 = s1.begin(); it1 != s1.end(); ++it1, ++i)
    {
        costs[0] = i + 1;
        size_t corner = i;

        size_t j = 0;
        for (std::string::const_iterator it2 = s2.begin(); it2 != s2.end(); ++it2, ++j)
        {
            size_t upper = costs[j + 1];
            if (*it1 == *it2)
            {
                costs[j + 1] = corner;
            }
            else
            {
                size_t t(upper < corner ? upper : corner);
                costs[j + 1] = (costs[j] < t ? costs[j] : t) + 1;
            }

            corner = upper;
        }
    }

    int result = costs[n];
    delete[] costs;

    return result;
}