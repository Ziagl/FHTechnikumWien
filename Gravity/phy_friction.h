#pragma once

#include "phy_forcegenerator.h"

class Friction :public ForceGenerator
{
public:
	Friction(float mat) { _material = mat; };

	void compute()
	{
		for (std::list<Particle*>::iterator it = _registeredClasses.begin();
			it != _registeredClasses.end();
			++it)
		{
			if ((*it)->_friction == true) {
				//Kraft holen
				clan::Vec2f force = (*it)->getForce();
				//float mass = (*it)->_mass;
				double angle = LINE_ANGLE;
				force = force.rotate(force, clan::Vec2f(0.0f, 0.0f), clan::Angle(clan::PI / 2 - angle, clan::AngleUnit::angle_radians));
				force *= sin(angle);
				force *= _material;
				(*it)->setForce(force);
			}
		}
	}
private:
	float _material;
};