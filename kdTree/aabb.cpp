#include "globals.h"
#include "ray.h"
#include "aabb.h"


/** Default constructor, initializing the box to the degenerate case
*	of minimum=+inf, maximum=-inf. This way the box works as expected
*	with the include() function even if it is empty at first.
*/
AABB::AABB()
	: mMin(INF)
	, mMax(-INF)
{
}


/** Constructor initializing the box to a single points. */
AABB::AABB(const Point& p)
{
	init(p);
}


/** Constructor initializing the box to include two points. */
AABB::AABB(const Point& p1, const Point& p2)
{
	init(p1);
	include(p2);
}


/** Constructor initializing the box to include three points. */
AABB::AABB(const Point& p1, const Point& p2, const Point& p3)
{
	init(p1);
	include(p2);
	include(p3);
}


/** Destructor. */
AABB::~AABB()
{
}


/** Initializes the bounding box to a single point. */
void AABB::init(const Point& p)
{
	mMin = mMax = p;
}


/** Sets the bounding box to the union of the current box and the point p. */
void AABB::include(const Point& p)
{
	mMin.x = std::min(mMin.x, p.x);
	mMin.y = std::min(mMin.y, p.y);
	mMin.z = std::min(mMin.z, p.z);
	mMax.x = std::max(mMax.x, p.x);
	mMax.y = std::max(mMax.y, p.y);
	mMax.z = std::max(mMax.z, p.z);
}


/** Sets the bounding box to the union of the current box and the box b. */
void AABB::include(const AABB& b)
{
	mMin.x = std::min(mMin.x, b.mMin.x);
	mMin.y = std::min(mMin.y, b.mMin.y);
	mMin.z = std::min(mMin.z, b.mMin.z);
	mMax.x = std::max(mMax.x, b.mMax.x);
	mMax.y = std::max(mMax.y, b.mMax.y);
	mMax.z = std::max(mMax.z, b.mMax.z);
}


/** Grow the box by a distance d in all directions. */
void AABB::grow(float d)
{
	mMin -= d;
	mMax += d;
}


/** Returns the volume of the bounding box. */
float AABB::getVolume() const
{
	Vector d = mMax - mMin;
	return d.x * d.y * d.z;
}


/** Returns the surface area of the bounding box. */
float AABB::getArea() const
{
	Vector d = mMax - mMin;
	return 2.0f * (d.x*d.y + d.x*d.z + d.y*d.z);
}

/** Returns the index of the largest axis in the range [0,2]. */
int AABB::getLargestAxis() const
{
	Vector d = mMax - mMin;
	return (d.x > d.y && d.x > d.z) ? 0 : ((d.y > d.z) ? 1 : 2);
}


/** Performs ray/box intersection. Returns true if the ray
*	intersects the box, and the hit time for the entry/exit
*	in tmin/tmax respectively.
*/
bool AABB::intersect(const Ray& ray, float& tmin, float& tmax) const
{
	float t0 = ray.minT;
	float t1 = ray.maxT;

	// Loop over the three axes and compute the hit time for the
	// two axis-aligned bounding box planes in each, decreasing the
	// parametric range of the ray until start>end time, which means
	// the ray missed the box, or until we finish which means there
	// is an intersection.
	for (int i = 0; i < 3; i++)
	{
		float invDir = 1.0f / ray.dir(i);
		float tNear = (mMin(i) - ray.orig(i)) * invDir;
		float tFar = (mMax(i) - ray.orig(i)) * invDir;
		if (tNear > tFar) swap(tNear, tFar);

		if (tNear > t0) t0 = tNear;
		if (tFar < t1) t1 = tFar;
		if (t0 > t1) return false;
	}

	tmin = t0;
	tmax = t1;
	return true;
}