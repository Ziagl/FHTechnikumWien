#pragma once

#include "matrix.h"

class Intersectable;
class Material;

/** Class representing a ray/object intersection point.
*	All the necessary information about the intersection is computed
*	by the intersected object and stored in this class.
*/
class Intersection
{
public:
	const Intersectable* mObject;	///< Pointer to the object hit by the ray.
	Material* mMaterial;			///< Pointer to the material at the hit point.
	Point mPosition;				///< Position of hit point in world coordinates.
	Vector mNormal;					///< Normal at hit point.
	Vector mView;					///< View direction at the hit point.
	UV mTexture;					///< Texture coordinate (u,v) at hit point.
	UV mHitParam;					///< Parametric description of hit point (u,v).
	float mHitTime;					///< Hit time along ray.

public:
	Intersection() : mObject(0), mMaterial(0) { }
};