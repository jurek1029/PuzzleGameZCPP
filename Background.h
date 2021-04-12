#pragma once
#include "iDrawable.h"
class Background : public iDrawable
{
public:
	Background(int _size, int _color_r, int _color_g, int _color_b);
	void Draw(sf::RenderWindow* window, int deltaTime = 0);
private:
	sf::RectangleShape bg;
	int size;
	int color_r, color_g, color_b;
};

