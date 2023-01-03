#pragma once
#include "cocos2d.h"
#include "Model.h"
#include "ui/CocosGUI.h"
#include "Player.h"

USING_NS_CC;

class DogMonster : public Model
{

public:
	static const int RUN_STEP = 10;
	static const int MAX_BULLET = 10;

	static const int ANIM_IDLE = 0;
	static const int ANIM_MOVE = ANIM_IDLE + 1;
	static const int ANIM_ATTACK = ANIM_MOVE + 1;
	static const int ANIM_DAMAGE = ANIM_ATTACK + 1;
	static const int ANIM_DIE = ANIM_DAMAGE + 1;
	static const int ANIM_TOTAl = ANIM_DIE + 1;
	//float COW_SCALE = 0.8f;


	static const int STATE_START = 0;
	static const int STATE_IDLE = STATE_START + 1;
	static const int STATE_MOVE = STATE_IDLE + 1;
	static const int STATE_ATTACK = STATE_MOVE + 1; //3
	static const int STATE_DAMAGE = STATE_ATTACK + 1; //7
	static const int STATE_DIE = STATE_DAMAGE + 1;
	float ACTIVE_RADIUS = 300;

private:
	cocos2d::Action* mAnimation[ANIM_TOTAl];
	bool mIsMcLeft;
	cocos2d::ui::LoadingBar *mDogHudBlood;
	Player *mPlayer;
	bool mDetect;
public:
	void setDetect(bool detect);
	bool getDetect();
	void setState(int nextState);
	void setFlip(bool direction);
	void update();
	DogMonster(cocos2d::Layer* layer);
	~DogMonster();
};