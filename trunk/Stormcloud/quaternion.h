#pragma once

#include <stdlib.h>
#include <gl/glut.h>
#include <math.h>
#include "vector3f.h"

/**
	Floating point number quaternion class.
*/

class Quaternion {
public:
	float w; /**< First element. */
	float x; /**< Second element. */
	float y; /**< Third element. */
	float z; /**< Fourth element. */

	/** Default constructor. Initializes to <1,0,0,0>, the identity quaternion (rotating by it will not change anything). */
	Quaternion();
	/** Constructor. */
	Quaternion(const float &_w, const float &_x, const float &_y, const float &_z);
	/** Initializing constructor. The vector's x,y,z are used for the quaternion's x,y,z. */
	Quaternion(const float &_w, const Vector3f &v);

	/** Returns the quaternion's length. */
	float length();
	/** Returns the quaternion's inverse. q^c = <w,-x,-y,-z>. */
	Quaternion conjugate();
	/** Changes this quaternion to have length 1, and also returns it. */
	Quaternion normalize();
	/** Returns v rotated by this quaternion. v' = quq^c, u = <0,v>. */
	Vector3f rotate(const Vector3f &v);
	/** Returns a matrix representation of this quaternion. This matrix can be applied to viewing matrices to apply the rotation. */
	void toMatrix(float store[]);
	/** Sets up this quaternion as a rotation of _theta degrees around _axis. */
	void rotationQuat(const float& _theta, Vector3f& axis);
	/** Applies this rotation quaternion to the current OpenGL matrix. */
	void applyRotation();
	/** Comparison operator. Returns true if w = q.w && x = q.x && y = q.y && z = q.z. */
	bool operator==(const Quaternion &q);
	/** Quaternion multiplication shorthand. */
	Quaternion& operator *=(const Quaternion &q);
	/** Assignment operator. */
	const Quaternion& operator =(const Quaternion &q);
	/** Quaternion multiplication. */
	friend Quaternion operator *(const Quaternion &q1, const Quaternion &q2);
	/** Scalar multiplcation. */
	friend Quaternion operator *(const Quaternion &q, const float &s);
	/** Quaternion addition. */
	friend Quaternion operator +(const Quaternion &q1, const Quaternion &q2);
	/** Quaternion subtraction. */
	friend Quaternion operator -(const Quaternion &q1, const Quaternion &q2);
	/** Output shorthand. output is the stream being used, q is the quaternion being put out. */
	friend std::ostream & operator <<(std::ostream &output, const Quaternion &q);
	/** Quaternion dot product. */
	friend float dot(const Quaternion &q1, const Quaternion &q2);
	/** Returns the parallel quaternion with length 1, leaving the original alone. */
	friend Quaternion normalize(const Quaternion &q);
	/** Returns the quaternion representing a rotation of _theta degrees around _axis. */
	friend Quaternion rotationQuat(const float& _theta, Vector3f& _axis);
};