#include <iostream>
#include <random>
#include <algorithm>
#include <ctime>

#include "chatlogic.h"
#include "graphnode.h"
#include "graphedge.h"
#include "chatbot.h"

// constructor WITHOUT memory allocation
ChatBot::ChatBot()
{
    // invalidate data handles
    _image = NULL;
    _currentNode = nullptr;
    _chatLogic = nullptr;
    _rootNode = nullptr;
}

// constructor WITH memory allocation
ChatBot::ChatBot(std::string filename)
{
    std::cout << "ChatBot Constructor" << std::endl;
    
    // invalidate data handles
    _currentNode = nullptr;
    _chatLogic = nullptr;
    _rootNode = nullptr;

    // load image into heap memory
    _image = new wxBitmap(filename, wxBITMAP_TYPE_PNG);
}

ChatBot::~ChatBot()
{
    std::cout << "ChatBot Destructor" << std::endl;

    // deallocate heap memory
    if(_image != NULL) // Attention: wxWidgets used NULL and not nullptr
    {
        delete _image;
        _image = NULL;
    }
}

//// STUDENT CODE
////

// Task 2: ChatBot class must comply with the Rule of Five

/*
NOTE: The wxWidgets documentation for the wxBitmap class 
(https://docs.wxwidgets.org/3.0/classwx_bitmap.html) states that the copy constructor
(https://docs.wxwidgets.org/3.0/classwx_bitmap.html#abfaa21ec563a64ea913af918150db900)
uses reference counting (https://docs.wxwidgets.org/3.0/overview_refcount.html), and
provides example code for when the developer wishes to make an actual copy.

In fact, according to the reference counting documentation (link above) the wxBitmap
class (as well as several other clases) effectively function as a shared pointer:

> Many wxWidgets objects use a technique known as reference counting, also known as
> copy on write (COW). This means that when an object is assigned to another, no 
> copying really takes place. Only the reference count on the shared object data is 
> incremented and both objects share the same data (a very fast operation).

and

> When a COW object destructor is called, it may not delete the data: if it's shared,
> the destructor will just decrement the shared data's reference count without 
> destroying it. Only when the destructor of the last object owning the data is 
> called, the data is really destroyed. Just like all other COW-things, this happens 
> transparently to the class users so that you shouldn't care about it.

If the above were not the case, one could have been tempted to wrap the wxBitmap 
(_image) member with a shared_ptr to ensure that this resource was
  a) only allocated (on the heap) once (for efficiency),
  b) was thus shared between all instances of ChatBot, and
  c) was correctly destroyed (once) once all ChatBot instances are themselves 
     destroyed.

However, given that wxBitmap already provides shared_ptr-like behaviour we will
take advantage of this and simply "copy" _image by employing its copy constructor.

*/

// Copy constructor
ChatBot::ChatBot(ChatBot& other)
{
    std::cout << "ChatBot Copy Constructor" << std::endl;

    if (&other != this) // No-op if placement-new is used on itself
    {
        this->_image = 
            (other._image == NULL) ?
                NULL : new wxBitmap(*other._image); // See NOTE above

        this->_currentNode = other._currentNode;
        this->_rootNode = other._rootNode;
        this->_chatLogic = other._chatLogic;
    }
}

// Move constructor
ChatBot::ChatBot(ChatBot&& other)
{
    std::cout << "ChatBot Move Constructor" << std::endl;

    if (&other != this) // No-op if placement-new is used on itself
    {
        this->_image = other._image;
        this->_currentNode = other._currentNode;
        this->_rootNode = other._rootNode;
        this->_chatLogic = other._chatLogic;

        // Invalidate internals of remaining "moved" ChatBot object
        other._image = NULL;
        other._currentNode = nullptr;
        other._rootNode = nullptr;
        other._chatLogic = nullptr;
    }
}

// Copy assignment operator
ChatBot& ChatBot::operator=(ChatBot& other)
{
    std::cout << "ChatBot Copy Assignment Operator" << std::endl;

    if (&other != this) // No-op if called on itself
    {
        this->_image = 
            (other._image == NULL) ?
                NULL : new wxBitmap(*other._image); // See NOTE above

        this->_currentNode = other._currentNode;
        this->_rootNode = other._rootNode;
        this->_chatLogic = other._chatLogic;
    }
    return *this;
}

// Move assignment operator
ChatBot& ChatBot::operator=(ChatBot&& other)
{
    std::cout << "ChatBot Move Assignment Operator" << std::endl;

    if (&other != this) // No-op if called on itself
    {
        this->_image = other._image;
        this->_currentNode = other._currentNode;
        this->_rootNode = other._rootNode;
        this->_chatLogic = other._chatLogic;

        // Invalidate internals of remaining "moved" ChatBot object
        other._image = NULL;
        other._currentNode = nullptr;
        other._rootNode = nullptr;
        other._chatLogic = nullptr;
    }
    return *this;
}

////
//// EOF STUDENT CODE

void ChatBot::ReceiveMessageFromUser(std::string message)
{
    // loop over all edges and keywords and compute Levenshtein distance to query
    typedef std::pair<GraphEdge *, int> EdgeDist;
    std::vector<EdgeDist> levDists; // format is <ptr,levDist>

    for (size_t i = 0; i < _currentNode->GetNumberOfChildEdges(); ++i)
    {
        GraphEdge *edge = _currentNode->GetChildEdgeAtIndex(i);
        for (auto keyword : edge->GetKeywords())
        {
            EdgeDist ed{edge, ComputeLevenshteinDistance(keyword, message)};
            levDists.push_back(ed);
        }
    }

    // select best fitting edge to proceed along
    GraphNode *newNode;
    if (levDists.size() > 0)
    {
        // sort in ascending order of Levenshtein distance (best fit is at the top)
        std::sort(levDists.begin(), levDists.end(), [](const EdgeDist &a, const EdgeDist &b) { return a.second < b.second; });
        newNode = levDists.at(0).first->GetChildNode(); // after sorting the best edge is at first position
    }
    else
    {
        // go back to root node
        newNode = _rootNode;
    }

    // tell current node to move chatbot to new node
    _currentNode->MoveChatbotToNewNode(newNode);
}

void ChatBot::SetCurrentNode(GraphNode *node)
{
    // update pointer to current node
    _currentNode = node;

    // Task 5: ChatBot is owned by one GraphNode (it is Moved between nodes)
    // We need to let ChatLogic know that this object is now the current
    // valid ChatBot object
    _chatLogic->SetChatbotHandle(this);

    // select a random node answer (if several answers should exist)
    std::vector<std::string> answers = _currentNode->GetAnswers();
    std::mt19937 generator(int(std::time(0)));
    std::uniform_int_distribution<int> dis(0, answers.size() - 1);
    std::string answer = answers.at(dis(generator));

    // send selected node answer to user
    _chatLogic->SendMessageToUser(answer);
}

int ChatBot::ComputeLevenshteinDistance(std::string s1, std::string s2)
{
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