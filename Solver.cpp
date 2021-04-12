#pragma once
#include <stdlib.h> 
#include <queue>
#include "Solver.h"
#include "Tile.h"
#include "Node.h"


Solver::Solver(int _size, Tile*** board, int ex, int ey)
{
    size = _size;
    sizeSq = size * size;
    empty_x = ex;
    empty_y = ey;
    values = new int[sizeSq];
    int ind = 0;
    if (board != NULL) { // copy values from board to 'values'
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                values[ind] = board[i][j]->GetVal();
                ind++;
            }
        }
    }
    else { // initilialize defalut values
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                values[ind] = ind - 1;
                ind++;
            }
        }
    }
}

Solver::Solver(const Solver& s)
{
    size = s.size;
    sizeSq = s.sizeSq;
    empty_x = s.empty_x;
    empty_y = s.empty_y;
    values = new int[sizeSq];
    for (int i = 0; i < sizeSq; ++i) {
        values[i] = s.values[i];
    }
}

Solver::~Solver()
{
    //delete[] values;
}

bool Solver::operator==(const Solver& s) const
{
    //Sovers are equal if tiles on their boards are in the same positions.
    for (int i = 0; i < sizeSq; ++i) {
        if (values[i] != s.values[i]) return false;
    }
    return true;
}

/// <summary>
/// Const function of A* algorithm, that adds all distances of tiles to their destination positions. Distance is calculated as manhatan distance.
/// </summary>
/// <returns>Sum of all tile distanves to their destination</returns>
int Solver::h() const
{
    int h = 0, x, y, empty_index = empty_y*size + empty_x;
    for (int i = 0; i < sizeSq; ++i) {
        if (i == empty_index) continue;
        x = i % size;
        y = i / size;
        h += abs(x - values[i] % size) +
            abs(y - values[i] / size);
    }
    return h;
}

/// <summary>
/// Perform move on the solver board 
/// </summary>
/// <param name="x">value of movment in x axies</param>
/// <param name="y">value of movment in y axies</param>
void Solver::ApplayAction(int x, int y)
{
    if ((abs(x - empty_x) == 1) ^ (abs(y - empty_y) == 1)) {
        //swap
        values[empty_y * size + empty_x] = values[y * size + x];
        values[y * size + x] = -1;
        empty_x = x;
        empty_y = y;
    }
}

/// <summary>
/// Returns all possible actions that can be performed on current board
/// </summary>
/// <returns>Vector of all possible actions that can be performed on current board</returns>
std::vector<std::pair<int, int>> Solver::Actions()
{
    std::vector<std::pair<int, int>> actions;

    if (empty_x > 0) actions.push_back(std::pair<int, int>(empty_x - 1, empty_y));
    if (empty_y > 0) actions.push_back(std::pair<int, int>(empty_x, empty_y - 1));
    if (empty_x < size - 1) actions.push_back(std::pair<int, int>(empty_x + 1, empty_y));
    if (empty_y < size - 1) actions.push_back(std::pair<int, int>(empty_x, empty_y + 1));

    return actions;
}

/// <summary>
/// Check if node 'n' is in vector 'v'
/// </summary>
/// <param name="v">Vector of nodes to be searched in</param>
/// <param name="n">Node of be found in 'v' </param>
/// <returns>True if 'v' contains 'n' else False</returns>
bool Contains(const std::vector<Node*>& v, const Node& n)
{
    for (auto it = v.begin(); it < v.end(); ++it) {
        if (**it == n) return true;
    }
    return false;
}

/// <summary>
/// Check if node 'n' is in queue 'q'
/// </summary>
/// <param name="q">Queue of nodes to be searched in</param>
/// <param name="n">Node of be found in 'q' </param>
/// <returns>True if 'q' contains 'n' else False</returns>
bool Contains(std::priority_queue <Node*, std::vector<Node*>, std::greater<Node*>> q, Node& n)
{
    while (!q.empty()) {
        if (*q.top() == n) return true;
        q.pop();
    }
    return false;
}


/// <summary>
/// Returns solution by performing A* on posible board states. 
/// </summary>
/// <returns>Vector of moves that are pairs of translation in x,y axies</returns>
std::deque<std::pair<int, int>> Solver::GetSolution()
{
    std::priority_queue <Node*, std::vector<Node*>, std::greater<Node*>> frontier; // Nodes to explore
    std::vector<Node*> explored;
    std::deque<std::pair<int, int>> moves; // Shortest path

    frontier.push(new Node(*this, std::pair<int, int>(-1, -1), 0, NULL)); // start Node od current board.
    Node* node;

    while (true && !frontier.empty()) {

        node = frontier.top();
        frontier.pop();
        int h = node->board.h();
        if (h == 0) { // sloved
            while (node->parent != NULL) {
                moves.push_front(node->action);
                node = node->parent;
            }

            for (auto it = explored.begin(); it < explored.end(); ++it) {
                delete *it;
            }
            while (!frontier.empty()) {
                delete frontier.top();
                frontier.pop();
            }

            return moves;
        }
        else {// not solved yet
            //Get all possible moves from current board state
            std::vector<Node*> nodesToExplore = node->Expand();
            for (auto newNode = nodesToExplore.begin(); newNode < nodesToExplore.end(); ++newNode) {
                if (!Contains(explored, **newNode) && !Contains(frontier, **newNode)) { 
                    //Add move if havent been explored yet.
                    frontier.push(*newNode);
                }
            }
            explored.push_back(node);
        }

    }

    return moves;
}


