#pragma once

// store 2d position
class Position2D {
public:
	int _x, _y;		// coordinates

	Position2D() { _x = 0; _y = 0; };
	Position2D(int x, int y) { _x = x; _y = y; }

	bool operator==(Position2D &p) { return((p._x == _x) && (p._y == _y)); }
	bool operator!=(Position2D &p) { return !operator==(p); }
};