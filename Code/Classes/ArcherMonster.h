#pragma once
#include "cocos2d.h"
#include "Model.h"
#include "ui/CocosGUI.h"
#include "Player.h"
#include "Arrow.h"

USING_NS_CC;

class ArcherMonster : public Model
{
public:
	static const int STATE_START = 0;
	static const int STATE_IDLE = STATE_START + 1;
	static const int STATE_MOVE = STATE_IDLE + 1;
	static const int STATE_FIRE_1 = STATE_MOVE + 1;
	static const int STATE_FIRE_2 = STATE_FIRE_1 + 1;
	static const int STATE_DAMAGE = STATE_FIRE_2 + 1;
	static const int STATE_ATTACK = STATE_DAMAGE + 1;
	static const int STATE_DIE = STATE_ATTACK + 1;




	static const int ANIM_IDLE = 0;
	static const int ANIM_DAMAGE = ANIM_IDLE + 1;
	static const int ANIM_MOVE = ANIM_DAMAGE + 1;
	static const int ANIM_ATTACK = ANIM_MOVE + 1;
	static const int ANIM_FIRE = ANIM_ATTACK + 1;
	static const int ANIM_DIE = ANIM_FIRE + 1;
	static const int ANIM_TOTAl = ANIM_DIE + 1;

private:
	int mCurrentState;
	int mBlood;
	cocos2d::Action* mAnimation[ANIM_TOTAl];
	cocos2d::ui::LoadingBar *mArcherHudBlood;
	Arrow* mArrow;
	bool mDetect;
public:
	ArcherMonster(cocos2d::Layer* layer);
	~ArcherMonster();
	void setState(int state);
	void update(bool isMcLeft);
	void bulletHasCollision();
	void setFlip(bool direction);
	void setDetect(bool detect);
	bool getDetect();
};