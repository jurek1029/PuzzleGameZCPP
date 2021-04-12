#pragma once
#include "Tile.h"
class TileTextured : public Tile
{
	sf::Sprite sprite;
	float tile_size;
public:
	TileTextured(sf::Texture* texture, sf::Font* tile_font, int size_n, int _position_x = 0, int _position_y = 0, int value = 0);
	void Draw(sf::RenderWindow* window, int deltaTime);
};

