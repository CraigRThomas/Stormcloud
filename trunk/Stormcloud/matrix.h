#pragma once

#include <vector>
#include "vector3f.h"
#include "quaternion.h"

/**
	Floating point matrix class.
*/

class Matrix {
public:
	/** Default constructor. */
	Matrix();
	/** Copy constructor. */
	Matrix(const Matrix& c);
	/** Constructor. Creates an _m rows X _n columns matrix, and initializes it to the identity matrix if _ident is true. */
	Matrix(int _m, int _n, bool = true);
	/** Destructor. */
	~Matrix();

	/** Access operator. Arguments are row, column. */
	float& operator()(unsigned,unsigned);
	/** Access operator. Arguments are row, column. */
	float operator()(unsigned,unsigned) const;
	/** Element-wise copy function. */
	const Matrix& operator=(const Matrix& m);
	/** Scalar multiplication. */
	friend Matrix operator*=(Matrix& m, const float s);
	/** Scalar multiplication. */
	friend Matrix operator*(const Matrix& m, const float s);
	/** Matrix multiplication. */
	friend Matrix operator*(const Matrix& m1, const Matrix& m2);
	/** Matrix addition. */
	friend Matrix operator+(const Matrix& m1, const Matrix& m2);
	/** Matrix subtraction. */
	friend Matrix operator-(const Matrix& m1, const Matrix& m2);
	/** Matrix multiplication with a vector. */
	friend Vector3f operator*(const Matrix& m, const Vector3f& v);

	/** Outputs the formatted matrix to the console. */
	void print();
	/** Creates a matrix from the contents of the array argument. Assumes the matrix is the correct size. */
	void populate(float[]);
	/** Creates a matrix from the contents of the array argument. Assumes the matrix is the correct size. */
	void populate(const float[]);
	/** Assigns a single element specified by the first two arguments. */
	void populate(unsigned, unsigned, float);
	void populate(Quaternion &rotation, Vector3f &translation);
	/** Modifies the specified element. */
	void modValue(unsigned, unsigned, float);
	/** Returns the minor of the matrix. The minor is the matrix without the _rowth row and _colth column. */
	Matrix minor(unsigned, unsigned);
	/** Returns the determinant of the matrix. */
	float determinant();
	/** Creates a 3D homogeneous matrix. */
	void homogeneous3D(const float& xRot, const float &yRot, const float &zRot, const Vector3f &translation);
	/** Returns the data as an array of floats. */
	const float* getData(float d[]);
private:
	unsigned int rows;					 /**< Number of rows the matrix has. */
	unsigned int cols;					 /**< Number of columns the matrix has. */
	std::vector<std::vector<float>> data;/**< 2D array of values. */
};