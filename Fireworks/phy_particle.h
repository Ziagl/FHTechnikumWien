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
		_force.x = 0.0f;
		_force.y = 0.0f;
	};

	//Get/Set
	clan::Vec2f getForce() { return _force; };
	void setForce(clan::Vec2f force) { _force.x = force.x; _force.y = force.y; };
	void resetForce() { _force.x = 0.0f; _force.y = 0.0f; };
	void addForce(float x, float y) { _force.x += x; _force.y += y; };
	void addForce(clan::Vec2f force) { _force.x += force.x; _force.y += force.y; };

	void integrate(unsigned int time) {
		float scaled_time = static_cast<float>(time*TIME_SCALING);
		_speed._y += (_force.y*_mass)*scaled_time;			// force / mass = acceleration
		_speed._x += (_force.x*_mass)*scaled_time;			// force / mass = acceleration
		_pos._y += _speed._y;								// use speed vector on position
		_pos._x += _speed._x;
	};

	float _mass;
	Position2D _pos;
	Position2D _speed;
protected:
	// all physical values
	clan::Vec2f _force;
};