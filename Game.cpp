#include <SFML/Graphics.hpp>
#include <cmath>
#include <QMessageBox>
#include "Game.h"
#include "Constants.h"
#include "Background.h"
#include "Tile.h"
#include "TileTextured.h"
#include "MainMenu.h"
#include "Solver.h"

/// <summary>
/// Initialization function, that is required to start a game.
/// </summary>
/// <param name="size">size of a board to generate</param>
/// <param name="_menu">pointer to menu that is running the game</param>
void Game::Init(int size, MainMenu* _menu) {
    size_n = size;
    menu = _menu;
    isSolving = false;
    moves.start = nullptr;

    //Load Font 
    if (menu->isWithNumbers()) {
        if (!tile_font.loadFromFile(FONT_PATH)) {
            error.showMessage(MSG_FONT_NOT_FOUND);
            return;
        }
    }
    //Load image texture
    if (menu->isWithImg()) {
        if (!texture.loadFromFile(menu->GetImgPath())) {
            error.showMessage(MSG_IMG_NOT_FOUND);
            return;
        }
    }

    //Create SFML window 
    int windowWith = TILE_SIZE_PX * size_n + TILE_PADDING_PX * (size_n + 1);
    if (menu->isWithImg()) windowWith -= TILE_PADDING_PX;
    window = new sf::RenderWindow(sf::VideoMode(windowWith, windowWith), TITLE);


    //Initialize board with tiles
    board = new Tile**[size_n];
    for (int i = 0; i < size_n; ++i) {
        board[i] = new Tile*[size_n];
        for (int j = 0; j < size_n; ++j) {
            if (!(i == size_n - 1 && j == size_n - 1))
            {
                if(menu->isWithImg())
                    board[i][j] = new TileTextured(&texture , menu->isWithNumbers()? &tile_font : nullptr, size_n, j, i, i * size_n + j);
                else
                    board[i][j] = new Tile(&tile_font, size_n, j, i, i * size_n + j);
            }
            else
            {
                board[i][j] = new Tile(&tile_font, size_n, -1, -1, -1); // create Empty tile
                free_position_x = j;
                free_position_y = i;
            }
        }
    }

    //Initialize d_objects with drawable objects
    d_objects.push_back(new Background(windowWith, bg_r, bg_g, bg_b));
    for (int i = 0; i < size_n; ++i) {
        for (int j = 0; j < size_n; ++j) {
            d_objects.push_back(board[i][j]);
        }
    }
    //Shoufle Board
    Randomise();

    time_start = stoper.getElapsedTime(); 
    time_last_anim_frame_milisec = time_start.asMilliseconds();
    sf::Clock stoper;
    inited = true;
    
    //Strat main game loop
    Game::MainLoop();
}

/// <summary>
/// Main loop of the game, all events and drawing are done here 
/// </summary>
void Game::MainLoop()
{
    int time_frame;
    while (window->isOpen())
    {
        //Check for key presses 
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window->close();
            else if (event.type == sf::Event::KeyPressed){
                if (!isSolving) {
                    if (event.key.code == sf::Keyboard::Left) {
                        OnKeyPressLeft();
                    }
                    else if (event.key.code == sf::Keyboard::Right) {
                        OnKeyPressRight();
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        OnKeyPressDown();
                    }
                    else if (event.key.code == sf::Keyboard::Up) {
                        OnKeyPressUp();
                    }
                }

                if (event.key.code == sf::Keyboard::S) {
                    if(size_n < 4)
                        FindSolution();
                    else
                        FindSolutionFromRNG();
                    isSolving = true;
                }
            }
        } 

        //if solving is on, start moving tiles one at a time 
        if (isSolving) {
            if (stoper.getElapsedTime().asMilliseconds() - time_last_anim_frame_milisec > SOLUTOIN_AMINATION_FRAME_TIME_MILISEC) {
                time_last_anim_frame_milisec = stoper.getElapsedTime().asMilliseconds();
                if (size_n < 4)
                    SolveStepByStep();
                else 
                    SolveStepByStepFromRNG();
            }
        }


        //clear window to redraw everything
        window->clear();

        time_frame = stoper.getElapsedTime().asMicroseconds() - time_last_frame_usec;
        //draw every drawable object
        for (auto it = std::begin(d_objects); it != std::end(d_objects); ++it) {
            (*it)->Draw(window, time_frame);
        }

        window->display();

        //check if all tiles are in the right place 
        if (DidGameFinished()) {
            QuitWon();
        }

        time_last_frame_usec = stoper.getElapsedTime().asMicroseconds();
    } 
}

/// <summary>
/// Randomly suffles the board by preforming n^3 random moves, and saves the moves that it took to randomise the board.
/// </summary>
void Game::Randomise()
{
    srand(time(0));
    int rng_move;
    for (int i = 0; i < size_n * size_n * size_n; i++) {
        rng_move = rand() % 4;
        
        switch (rng_move) {
        case 0:// left   
            if (OnKeyPressLeft()) //moves.Add(-1);
            break;
        case 1: //right
            if (OnKeyPressRight())// moves.Add(1);
            break;
        case 2: //down
            if (OnKeyPressDown())// moves.Add(-2);
            break;
        case 3: //up
            if (OnKeyPressUp())// moves.Add(2);
            break;
        }
    }
}

/// <summary>
/// Swaps first tile with second tile in 'board' 
/// </summary>
/// <param name="firstX">x position of first tile</param>
/// <param name="firstY">y position of first tile</param>
/// <param name="secondX">x position of second tile</param>
/// <param name="secondY">y position of second tile</param>
void Game::SwapBoardTiles(int firstX, int firstY, int secondX, int secondY)
{
    Tile* t = board[firstY][firstX];
    board[firstY][firstX] = board[secondY][secondX];
    board[secondY][secondX] = t;
}

