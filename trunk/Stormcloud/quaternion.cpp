#include "quaternion.h"

Quaternion::Quaternion(){
	w = 1.f;
	x = y = z = 0.f;
}

Quaternion::Quaternion(const float &_w, const float &_x, const float &_y, const float &_z){
	w = _w;
	x = _x;
	y = _y;
	z = _z;
}

Quaternion::Quaternion(const float &_w, const Vector3f &v){
	w = _w;
	x = v.x;
	y = v.y;
	z = v.z;
}

void Quaternion::rotationQuat(const float& _theta, Vector3f& _axis){
	_axis.normalize();
	w = cos(_theta*3.1415/360.f);
	float st = sin(_theta*3.1415/360.f);
	x = st*_axis.x;
	y = st*_axis.y;
	z = st*_axis.z;
}

Quaternion rotationQuat(const float& _theta, Vector3f& _axis){
	Quaternion q;
	_axis.normalize();
	q.w = cos(_theta*3.1415/360.f);
	float st = sin(_theta*3.1415/360.f);
	q.x = st*_axis.x;
	q.y = st*_axis.y;
	q.z = st*_axis.z;
	return q;
}

void Quaternion::applyRotation(){
	normalize();
	float m[16];
	toMatrix(m);
	glMultMatrixf(m);
}

float Quaternion::length(){
	return sqrt(w*w + x*x + y*y + z*z);
}

Quaternion Quaternion::conjugate(){
	return Quaternion(w, -x, -y, -z);
}

Quaternion Quaternion::normalize(){
	float len = length();
	if (len == 0) len = 0.001; //only you can prevent div by 0 errors 
	w /= len;
	x /= len;
	y /= len;
	z /= len;
	return *this;
}

Vector3f Quaternion::rotate(const Vector3f &v){
	Vector3f vect = v;
	Quaternion vec(0.f, vect);
	vec.normalize();
	vec = (*this * vec) * this->conjugate();
	return Vector3f(vec.x, vec.y, vec.z)  * vect.length();
}

void Quaternion::toMatrix(float store[]){ //this matrix is in a form that will apply the rotation to the openGL transformation matrix
	store[ 0] = 1.f - 2.f * (y*y + z*z);
	store[ 1] = 2.f * (x*y + z*w);
	store[ 2] = 2.f * (x*z - y*w);
	store[ 3] = 0.f;

	store[ 4] = 2.f * (x*y - z*w);
	store[ 5] = 1.f - 2.f * (x*x + z*z);
	store[ 6] = 2.f * (z*y + x*w);
	store[ 7] = 0.f;

	store[ 8] = 2.f * (x*z + y*w);
	store[ 9] = 2.f * (y*z - x*w);
	store[10] = 1.f - 2.f * (x*x + y*y);
	store[11] = 0.f;

	store[12] = 0.f;
	store[13] = 0.f;
	store[14] = 0.f;
	store[15] = 1.f;
}

bool Quaternion::operator==(const Quaternion &q){
	return (w == q.w && x == q.x && y == q.y && z == q.z);
}

Quaternion& Quaternion::operator *=(const Quaternion&q){
	*this = *this * q;
	return *this;
}

const Quaternion& Quaternion::operator =(const Quaternion &q){
	w = q.w;
	x = q.x;
	y = q.y;
	z = q.z;
	return *this;
}

Quaternion operator*(const Quaternion &q1, const Quaternion &q2){
	return Quaternion(	q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z,
						q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y,
						q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x,
						q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w);
}

Quaternion operator*(const Quaternion &q, const float &s){
	return Quaternion(q.w*s, q.x*s, q.y*s, q.z*s);
}

Quaternion operator+(const Quaternion &q1, const Quaternion &q2){
	return Quaternion(q1.w + q2.w, q1.x + q2.x, q1.y + q2.y, q1.z + q2.z);
}

Quaternion operator-(const Quaternion &q1, const Quaternion &q2){
	return Quaternion(q1.w - q2.w, q1.x - q2.x, q1.y - q2.y, q1.z - q2.z);
}

std::ostream & operator <<(std::ostream &output, const Quaternion &q){
	output << "(" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ")" << std::endl;
	return output;
}

float dot(const Quaternion &q1, const Quaternion &q2){
	return (q1.w*q2.w + q1.x*q2.x + q1.y*q2.y + q1.z*q2.z);
}

Quaternion normalize(const Quaternion &q){
	float len = sqrt(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);
	return Quaternion(q.w/len, q.x/len, q.y/len, q.z/len);
}