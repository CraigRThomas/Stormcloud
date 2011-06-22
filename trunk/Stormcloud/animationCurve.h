#pragma once

#include <math.h>
#include "matrix.h"

float LERP(const float& a, const float& b, const float& dt);
float CRS (const float bData[], const float& dt);//catmull-rom spline interpolation; b is Pi-2, Pi-1, Pi, Pi+1, where the current range is Pi-1 to Pi
const float cr_mData[] = { -1, 3, -3, 1, 2, -5, 4, -1, -1, 0, 1, 0, 0, 2, 0, 0 }; //m Matrix data for CatmullRom splines

enum AnimType {
	TRANSLATE_X = 0,
	TRANSLATE_Y = 1,
	TRANSLATE_Z = 2,
	ROTATE_X = 4,
	ROTATE_Y = 8,
	ROTATE_Z = 16
};

class Curve {
public:
	unsigned int numFrames;
	unsigned int *frames;
	float *values;
	AnimType type;

	void update(Matrix & t, float curFrame);
};