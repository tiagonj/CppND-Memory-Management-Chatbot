#include "graphedge.h"
#include "graphnode.h"

GraphNode::GraphNode(int id)
{
    _id = id;
}

GraphNode::~GraphNode()
{
    //// STUDENT CODE
    ////

    // Task 0: ChatBot is being deleted several times. Let's
    // temporarily circumvent this by not deleting it from
    // GraphNode, until we have proper memory management in place
    //delete _chatBot;

    // Task 4: Make outgoing (child) edges an exclusive resource of GraphNode
    // Note that outgoing (child) edges are now automatically 
    // deleted as part of the tear-down of this GraphNode

    ////
    //// EOF STUDENT CODE
}

void GraphNode::AddToken(std::string token)
{
    _answers.push_back(token);
}

void GraphNode::AddParentEdge(GraphEdge *edge)
{
    _parentEdges.push_back(edge);
}

//// STUDENT CODE
////

// Task 4: Make outgoing (child) edges an exclusive resource of GraphNode
// Child edges are now owned by the GraphNode object (not ChatLogic)
void GraphNode::AddChildEdge(std::unique_ptr<GraphEdge>&& edge)
{
    _childEdges.emplace_back(std::move(edge));
}

void GraphNode::MoveChatbotHere(ChatBot *chatbot)
{
    _chatBot = chatbot;
    _chatBot->SetCurrentNode(this);
}

void GraphNode::MoveChatbotToNewNode(GraphNode *newNode)
{
    newNode->MoveChatbotHere(_chatBot);
    _chatBot = nullptr; // invalidate pointer at source
}
////
//// EOF STUDENT CODE

GraphEdge *GraphNode::GetChildEdgeAtIndex(int index)
{
    //// STUDENT CODE
    ////

    return _childEdges[index].get();

    ////
    //// EOF STUDENT CODE
}