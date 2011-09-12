#pragma once

#include <iostream>

/**
	3D floating point vector class.
*/

class Vector3f {
public:
	float x; /**< X-axis magnitude. */
	float y; /**< Y-axis magnitude. */
	float z; /**< Z-axis magnitude. */

	/** Default constructor. */
	Vector3f();
	/** Constructor. */
	Vector3f(const float& _x, const float& _y, const float& _z = 0.f);
	/** Copy constructor. */
	Vector3f(const Vector3f& v);

	/** Assignment operator. v is the vector to be copied. */
	const Vector3f& operator=(const Vector3f& v);
	/** Comparison operator. Returns true if x = v.x && y = v.y && z = v.z. */
	bool operator==(const Vector3f& v);
	/** Access operator. x=0, y=1, z=2+.  */
	float operator[](const int& index);
	/** Returns the length of the vector. */
	float length();
	/** Changes the vector to make its length 1. */
	void normalize();

	/** Output shorthand operator. output is the stream being used, and v is the vector being put out. */
	friend std::ostream& operator<<(std::ostream& output, const Vector3f& v);
	/** Vector addition. */
	friend Vector3f operator+(const Vector3f& v1, const Vector3f& v2);
	/** Vector subtraction. */
	friend Vector3f operator-(const Vector3f& v1, const Vector3f& v2);
	/** Scalar multiplication. */
	friend Vector3f operator*(const Vector3f& v, const float& s);
	/** Scalar division. */
	friend Vector3f operator/(const Vector3f& v, const float& s);
	/** Returns the dot product of v1 dot v2. d = |v1||v2|cos(theta), where theta = angle between v1 and v2. */
	friend float dot(const Vector3f& v1, const Vector3f& v2);
	/** Returns the vector perpendicular to v1 and v2. */
	friend Vector3f cross(const Vector3f& v1, const Vector3f& v2);
	/** Returns the normalized vector v while leaving v alone. */
	friend Vector3f normalize(const Vector3f& v);
	/** Returns true if v1's length is less than v2. */
	friend bool operator<(Vector3f& v1, Vector3f& v2);
	/** Returns true if v1's length is greater than v2. */
	friend bool operator>(Vector3f& v1, Vector3f& v2);
};