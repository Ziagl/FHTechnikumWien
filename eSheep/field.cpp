#include "field.h"

Field::Field(void)
{
}

Field::Field(clan::Image* tile, int value)
{
	_tile = tile;
	_value = value;
}

Field::~Field(void)
{
}

clan::Image* Field::getTile() { return _tile; }

int Field::getValue() { return _value; }
