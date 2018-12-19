#pragma once

#include "precomp.h"
#include "globals.h"

// position on a 2d map
class Position2D {
public:
	float _x, _y; // coordinates

	Position2D() { _x = 0; _y = 0; };
	Position2D(float x, float y) { _x = x; _y = y; }

	double getAngle(Position2D &other)
	{
		double a = _x * other._x + _y * other._y;
		double b = sqrt(static_cast<double>(_x*_x + _y * _y)) * sqrt(static_cast<double>(other._x*other._x + other._y*other._y));
		if (b != 0.0f)
		{
			if ((_y < 0 && other._x>0) || (_y > 0 && other._x < 0))
				return (2 * clan::PI) - acos(a / b);
			else
				return acos(a / b);
		}
		else
		{
			return 0.0f;
		}
	};

	double getLength()
	{
		// performance optimization --> remove squareroot
		return /*sqrt(*/static_cast<double>(_x*_x + _y * _y)/*)*/;
	}

	// add
	Position2D operator+(Position2D &other) { return Position2D(_x + other._x, _y + other._y); }
	Position2D operator-(Position2D &other) { return Position2D(_x - other._x, (-_y) - (-other._y)); }

	// compare
	bool operator==(Position2D &p) { return((p._x == _x) && (p._y == _y)); }
	bool operator!=(Position2D &p) { return !operator==(p); }
};