#pragma once

#include "precomp.h"
#include "globals.h"
#include "particle.h"

class ForceGenerator
{
public:
	ForceGenerator() {};

	void registerClass(Particle* classPointer);
	void deregisterClass(Particle* classPointer);

	virtual void compute() = 0;

protected:
	std::vector<Particle*> _registeredClasses;
};