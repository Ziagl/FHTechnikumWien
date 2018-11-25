#pragma once

#include "globals.h"
#include "field.h"				// for field class

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
	void changeTile(Position2D pos);

	Field _fields[LEVEL_WIDTH][LEVEL_HEIGHT];			// all tiles of a map
	std::map<std::string,clan::Image*> _tilesMap;		// all images used by this map
	int isValid(Position2D &p);

	void growGras();
	void resetSheep();
	void resetSheep(Position2D pos);
	void setSheep(Position2D pos);
	void resetDirt();
	void resetDirt(Position2D pos);
	void addDirt(Position2D pos, int value);
	int getValue(int x, int y){return _buffer[x][y][_activeBuffer];}
	void setValue(int x, int y, int value){_buffer[x][y][_activeBuffer]=value;}

	clan::Font* _font;
private:
	void parseTiles(char identifier, Position2D position);
	void readTiles();
	void generateRandomMap();
	bool hasNeighbour(int x, int y);

	int irand( int a, int b);

	int _dirt[LEVEL_WIDTH][LEVEL_HEIGHT];
	int _buffer[LEVEL_WIDTH][LEVEL_HEIGHT][2];
	int _activeBuffer;
	bool _sheeps[LEVEL_WIDTH][LEVEL_HEIGHT];
	clan::Canvas _canvas;
};
