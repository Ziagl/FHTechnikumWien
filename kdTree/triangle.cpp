#include "mesh.h"

TRIANGLE::TRIANGLE(float x, float y, float z)
{
	vertices[0].position.Set(x, y, z);
	vertices[1].position.Set(x + 2.0f, y, z - 2.0f);
	vertices[2].position.Set(x + 1.0f, y + 2.0f, z - 1.0f);
	_intersect = false;
	_tested = false;
}

TRIANGLE::TRIANGLE(Point p0, Point p1, Point p2)
{
	vertices[0].position.Set(p0.x, p0.y, p0.z);
	vertices[1].position.Set(p1.x, p1.y, p1.z);
	vertices[2].position.Set(p2.x, p2.y, p2.z);
	_intersect = false;
	_tested = false;
}

TRIANGLE::~TRIANGLE(void)
{
}

void TRIANGLE::draw()
{
	glBegin(GL_TRIANGLES);
	glColor3f(0.0f, 0.0f, 1.0f);
	if (_intersect)
	{
		glColor3f(0.0f, 1.0f, 0.0f);
		_intersect = false;
		_tested = false;
	}
	if (_tested)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		_tested = false;
	}
	glVertex3f(vertices[0].position.x, vertices[0].position.y, vertices[0].position.z);
	glVertex3f(vertices[1].position.x, vertices[1].position.y, vertices[1].position.z);
	glVertex3f(vertices[2].position.x, vertices[2].position.y, vertices[2].position.z);
	glEnd();
}

void TRIANGLE::getAABB(AABB &box)
{
	float min_x, min_y, min_z;
	float max_x, max_y, max_z;

	min_x = vertices[0].position.x;
	min_y = vertices[0].position.y;
	min_z = vertices[0].position.z;

	max_x = vertices[0].position.x;
	max_y = vertices[0].position.y;
	max_z = vertices[0].position.z;

	for (int i = 1; i < 3; ++i)
	{
		if (vertices[i].position.x < min_x)
			min_x = vertices[i].position.x;
		if (vertices[i].position.y < min_y)
			min_y = vertices[i].position.y;
		if (vertices[i].position.z < min_z)
			min_z = vertices[i].position.z;
		if (vertices[i].position.x > max_x)
			max_x = vertices[i].position.x;
		if (vertices[i].position.y > max_y)
			max_y = vertices[i].position.y;
		if (vertices[i].position.z > max_z)
			max_z = vertices[i].position.z;
	}

	box.mMin = Point(min_x, min_y, min_z);
	box.mMax = Point(max_x, max_y, max_z);
}

int check_same_clock_dir(VECTOR3D pt1, VECTOR3D pt2, VECTOR3D pt3, VECTOR3D norm)
{
	float testi, testj, testk;
	float dotprod;
	// normal of trinagle
	testi = (((pt2.y - pt1.y)*(pt3.z - pt1.z)) - ((pt3.y - pt1.y)*(pt2.z - pt1.z)));
	testj = (((pt2.z - pt1.z)*(pt3.x - pt1.x)) - ((pt3.z - pt1.z)*(pt2.x - pt1.x)));
	testk = (((pt2.x - pt1.x)*(pt3.y - pt1.y)) - ((pt3.x - pt1.x)*(pt2.y - pt1.y)));

	// Dot product with triangle normal
	dotprod = testi * norm.x + testj * norm.y + testk * norm.z;

	//answer
	if (dotprod < 0) return 0;
	else return 1;
}


//1. Normale des Dreiecks finden und prüfen ob Ray parallel dazu
//	--> negativ --> ray schneidet Front Face
//2. Testen ob Punkt im Dreieck
//	--> Punkt mit jeweils 2 Punkten des Dreiecks testen
//	--> muss auch im Uhrzeigersinn sein
bool TRIANGLE::intersect(const Ray &r)
{
	/* if ( _mesh )
	   _mesh->markTested(true);
	 _tested = true;*/

	float V1x, V1y, V1z;
	float V2x, V2y, V2z;
	VECTOR3D norm;
	float dotprod;
	float t;

	// vector form triangle pt1 to pt2
	V1x = vertices[1].position.x - vertices[0].position.x;
	V1y = vertices[1].position.y - vertices[0].position.y;
	V1z = vertices[1].position.z - vertices[0].position.z;

	// vector form triangle pt2 to pt3
	V2x = vertices[2].position.x - vertices[1].position.x;
	V2y = vertices[2].position.y - vertices[1].position.y;
	V2z = vertices[2].position.z - vertices[1].position.z;

	// vector normal of triangle
	norm.x = V1y * V2z - V1z * V2y;
	norm.y = V1z * V2x - V1x * V2z;
	norm.z = V1x * V2y - V1y * V2x;

	dotprod = norm.x*r.dir.x + norm.y*r.dir.y + norm.z*r.dir.z;

	if (dotprod < 0)
	{
		t = -(norm.x*(r.orig.x - vertices[0].position.x) + norm.y*(r.orig.y - vertices[0].position.y) + norm.z*(r.orig.z - vertices[0].position.z)) /
			(norm.x*r.dir.x + norm.y*r.dir.y + norm.z*r.dir.z);

		// if ds is neg line started past triangle so can't hit triangle.
		//if(t < 0) return false;

		VECTOR3D pt_int;
		pt_int.x = r.orig.x + r.dir.x*t;
		pt_int.y = r.orig.y + r.dir.y*t;
		pt_int.z = r.orig.z + r.dir.z*t;

		if (check_same_clock_dir(vertices[0].position, vertices[1].position, pt_int, norm) == 1)
		{
			if (check_same_clock_dir(vertices[1].position, vertices[2].position, pt_int, norm) == 1)
			{
				if (check_same_clock_dir(vertices[2].position, vertices[0].position, pt_int, norm) == 1)
				{
					_intersect = true;
					/*if ( _mesh ) {
					  _mesh->markIntersect(true);
					}*/
					// answer in pt_int is insde triangle
					return true;
				}
			}
		}
	}

	return false;
}
