#pragma once

#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <limits>
#include <cmath>
#include "Maths/Maths.h"
#include <GL/glut.h>

struct TRIANGLE_VERTEX
{
	VECTOR3D position;
	/*float s, t;
	VECTOR3D sTangent, tTangent;
	VECTOR3D normal;
	VECTOR3D tangentSpaceLight;*/
};

/// Define pi.
#define M_PI 3.14159265358979323846

/// Define positive infinity for floats.
static const float INF = std::numeric_limits<float>::infinity();

/// Small value.
static const float epsilon = 1e-6f;


/// Returns a uniform random number in the range [0,1).
static float uniform() { return (float)std::rand() / RAND_MAX; }

/// Helper function for converting an int to a string.
static std::string int2str(int i)
{
	std::ostringstream oss;
	oss << i;
	return oss.str();
}

/// Swaps two floats.
static void swap(float& a, float& b) { float t = a; a = b; b = t; }

/// Round value to nearest integer.
#define ROUND(x) (std::floor((x)+0.5))

/// Returns the maximum of the two elements.
template<class T> T max(T a, T b) { return a > b ? a : b; }

/// Returns the minimum of the two elements.
template<class T> T min(T a, T b) { return a < b ? a : b; }