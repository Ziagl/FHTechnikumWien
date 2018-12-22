#pragma once

// position on a 2d map
class Position2D {
public:
	float _x, _y;		// coordinates

	Position2D() { _x = 0; _y = 0; };
	Position2D(float x, float y) { _x = x; _y = y; }

	// compare operators
	bool operator==(Position2D &p) { return((p._x == _x) && (p._y == _y)); }
	bool operator!=(Position2D &p) { return !operator==(p); }
};