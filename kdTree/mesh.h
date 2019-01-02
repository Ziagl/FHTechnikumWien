#pragma once

#include <vector>

#include "triangle.h"

class Mesh
{
public:
	Mesh(void);
	~Mesh(void);

	void addTriangle(TRIANGLE* triangle);
	void markTested(bool bValue);
	void markIntersect(bool bValue);

private:

	std::vector<TRIANGLE*> _vTriangles;

};
