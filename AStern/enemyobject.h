#pragma once

#include "gameobject.h"
#include "astar.h"

class EnemyObject :public GameObject
{
	friend class GameObject;
public:
	EnemyObject(void);
	EnemyObject(clan::Canvas &canvas, clan::Image* tile, clan::Image* pathtile, Position2D &pos);
	~EnemyObject(void);

	void setWaypoints(Position2D start, Position2D ziel);
	bool hasPath();

	Position2D _start;
	Position2D _ziel;

	int _agression;
};
