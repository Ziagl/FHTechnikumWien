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
