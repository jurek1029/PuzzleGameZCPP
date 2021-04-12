#pragma once
#include <stdlib.h> 
#include <vector>
#include "Solver.h"

class Node
{
public:
    Solver board;
    std::pair<int, int> action;
    int cost;
    Node* parent;
    int estimate;

    Node();
    Node(const Node& n);
    Node(Solver _board, std::pair<int, int> _action, int _cost, Node* _parent);
  
    std::vector<Node*> Expand();

    bool operator==(const Node& n)const;
    bool operator<(const Node& n)const;
    bool operator>(const Node& n)const;
 
};

//bool operator==(const Node& a, const Node& b) {
//}


