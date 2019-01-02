#include "mesh.h"

Mesh::Mesh(void)
{
}

Mesh::~Mesh(void)
{
}

void Mesh::addTriangle(TRIANGLE* triangle)
{
	_vTriangles.push_back(triangle);
}

void Mesh::markTested(bool bValue)
{
	std::vector<TRIANGLE*>::iterator it = _vTriangles.begin();
	for (; it != _vTriangles.end(); ++it)
	{
		(*it)->_tested = bValue;
	}
}

void Mesh::markIntersect(bool bValue)
{
	std::vector<TRIANGLE*>::iterator it = _vTriangles.begin();
	for (; it != _vTriangles.end(); ++it)
	{
		(*it)->_intersect = bValue;
	}
}