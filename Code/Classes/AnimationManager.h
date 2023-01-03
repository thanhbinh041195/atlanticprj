#pragma once
#include "cocos2d.h"
#include "Define.h"

using namespace cocos2d;

class AnimationManager
{
public:
	static AnimationManager* getInstance();
	Animation* createAnimation(const char* prefixName, int start, int end, float delay, std::string lastFrame);
	Animation* createAnimation(const char* prefixName, int start, int end, float delay);
	Animation* createAnimationR(const char* prefixName, int start, int end, float delay);

	void init_desert();
	void init_jungle();
	void init_underground();
	void init_towers();
	void init_player();
	void init_item();

private:
	static AnimationManager* mInstance;
	AnimationManager();
	~AnimationManager();
};