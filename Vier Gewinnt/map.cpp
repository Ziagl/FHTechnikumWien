#include "map.h"

Map::Map(clan::Canvas &canvas)
{
	_canvas = canvas;
	readTiles();
	_field.empty();			// empty field
}

Map::~Map(void)
{
}

// renders all fields
void Map::render()
{
	clan::Image* tile;

	for (int y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (int x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = _tilesVec[_field.getField(x, y)];

			if (tile != NULL) {
				tile->set_scale(TILES_SCALE_FACTOR, TILES_SCALE_FACTOR);
				tile->draw(_canvas, static_cast<float>(x*TILES_SIZE), static_cast<float>(y*TILES_SIZE) + MAP_OFFSET);
			}
		}
	}
}

void Map::readTiles(void)
{
	_tilesVec.clear();
	_tilesVec.push_back(new clan::Image(_canvas, ".\\Data\\Tiles\\empty.png"));
	_tilesVec.push_back(new clan::Image(_canvas, ".\\Data\\Tiles\\red.png"));
	_tilesVec.push_back(new clan::Image(_canvas, ".\\Data\\Tiles\\yellow.png"));
}

int Map::getWinner()
{
	// search for a 4 in row
	int red = _field.getChain4(PLAYER_COLOR);
	int yellow = _field.getChain4(AI_COLOR);

	if (red == yellow)
		return -1;
	else
	{
		if (red > yellow)
			return 0;
		else
			return 1;
	}
}
