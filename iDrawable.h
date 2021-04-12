#pragma once
#include <SFML/Graphics.hpp>
class iDrawable
{
public:
	virtual void Draw(sf::RenderWindow* window, int deltaTime) = 0;
};