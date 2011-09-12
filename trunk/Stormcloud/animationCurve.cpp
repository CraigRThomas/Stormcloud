#include "animationCurve.h"

void Curve::update(Matrix &t, float curFrame){
	float f = 0;
	if (curFrame < 0) curFrame += numFrames;
	for (unsigned int i=0;i<numFrames;i++){
		if (frames[i] > curFrame){
			unsigned int fn = (i==0) ? frames[numFrames-1] : frames[i-1];
			float dt = (float) (curFrame - fn) / (frames[i] - fn);
			if (numFrames > 3) {
				float b[] = {0,0,0,0};
				if (i==0){ b[0] = values[numFrames-2]; b[1] = values[numFrames-1]; b[2] = values[0]; b[3] = values[1];}
				else if (i==1) { b[0] = values[numFrames-1]; b[1] = values[0]; b[2] = values[1]; b[3] = values[2]; }
				else if (i==numFrames-1) { b[0] = values[i-2]; b[1] = values[i-1]; b[2] = values[i]; b[3] = values[0]; }
				else { b[0] = values[i-2]; b[1] = values[i-1]; b[2] = values[i]; b[3] = values[i+1]; }
				f = CRS(b,dt);
			} else {
				float val = (i > 0) ? values[i-1] : values[numFrames-1];
				f = LERP(val, values[i], dt);
			} break;
		} else if (i == numFrames-1 && curFrame > frames[i]){
			float dt = (float) (curFrame - frames[i]) / (frames[i] - frames[i-1]);
			if (numFrames > 3) {
				float b[] = {values[numFrames-2],values[numFrames-1],values[0],values[1]};
				f = CRS(b,dt);
			} else { 
				f = LERP(values[i],values[0],dt);
			} break;
		}
	}
	if (type == TRANSLATE_X){
		t.populate(0,3,f);
	} else if (type == TRANSLATE_Y){
		t.populate(1,3,f);
	} else if (type == TRANSLATE_Z){
		t.populate(2,3,f);
	}
}

float CRS(const float bData[], const float& dt){
	Matrix u(1,4),m(4,4),b(4,1);
	float uData[] = { (float)pow(dt,3.0f), (float)pow(dt,2.0f), dt, 1.0f };
	
	u.populate(uData);//fill the matrices with their respective data
	m.populate(cr_mData);
	b.populate(bData);
	Matrix p = (u*m)*b;
	return p(0,0)/2.f; //cr spline interpolation requires halving or else it breaks a lot
}

float LERP(const float& a, const float& b, const float& dt){ return (1-dt)*a + dt*b; }