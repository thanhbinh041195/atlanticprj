#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Model.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "McSetting.h"

class Player : public Model
{
public:
	static const int RUN_STEP = 10;
	static const int MAX_BULLET = 10;

	static const int ANIM_IDLE = 0;
	static const int ANIM_MOVE = ANIM_IDLE + 1;
	static const int ANIM_ATTACK = ANIM_MOVE + 1;
	static const int ANIM_SKILL1 = ANIM_ATTACK + 1;
	static const int ANIM_SKILL2 = ANIM_SKILL1 + 1;
	static const int ANIM_SKILL3 = ANIM_SKILL2 + 1;
	static const int ANIM_SKILL3_IDLE = ANIM_SKILL3 + 1;
	static const int ANIM_SKILL3_MOVE = ANIM_SKILL3_IDLE + 1;
	static const int ANIM_SKILL3_ATTACK = ANIM_SKILL3_MOVE + 1;
	static const int ANIM_SKILL1_ULTI = ANIM_SKILL3_ATTACK + 1;
	static const int ANIM_SKILL2_ULTI = ANIM_SKILL1_ULTI + 1;
	static const int ANIM_DAMAGE = ANIM_SKILL2_ULTI + 1;
	static const int ANIM_DELAY = ANIM_DAMAGE + 1;
	static const int ANIM_ULTI = ANIM_DELAY + 1;
	static const int ANIM_DIE = ANIM_ULTI + 1;
	static const int ANIM_TOTAl = ANIM_DIE + 1;
	float PLAYER_SCALE = 1.5;


	static const int STATE_START = 0;
	static const int STATE_IDLE = STATE_START + 1;
	static const int STATE_MOVE = STATE_IDLE + 1;
	static const int STATE_ATTACK = STATE_MOVE + 1; //3
	static const int STATE_SKILL1 = STATE_ATTACK + 1; //4
	static const int STATE_SKILL2 = STATE_SKILL1 + 1; //5
	static const int STATE_SKILL3 = STATE_SKILL2 + 1; //6
	static const int STATE_DAMAGE = STATE_SKILL3 + 1; //7
	static const int STATE_DIE = STATE_DAMAGE + 1; //8

protected:
	int mMana;
	int mCurrent_Mana;

private:
	static Player* sInstance;
	cocos2d::Action* mAnimation[ANIM_TOTAl];
	cocos2d::PhysicsBody* mAttackBody1;
	cocos2d::Sprite* mUlti;

public:
	static Player* getInstance();
	int posPlayerX;
	int posPlayerY;
	Player(cocos2d::Layer* layer);
	~Player();
	float deltaTime = 0;

	void setState(int nextState);
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void fire();
	void update(float dt);
	void bulletHasCollision(int bulletIndex);
	void setFlip(bool direction);
	int getCurrent_Mana();
	void setCurrent_Mana(int curMana);
	int getMana();
	void upLevel();

private:
	void updateBullets();
};

#endif