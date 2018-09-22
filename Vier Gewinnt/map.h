#pragma once

#include "precomp.h"
#include "globals.h"
#include "field.h"

class Map
{
public:
	Map(clan::Canvas &canvas);
	~Map(void);
	void render();							// render map
	int getWinner();						// check if there is a winner
	Field _field;
private:
	void readTiles();

	clan::Canvas _canvas;
	std::vector<clan::Image*> _tilesVec;	// all map images
};