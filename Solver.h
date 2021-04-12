#pragma once
#include <stdlib.h> 
#include "Tile.h"
#include <deque>

class Solver
{

    int size, sizeSq;
    int* values;
    int empty_x, empty_y;
public:
    Solver(int _size = 3, Tile*** board = NULL, int ex = 0, int ey = 0);
    Solver(const Solver& s);
    ~Solver();
    bool operator==(const Solver& s) const;

    int h() const;
    void ApplayAction(int x, int y);
    std::vector<std::pair<int, int>> Actions();
    std::deque<std::pair<int, int>> GetSolution();



};

