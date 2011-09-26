#include "matrix.h"
#include <math.h>
#include <iostream>

Matrix::Matrix(){
	rows = 1;
	cols = 1;

	std::vector<float> d;
	data.clear();
	for (unsigned int i=0;i<rows;i++){		
		data.push_back(d);
		for (unsigned int j=0;j<cols;j++){		
			data[i].push_back(0.f);
		}
	}
}

Matrix::Matrix(const Matrix& _c){
	operator=(_c);
}

Matrix::Matrix(int _rows, int _cols, bool _ident){
	rows = _rows;
	cols = _cols;
	if (rows != cols) { _ident = false; }

	std::vector<float> d;
	float f;
	data.clear();
	for (unsigned int i=0;i<rows;i++){		
		data.push_back(d);
		for (unsigned int j=0;j<cols;j++){		
			f = (i == j && _ident) ? 1.0f : 0.0f;
			data[i].push_back(f);
		}
	}
}

Matrix::~Matrix(){
	data.clear();
}

const Matrix& Matrix::operator=(const Matrix& m){
	rows = m.rows;
	cols = m.cols;

	std::vector<float> d;
	data.clear();
	for (unsigned int i=0;i<m.rows;i++){		
		data.push_back(d);
		for (unsigned int j=0;j<m.cols;j++){	
			data[i].push_back(m.data[i][j]);
		}
	}
	return *this;
}

Matrix operator*(const Matrix& m1, const Matrix& m2){
	try {
		if (m1.cols == m2.rows){
			Matrix m(m1.rows, m2.cols, false);
			for (unsigned i=0;i<m.rows;i++){
				for (unsigned j=0;j<m.cols;j++){
					for (unsigned k=0;k<m1.cols;k++){
						m.data[i][j] += m1.data[i][k]*m2.data[k][j];
					}
				}
			}
			return m;
		} else {
			throw 200;
		}
	} catch (int e) {
		e++;
		std::cout<<"Error when multiplying matrices: invalid matrix sizes.\n";
		return Matrix(0,0);
	}
}

Matrix operator*=(Matrix& m, const float s){
	for (unsigned i=0;i<m.rows;i++){
		for (unsigned j=0;j<m.cols;j++){
			m.data[i][j] *= s;
		}
	}
	return m;
}

Matrix operator*(const Matrix& m, const float s){
	Matrix sM = m;
	sM *= s;
	return sM;
}

Matrix operator+(const Matrix& m1, const Matrix& m2){
	try {
		if (m1.rows == m2.rows && m1.cols == m2.cols){
			Matrix sum(m1.rows,m1.cols,false);
			for (unsigned i=0;i<m1.rows;i++){
				for (unsigned j=0;j<m1.cols;j++){
					sum.data[i][j] = m1.data[i][j] + m2.data[i][j];
				}
			}
			return sum;
		} else {
			throw 200;
		}
	} catch (int err){
		err++;
		std::cout<<"Error when adding matrices: invalid matrix sizes.\n";
		return m1;
	}
}

Matrix operator-(const Matrix& m1, const Matrix& m2){
	try {
		if (m1.rows == m2.rows && m1.cols == m2.cols){
			Matrix sum(m1.rows,m1.cols,false);
			for (unsigned i=0;i<m1.rows;i++){
				for (unsigned j=0;j<m1.cols;j++){
					sum.data[i][j] = m1.data[i][j] - m2.data[i][j];
				}
			}
			return sum;
		} else {
			throw 200;
		}
	} catch (int err){
		err++;
		std::cout<<"Error when subtracting matrices: invalid matrix sizes.\n";
		return m1;
	}
}

float& Matrix::operator()(unsigned int row, unsigned int col){
	if (row >= rows || col >= cols){
		throw "Matrix subscript out of bounds";
	}
	return data[row][col];
}

float Matrix::operator()(unsigned int row, unsigned int col) const{
	if (row >= rows || col >= cols){
		throw "Matrix subscript out of bounds";
	}
	return data[row][col];
}

Matrix Matrix::minor(unsigned row, unsigned col){
	Matrix m(rows-1, cols-1);
	int rowCount=0, colCount=0;
	for (unsigned i=0;i<rows;i++){
		if (i!=row){
			for (unsigned j=0;j<cols;j++){
				if (j!=col){
					m.populate(rowCount,colCount,data[i][j]);
					colCount++;
				}
			}
			rowCount++;
			colCount = 0;
		}
	}
	return m;
}

