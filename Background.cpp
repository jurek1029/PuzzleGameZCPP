#include "Background.h"
#include <SFML/Graphics.hpp>

Background::Background(int _size, int _color_r, int _color_g, int _color_b)
{
    size = _size;
    color_r = _color_r;
    color_g = _color_g;
    color_b = _color_b;
    bg = sf::RectangleShape(sf::Vector2f(size, size));
    bg.setPosition(0, 0);
    bg.setFillColor(sf::Color(color_r, color_g, color_b));
}

void Background::Draw(sf::RenderWindow* window, int deltaTime)
{
    window->draw(bg);
}
