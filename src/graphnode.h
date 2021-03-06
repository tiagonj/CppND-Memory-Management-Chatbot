#ifndef GRAPHNODE_H_
#define GRAPHNODE_H_

#include <memory>
#include <vector>
#include <string>
#include "chatbot.h"


// forward declarations
class GraphEdge;

class GraphNode
{
private:
    //// STUDENT CODE
    ////

    // Task 4: Make outgoing (child) edges an exclusive resource of GraphNode
    // GraphNode now owns and manages the memory for outgoing (child) edges

    // data handles (owned)
    std::vector<std::unique_ptr<GraphEdge>> _childEdges;  // edges to subsequent nodes

    // data handles (not owned)
    std::vector<GraphEdge *> _parentEdges; // edges to preceding nodes

    // Task 5: ChatBot is owned by one GraphNode (it is Moved between nodes)
    // There is only one actual ChatBot at any given time, which means that for
    // all nodes bar one their ChatBot object member variable (_chatBot) will
    // not contain valid information. The one which does contain the actual
    // chat bot is the node where the chat bot is located at that point in time.
    ChatBot _chatBot;

    ////
    //// EOF STUDENT CODE

    // proprietary members
    int _id;
    std::vector<std::string> _answers;

public:
    // constructor / destructor
    GraphNode(int id);
    ~GraphNode();

    // getter / setter
    int GetID() { return _id; }
    int GetNumberOfChildEdges() { return _childEdges.size(); }
    GraphEdge *GetChildEdgeAtIndex(int index);
    std::vector<std::string> GetAnswers() { return _answers; }
    int GetNumberOfParents() { return _parentEdges.size(); }

    // proprietary functions
    void AddToken(std::string token); // add answers to list
    void AddParentEdge(GraphEdge *edge);
    void AddChildEdge(std::unique_ptr<GraphEdge>&& edge);

    //// STUDENT CODE
    ////

    // Task 5: ChatBot is owned by one GraphNode (it is Moved between nodes)
    void MoveChatbotHere(ChatBot chatbot);

    ////
    //// EOF STUDENT CODE

    void MoveChatbotToNewNode(GraphNode *newNode);
};

#endif /* GRAPHNODE_H_ */