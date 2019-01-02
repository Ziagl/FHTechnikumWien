#pragma once

#include "matrix.h"

class Ray;

/** Class representing an axis-aligned bounding box (AABB). */
class AABB
{
public:
	Point mMin;			///< Minimum.
	Point mMax;			///< Maximum.

public:
	AABB();
	AABB(const Point& p);
	AABB(const Point& p1, const Point& p2);
	AABB(const Point& p1, const Point& p2, const Point& p3);
	virtual ~AABB();

	void init(const Point& p);
	void include(const Point& p);
	void include(const AABB& b);
	void grow(float d);
	float getVolume() const;
	float getArea() const;
	int getLargestAxis() const;
	bool intersect(const Ray& r, float& tmin, float& tmax) const;
};