float Matrix::determinant(){
	try {
		if (rows == cols){ //the matrix has to be square to calculate determinant
			float d = 0;
			if (rows == 2) { //base case; a 2x2 matrix determinant is easy to calculate
				d = (data[0][0] * data[1][1]) - (data[0][1] * data[1][0]);
			} else {
				for (int i=cols-1;i>=0;i--){
					if (i%2 == 0){ //if the size of the matrix is even, the determinant is added
						d += data[i][0] * minor(i,0).determinant(); //woo recursion
					} else {	//if the size is odd, it's subtracted
						d -= data[i][0] * minor(i,0).determinant();
					}
				}
			}
			return d;
		} else {
			throw 200;
		}
	} catch (int err){
		err++;
		std::cout<<"Error when calculating determinant: invalid matrix size.\n";
		return 0.0f;
	}
}

void Matrix::print(){
	for (unsigned i=0;i<rows;i++){
		for (unsigned j=0;j<cols;j++){
			std::cout<<data[i][j]<<"\t";
		}
		std::cout<<std::endl;
	}
}

void Matrix::populate(float f[]){
	for (unsigned i=0;i<rows;i++){
		for (unsigned j=0;j<cols;j++){
			data[j][i] = f[(i*cols)+j];
		}
	}
}

void Matrix::populate(const float f[]){
	for (unsigned i=0;i<rows;i++){
		for (unsigned j=0;j<cols;j++){
			data[j][i] = f[(i*cols)+j];
		}
	}
}

void Matrix::populate(unsigned row, unsigned col, float value){
	data[row][col] = value;
}

void Matrix::populate(Quaternion &rotation, Vector3f &translation){
	float x = rotation.x, y = rotation.y, z = rotation.z, w = rotation.w;
	data[0][0] = 1.f - 2.f * (y*y + z*z);
	data[0][1] = 2.f * (x*y + z*w);
	data[0][2] = 2.f * (x*z - y*w);
	data[0][3] = 0.f;

	data[1][0] = 2.f * (x*y - z*w);
	data[1][1] = 1.f - 2.f * (x*x + z*z);
	data[1][2] = 2.f * (z*y + x*w);
	data[1][3] = 0.f;

	data[2][0] = 2.f * (x*z + y*w);
	data[2][1] = 2.f * (y*z - x*w);
	data[2][2] = 1.f - 2.f * (x*x + y*y);
	data[2][3] = 0.f;

	data[3][0] = translation.x;
	data[3][1] = translation.y;
	data[3][2] = translation.z;
	data[3][3] = 1.f;
}

Vector3f operator*(const Matrix& m, const Vector3f& v){
	Matrix vec(3,1);
	vec.populate(0,0,v.x);
	vec.populate(1,0,v.y);
	vec.populate(2,0,v.z);

	vec = m * vec;
	return Vector3f(vec(0,0),vec(1,0),vec(2,0));
}

void Matrix::homogeneous3D(const float &xRot, const float &yRot, const float &zRot, const Vector3f &translation){
	float sx = sin(xRot*3.1415f/180.f), cx = cos(xRot*3.1415f/180.f);
	float sy = sin(yRot*3.1415f/180.f), cy = cos(yRot*3.1415f/180.f);
	float sz = sin(zRot*3.1415f/180.f), cz = cos(zRot*3.1415f/180.f);

	rows = 4;
	cols = 4;
	std::vector<float> d;
	data.clear();
	for (unsigned int i=0;i<rows;i++){		
		data.push_back(d);
		for (unsigned int j=0;j<cols;j++){		
			data[i].push_back(0.f);
		}
	}

	data[0][0] = cz*cy - sz*sx*sy;
	data[0][1] = -sz*cx;
	data[0][2] = cz*sy + sz*sx*cy;
	
	data[1][0] = sz*cy + cz*sx*sy;
	data[1][1] = cz*cx;
	data[1][2] = sz*sy - cz*sx*cy;

	data[2][0] = -cx*sy;
	data[2][1] = sx;
	data[2][2] = cx*cy;

	/*data[0][0] = cz*cy;
	data[0][1] = -sz*cy + sx*sy*cz;
	data[0][2] = sx*sz + cx*sy*cz;
	
	data[1][0] = sz*cy;
	data[1][1] = cx*cz + sx*sy*sz;
	data[1][2] = -sx*cz + cz*sy*sz;

	data[2][0] = -sy;
	data[2][1] = sx*cy;
	data[2][2] = cx*cy;*/

	data[0][3] = translation.x;
	data[1][3] = translation.y;
	data[2][3] = translation.z;

	data[3][0] = 0;
	data[3][1] = 0;
	data[3][2] = 0;
	data[3][3] = 1;
}

const float* Matrix::getData(float d[]){
	for (unsigned int i=0;i<rows*cols;i++){
		d[i] = data[i%cols][(int)floor((float)i/rows)];
	}
	return d;
}

void Matrix::modValue(unsigned _row, unsigned _col, float val){
	data[_row][_col] += val;
}