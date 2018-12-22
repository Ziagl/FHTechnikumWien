#pragma once

#include "globals.h "
#include "position2d.h"

class Particle
{
public:
	Particle() {
		_speed = Position2D(0.0f, 0.0f);
		_pos = Position2D(0.0f, 0.0f);
		_mass = 1.0;
		_force = 0.0;
	};

	//Get/Set
	double getForce() { return _force; };
	void setForce(double force) { _force = force; };
	void addForce(double force) { _force += force; };

	void integrate(unsigned int time) {
		double scaled_time = time * TIME_SCALING;
		_speed._y += (_force*_mass)*scaled_time;
		_pos._y += _speed._y;
	};

	double _mass;
	Position2D _pos;
	Position2D _speed;
private:
	double _force;
};