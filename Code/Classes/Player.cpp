#include "Player.h"
#include "MenuScene.h"
#include "InGameScene.h"
#include "SimpleAudioEngine.h"
#include <cstdio>
#include <math.h>
#include <cstdarg>

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;
extern string gMapName;
extern int gMapNumber;
Node * nodeBodyPlayer;
cocos2d::PhysicsBody* mAttackBody2;

Player* Player::sInstance = nullptr;

Player* Player::getInstance()
{
	if (sInstance == nullptr)
	{
		//sInstance = new Player();
	}
	return sInstance;
}

Player::Player(cocos2d::Layer* layer)
{
	McSetting* mcSetting = McSetting::getInstance();
	//Player stats
	mDamage = mcSetting->getDamgePlayer();
	mArmor = mcSetting->getArmor();
	mBlood = mcSetting->getBloodPlayer();
	mMana = mcSetting->getMana();
	mCurrent_Exp = mcSetting->getEXP();
	mGold = mcSetting->getGold();
	mMaxExp = mcSetting->getMaxEXP();
	mLevel = mcSetting->getPlayerLevel();
	mCurrent_Mana = mMana;
	mCurrent_Blood = mBlood;
	mCurrentState = STATE_START;
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	SpriteFrameCache* cache = SpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("MC-sword/MC-sprite.plist", "MC-sword/MC-sprite.png");
	mSprite = Sprite::createWithSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("MC-sheet_01.png"));
	mSprite->setScale(PLAYER_SCALE);
	mSprite->setAnchorPoint(Vec2(0, 0));
	mUlti = Sprite::createWithSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("skill1.png"));
	mUlti->setScale(1.1);
	mUlti->setAnchorPoint(Vec2(0, 0));
	mUlti->setPosition(mSprite->getPositionX() + 20, mSprite->getPositionY() + 20);

	//-------------PhysicsFoot Player--------------//
	mPhysicsFoot = PhysicsBody::createBox(Size(mSprite->getContentSize().width - 20, mSprite->getContentSize().height - 30), PhysicsMaterial(1.0f, 0.0f, 1.0f));
	mPhysicsFoot->setRotationEnable(false);
	mPhysicsFoot->setDynamic(true);
	mPhysicsFoot->setCollisionBitmask(Model::BITMASK_PLAYER_FOOT);
	mPhysicsFoot->setContactTestBitmask(true);
	mPhysicsFoot->setPositionOffset(Vec2(0, -20));
	mPhysicsFoot->setAngularDamping(1.0f);
	mSprite->setPhysicsBody(mPhysicsFoot);

	//-------------PhysicsBody Player-------------//
	mPhysicsBody = PhysicsBody::createBox(mSprite->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 1.0f));
	mPhysicsBody->setRotationEnable(false);
	mPhysicsBody->setMass(50);
	mPhysicsBody->setDynamic(false);
	mPhysicsBody->setCollisionBitmask(Model::BITMASK_PLAYER);
	mPhysicsBody->setContactTestBitmask(true);

	nodeBodyPlayer = Node::create();
	nodeBodyPlayer->setPhysicsBody(mPhysicsBody);
	nodeBodyPlayer->setPosition(Vec2(16, 23));

	mSprite->addChild(nodeBodyPlayer);

	//----------fix!!!
	auto node = Node::create();
	mAttackBody = PhysicsBody::createBox(Size(40.0f, 35.0f));
	mAttackBody->setRotationEnable(false);
	mAttackBody->setDynamic(false);
	mAttackBody->setEnabled(false);

	mAttackBody->setCollisionBitmask(Model::BITMASK_PLAYER_ATTACK);
	mAttackBody->setContactTestBitmask(true);
	node->setPhysicsBody(mAttackBody);
	node->setPosition(Vec2(45, 24));

	auto node1 = Node::create();
	mAttackBody1 = PhysicsBody::createBox(Size(45.0f, 20.0f));
	mAttackBody1->setRotationEnable(false);
	mAttackBody1->setDynamic(false);
	mAttackBody1->setEnabled(false);

	mAttackBody1->setCollisionBitmask(Model::BITMASK_PLAYER_ATTACK);
	mAttackBody1->setContactTestBitmask(true);
	node1->setPhysicsBody(mAttackBody1);
	node1->setPosition(Vec2(60, 22));

	auto node2 = Node::create();
	mAttackBody2 = PhysicsBody::createBox(Size(40.0f, 20.0f));
	mAttackBody2->setRotationEnable(false);
	mAttackBody2->setDynamic(false);
	mAttackBody2->setEnabled(false);

	mAttackBody2->setCollisionBitmask(Model::BITMASK_PLAYER_ATTACK);
	mAttackBody2->setContactTestBitmask(true);
	node2->setPhysicsBody(mAttackBody2);
	node2->setPosition(Vec2(55, 22));

	mSprite->addChild(node2);
	mSprite->addChild(node1);
	mSprite->addChild(node);

	mUlti->setVisible(false);
	mUlti->setGlobalZOrder(1);
	mUlti->setOpacity(-1);
	mSprite->setGlobalZOrder(2);


	mSprite->addChild(mUlti);
	layer->addChild(mSprite);

	Vector<SpriteFrame*> animFrames;

	//Move
	char str[100] = { 0 };
	for (int i = 11; i <= 14; ++i)
	{
		sprintf(str, "MC-sheet_%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_MOVE] = Repeat::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.2f)), 1);

	//Idle
	animFrames.clear();
	for (int i = 1; i <= 3; ++i)
	{
		sprintf(str, "MC-sheet_0%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_IDLE] = Repeat::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.2f)), 1);

	//Hitted
	animFrames.clear();
	sprintf(str, "MC-sheet_05.png");
	SpriteFrame* frame = cache->spriteFrameByName(str);
	animFrames.pushBack(frame);
	mAnimation[ANIM_DAMAGE] = Repeat::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.1f)), 1);

	//Attack
	animFrames.clear();
	for (int i = 30; i <= 33; ++i)
	{
		sprintf(str, "MC-sheet_%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_ATTACK] = Repeat::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.07f)), 1);

	//Skill 1
	animFrames.clear();
	for (int i = 69; i <= 73; ++i)
	{
		sprintf(str, "MC-sheet_%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_SKILL1] = Repeat::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.08f)), 1);

	//skill 2
	animFrames.clear();
	for (int i = 15; i <= 23; ++i)
	{
		sprintf(str, "MC-sheet_%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_SKILL2] = Repeat::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.06f)), 1);

	animFrames.clear();
	for (int i = 24; i >= 19; --i)
	{
		sprintf(str, "MC-sheet_%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_DELAY] = Repeat::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.02f)), 1);

	//skill 3
	animFrames.clear();
	for (int i = 1; i <= 3; ++i)
	{
		sprintf(str, "MC-sheet-ulti_00%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_SKILL3] = Repeat::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.1f)), 1);

	//ulti animate
	animFrames.clear();
	for (int i = 1; i <= 8; ++i)
	{
		sprintf(str, "skill%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_ULTI] = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.08f)));

	//Die
	animFrames.clear();
	for (int i = 5; i <= 6; ++i)
	{
		sprintf(str, "MC-sheet_0%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_DIE] = Repeat::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.2f)), 1);

	//retain
	CC_SAFE_RETAIN(mAnimation[ANIM_IDLE]);
	CC_SAFE_RETAIN(mAnimation[ANIM_MOVE]);
	CC_SAFE_RETAIN(mAnimation[ANIM_ATTACK]);
	CC_SAFE_RETAIN(mAnimation[ANIM_SKILL1]);
	CC_SAFE_RETAIN(mAnimation[ANIM_SKILL2]);
	CC_SAFE_RETAIN(mAnimation[ANIM_SKILL3]);
	CC_SAFE_RETAIN(mAnimation[ANIM_DAMAGE]);
	CC_SAFE_RETAIN(mAnimation[ANIM_DELAY]);
	CC_SAFE_RETAIN(mAnimation[ANIM_ULTI]);
	CC_SAFE_RETAIN(mAnimation[ANIM_DIE]);
	this->setAlive(true);

}


Player::~Player()
{
}

void Player::setCurrent_Mana(int curMana)
{
	mCurrent_Mana = curMana;
}

int Player::getCurrent_Mana()
{
	return mCurrent_Mana;
}

int Player::getMana()
{
	return mMana;
}

void Player::moveLeft()
{
	mIsRunLeftToRight = false;
	mSprite->setFlippedX(true);
	setState(Player::STATE_MOVE);
}

void Player::moveRight()
{
	mIsRunLeftToRight = true;
	mSprite->setFlippedX(false);
	setState(Player::STATE_MOVE);
}

void Player::moveUp()
{
	mIsRunBottomUp = true;
	setState(Player::STATE_MOVE);
}


void Player::moveDown()
{
	mIsRunBottomUp = false;
	setState(Player::STATE_MOVE);
}

void Player::setState(int nextState)
{
	if (!((mCurrentState == STATE_ATTACK || mCurrentState == STATE_SKILL1 || mCurrentState == STATE_SKILL2 || mCurrentState == STATE_SKILL3 || mCurrentState == STATE_DAMAGE) && mSprite->getNumberOfRunningActions() > 0))
	{
		switch (nextState) {
		case STATE_START:
			mIsRunLeftToRight = true;
			mCurrent_Blood = mBlood;
			mCurrent_Mana = mMana;
			setAlive(true);
			setState(STATE_IDLE);
			break;
		case STATE_IDLE:
			if (nextState != mCurrentState)
			{
				//mSprite->setAnchorPoint(Vec2(0.3, 0));
				mSprite->stopAllActions();
				mSprite->runAction(mAnimation[ANIM_IDLE]);
				mAttackBody->setEnabled(false);
				mAttackBody1->setEnabled(false);
				mAttackBody2->setEnabled(false);
			}
			else if (mSprite->getNumberOfRunningActions() == 0)//alway set if action finished
			{
				//mSprite->setAnchorPoint(Vec2(0.3, 0));
				mSprite->runAction(mAnimation[ANIM_IDLE]);
				mAttackBody->setEnabled(false);
				mAttackBody1->setEnabled(false);
				mAttackBody2->setEnabled(false);
			}
			break;
		case STATE_MOVE:
			if (nextState != mCurrentState)
			{
				//mSprite->setAnchorPoint(Vec2(0.4, 0));
				mSprite->stopAllActions();
				mSprite->runAction(mAnimation[ANIM_MOVE]);
			}
			else if (mSprite->getNumberOfRunningActions() == 0)//alway set if action finished
			{
				//mSprite->setAnchorPoint(Vec2(0.4, 0));
				mSprite->runAction(mAnimation[ANIM_MOVE]);
			}
			break;
		case STATE_ATTACK:
			if (nextState != mCurrentState) //one time
			{
				//mSprite->setAnchorPoint(Vec2(0, 0));
				mSprite->stopAllActions();
				mSprite->runAction(mAnimation[ANIM_ATTACK]);
				mAttackBody->setEnabled(true);
			}
			else if (mSprite->getNumberOfRunningActions() == 0)//alway set if action finished
			{
				//mSprite->setAnchorPoint(Vec2(0, 0));
				mSprite->runAction(mAnimation[ANIM_ATTACK]);
				mAttackBody->setEnabled(true);
			}

			break;
		case STATE_SKILL1:
			if (nextState != mCurrentState) //one time
			{
				//mSprite->setAnchorPoint(Vec2(0, 0));
				mCurrent_Mana -= 10;
				mSprite->stopAllActions();
				mAttackBody1->setEnabled(true);
				auto attack = CallFunc::create([&]()
				{
					mDamage = mDamage + (mLevel * 1 + 30);
				});
				auto finishAttack = CallFunc::create([&]()
				{
					mDamage = mDamage - (mLevel * 1 + 30);
				});
				mSprite->runAction(Sequence::create(attack, mAnimation[ANIM_SKILL1], finishAttack, nullptr));
			}
			else if (mSprite->getNumberOfRunningActions() == 0)//alway set if action finished
			{
				//mSprite->setAnchorPoint(Vec2(0, 0));
				mCurrent_Mana -= 10;
				mSprite->runAction(mAnimation[ANIM_SKILL1]);
				mAttackBody1->setEnabled(true);
				auto attack = CallFunc::create([&]()
				{
					mDamage = mDamage + (mLevel * 1 + 30);
				});
				auto finishAttack = CallFunc::create([&]()
				{
					mDamage = mDamage - (mLevel * 1 + 30);
				});
				mSprite->runAction(Sequence::create(attack, mAnimation[ANIM_SKILL1], finishAttack, nullptr));
			}
			break;
		case STATE_SKILL2:
			if (nextState != mCurrentState) //one time
			{
				//mSprite->setAnchorPoint(Vec2(0, 0));
				mCurrent_Mana -= 10;
				mSprite->stopAllActions();
				//mSprite->runAction(mAnimation[ANIM_SKILL2]);
				auto moveBy = MoveBy::create(0.5, Vec2(isMoveRight() ? 100 : -100, 0));
				auto spa = Spawn::create(moveBy, mAnimation[ANIM_SKILL2], nullptr);
				auto attack = CallFunc::create([&]()
				{
					mDamage = mDamage + (mLevel * 2 + 60);
					mAttackBody2->setEnabled(true);
				});
				auto finishAttack = CallFunc::create([&]()
				{
					mDamage = mDamage - (mLevel * 2 + 60);
					mAttackBody2->setEnabled(false);
				});
				auto seque = Sequence::create(spa, attack, mAnimation[ANIM_DELAY], finishAttack, nullptr);
				mSprite->runAction(seque);
			}
			else if (mSprite->getNumberOfRunningActions() == 0)//alway set if action finished
			{
				//mSprite->setAnchorPoint(Vec2(0, 0));
				mCurrent_Mana -= 10;
				//mSprite->runAction(mAnimation[ANIM_SKILL2]);
				auto moveBy = MoveBy::create(0.5, Vec2(isMoveRight() ? 60 : -60, 0));
				auto spa = Spawn::create(moveBy, mAnimation[ANIM_SKILL2], nullptr);
				auto attack = CallFunc::create([&]()
				{
					mDamage = mDamage + (mLevel * 2 + 60);
					mAttackBody2->setEnabled(true);
				});
				auto finishAttack = CallFunc::create([&]()
				{
					mDamage = mDamage - (mLevel * 2 + 60);
					mAttackBody2->setEnabled(false);
				});
				auto seque = Sequence::create(spa, attack, mAnimation[ANIM_DELAY], finishAttack, nullptr);
				mSprite->runAction(seque);
			}
			break;
		case STATE_SKILL3:
			if (nextState != mCurrentState) //one time
			{
				mSprite->setAnchorPoint(Vec2(0.1, 0));
				mCurrent_Mana -= 30;
				mSprite->stopAllActions();
				mSprite->runAction(mAnimation[ANIM_SKILL3]);
				auto action = Sequence::create(
					CallFunc::create(
					[&]() {
					mUlti->setVisible(true);
					mUlti->runAction(mAnimation[ANIM_ULTI]);
					mBlood = mBlood + (300 + (mLevel * 5));
					mCurrent_Blood = mCurrent_Blood + (300 + (mLevel * 5));
					mDamage = mDamage + (mLevel * 3 + 25);
					mArmor = mArmor + (mLevel * 1 + 5);
					mSprite->setColor(Color3B(255, 255, 155));
				}
					),
					DelayTime::create(10),
					CallFunc::create(
					[&]() {
					mUlti->setVisible(false);
					mUlti->stopAction(mAnimation[ANIM_ULTI]);
					mBlood = mBlood - (300 + (mLevel * 5));
					mCurrent_Blood = mCurrent_Blood - (300 + (mLevel * 5));
					mDamage = mDamage - (mLevel * 3 + 25);
					mArmor = mArmor - (mLevel * 1 + 5);
					mSprite->setColor(Color3B(255, 255, 255));
				}
					),
					NULL
					);
				mUlti->runAction(action);
			}
			else if (mSprite->getNumberOfRunningActions() == 0)//alway set if action finished
			{
				mSprite->setAnchorPoint(Vec2(0.1, 0));
				mCurrent_Mana -= 30;
				mSprite->runAction(mAnimation[ANIM_SKILL3]);
				auto action = Sequence::create(
					CallFunc::create(
					[&]() {
					mUlti->setVisible(true);
					mUlti->runAction(mAnimation[ANIM_ULTI]);
					mBlood = mBlood + (300 + (mLevel * 5));
					mCurrent_Blood = mCurrent_Blood + (300 + (mLevel * 5));
					mDamage = mDamage + (mLevel * 3 + 25);
					mArmor = mArmor + (mLevel * 1 + 5);
					mSprite->setColor(Color3B(255, 255, 155));
				}
					),
					DelayTime::create(10),
					CallFunc::create(
					[&]() {
					mUlti->setVisible(false);
					mUlti->stopAction(mAnimation[ANIM_ULTI]);
					mBlood = mBlood - (300 + (mLevel * 5));
					mCurrent_Blood = mCurrent_Blood - (300 + (mLevel * 5));
					mDamage = mDamage - (mLevel * 3 + 25);
					mArmor = mArmor - (mLevel * 1 + 5);
					mSprite->setColor(Color3B(255, 255, 255));
				}
					),
					NULL
					);
				mUlti->runAction(action);
			}
			break;
		case STATE_DAMAGE:
			if (mCurrentState != STATE_DAMAGE)
			{
				mSprite->stopAllActions();
				mSprite->runAction(mAnimation[ANIM_DAMAGE]);
				SimpleAudioEngine::getInstance()->playEffect("sounds/BiDanhTrung.mp3");
			}
			break;
		case STATE_DIE:
			if (mCurrentState != STATE_DIE)
			{
				mSprite->stopAllActions();
				auto anim = CallFunc::create([=]() {
					mSprite->runAction(mAnimation[ANIM_DIE]);
				});
				mSprite->setOpacity(130);
				auto fadeOut = FadeOut::create(2);
				mSprite->runAction(Sequence::create(anim, fadeOut, nullptr));

			}
			else if (mAnimation[ANIM_DIE]->isDone())
			{
				setAlive(false);
			}
			break;
		}
		mCurrentState = nextState;
	}
}

void Player::update(float dt)
{
	if (isAlive() == true)
	{
		deltaTime += dt;
		if (deltaTime >= 10)
		{
			deltaTime = 0;
			setCurrent_Blood(getCurrent_Blood() + 5);
			setCurrent_Mana(getCurrent_Mana() + 3);
			if (getCurrent_Blood() >= getBlood())
				setCurrent_Blood(getBlood());
			if (getCurrent_Mana() >= getMana())
				setCurrent_Mana(getMana());
		}
		if (mCurrent_Exp >= mMaxExp)
		{
			upLevel();
		}
	}
	

	if (mCurrent_Blood <= 0)
	{

		//setAlive(false);
		//mSprite->stopAllActions();
		mAttackBody1->setEnabled(false);
		mAttackBody2->setEnabled(false);
		mPhysicsBody->setEnabled(false);
		setState(STATE_DIE);
		auto funcPause = CallFunc::create([]() {
			Director::getInstance()->pause();
		});
	}
	//log("%d", mBlood);
}


void Player::setFlip(bool direction)
{
	if (direction)
	{
		mSprite->setScaleX(PLAYER_SCALE);
	}
	else
	{
		mSprite->setScaleX(-PLAYER_SCALE);
	}
}

void Player::upLevel()
{
	McSetting* mcSetting = McSetting::getInstance();

	mBlood = mBlood + 20 * (mcSetting->getPlayerLevel() * 0.25) + 5;
	mDamage = mDamage + 10 * (mcSetting->getPlayerLevel() * 0.2) + mcSetting->getPlayerLevel();
	mArmor = mArmor + 5 * mcSetting->getPlayerLevel() * 0.3 + 4;
	mMaxExp = mMaxExp + mcSetting->getPlayerLevel() * 5;
	mCurrent_Exp = 0;
	mLevel = mLevel + 1;

	mcSetting->setBloodPlayer(mBlood);
	mcSetting->setDamePlayer(mDamage);
	mcSetting->setArmor(mArmor);
	mcSetting->setEXP(0);
	mcSetting->setMaxEXP(mMaxExp);
	mcSetting->setPlayerLevel(mcSetting->getPlayerLevel() + 1);

}