#pragma once

#include "precomp.h"
#include "globals.h"
#include "particle.h"
#include "position2d.h"

class Lander :public Particle
{
public:
	Lander() {};
	Lander(clan::Canvas &canvas);
	~Lander(void);
	void render();					// renders moon lander
	void move(unsigned int time);
	bool _engine;
	bool _landed;
	bool _save;
	//double _speed;
	double _fuel;
	double _t;
	bool registered;
private:
	clan::Canvas _canvas;
	std::vector<clan::Image*> _tilesVec;
};