#pragma once

#include "phy_forcegenerator.h"

class Explosion :public ForceGenerator
{
public:
	Explosion(int min, int max) {
		_min = min;
		_max = max;
	};

	void compute()
	{
		//alle registrierte Partikel abarbeiten
		while (_registeredClasses.size() > 0)
		{
			//zufällige Kraft in eine Richtung
			clan::Vec2f force;
			int angle = irand(0, 360);
			force.x = 0.0f;
			force.y = 1.0f * irand(_min, _max);
			force = force.rotate(clan::Vec2f(0.0f, 0.0f), clan::Angle::from_radians(static_cast<float>(angle)));

			(*_registeredClasses.begin())->addForce(force);
			_registeredClasses.pop_front();
		}
	}
private:
	int _min;
	int _max;
};