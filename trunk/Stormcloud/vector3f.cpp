#include <math.h>

#include "vector3f.h"

Vector3f::Vector3f(){
	x = y = z = 0.f;
}

Vector3f::Vector3f(const float &_x, const float &_y, const float &_z){
	x = _x;
	y = _y;
	z = _z;
}

Vector3f::Vector3f(const Vector3f &v){
	x = v.x;
	y = v.y;
	z = v.z;
}

const Vector3f& Vector3f::operator =(const Vector3f &v){
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

bool Vector3f::operator ==(const Vector3f &v){
	return (x == v.x && y == v.y && z == v.z);
}

float Vector3f::operator [](const int &index){
	float ret = 0.f;
	if (index == 0) { ret = x; }
	else if (index == 1) { ret = y; }
	else { ret = z; }
	return ret;
}

float Vector3f::length(){
	return sqrt(x*x + y*y + z*z);
}

void Vector3f::normalize(){
	float len = length();
	if (len == 0) { //every time you div by 0, a harddrive somewhere in the world dies
		x = 0; 
		y = 0; 
		z = 0; 
	} else {
		x /= len;
		y /= len;
		z /= len;
	}
}

std::ostream& operator<<(std::ostream& output, const Vector3f &v){
	output << "(" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
	return output;
}

Vector3f operator+(const Vector3f &v1, const Vector3f &v2){
	return Vector3f(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3f operator-(const Vector3f &v1, const Vector3f &v2){
	return Vector3f(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vector3f operator*(const Vector3f &v, const float &s){
	return Vector3f(v.x*s, v.y*s, v.z*s);
}

Vector3f operator/(const Vector3f &v, const float &s){
	return Vector3f(v.x/s, v.y/s, v.z/s);
}

float dot(const Vector3f &v1, const Vector3f &v2){
	return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

Vector3f cross(const Vector3f &v1, const Vector3f &v2){
	return Vector3f(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

Vector3f normalize(const Vector3f &v){
	float len = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	if (len == 0){
		return Vector3f(0,0,0);
	} else {
		return Vector3f(v.x/len, v.y/len, v.z/len);
	}
}

bool operator<(Vector3f& v1, Vector3f&v2){
	return (v1.length() < v2.length());
}

bool operator>(Vector3f& v1, Vector3f&v2){
	return (v1.length() > v2.length());
}