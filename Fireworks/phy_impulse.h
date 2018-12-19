#pragma once

#include "phy_forcegenerator.h"

class Impulse :public ForceGenerator
{
public:
	Impulse(clan::Vec2f force) { _force = force; };

	void compute()
	{
		// compute all registered particles
		while (_registeredClasses.size() > 0)
		{
			(*_registeredClasses.begin())->addForce(_force);
			_registeredClasses.pop_front();
		}
	}
private:
	clan::Vec2f _force;
};