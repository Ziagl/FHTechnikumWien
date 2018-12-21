#pragma once

#include "phy_particle.h"

class FireworksParticle :public Particle
{
public:
	FireworksParticle() {};
	FireworksParticle(float x, float y, float maxlifetime)
	{
		_pos._x = x;
		_pos._y = y;
		_maxlifetime = _lifetime = maxlifetime;
		_r = 1.0f;
		_g = 1.0f;
		_b = 0.0f;
		_active = false;
		_maxtimer = _explosionstimer = TIMER;
	};

	void update(unsigned int time)		// Partikel updaten (Lebenszeit anpassen)
	{
		integrate(time);
		float scaled_time = static_cast<float>(time*TIME_SCALING);
		_explosionstimer -= time;
		_lifetime -= scaled_time;
		if (_lifetime < 0.0f)
		{
			_lifetime = 0.0f;
		}
	};

	float _lifetime;		// Lebenszeit des Partikels
	float _maxlifetime;
	float _r;
	float _g;
	float _b;
	float _size;
	int _tiefe;
	bool _active;
	int _explosionstimer;
	int _maxtimer;
};