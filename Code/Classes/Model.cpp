#include "Model.h"
USING_NS_CC;

Model::Model()
{

}

Model::~Model()
{
}

cocos2d::Sprite* Model::getSprite()
{
	return mSprite;
}

cocos2d::PhysicsBody* Model::getPhysicsBody()
{
	return mPhysicsBody;
}

cocos2d::PhysicsBody* Model::getPhysicsFoot()
{
	return mPhysicsFoot;
}
cocos2d::PhysicsBody* Model::getPhysicsAttack()
{
	return mAttackBody;
}
void Model::update()
{

}

void Model::setScale(float scale)
{
	mSprite->setScale(scale);
}

Vec2 Model::getPosition()
{
	return mSprite->getPosition();
}

Size Model::getContentSize()
{
	return mSprite->getContentSize();
}


void Model::setAlive(bool alive)
{
	mIsALive = alive;
	mSprite->setVisible(mIsALive);
	mAttackBody->setEnabled(alive);
	mPhysicsBody->setEnabled(alive);
	mPhysicsFoot->setEnabled(alive);
}

void Model::setArrowAlive(bool alive)
{
	mIsALive = alive;
	mSprite->setVisible(mIsALive);
	mPhysicsBody->setEnabled(alive);
}

void Model::setMoveRight(bool move)
{
	mIsRunLeftToRight = move;
}

bool Model::isMoveRight()
{
	return mIsRunLeftToRight;
}

bool Model::isMoveUp()
{
	return mIsRunBottomUp;
}

void Model::setMoveUp(bool move)
{
	mIsRunBottomUp = move;
}

bool Model::isAlive()
{
	return mIsALive;
}

void Model::setPosition(cocos2d::Vec2 pos)
{
	mSprite->setPosition(pos);
}

void Model::setIndex(int index)
{
	mPhysicsBody->setGroup(index);
}

int Model::getIndex()
{
	return mPhysicsBody->getGroup();
}


int Model::getDamage()
{
	return mDamage;
}

void Model::setDamage(int a)
{
	mDamage = a;
}

void Model::setBlood(int b)
{
	mBlood = b;
}

int Model::getBlood()
{
	return mBlood;
}

void Model::setArmor(int c)
{
	mArmor = c;
}

int Model::getArmor()
{
	return mArmor;
}

void Model::setCurrent_Blood(int d)
{
	mCurrent_Blood = d;
}

int Model::getCurrent_Blood()
{
	return mCurrent_Blood;
}

void Model::setState(int f)
{
	mCurrent_Blood = f;
}

int Model::getCurrentState()
{
	return mCurrentState;
}

int Model::getLevel()
{
	return mLevel;
}

void Model::setLevel(int a)
{
	mLevel = a;
}

void Model::setMaxExp(int exp)
{
	mMaxExp = exp;
}

int Model::getMaxExp()
{
	return mMaxExp;
}

void Model::setGold(int gold)
{
	mGold = gold;
}

int Model::getGold()
{
	return mGold;
}

void Model::setCurrent_Exp(int exp)
{
	mCurrent_Exp = exp;
}

int Model::getCurrent_Exp()
{
	return mCurrent_Exp;
}

