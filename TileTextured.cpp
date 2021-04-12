#include "TileTextured.h"
#include "Constants.h"

TileTextured::TileTextured(sf::Texture* texture, sf::Font* tile_font, int size_n, int _position_x, int _position_y, int value) 
	:Tile(tile_font,size_n,_position_x,_position_y,value)
{

	int windowWith = (TILE_SIZE_PX + TILE_PADDING_PX) * size_n;
    float tw = texture->getSize().x;
    float th = texture->getSize().y;
    float block_w = tw / size_n;
    float block_h = th / size_n;
    sprite = sf::Sprite(*texture, sf::IntRect(block_w*position_x, block_h * position_y, block_w, block_h));
	tile_size = (float)windowWith / (float)size_n;
    float sx = tile_size / block_w;
    float sy = tile_size / block_h;
    sprite.setScale(sx, sy);
}


void TileTextured::Draw(sf::RenderWindow* window, int deltaTime)
{
	int posx = position_x * (tile_size);
	int posy = position_y * (tile_size);
	draw_time_acc += deltaTime;
	int d_posx = Lerp(start_draw_pos_x, posx, draw_time_acc, TILE_ANIMATION_TIME_USEC);
	int d_posy = Lerp(start_draw_pos_y, posy, draw_time_acc, TILE_ANIMATION_TIME_USEC);

	sprite.setPosition(d_posx, d_posy);
	window->draw(sprite);

	if (drawText) {
		text.setPosition(d_posx + TILE_TEXT_PADDING_PX, d_posy + TILE_TEXT_PADDING_PX);
		window->draw(text);
	}
}