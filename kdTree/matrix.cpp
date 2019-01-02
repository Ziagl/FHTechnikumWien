#include <iomanip>
#include "globals.h"
#include "matrix.h"

// ------------------------ Matrix functions ------------------------ //

/// Write matrix elements to an output stream.	
std::ostream& operator<<(std::ostream& os, const Matrix& A)
{
	std::streamsize p = os.precision();
	std::streamsize w = os.width();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++) os << std::setprecision(5) << std::setw(10) << A(i, j);
		os << std::endl;
	}
	os.precision(p);
	os.width(w);
	return os;
}

/// Matrix/point multiplication. The point is translated by the last column.
Point Matrix::operator*(const Point& p) const
{
	Point r(0);
	for (int i = 0; i < 3; i++)
		r(i) = (*this)(i, 0)*p.x + (*this)(i, 1)*p.y + (*this)(i, 2)*p.z + (*this)(i, 3);
	return r;
}

/// Matrix/vector multiplication. The vector is not translated by the last column.
Vector Matrix::operator*(const Vector& v) const
{
	Vector r(0);
	for (int i = 0; i < 3; i++)
		r(i) = (*this)(i, 0)*v.x + (*this)(i, 1)*v.y + (*this)(i, 2)*v.z;
	return r;
}

/// Setup rotation matrix around the x-axis, where the rotation angle is given in degrees.
Matrix& Matrix::rotX(float rx)
{
	identity();
	float a = rx * M_PI / 180.0f;
	float s = std::sin(a);
	float c = std::cos(a);
	(*this)(1, 1) = c;	(*this)(1, 2) = -s;
	(*this)(2, 1) = s;	(*this)(2, 2) = c;
	return *this;
}

/// Setup rotation matrix around the y-axis, where the rotation angle is given in degrees.
Matrix& Matrix::rotY(float ry)
{
	identity();
	float a = ry * M_PI / 180.0f;
	float s = std::sin(a);
	float c = std::cos(a);
	(*this)(0, 0) = c;	(*this)(0, 2) = s;
	(*this)(2, 0) = -s;	(*this)(2, 2) = c;
	return *this;
}

/// Setup rotation matrix around the z-axis, where the rotation angle is given in degrees.
Matrix& Matrix::rotZ(float rz)
{
	identity();
	float a = rz * M_PI / 180.0f;
	float s = std::sin(a);
	float c = std::cos(a);
	(*this)(0, 0) = c;	(*this)(0, 1) = -s;
	(*this)(1, 0) = s;	(*this)(1, 1) = c;
	return *this;
}


/// Compute the determinant of the matrix.
float Matrix::determinant() const
{
	const Matrix& m = *this;
	return ((m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1)) * (m(2, 2) * m(3, 3) - m(3, 2) * m(2, 3)) -
		(m(0, 0) * m(2, 1) - m(2, 0) * m(0, 1)) * (m(1, 2) * m(3, 3) - m(3, 2) * m(1, 3)) +
		(m(0, 0) * m(3, 1) - m(3, 0) * m(0, 1)) * (m(1, 2) * m(2, 3) - m(2, 2) * m(1, 3)) +
		(m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1)) * (m(0, 2) * m(3, 3) - m(3, 2) * m(0, 3)) -
		(m(1, 0) * m(3, 1) - m(3, 0) * m(1, 1)) * (m(0, 2) * m(2, 3) - m(2, 2) * m(0, 3)) +
		(m(2, 0) * m(3, 1) - m(3, 0) * m(2, 1)) * (m(0, 2) * m(1, 3) - m(1, 2) * m(0, 3)));
}


