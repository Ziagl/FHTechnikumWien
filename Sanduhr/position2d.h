#pragma once

#include <math.h>
#define PI (3.141592653589793)

//Klasse für die Position auf einer 2D Karte
class Position2D {
public:
	int _x, _y; //Koordinaten

	Position2D() { _x = 0; _y = 0; };
	Position2D(int x, int y) { _x = x; _y = y; }

	double getAngle(Position2D &other)
	{
		double a = _x * other._x + _y * other._y;
		double b = sqrt(static_cast<double>(_x*_x + _y * _y)) * sqrt(static_cast<double>(other._x*other._x + other._y*other._y));
		if (b != 0.0f)
		{
			if ((_y<0 && other._x>0) || (_y>0 && other._x<0))
				return (2 * PI) - acos(a / b);
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
		//Performancesteigerung --> Wurzel weglassen
		return /*sqrt(*/static_cast<double>(_x*_x + _y * _y)/*)*/;
	}

	//Addieren
	Position2D operator+(Position2D &other) { return Position2D(_x + other._x, _y + other._y); }
	Position2D operator-(Position2D &other) { return Position2D(_x - other._x, (-_y) - (-other._y)); }

	//Vergleichsoperatoren
	bool operator==(Position2D &p) { return((p._x == _x) && (p._y == _y)); }
	bool operator!=(Position2D &p) { return !operator==(p); }
};