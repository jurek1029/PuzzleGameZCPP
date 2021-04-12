#include "Node.h"
#include "Solver.h"
Node::Node()
{
    board = NULL;
    action = std::pair<int, int>(-1, -1);
    cost = 0;
    parent = NULL;
    estimate = 0;
}
Node::Node(const Node& n)
{
    board = n.board;
    action = n.action;
    cost = n.cost;
    parent = n.parent;
    estimate = n.estimate;
}


Node::Node(Solver _board, std::pair<int, int> _action, int _cost, Node* _parent)
{
    board = _board;
    action = _action;
    cost = _cost;
    parent = _parent;
    estimate = cost + board.h();
}

/// <summary>
/// Returns all nodes that can be explored from current node.
/// </summary>
/// <returns>Vector of pointers to all Nodes that can be reached from this Node by the rules of the game</returns>
std::vector<Node*> Node::Expand(){
    std::vector<Node*> nodes;
    std::vector<std::pair<int, int>> actions = board.Actions();
    for (auto it = actions.begin(); it != actions.end(); ++it) {
        Solver s = board;
        s.ApplayAction(it->first, it->second);
        nodes.push_back(new Node(s, *it, cost + 1, this));
    }
    return nodes;
}


bool Node::operator==(const Node& n)const
{
    return board == n.board;
}

bool Node::operator<(const Node& n)const
{
    return estimate < n.estimate;
}

bool Node::operator>(const Node& n)const 
{
    return estimate > n.estimate;
}