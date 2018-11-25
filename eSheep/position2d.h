#pragma once

#include "globals.h"

// position on a 2D map
class Position2D {
public:
	int _x, _y;		// coordinates

	Position2D() { _x = 0; _y = 0; };
	Position2D(int x, int y) { _x = x; _y = y; }

	const Position2D neighbour(const int d) const {
		if (_y % 2 == 0)
			return Position2D(_x + xofsEven[d], _y + yofs[d]);
		else
			return Position2D(_x + xofs[d], _y + yofs[d]);
	}

	// compare operators
	const bool operator==(const Position2D &p) const { return((p._x == _x) && (p._y == _y)); }
	const bool operator!=(const Position2D &p) const { return !operator==(p); }
};
