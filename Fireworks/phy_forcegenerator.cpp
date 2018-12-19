#include "phy_forcegenerator.h"

void ForceGenerator::registerClass(Particle *classPointer)
{
	_registeredClasses.push_back(classPointer);
}

void ForceGenerator::deregisterClass(Particle *classPointer)
{
	/*_registeredClasses.remove(classPointer);*/
	for (std::list<Particle*>::iterator it = _registeredClasses.begin();
		it != _registeredClasses.end();
		++it)
	{
		if ((*it) == classPointer)
		{
			it = _registeredClasses.erase(it);
			return;
		}
	}
}