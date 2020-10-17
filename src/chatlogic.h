#ifndef CHATLOGIC_H_
#define CHATLOGIC_H_

#include <memory>
#include <vector>
#include <string>
#include "chatgui.h"

// forward declarations
class ChatBot;
class GraphEdge;
class GraphNode;

class ChatLogic
{
private:
    //// STUDENT CODE
    ////

    // data handles (owned)
    // Task 3: Make nodes an exclusive resource of ChatLogic
    std::vector<std::unique_ptr<GraphNode>> _nodes;

    // Task 4: Make outgoing (child) edges an exclusive resource of GraphNode
    // ChatLogic no longer needs to keep a vector of edges:
    //   a) They're not manipulated by ChatLogic, and
    //   b) As soon as one is created, a GraphEdges is moved
    //      to its new owner (a GraphNode)
    // (the declaration below must remain commented out)
    //std::vector<GraphEdge *> _edges;

    ////
    //// EOF STUDENT CODE

    // data handles (not owned)
    GraphNode *_currentNode;
    ChatBot *_chatBot;
    ChatBotPanelDialog *_panelDialog;

    // proprietary type definitions
    typedef std::vector<std::pair<std::string, std::string>> tokenlist;

    // proprietary functions
    template <typename T>
    void AddAllTokensToElement(std::string tokenID, tokenlist &tokens, T &element);

public:
    // constructor / destructor
    ChatLogic();
    ~ChatLogic();

    // getter / setter
    void SetPanelDialogHandle(ChatBotPanelDialog *panelDialog);
    void SetChatbotHandle(ChatBot *chatbot);

    // proprietary functions
    void LoadAnswerGraphFromFile(std::string filename);
    void SendMessageToChatbot(std::string message);
    void SendMessageToUser(std::string message);
    wxBitmap *GetImageFromChatbot();
};

#endif /* CHATLOGIC_H_ */