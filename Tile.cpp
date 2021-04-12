#include "Tile.h"
#include "Constants.h"
#include <SFML/Graphics.hpp>


Tile::Tile(sf::Font* tile_font, int _size_n, int _position_x, int _position_y, int _value)
{
	position_x = _position_x;
	position_y = _position_y;
	value = _value;
	size_n = _size_n;

	tile = sf::RectangleShape(sf::Vector2f(TILE_SIZE_PX, TILE_SIZE_PX));
	tile.setFillColor(TILE_COLOR_NOT_READY);

	if (tile_font != nullptr) {
		text.setFont(*tile_font);
		text.setString(std::to_string(value));
		text.setCharacterSize(TILE_CHARACTER_SIZE);
		text.setFillColor(TILE_FONT_COLOR);
		sf::FloatRect textRect = text.getLocalBounds();
		text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		drawText = true;
	}
	else {
		drawText = false;
	}

}

bool Tile::IsReal()
{
	return value != -1;
}

int Tile::GetVal()
{
	return value;
}

bool Tile::CorrectPosition()
{
	return value == position_y * size_n + position_x && draw_time_acc >= TILE_ANIMATION_TIME_USEC;
}

int Tile::Lerp(float start, float end, float t, float duration)
{
	return (start * (1 - t / duration) + end * (t / duration)) * (t < duration) + end * (t > duration);
}

void Tile::Draw(sf::RenderWindow* window, int deltaTime)
{
	int posx = position_x * (TILE_SIZE_PX + TILE_PADDING_PX) + TILE_PADDING_PX;
	int posy = position_y * (TILE_SIZE_PX + TILE_PADDING_PX) + TILE_PADDING_PX;
	draw_time_acc += deltaTime;
	draw_pos_x = Lerp(start_draw_pos_x, posx, draw_time_acc, TILE_ANIMATION_TIME_USEC);
	draw_pos_y = Lerp(start_draw_pos_y, posy, draw_time_acc, TILE_ANIMATION_TIME_USEC);

	tile.setPosition(draw_pos_x, draw_pos_y);
	tile.setFillColor(CorrectPosition() ? TILE_COLOR_READY : TILE_COLOR_NOT_READY);
	window->draw(tile);
	if (drawText) {
		text.setPosition(draw_pos_x + TILE_TEXT_PADDING_PX, draw_pos_y + TILE_TEXT_PADDING_PX);
		window->draw(text);
	}
}

void Tile::Move(int x, int y)
{
	draw_time_acc = 0;
	start_draw_pos_x = position_x * (TILE_SIZE_PX + TILE_PADDING_PX) + TILE_PADDING_PX;
	start_draw_pos_y = position_y * (TILE_SIZE_PX + TILE_PADDING_PX) + TILE_PADDING_PX;
	position_x = x;
	position_y = y;
	
}