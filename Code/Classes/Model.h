#ifndef _MODEL_H_
#define _MODEL_H_

#include "cocos2d.h"
USING_NS_CC;
class Model
{
public:
	static const int BITMASK_WORLD = 100;
	static const int BITMASK_PLAYER = BITMASK_WORLD + 1;
	static const int BITMASK_PLAYER_BULLET = BITMASK_PLAYER + 1;
	static const int BITMASK_PLAYER_ATTACK = BITMASK_PLAYER_BULLET + 1;
	static const int BITMASK_PLAYER_FOOT = BITMASK_PLAYER_ATTACK + 1;
	static const int BITMASK_GROUND = BITMASK_PLAYER_FOOT + 1;
	static const int BITMASK_TELEPORT = BITMASK_GROUND + 1;
	static const int BITMASK_MONSTER = BITMASK_TELEPORT + 1;
	static const int BITMASK_MONSTER_ATTACK = BITMASK_MONSTER + 1;
	static const int BITMASK_MONSTER_FOOT = BITMASK_MONSTER_ATTACK + 1;
	static const int BITMASK_COW_MONSTER = BITMASK_MONSTER_FOOT + 1;
	static const int BITMASK_COWMONSTER_ATTACK = BITMASK_COW_MONSTER + 1;
	static const int BITMASK_ARCHER_MONSTER = BITMASK_COWMONSTER_ATTACK + 1;
	static const int BITMASK_ARCHER_ARROW = BITMASK_ARCHER_MONSTER + 1;
	static const int BITMASK_ARCHER_ATTACK = BITMASK_ARCHER_ARROW + 1;


	static const int BITMASK_BOSS_ATTACK = BITMASK_ARCHER_ATTACK + 1;
	static const int BITMASK_BOSS_MONSTER = BITMASK_BOSS_ATTACK + 1;
	static const int BITMASK_DOG_MONSTER = BITMASK_BOSS_MONSTER + 1;
	static const int BITMASK_DOG_ATTACK = BITMASK_DOG_MONSTER + 1;


	//NPCs Bitmask
	static const int BITMASK_TalkNPC_Keeper = BITMASK_DOG_ATTACK + 1;
	static const int BITMASK_TalkNPC_Medicine = BITMASK_TalkNPC_Keeper + 1;
	static const int BITMASK_TalkNPC_ArmorSeller = BITMASK_TalkNPC_Medicine + 1;
	static const int BITMASK_TalkNPC_ArmsSeller = BITMASK_TalkNPC_ArmorSeller + 1;
	static const int BITMASK_Teleport = BITMASK_TalkNPC_ArmsSeller + 1;

	static const int BITMASK_GOLD = BITMASK_Teleport + 1;
	static const int BITMASK_ITEM = BITMASK_GOLD + 1;

	static const int MODLE_TYPE_MAIN_CHARACTER = 1;
	static const int MODLE_TYPE_COW_MONSTER = 2;
	static const int MODLE_TYPE_Archer_MONSTER = 3;
	static const int MODLE_TYPE_DOG_MONSTER = 4;
	static const int MODLE_TYPE_BOSS_MONSTER = 5;
protected:
	cocos2d::Sprite* mSprite;
	cocos2d::PhysicsBody* mPhysicsBody;
	cocos2d::PhysicsBody* mAttackBody;
	cocos2d::PhysicsBody* mPhysicsFoot;
	bool mIsALive;
	bool mIsRunLeftToRight;
	bool mIsRunBottomUp;
	bool mIsMcLeft;
	
public:
	Model();
	~Model();
	
	cocos2d::Sprite* getSprite();
	cocos2d::PhysicsBody* getPhysicsBody();
	cocos2d::PhysicsBody* getPhysicsFoot();
	cocos2d::PhysicsBody* getPhysicsAttack();

	int mDamage;
	int mBlood;
	int mArmor;
	int mCurrent_Blood;
	int mCurrentState;
	int mLevel;
	int mMaxExp;
	int mCurrent_Exp;
	int mGold;

	float scale;
	void update();
	bool isMoveRight();
	void setMoveRight(bool move);
	bool isMoveUp();
	void setMoveUp(bool move);
	void setAlive(bool alive);
	void setArrowAlive(bool alive);
	bool isAlive();
	void setPosition(cocos2d::Vec2 pos);
	void setScale(float scale);
	void setIndex(int index);
	int getIndex();
	void setDamage(int a);
	int getDamage();
	void setBlood(int b);
	int getBlood();
	void setArmor(int c);
	int getArmor();
	void setCurrent_Blood(int d);
	int getCurrent_Blood();
	void setState(int nextState);
	int getCurrentState();
	void setLevel(int a);
	int getLevel();
	void setCurrent_Exp(int exp);
	int getCurrent_Exp();
	void setMaxExp(int exp);
	int getMaxExp();
	void setGold(int gold);
	int getGold();
	cocos2d::Vec2 getPosition();
	cocos2d::Size getContentSize();
};
#endif