/// Compute the inverse of the matrix.
Matrix Matrix::inverse() const
{
	Matrix result;
	float d = determinant();
	if (d == 0.0f) return result.zero();
	d = 1.0f / d;

	const Matrix& m = *this;
	result(0, 0) = d * (m(1, 1) * (m(2, 2) * m(3, 3) - m(3, 2) * m(2, 3)) + m(2, 1) * (m(3, 2) * m(1, 3) - m(1, 2) * m(3, 3)) + m(3, 1) * (m(1, 2) * m(2, 3) - m(2, 2) * m(1, 3)));
	result(1, 0) = d * (m(1, 2) * (m(2, 0) * m(3, 3) - m(3, 0) * m(2, 3)) + m(2, 2) * (m(3, 0) * m(1, 3) - m(1, 0) * m(3, 3)) + m(3, 2) * (m(1, 0) * m(2, 3) - m(2, 0) * m(1, 3)));
	result(2, 0) = d * (m(1, 3) * (m(2, 0) * m(3, 1) - m(3, 0) * m(2, 1)) + m(2, 3) * (m(3, 0) * m(1, 1) - m(1, 0) * m(3, 1)) + m(3, 3) * (m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1)));
	result(3, 0) = d * (m(1, 0) * (m(3, 1) * m(2, 2) - m(2, 1) * m(3, 2)) + m(2, 0) * (m(1, 1) * m(3, 2) - m(3, 1) * m(1, 2)) + m(3, 0) * (m(2, 1) * m(1, 2) - m(1, 1) * m(2, 2)));
	result(0, 1) = d * (m(2, 1) * (m(0, 2) * m(3, 3) - m(3, 2) * m(0, 3)) + m(3, 1) * (m(2, 2) * m(0, 3) - m(0, 2) * m(2, 3)) + m(0, 1) * (m(3, 2) * m(2, 3) - m(2, 2) * m(3, 3)));
	result(1, 1) = d * (m(2, 2) * (m(0, 0) * m(3, 3) - m(3, 0) * m(0, 3)) + m(3, 2) * (m(2, 0) * m(0, 3) - m(0, 0) * m(2, 3)) + m(0, 2) * (m(3, 0) * m(2, 3) - m(2, 0) * m(3, 3)));
	result(2, 1) = d * (m(2, 3) * (m(0, 0) * m(3, 1) - m(3, 0) * m(0, 1)) + m(3, 3) * (m(2, 0) * m(0, 1) - m(0, 0) * m(2, 1)) + m(0, 3) * (m(3, 0) * m(2, 1) - m(2, 0) * m(3, 1)));
	result(3, 1) = d * (m(2, 0) * (m(3, 1) * m(0, 2) - m(0, 1) * m(3, 2)) + m(3, 0) * (m(0, 1) * m(2, 2) - m(2, 1) * m(0, 2)) + m(0, 0) * (m(2, 1) * m(3, 2) - m(3, 1) * m(2, 2)));
	result(0, 2) = d * (m(3, 1) * (m(0, 2) * m(1, 3) - m(1, 2) * m(0, 3)) + m(0, 1) * (m(1, 2) * m(3, 3) - m(3, 2) * m(1, 3)) + m(1, 1) * (m(3, 2) * m(0, 3) - m(0, 2) * m(3, 3)));
	result(1, 2) = d * (m(3, 2) * (m(0, 0) * m(1, 3) - m(1, 0) * m(0, 3)) + m(0, 2) * (m(1, 0) * m(3, 3) - m(3, 0) * m(1, 3)) + m(1, 2) * (m(3, 0) * m(0, 3) - m(0, 0) * m(3, 3)));
	result(2, 2) = d * (m(3, 3) * (m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1)) + m(0, 3) * (m(1, 0) * m(3, 1) - m(3, 0) * m(1, 1)) + m(1, 3) * (m(3, 0) * m(0, 1) - m(0, 0) * m(3, 1)));
	result(3, 2) = d * (m(3, 0) * (m(1, 1) * m(0, 2) - m(0, 1) * m(1, 2)) + m(0, 0) * (m(3, 1) * m(1, 2) - m(1, 1) * m(3, 2)) + m(1, 0) * (m(0, 1) * m(3, 2) - m(3, 1) * m(0, 2)));
	result(0, 3) = d * (m(0, 1) * (m(2, 2) * m(1, 3) - m(1, 2) * m(2, 3)) + m(1, 1) * (m(0, 2) * m(2, 3) - m(2, 2) * m(0, 3)) + m(2, 1) * (m(1, 2) * m(0, 3) - m(0, 2) * m(1, 3)));
	result(1, 3) = d * (m(0, 2) * (m(2, 0) * m(1, 3) - m(1, 0) * m(2, 3)) + m(1, 2) * (m(0, 0) * m(2, 3) - m(2, 0) * m(0, 3)) + m(2, 2) * (m(1, 0) * m(0, 3) - m(0, 0) * m(1, 3)));
	result(2, 3) = d * (m(0, 3) * (m(2, 0) * m(1, 1) - m(1, 0) * m(2, 1)) + m(1, 3) * (m(0, 0) * m(2, 1) - m(2, 0) * m(0, 1)) + m(2, 3) * (m(1, 0) * m(0, 1) - m(0, 0) * m(1, 1)));
	result(3, 3) = d * (m(0, 0) * (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) + m(1, 0) * (m(2, 1) * m(0, 2) - m(0, 1) * m(2, 2)) + m(2, 0) * (m(0, 1) * m(1, 2) - m(1, 1) * m(0, 2)));
	return result;
}




// ------------------------ Vector functions ------------------------ //

/// Constructor initializing the vector from a point.
Vector::Vector(const Point& p) : x(p.x), y(p.y), z(p.z) { }

/// Write vector elements to an output stream.
std::ostream& operator<<(std::ostream& os, const Vector& A)
{
	std::streamsize p = os.precision();
	std::streamsize w = os.width();
	for (int i = 0; i < 3; i++) os << std::setprecision(5) << std::setw(10) << A(i);
	os << std::endl;
	//	os << A.x << A.y << A.z << std::endl;
	os.precision(p);
	os.width(w);
	return os;
}


// ------------------------ Point functions ------------------------ //

/// Write point elements to an output stream.
std::ostream& operator<<(std::ostream& os, const Point& A)
{
	std::streamsize p = os.precision();
	std::streamsize w = os.width();
	for (int i = 0; i < 3; i++) os << std::setprecision(5) << std::setw(10) << A(i);
	os << std::endl;
	//	os << A.x << A.y << A.z << std::endl;
	os.precision(p);
	os.width(w);
	return os;
}
