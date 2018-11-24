#include "ant.h"

void Ant::move(Map* map, Position2D goal)
{
	float max_pheromones = 0.0f;
	bool wallcollision = false;

	// if q is not already set, estimate path length (manhattan)
	if (_q < 0.0f)
	{
		_q = static_cast<float>(abs(goal._x - _pos._x) + abs(goal._y - _pos._y));
	}

	// check all neighbours
	for (int i = 0; i < NEIGHBOURS; ++i)
	{
		neighbours[i]._pos = _pos.neighbour(i);
		if (map->isValid(neighbours[i]._pos))
		{
			neighbours[i]._possible = true;
			neighbours[i]._pheromones = static_cast<float>(map->_pheromones[neighbours[i]._pos._x][neighbours[i]._pos._y] * WEIGHT_PHEROMONES);
		}
		else
		{
			neighbours[i]._possible = false;
			neighbours[i]._pheromones = 0.0f;
			wallcollision = true;
		}
	}

	// pheromones favor choose of next tile, but also current direction
	// if this is not first move, change direction with given propability
	if (_lastDir != -1 && wallcollision == false)
	{
		neighbours[wrap_around(_lastDir - 3, NEIGHBOURS)]._pheromones *= WEIGHT_DIRECTION;		// streight 100%
		neighbours[wrap_around(_lastDir - 4, NEIGHBOURS)]._pheromones *= 0.6f*WEIGHT_DIRECTION;	// left 60%
		neighbours[wrap_around(_lastDir - 2, NEIGHBOURS)]._pheromones *= 0.6f*WEIGHT_DIRECTION;	// right 60%
		neighbours[wrap_around(_lastDir, NEIGHBOURS)]._pheromones *= 0.01f*WEIGHT_DIRECTION;	// other 10%
		neighbours[wrap_around(_lastDir - 1, NEIGHBOURS)]._pheromones *= 0.1f*WEIGHT_DIRECTION;
		neighbours[wrap_around(_lastDir + 1, NEIGHBOURS)]._pheromones *= 0.1f*WEIGHT_DIRECTION;
	}

	// optimization: do not go to tile that was already visited
	for (std::list<Position2D>::iterator it = _path.begin();
		it != _path.end();
		++it)
	{
		for (int i = 0; i < NEIGHBOURS; i++)
			if (neighbours[i]._pos == (*it))
			{
				neighbours[i]._possible = false;
				neighbours[i]._pheromones = 0.0f;
			}
	}

	// get max. pheromones
	for (int i = 0; i < NEIGHBOURS; ++i)
	{
		max_pheromones += neighbours[i]._pheromones;
	}

	// optimization: stop if there is no path
	if (max_pheromones < 0.0001f)
	{
		clan::Console::write_line("Abbruch");
		_reachedGoal = true;
	}
	else
	{
		// randomize
		float randomValue = irand(1, 100) / 100.0f * max_pheromones;
		int nextTile = NEIGHBOURS - 1;						// next position
		for (int i = 0; i < NEIGHBOURS; ++i)
		{
			// only for positions that on the map
			if (neighbours[i]._possible)
			{
				randomValue -= neighbours[i]._pheromones;
				if (randomValue < 0)
				{
					nextTile = i;
					break;
				}
			}
		}

		_path.push_back(neighbours[nextTile]._pos);
		_pos = neighbours[nextTile]._pos;
		_lastDir = wrap_around(nextTile - 3, NEIGHBOURS);
	}

	// optimization: stop if ant moves too long
	if (_path.size() >= PATH_MAX)
	{
		clan::Console::write_line("Abbruch");
		_reachedGoal = true;
	}
	// reached goal?
	if (_pos == goal)
	{
		map->computePheromones();

		float q_l = _q / _path.size();

		// if ant reached goal, then place its pheromone path to map
		for (std::list<Position2D>::iterator it = _path.begin();
			it != _path.end();
			++it)
		{
			// optimization: we do not add one on each tile, we use Q/L
			// Q: estimated lenght of path
			// L: moved tiles
			map->addPheromones((*it), q_l);
		}

		_reachedGoal = true;
	}
}

int Ant::wrap_around(int a, int b)
{
	if (a < 0)
		return a + b;
	if (a >= b)
		return a - b;
	return a;
}

int Ant::irand(int a, int b) {
	double r = b - a + 1;
	return a + static_cast<int>(r * rand() / (RAND_MAX + 1.0));
}
