#pragma once

#include <iostream>
#include <stdexcept>
#include <cmath>
#include <algorithm>

class Point;
class Vector;
class Matrix;

/** Class representing a 4x4 transformation matrix. */
class Matrix
{
public:
	/// Default constructor. The matrix is initialized to identity.
	Matrix() { identity(); }

	/// Constructor initializing all elements to the given value.
	explicit Matrix(float s) { for (int i = 0; i < 16; i++) e[i] = s; }

	/// Destructor. Does nothing.
	virtual ~Matrix() { }

	/// Assignment from a scalar value.
	Matrix& operator=(float s) { for (int i = 0; i < 16; i++) e[i] = s; return *this; }

	/// Returns a copy of the element at position row,column (i,j).
	/// @remark The range of the parameters (i,j) is not checked.
	float operator()(int i, int j) const { return e[4 * i + j]; }

	/// Returns a reference to the element at position row,column (i,j).
	/// @remark The range of the parameters (i,j) is not checked.
	float& operator()(int i, int j) { return e[4 * i + j]; }

	/// Clear all elements to zero.
	Matrix& zero() { for (int i = 0; i < 16; i++) e[i] = 0.0f; return *this; }

	/// Set matrix to identity.
	Matrix& identity()
	{
		e[0] = 1.0f;	e[1] = 0.0f;	e[2] = 0.0f;	e[3] = 0.0f;
		e[4] = 0.0f;	e[5] = 1.0f;	e[6] = 0.0f;	e[7] = 0.0f;
		e[8] = 0.0f;	e[9] = 0.0f;	e[10] = 1.0f;	e[11] = 0.0f;
		e[12] = 0.0f;	e[13] = 0.0f;	e[14] = 0.0f;	e[15] = 1.0f;
		return *this;
	}

	/// Setup scaling matrix.
	Matrix& scale(float sx, float sy, float sz) { identity(); e[0] = sx; e[5] = sy; e[10] = sz; return *this; }

	/// Setup translation matrix.
	Matrix& translation(float tx, float ty, float tz) { identity(); e[3] = tx; e[7] = ty; e[11] = tz; return *this; }

	/// Setup rotation matrix around the x-axis, where the rotation angle is given in degrees.
	Matrix& rotX(float rx);

	/// Setup rotation matrix around the y-axis, where the rotation angle is given in degrees.
	Matrix& rotY(float ry);

	/// Setup rotation matrix around the z-axis, where the rotation angle is given in degrees.
	Matrix& rotZ(float rz);

	/// Addition of two matrices.
	Matrix operator+(const Matrix& m) const { Matrix r; for (int i = 0; i < 16; i++) r.e[i] = e[i] + m.e[i]; return r; }

	/// Addition of two matrices in place.
	Matrix& operator+=(const Matrix& m) { for (int i = 0; i < 16; i++) e[i] += m.e[i]; return *this; }

	/// Subtraction of two matrices.
	Matrix operator-(const Matrix& m) const { Matrix r; for (int i = 0; i < 16; i++) r.e[i] = e[i] - m.e[i]; return r; }

	/// Subtraction of two matrices in place.
	Matrix& operator-=(const Matrix& m) { for (int i = 0; i < 16; i++) e[i] -= m.e[i]; return *this; }

	/// Multiplication of sclar by matrix.
	friend Matrix operator*(float s, const Matrix& m) { Matrix r; for (int i = 0; i < 16; i++) r.e[i] = s * m.e[i]; return r; }

	/// Multiplication of matrix by scalar.
	Matrix operator*(float s) const { Matrix r; for (int i = 0; i < 16; i++) r.e[i] = s * e[i]; return r; }

	/// Multiplication by scalar in place.
	Matrix& operator*=(float s) { for (int i = 0; i < 16; i++) e[i] *= s; return *this; }

	/// Matrix/point multiplication. The point is translated by the last column.
	Point operator*(const Point& p) const;

	/// Matrix/vector multiplication. The vector is not translated by the last column.
	Vector operator*(const Vector& v) const;

	/// Matrix multiplication.
	Matrix operator*(const Matrix& m) const
	{
		Matrix r(0);
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				for (int k = 0; k < 4; k++)
					r(i, j) += (*this)(i, k) * m(k, j);
		return r;
	}

	/// Matrix multiplication in place.
	Matrix& operator*=(const Matrix& m)
	{
		Matrix r(0);
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				for (int k = 0; k < 4; k++)
					r(i, j) += (*this)(i, k) * m(k, j);
		return (*this = r);
	}

	/// Compute the determinant of the matrix.
	float determinant() const;

	/// Compute the inverse of the matrix.
	Matrix inverse() const;

	/// Write matrix elements to an output stream.
	friend std::ostream& operator<<(std::ostream& os, const Matrix& A);

protected:
	float e[16];			///< The matrix elements.

	friend class Vector;
	friend class Point;
};





/** Class representing a 3D vector. */
class Vector
{
public:
	float x;		///< First component.
	float y;		///< Second component.
	float z;		///< Third component.

public:
	/// Default constructor. Initializes vector to (0,0,0).
	Vector() : x(0.0f), y(0.0f), z(0.0f) { }

	/// Constructor initializing all elements to the given value.
	explicit Vector(float s) : x(s), y(s), z(s) { }

	/// Constructor initializing vector to (x,y,z).
	Vector(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }

	/// Constructor initializing the vector from a point.
	Vector(const Point& p);

	/// Destructor. Does nothing.
	virtual ~Vector() { }

	/// Returns a copy of the element at position (i).
	/// @remark The range of the parameter is not checked.
	float operator()(int i) const { return (&x)[i]; }

