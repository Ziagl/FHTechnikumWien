#pragma once

#include "globals.h"
#include "field.h"				// for field class
#include "gameobject.h"			// for start and goal

// class map represents a 2D map of tiles
// with MaxX, MaxY tiles
// a map can be loaded from a file (ascii txt)
class Map
{
public:
	Map(clan::Canvas &canvas);
	~Map(void);
	void render();									// renders a map
	void readLevelFile(std::string file);			// read a level file from disk

	void clearPotentials();											// clear potential fields
	void setPotentials(Position2D pos, int maxValue, int deep);		// create potential fields around position

	// variables
	Field _fields[LEVEL_WIDTH][LEVEL_HEIGHT];			// all tiles of a map
	int _potentials[LEVEL_WIDTH][LEVEL_HEIGHT];
	std::map<std::string, clan::Image*> _tilesMap;		// all images used by this map
private:
	void parseTiles(char identifier, Position2D position);
	void readTiles();
	int isValid(Position2D &p);

	clan::Canvas _canvas;
};