/// <summary>
/// If its possible moves right neighbour of the free tile to the left, and updates current position of the empty/free tile.
/// </summary>
/// <returns>If it was a possilbe to do the move</returns>
bool Game::OnKeyPressLeft()
{
    if (free_position_x + 1 < size_n) {
        board[free_position_y][free_position_x + 1]->Move(free_position_x,free_position_y);
        SwapBoardTiles(free_position_x, free_position_y,free_position_x + 1, free_position_y);
        free_position_x += 1;
        if(!isSolving) moves.Add(-1);
        return true;
    }
    return false;
}

/// <summary>
/// If its possible moves left neighbour of the free tile to the right, and updates current position of the empty/free tile.
/// </summary>
/// <returns>If it was a possilbe to do the move</returns>
bool Game::OnKeyPressRight()
{
    if (free_position_x - 1 >= 0) {
        board[free_position_y][free_position_x - 1]->Move(free_position_x, free_position_y);
        SwapBoardTiles(free_position_x, free_position_y, free_position_x - 1, free_position_y);
        free_position_x -= 1;
        if (!isSolving) moves.Add(1);
        return true;
    }
    return false;
}

/// <summary>
/// If its possible moves top neighbour of the free tile down, and updates current position of the empty/free tile.
/// </summary>
/// <returns>If it was a possilbe to do the move</returns>
bool Game::OnKeyPressDown()
{
    if (free_position_y - 1 >= 0) {
        board[free_position_y - 1][free_position_x]->Move(free_position_x, free_position_y);
        SwapBoardTiles(free_position_x, free_position_y, free_position_x, free_position_y - 1);
        free_position_y -= 1;
        if (!isSolving) moves.Add(-2);
        return true;
    }
    return false;
}

/// <summary>
/// If its possible moves down neighbour of the free tile up, and updates current position of the empty/free tile.
/// </summary>
/// <returns>If it was a possilbe to do the move</returns>
bool Game::OnKeyPressUp()
{
    if (free_position_y + 1 < size_n) {
        board[free_position_y + 1][free_position_x]->Move(free_position_x, free_position_y);
        SwapBoardTiles(free_position_x, free_position_y, free_position_x, free_position_y + 1);
        free_position_y += 1;
        if (!isSolving) moves.Add(2);
        return true;
    }
    return false;
}

/// <summary>
/// Checks every tile if its in the right position
/// </summary>
/// <returns>True if every tile is in the right position</returns>
bool Game::DidGameFinished()
{
    for (int i = 0; i < size_n; ++i) {
        for (int j = 0; j < size_n; ++j) {
            if (board[i][j]->IsReal())
                if (!board[i][j]->CorrectPosition()) return false;
        }
    }
    return true;
}

/// <summary>
/// Displays time that took to finish the game.
/// Updates current best time for this size.
/// Closes the SFML window.
/// </summary>
void Game::QuitWon()
{
    time_stop = stoper.getElapsedTime();
    int time = time_stop.asMilliseconds() - time_start.asMilliseconds();
    QMessageBox message;
    int sec, min;
    sec = time / 1000 % 60;
    min = time / 60000;
    std::string text = MSG_WON.toStdString();
    text += std::to_string(min);
    text += ":";
    text += std::to_string(sec);
    text += ":";
    text += std::to_string(time % 1000);
    message.setText(text.c_str());
    int ret = message.exec();

    menu->UpdateBestTimes(size_n, time);
    window->close();
}

/// <summary>
/// Finds solution using A* algorithm
/// </summary>
void Game::FindSolution() {
    Solver s(size_n, board, free_position_x, free_position_y);
    solution_moves = s.GetSolution();
}

/// <summary>
/// Finds solution from random shuffle that this board recices, by removing redundant moves.
/// </summary>
void Game::FindSolutionFromRNG() {
   
    ListNode* node = moves.start;
    while (node->next != nullptr) {
        if (node->value + node->next->value == 0) { //if 2 next nodes are complementary
            if (node->prev != nullptr) {
                node->prev->next = node->next->next;
                if (node->next->next != nullptr)
                    node->next->next->prev = node->prev;
                else // node->next was end node
                    moves.end = node->prev;
                delete node->next;
                ListNode* temp = node->prev;
                delete node;
                node = temp;
                continue;
            }
            else { // node is starting node
                moves.start = node->next->next;
                moves.start->prev = nullptr;
                delete node->next;
                delete node;
                node = moves.start;
                continue;
            }
        }
        node = node->next;
    }
}

/// <summary>
/// Preform one move from A* solution.
/// </summary>
void Game::SolveStepByStep() {
    if (solution_moves.empty()) return;
    std::pair<int,int> move = solution_moves.front();
    solution_moves.pop_front();
    int x = move.first - free_position_x;
    int y = move.second - free_position_y;

    if (x > 0) OnKeyPressLeft();
    else if (x < 0) OnKeyPressRight();
    else if (y > 0) OnKeyPressUp();
    else if (y < 0) OnKeyPressDown();
}
/// <summary>
/// Perform one step from reverse random solution
/// </summary>
void Game::SolveStepByStepFromRNG() {
    if (moves.start == nullptr) return;
    ListNode* move = moves.Pop();

    if (move->value == -1) OnKeyPressRight();
    else if (move->value == 1) OnKeyPressLeft();
    else if (move->value == -2) OnKeyPressUp();
    else if (move->value == 2) OnKeyPressDown();
}

Game::~Game()
{
    //delete window;
    if (inited) {
        for (int i = 0; i < size_n; ++i) {
            for (int j = 0; j < size_n; ++j)
                delete board[i][j];
            delete[] board[i];
        }
        delete[] board;
    }
}