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
	void render();							// renders a map
	void readLevelFile(std::string);		// read a level file from disk

	void clearPheromones();									// clear pheromones
	void addPheromones(Position2D pos, float value);
	void computePheromones();

	void changeTile(Position2D pos);

	// variables
	Field _fields[LEVEL_WIDTH][LEVEL_HEIGHT];			// all tiles of a map
	float _pheromones[LEVEL_WIDTH][LEVEL_HEIGHT];
	std::map<std::string, clan::Image*> _tilesMap;		// all images used by this map

	int isValid(Position2D &p);
private:
	void parseTiles(char identifier, Position2D position);
	void readTiles();

	//Variablen
	clan::Canvas _canvas;
	clan::Font* _font;

	float _maxPheromones;
};
