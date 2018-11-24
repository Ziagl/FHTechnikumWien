#pragma once

#include "globals.h"
#include "position2d.h"

// field class represents a single game field
class Field
{
public:
	Field(void);
	Field(clan::Image* tile, int value);
	~Field(void);
	void setTile(clan::Image* tile) { _tile = tile; }
	clan::Image* getTile() { return _tile; }
	int getValue() { return _value; }
	void setValue(int value) { _value = value; }
private:
	clan::Image* _tile;		// visualization
	int _value;				// value
};
