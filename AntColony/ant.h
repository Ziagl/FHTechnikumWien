#pragma once

#include "globals.h"
#include "position2d.h"
#include "gameobject.h"
#include "node.h"
#include "map.h"

class Ant :public GameObject
{
public:
	Ant(clan::Canvas &canvas, clan::Image* tile, clan::Image* pathtile, Position2D &pos) :GameObject(canvas, tile, pathtile, pos) {
		srand(SEED);
		reset();
	};

	void move(Map* map, Position2D pos);

	int irand(int a, int b);	// random value
	int wrap_around(int a, int b);

	void reset() {
		_lastDir = -1;
		_q = -1.0f;
		_reachedGoal = false;
		_path.clear();
		_pheromones_old = 0.0f;
	}

	bool _reachedGoal;
private:
	std::list<Position2D> _path;
	int _lastDir;						// last direction (ant should not change direction often, never 180°)
	float _q;
	float _pheromones_old;
	MapNode neighbours[NEIGHBOURS];
};
