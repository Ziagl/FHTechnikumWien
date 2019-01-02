#pragma once

#include "globals.h"
#include "aabb.h"
#include "ray.h"
#include "matrix.h"

// forward declaration of Mesh class
class Mesh;

class TRIANGLE
{
public:
	TRIANGLE(float x, float y, float z);
	TRIANGLE(Point p0, Point p1, Point p2);
	~TRIANGLE(void);

	void getAABB(AABB &box);
	bool intersect(const Ray &r);

	void draw();

	TRIANGLE_VERTEX vertices[3];
	bool _intersect;
	bool _tested;
	Mesh* _mesh;
};