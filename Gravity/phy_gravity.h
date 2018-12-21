#pragma once

#include "phy_forcegenerator.h"

class Gravity :public ForceGenerator
{
public:
	Gravity(clan::Vec2f force) { _force = force; };

	void compute()
	{
		for (std::list<Particle*>::iterator it = _registeredClasses.begin();
			it != _registeredClasses.end();
			++it)
		{
			(*it)->addForce(_force);
		}
	}

private:
	clan::Vec2f _force;
};