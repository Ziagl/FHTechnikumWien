#pragma once

class MapNode
{
public:
	MapNode() {};

	Position2D _pos;
	float _pheromones;
	bool _possible;
};
