#pragma once

#include "globals.h "
#include "position2d.h"

class Particle
{
public:
	Particle() {
		_speed.x = _speed.y = 0.0f;
		_pos = Position2D(0.0f, 0.0f);
		_oldpos = Position2D(0.0f, 0.0f);
		_mass = 1.0;
		_force.x = 0.0f;
		_force.y = 0.0f;
		_decay = 1.0f;
		//Ebenennormale berechnen
		_normale.x = 0.0f;
		_normale.y = 1.0f;
		_normale = _normale.rotate(_normale, clan::Vec2f(0, 0), clan::Angle(-LINE_ANGLE, clan::AngleUnit::angle_degrees));
		_horizontale.x = 1.0f;
		_horizontale.y = 0.0f;
		_friction = false;
	};

	//Get/Set
	clan::Vec2f getForce() { return _force; };
	void setForce(clan::Vec2f force) { _force.x = force.x; _force.y = force.y; };
	void resetForce() { _force.x = 0.0f; _force.y = 0.0f; };
	void addForce(float x, float y) { _force.x += x; _force.y += y; };
	void addForce(clan::Vec2f force) { _force.x += force.x; _force.y += force.y; };

	void integrate(unsigned int time) {
		float scaled_time = static_cast<float>(time*TIME_SCALING);
		_speed.y += (_force.y*_mass)*scaled_time;			//Kraft / Masse = Beschleunigung
		_speed.x += (_force.x*_mass)*scaled_time;			//Kraft / Masse = Beschleunigung
		_oldpos._x = _pos._x;
		_oldpos._y = _pos._y;
		_pos._y += _speed.y*_decay;								//Geschwindigkeitsvektor auf Position anwenden
		_pos._x += _speed.x*_decay;

		float y = _pos._x*asin((float)LINE_ANGLE);
		//Kollisionserkennung
		if (_pos._y > FLOOR - y && _friction != true)
		{
			_decay *= DECAY;

			//Speedvektor Normalisieren
			clan::Vec2f speed = _speed;
			speed.normalize();

			//Den Winkel dazwischen ausrechnen
			clan::Angle angle = _normale.angle(speed);
			clan::Angle normalWinkel = _horizontale.angle(_normale);
			clan::Angle speedWinkel = _horizontale.angle(speed);

			//Wenn Speed>Normal
			if (speedWinkel < normalWinkel)
			{
				angle = (clan::Angle(180, clan::AngleUnit::angle_degrees) + angle) + angle;
				angle.normalize();
			}
			else
			{
				angle = (clan::Angle(180, clan::AngleUnit::angle_degrees) - angle) - angle;
				angle.normalize();
			}
			//Allgemein
			clan::Vec2f vec = _force.rotate(_force, clan::Vec2f(0, 0), angle);
			_pos._y += vec.y;
			_pos._x += vec.x;
			if (_pos._y > FLOOR - y)
				_pos._y = FLOOR - y;
			_speed = _speed.rotate(_speed, clan::Vec2f(0, 0), angle);
			_speed.y *= _decay;
			_speed.x *= _decay;
		}
		else
		{
			if (_friction == true)
				_pos._y = FLOOR - y;
		}

		//Reibung setzen (nur wenn das Partikel in Ruhe) und noch nicht gesetzt
		if (_decay < MIN_DECAY && _friction != true)
		{
			_speed.x = 0.0f;
			_speed.y = 0.0f;
			_friction = true;
		}
	};

	float _mass;
	float _decay;
	Position2D _pos;
	Position2D _oldpos;
	clan::Vec2f _speed;
	bool _friction;
protected:
	//hier kommen physikalische Werte rein
	clan::Vec2f _force;
	clan::Vec2f _normale;
	clan::Vec2f _horizontale;
};