#pragma once

#define AnimationMgr AnimationManager::AnimMgr()

#include <vector>
#include <time.h>
#include "animationCurve.h"
#include "matrix.h"

const float MSPF = 15;

struct AnimPair {
	Matrix transform;
	unsigned int index;
	unsigned int animId;
	float startTime;
	float runTime;
	float speed;
	bool cyclic;
};

struct Animation {
	std::vector<Curve> curves;
	unsigned int id;
	float startTime;
	float runTime;
	float speed;
	bool cyclic;
};

class AnimationManager {
public:
	std::vector<AnimPair> pairs;
	std::vector<Animation> animations;
	float t;
	unsigned int pairId;

	void update(const float &dt);
	AnimPair* registerAnimation(const unsigned int &animId, bool _cyclic = false, float _startTime = -1);
	void deregisterAnimation(const unsigned int &index);
	unsigned int addAnimation(Animation anim);
	static AnimationManager& AnimMgr();
protected:
	AnimationManager();
	~AnimationManager();
private:
	static AnimationManager* singleton;
	static void init();
};