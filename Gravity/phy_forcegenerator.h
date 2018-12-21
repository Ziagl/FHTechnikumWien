#pragma once

#include "globals.h"
#include "phy_particle.h"

class ForceGenerator
{
public:
	ForceGenerator() {};

	void registerClass(Particle * classPointer);
	void deregisterClass(Particle *classPointer);

	int irand(int a, int b) {
		double r = b - a + 1;
		return a + static_cast<int>(r * rand() / (RAND_MAX + 1.0));
	}

	virtual void compute() = 0;

	//protected:
	std::list<Particle*> _registeredClasses;
};