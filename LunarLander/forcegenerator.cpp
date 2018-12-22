#include "forcegenerator.h"

void ForceGenerator::registerClass(Particle *classPointer)
{
	_registeredClasses.push_back(classPointer);
}

void ForceGenerator::deregisterClass(Particle *classPointer)
{
	for (std::vector<Particle*>::iterator it = _registeredClasses.begin();
		it != _registeredClasses.end();
		++it)
	{
		if ((*it) == classPointer)
		{
			_registeredClasses.erase(it);
			break;
		}
	}
}