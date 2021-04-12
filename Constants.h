#pragma once
#include <QString>

const int TILE_SIZE_PX = 80;
const int TILE_PADDING_PX = 5;
const int TILE_ANIMATION_TIME_USEC = 5000;

const int bg_r = 91;
const int bg_g = 235;
const int bg_b = 124;

const sf::Color TILE_COLOR_NOT_READY = sf::Color(187, 246, 200);
const sf::Color TILE_COLOR_READY = sf::Color(13, 106, 35);

const int TILE_CHARACTER_SIZE = 60;
const std::string FONT_PATH = "Marlboro.ttf";
const sf::Color TILE_FONT_COLOR = sf::Color::Black;
const int TILE_TEXT_PADDING_PX = (TILE_SIZE_PX) / 2;

const std::string TITLE = "Puzzle";
const QString MSG_WON = "You won";
const QString MSG_FONT_NOT_FOUND = "Tile font not found";
const QString MSG_IMG_NOT_FOUND = "Image font not found";

const std::string TIMES_FILE_PATH = "save.bin";

const int SMALEST_GAME_POSIBLE = 2;
const int SOLUTOIN_AMINATION_FRAME_TIME_MILISEC = 200;