	/// Returns a reference to the element at position (i).
	/// @remark The range of the parameter is not checked.
	float& operator()(int i) { return (&x)[i]; }

	/// Addition of two vectors.
	Vector operator+(const Vector& v) const { return Vector(x + v.x, y + v.y, z + v.z); }

	/// Addition of two vectors in place.
	Vector& operator+=(const Vector& v) { x += v.x; y += v.y; z += v.z; return *this; }

	/// Negation of vector.
	Vector operator-() const { return Vector(-x, -y, -z); }

	/// Subtraction of two vectors.
	Vector operator-(const Vector& v) const { return Vector(x - v.x, y - v.y, z - v.z); }

	/// Subtraction of two vectors in place.
	Vector& operator-=(const Vector& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }

	/// Multiplication of scalar by vector.
	friend Vector operator*(float s, const Vector& v) { return Vector(s*v.x, s*v.y, s*v.z); }

	/// Multiplication of vector by scalar.
	Vector operator*(float s) const { return Vector(s*x, s*y, s*z); }

	/// Multiplication by scalar in place.
	Vector& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }

	/// Division of vector by scalar.
	Vector operator/(float s) const { s = 1.0f / s; return Vector(s*x, s*y, s*z); }

	/// Division by scalar in place.
	Vector& operator/=(float s) { s = 1.0f / s; x *= s; y *= s; z *= s; return *this; }

	/// Division of scalar by vector.
	friend Vector operator/(float s, const Vector& v) { return Vector(s / v.x, s / v.y, s / v.z); }

	/// Dot product of two vectors.
	float dot(const Vector& v) const { return x * v.x + y * v.y + z * v.z; }

	/// Dot product of two vectors (short form).
	float operator*(const Vector& v) const { return x * v.x + y * v.y + z * v.z; }

	/// Cross product of two vectors.
	Vector cross(const Vector& v) const { return Vector(y*v.z - z * v.y, z*v.x - x * v.z, x*v.y - y * v.x); }

	/// Cross product of two vectors (short form).
	Vector operator%(const Vector& v) const { return Vector(y*v.z - z * v.y, z*v.x - x * v.z, x*v.y - y * v.x); }

	/// Returns length of vector.
	float length() const { return std::sqrt(x*x + y * y + z * z); }

	/// Returns square length of vector.
	float length2() const { return x * x + y * y + z * z; }

	/// Normalize vector to unit length.
	Vector& normalize() { float l = 1.0f / length(); x *= l; y *= l; z *= l; return *this; }

	/// Write elements to an output stream.
	friend std::ostream& operator<<(std::ostream& os, const Vector& A);
};





/** Class representing a 3D point.
*	The main difference between Point and Vector is how they are transformed
*	by a transformation matrix. The point is assumed to have the 4th component
*	set to 1, so that it is affected by translations, while the vector just
*	represents a direction and hence is not affected by translations.
*/
class Point
{
public:
	float x;		///< First component.
	float y;		///< Second component.
	float z;		///< Third component.

public:
	/// Default constructor. Initializes point to (0,0,0).
	Point() : x(0.0f), y(0.0f), z(0.0f) { }

	/// Constructor initializing all elements to the given value.
	explicit Point(float s) : x(s), y(s), z(s) { }

	/// Constructor initializing point to (x,y,z).
	Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }

	/// Destructor. Does nothing.
	virtual ~Point() { }

	/// Returns a copy of the element at position (i).
	/// @remark The range of the parameter is not checked.
	float operator()(int i) const { return (&x)[i]; }

	/// Returns a reference to the element at position (i).
	/// @remark The range of the parameter is not checked.
	float& operator()(int i) { return (&x)[i]; }

	/// Addition of point and vector.
	Point operator+(const Vector& v) const { return Point(x + v.x, y + v.y, z + v.z); }

	/// Addition of two points. Not mathematically sensible but convenient to have.
	Point operator+(const Point& p) const { return Point(x + p.x, y + p.y, z + p.z); }

	/// Addition of scalar in place.
	Point& operator+=(float s) { x += s; y += s; z += s; return *this; }

	/// Subtraction of point and vector.
	Point operator-(const Vector& v) const { return Point(x - v.x, y - v.y, z - v.z); }

	/// Subtraction of two points. Not mathematically sensible but convenient to have.
	Point operator-(const Point& p) const { return Point(x - p.x, y - p.y, z - p.z); }

	/// Subtraction of scalar in place.
	Point& operator-=(float s) { x -= s; y -= s; z -= s; return *this; }

	/// Write elements to an output stream.
	friend std::ostream& operator<<(std::ostream& os, const Point& A);
};





/** Class representing a texture coordinate (u,v) pair. */
class UV
{
public:
	float u;		///< First texture coordinate (u).
	float v;		///< Second texture coordinate (v).

public:
	/// Default constructor. Initializes the coordinate to (0,0).
	UV() : u(0), v(0) { }

	/// Constructor initializing all elements to the given value.
	explicit UV(float s) : u(s), v(s) { }

	/// Constructor initializing the coordinate to (u,v).
	UV(float _u, float _v) : u(_u), v(_v) { }

	/// Destructor. Does nothing.
	virtual ~UV() { }

	/// Addition of two uv coordinates.
	UV operator+(const UV& c) const { return UV(u + c.u, v + c.v); }

	/// Multiplication of scalar by uv coordinate.
	friend UV operator*(float s, const UV& c) { return UV(s*c.u, s*c.v); }

	/// Multiplication of uv coordinate by scalar.
	UV operator*(float s) { return UV(s*u, s*v); }
};