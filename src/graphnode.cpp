#include "graphedge.h"
#include "graphnode.h"

// Task 5: ChatBot is owned by one GraphNode (it is Moved between nodes)
// Explicit call to the no-arguments constructor for ChatBot object
GraphNode::GraphNode(int id) : _chatBot()
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

    // Task 5: ChatBot is owned by one GraphNode (it is Moved between nodes)
    // NOTE: the ChatBot object owned by this class is now automatically
    // destroyed when this destructor executes, regardless of whether the
    // chat bot is currently located in this node. If the chat bot is located
    // in another node then this node's _chatBot contains null data and its
    // destructor doesn't do anything special, other than print to cout.

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

// Task 5: ChatBot is owned by one GraphNode (it is Moved between nodes)
void GraphNode::MoveChatbotHere(std::unique_ptr<ChatBot>&& chatbot)
{
    // Transfer ownership of ChatBot to this node
    _chatBot = std::move(chatbot);

    // Note that what is being moved here (line above) is the 
    // unique_ptr ownership. The ChatBot object instance is 
    // still living in the (same memory location within the)
    // Heap.

    // We could have instead moved the ChatBot object itself
    // around but that would have required that each node have
    // a ChatBot object member (instead of a (smart) pointer)
    // in order to be able to "receive" a chat bot that is 
    // moved from a different node, and this would be a waste
    // of memory.

    _chatBot->SetCurrentNode(this);
}

void GraphNode::MoveChatbotToNewNode(GraphNode *newNode)
{
    newNode->MoveChatbotHere(std::move(_chatBot));
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