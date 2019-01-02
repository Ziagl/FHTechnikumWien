#pragma once

#include "globals.h"
#include "matrix.h"

class Ray
{
public:
	Point orig;		//Startpounkt
	Vector dir;		//Richtungsvektor
	float minT;
	float maxT;

public:
	Ray() : minT(0.001f), maxT(INF) { }

	Ray(const Point& o, const Vector& d, float mint = 0.001f, float maxt = INF)
		: orig(o), dir(d), minT(mint), maxT(maxt) {
		dir.normalize();
	}

	~Ray() { }
};
