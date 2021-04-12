#pragma once
#include "iDrawable.h"
class Tile : public iDrawable
{
protected:
	int position_x, position_y;
	int draw_pos_x, draw_pos_y, start_draw_pos_x = 0, start_draw_pos_y = 0;
	int draw_time_acc = 0;
	int value;
	sf::RectangleShape tile;
	sf::Text text;
	int size_n;
	bool drawText = false;
	int Tile::Lerp(float start, float end, float t, float duration);

public:
	Tile() {}
	Tile(sf::Font* tile_font, int size_n, int _position_x = 0, int _position_y = 0, int value = 0);
	bool CorrectPosition();
	void Move(int x, int y);
	virtual void Draw(sf::RenderWindow* window, int deltaTime);
	int GetVal();
	bool IsReal();
};

