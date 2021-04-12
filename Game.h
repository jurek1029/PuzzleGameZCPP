#pragma once

#include<vector>
#include"iDrawable.h"
#include "Tile.h"
#include <SFML/Graphics.hpp>
#include <QErrorMessage>
#include <deque>
#include "List.h"

class MainMenu;

class Game
{
	int size_n;
	Tile*** board;
	int free_position_x, free_position_y;

	MainMenu* menu;
	QErrorMessage error;

	sf::Time time_start, time_stop;
	sf::Clock stoper;
	sf::RenderWindow* window;
	sf::Font tile_font;
	sf::Texture texture;
	sf::Sprite sprite;
	std::vector<iDrawable*> d_objects;

	bool inited = false;
	bool isSolving = false;

	std::deque<std::pair<int, int>> solution_moves;
	int solution_move_counter = 0;
	int time_last_anim_frame_milisec, time_last_frame_usec;
	List moves;

public:
	void Init(int size , MainMenu* menu);
	void MainLoop();
	bool DidGameFinished();
	void QuitWon();
	void Randomise();
	~Game();
private:
	void SwapBoardTiles(int firstX, int firstY, int secondX, int secondY);
	bool OnKeyPressLeft();
	bool OnKeyPressRight();
	bool OnKeyPressDown();
	bool OnKeyPressUp();
	void FindSolution();
	void FindSolutionFromRNG();
	void SolveStepByStep();
	void SolveStepByStepFromRNG();
};
