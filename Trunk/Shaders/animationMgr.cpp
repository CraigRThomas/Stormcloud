#include "animationMgr.h"

AnimationManager* AnimationManager::singleton = 0;

void AnimationManager::update(const float &dt){
	t += dt;
	std::vector<AnimPair>::iterator i = pairs.begin();
	for (i;i<pairs.end();i++){
		bool cycle_finished = (t - i->startTime >= i->runTime);
		if (i->startTime < 0 || (cycle_finished && i->cyclic)){
			i->startTime = t;
		}
		if (cycle_finished && !i->cyclic){
			i = pairs.erase(i);
			if (pairs.size() == 0) break;
		} else {
			float curFrame = (float)(t - i->startTime) / i->speed;
			std::vector<Curve>::iterator j = animations[i->animId].curves.begin();
			for (j;j<animations[i->animId].curves.end();j++){
				j->update(i->transform, curFrame);
			}
		}
	}
}

AnimPair* AnimationManager::registerAnimation(const unsigned int & _animId, bool _cyclic, float _startTime){
	AnimPair a;
	a.animId = _animId;
	a.cyclic = _cyclic;
	a.index = pairId;
	pairId++;
	a.speed = MSPF;
	a.runTime = animations[a.animId].runTime * a.speed;
	a.startTime = _startTime;
	pairs.push_back(a);
	return &pairs.back();
}

void AnimationManager::deregisterAnimation(const unsigned int &index){
	std::vector<AnimPair>::iterator i = pairs.begin();
	for (i;i<pairs.end();i++){
		if (i->index == index){
			i = pairs.erase(i);
			break;
		}
	}
}

unsigned int AnimationManager::addAnimation(Animation anim){
	animations.push_back(anim);
	return animations.size()-1;
}

AnimationManager& AnimationManager::AnimMgr(){
	if (!singleton){
		singleton = new AnimationManager();
		init();
	}
	return *singleton;
}

AnimationManager::AnimationManager(){
	pairId = 0;
	t = 0;
}

AnimationManager::~AnimationManager(){
	delete singleton;
}

void AnimationManager::init(){
	std::cout<<"Animation Manager initialized.\n";
}