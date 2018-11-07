#include "enemyobject.h"

EnemyObject::EnemyObject(void)
{
}

EnemyObject::EnemyObject(clan::Canvas &canvas, clan::Image* tile, clan::Image* pathtile, Position2D &pos)
{
	_canvas = canvas;
	_tile = tile;
	_pathtile = pathtile;
	_pos = pos;
	_path = 0;
	_pathlength = 0;
	_pathpos = 0;
	_newpathlength = 0;
	_newpath = 0;
	_timer = 0;
	_agression = 0;
}

EnemyObject::~EnemyObject(void)
{
}

void EnemyObject::setWaypoints(Position2D start, Position2D ziel)
{
	_start = start;
	_ziel = ziel;
}

bool EnemyObject::hasPath()
{
	if (_path == 0 && _pathlength == 0)
	{
		Position2D h = _start;
		_start = _ziel;
		_ziel = h;
		return false;
	}
	return true;
}