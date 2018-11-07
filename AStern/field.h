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

	void setTile(clan::Image* tile);
	clan::Image* getTile();
	int getValue();
private:
	clan::Image* _tile;		// visualization
	int _value;				// value
};
