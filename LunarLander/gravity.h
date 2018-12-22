#pragma once

#include "forcegenerator.h"

class Gravity :public ForceGenerator
{
public:
	Gravity() {};

	void compute()
	{
		for (std::vector<Particle*>::iterator it = _registeredClasses.begin();
			it != _registeredClasses.end();
			++it)
		{
			(*it)->addForce(_gravity);
		}
	}

	double _gravity;
};