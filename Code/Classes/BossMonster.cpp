#include "BossMonster.h"

#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

Node * nodeBodyMonsterBoss;
Node * hud;

BossMonster::BossMonster(cocos2d::Layer* layer)
{
	//Monster stats
	mDamage = 40;
	mBlood = 800;
	mCurrent_Blood = mBlood;
	mArmor = 20;
	mCurrent_Exp = 20;
	mCurrentState = STATE_START;
	mIsALive = true;
	scale = 1.4f;
	mIsMcLeft = true;

	SpriteFrameCache* cache = SpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("Creep/dragon.plist", "Creep/dragon.png");
	mSprite = Sprite::createWithSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("BlackDragon_115.png"));
	mSprite->setScale(scale);
	mSprite->setScaleX(-scale);
	mSprite->setAnchorPoint(Vec2(0.5, 0));
	mSprite->setPosition(Vec2(600, 300));

	//physics body
	//------------PhysicFoot CowMonster----------------//
	mPhysicsFoot = PhysicsBody::createBox(Size(mSprite->getContentSize().width - 120, mSprite->getContentSize().height - 100), PhysicsMaterial(1.0f, 0.0f, 1.0f));
	mPhysicsFoot->setDynamic(true);
	mPhysicsFoot->setRotationEnable(false);
	mPhysicsFoot->setCollisionBitmask(BITMASK_MONSTER_FOOT);
	mPhysicsFoot->setContactTestBitmask(true);
	mPhysicsFoot->setPositionOffset(Vec2(20, -48));
	mPhysicsFoot->setMass(9999);
	mPhysicsFoot->setVelocityLimit(0);
	mSprite->setPhysicsBody(mPhysicsFoot);

	//------------PhysicsBody CowMonster-----------------//
	mPhysicsBody = PhysicsBody::createBox(Size(mSprite->getContentSize().width-70, mSprite->getContentSize().height - 45), PhysicsMaterial(1.0f, 0.0f, 1.0f));
	mPhysicsBody->setRotationEnable(false);
	mPhysicsBody->setDynamic(false);
	mPhysicsBody->setCollisionBitmask(Model::BITMASK_BOSS_MONSTER);
	mPhysicsBody->setContactTestBitmask(true);
	nodeBodyMonsterBoss = Node::create();
	nodeBodyMonsterBoss->setPhysicsBody(mPhysicsBody);
	nodeBodyMonsterBoss->setPosition(Vec2(80, 45));
	mSprite->addChild(nodeBodyMonsterBoss);

	//Attack Physic of Cow Monster
	auto *node = Node::create();
	mAttackBody = PhysicsBody::createBox(Size(60.0f, 50.0f));
	mAttackBody->setRotationEnable(false);
	mAttackBody->setDynamic(false);
	mAttackBody->setEnabled(false);

	mAttackBody->setCollisionBitmask(Model::BITMASK_BOSS_ATTACK);
	mAttackBody->setContactTestBitmask(true);
	node->setPhysicsBody(mAttackBody);
	node->setPosition(Vec2(150, 24));

	//Create hud for cow
	hud = Sprite::create("Hud/monster_blood_bg.png");
	hud->setAnchorPoint(Vec2(0, 0));
	//hud->setPosition(Vec2(0,80));
	hud->setScale(0.09f, 0.06f);

	auto hudNode = Node::create();
	mBossHudBlood = ui::LoadingBar::create("Hud/monster_blood.png");
	mBossHudBlood->setAnchorPoint(Vec2(0,0));
	//mBossHudBlood->setPosition(Vec2(0,80));
	mBossHudBlood->setDirection(ui::LoadingBar::Direction::LEFT);
	mBossHudBlood->setPercent(100);
	mBossHudBlood->setScale(0.09f, 0.06f);
	hudNode->addChild(hud);
	hudNode->addChild(mBossHudBlood);

	mSprite->addChild(hudNode);
	mSprite->addChild(node);
	layer->addChild(mSprite);


	Vector<SpriteFrame*> animFrames;
	char str[100] = { 0 };
	//attack
	for (int i = 107; i <= 111; ++i)
	{
		sprintf(str, "BlackDragon_%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_ATTACK] = Repeat::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.15f)),1);

	//attack 1
	animFrames.clear();
	for (int i = 115; i <= 119; ++i)
	{
		sprintf(str, "BlackDragon_%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_ATTACK_1] = Repeat::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.2f)),1);

	//attack 2
	animFrames.clear();
	for (int i = 137; i <= 141; ++i)
	{
		sprintf(str, "BlackDragon_%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_ATTACK_2] = Repeat::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.2f)),1);

	//Idle
	animFrames.clear();
	for (int i = 1; i <= 6; ++i)
	{
		sprintf(str, "BlackDragon_%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_IDLE] = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.3f)));

	//Move
	animFrames.clear();
	for (int i = 16; i <= 17; ++i)
	{
		sprintf(str, "BlackDragon_%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_START_MOVE] = Repeat::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.1f)),1);

	animFrames.clear();
	for (int i = 18; i <= 20; ++i)
	{
		sprintf(str, "BlackDragon_%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	for (int i = 20; i >= 18; --i)
	{
		sprintf(str, "BlackDragon_%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_MOVE] = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.2f)));

	//die
	animFrames.clear();
	for (int i = 28; i <= 34; ++i)
	{
		sprintf(str, "BlackDragon_%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_DIE] = Repeat::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.1f)),1);

	CC_SAFE_RETAIN(mAnimation[ANIM_ATTACK]);
	CC_SAFE_RETAIN(mAnimation[ANIM_ATTACK_1]);
	CC_SAFE_RETAIN(mAnimation[ANIM_ATTACK_2]);
	CC_SAFE_RETAIN(mAnimation[ANIM_IDLE]);
	CC_SAFE_RETAIN(mAnimation[ANIM_START_MOVE]);
	CC_SAFE_RETAIN(mAnimation[ANIM_MOVE]);
	CC_SAFE_RETAIN(mAnimation[ANIM_DIE]);	
}

BossMonster::~BossMonster()
{

}

void BossMonster::setState(int state)
{
	switch (state)
	{
	case STATE_START:
		mIsRunLeftToRight = true;
		mCurrent_Blood = mBlood;
		setAlive(true);
		setState(STATE_IDLE);
		break;
	case STATE_IDLE:
		if (mCurrentState != state)
		{
			mAttackBody->setEnabled(false);
			mSprite->stopAllActions();
			mSprite->runAction(mAnimation[ANIM_IDLE]);
		}
		break;
	case STATE_ATTACK:
		if (mCurrentState != state)
		{
			if (rd == 5 || rd == 6)
			{
				mDamage = 55;
				mSprite->stopAllActions();
				auto anim = CallFunc::create([=]()
				{
					mSprite->runAction(mAnimation[ANIM_ATTACK_2]);
				});
				auto attack = CallFunc::create([=]()
				{
					mAttackBody->setEnabled(true);
				});
				auto finish = CallFunc::create([=]()
				{
					//mSprite->setAnchorPoint(Vec2(0.5, 0.5));
					mAttackBody->setEnabled(false);
				});
				mSprite->runAction(Sequence::create(anim, DelayTime::create(0.5f), attack, DelayTime::create(0.1f), finish, nullptr));
			}
			else
			{ 
				mDamage = 40;
				mSprite->stopAllActions();
				auto anim = CallFunc::create([=]()
				{
					mSprite->runAction(mAnimation[ANIM_ATTACK]);
				});
				auto attack = CallFunc::create([=]()
				{
					mAttackBody->setEnabled(true);
				});
				auto finish = CallFunc::create([=]()
				{
					//mSprite->setAnchorPoint(Vec2(0.5, 0.5));
					mAttackBody->setEnabled(false);
				});
				mSprite->runAction(Sequence::create(anim, DelayTime::create(0.5f), attack, DelayTime::create(0.1f), finish, nullptr));
			}
		}
		break;
	case STATE_MOVE:
		//setFlip(mIsMcLeft);
		if (mCurrentState != state)
		{
			mAttackBody->setEnabled(false);
			mSprite->stopAllActions();
			auto start = CallFunc::create([=]()
			{
				mSprite->runAction(mAnimation[ANIM_START_MOVE]);
			});
			auto move = CallFunc::create([=]()
			{
				mSprite->runAction(mAnimation[ANIM_MOVE]);
			});
			mSprite->runAction(Sequence::create(start, DelayTime::create(0.35f), move, nullptr));		
		}
		break;
	case STATE_ATTACK_1:
		SimpleAudioEngine::getInstance()->playEffect("sounds/dragon.aiff");
		if (mCurrentState != state)
		{
			if (rd == 5 || rd == 9)
			{
				mDamage = 60;
				mSprite->stopAllActions();
				auto anim = CallFunc::create([=]()
				{
					mSprite->runAction(mAnimation[ANIM_ATTACK_2]);
				});
				auto attack = CallFunc::create([=]()
				{
					mAttackBody->setEnabled(true);
				});
				auto finish = CallFunc::create([=]()
				{
					//mSprite->setAnchorPoint(Vec2(0.5, 0.5));
					mAttackBody->setEnabled(false);
				});
				mSprite->runAction(Sequence::create(anim, DelayTime::create(0.5f), attack, DelayTime::create(0.1f), finish, nullptr));
			}
			else
			{ 
				mSprite->stopAllActions();
				mDamage = 45;
				auto anim = CallFunc::create([=]()
				{
					mSprite->runAction(mAnimation[ANIM_ATTACK_1]);
				});
				auto attack = CallFunc::create([=]()
				{
					mAttackBody->setEnabled(true);
				});
				auto finish = CallFunc::create([=]()
				{
					//mSprite->setAnchorPoint(Vec2(0.5, 0.5));
					mAttackBody->setEnabled(false);
				});
				mSprite->runAction(Sequence::create(anim, DelayTime::create(0.5f), attack, DelayTime::create(0.1f), finish, nullptr));
			}
		}
		break;
	case STATE_ATTACK_2:
		if (mCurrentState != state)
		{
			mSprite->stopAllActions();
			auto anim = CallFunc::create([=]()
			{
				mSprite->runAction(mAnimation[ANIM_ATTACK_2]);
			});
			auto attack = CallFunc::create([=]()
			{
				mAttackBody->setEnabled(true);
			});
			auto finish = CallFunc::create([=]()
			{
				//mSprite->setAnchorPoint(Vec2(0.5, 0.5));
				mAttackBody->setEnabled(false);
			});
			mSprite->runAction(Sequence::create(anim, DelayTime::create(0.5f), attack, DelayTime::create(0.2f), finish, nullptr));
		}
		break;
	case STATE_DIE:
		if (mCurrentState != STATE_DIE)
		{
			mPhysicsBody->setEnabled(false);
			mSprite->setOpacity(130);
			mSprite->runAction(Spawn::create(FadeOut::create(2.0f), mAnimation[ANIM_DIE], nullptr));
		}
		else if (mAnimation[ANIM_DIE]->isDone())
		{
			setAlive(false);
		}
	}
	mCurrentState = state;
}

void BossMonster::setFlip(bool direction)
{
	if (direction)
	{
		mSprite->setScaleX(scale);
		mBossHudBlood->setDirection(ui::LoadingBar::Direction::LEFT);
		mPhysicsFoot->setPositionOffset(Vec2(-20, -48));
	}
	else
	{
		mSprite->setScaleX(-scale);
		mBossHudBlood->setDirection(ui::LoadingBar::Direction::RIGHT);
		mPhysicsFoot->setPositionOffset(Vec2(20, -48));
	}
}

void BossMonster::update()
{
	mSprite->setAnchorPoint(Vec2(0.5, 0));
	rd = rand() % 10 + 1;
	//change state
	if (mSprite->getNumberOfRunningActions() == 0)
	{
		if (mCurrentState == STATE_ATTACK)
		{
			//mAttackBody->setEnabled(true);
			setState(STATE_IDLE);
		}
		else if (mCurrentState == STATE_DAMAGE)
		{
			setState(STATE_IDLE);
		}
		else if (mCurrentState == STATE_ATTACK_1)
		{
			//mAttackBody->setEnabled(true);
			setState(STATE_IDLE);
		}
	}
	if (mCurrent_Blood <= 0)
	{
		setState(STATE_DIE);
	}
	//nodeBodyMonsterCow->setPosition(Vec2(mSprite->getContentSize().width / 2, mSprite->getContentSize().height / 2));
	//this->mSprite->setAnchorPoint({ 0.5f, 0 });
	//Set blood for Cow
	mBossHudBlood->setPercent((mCurrent_Blood * 100) / mBlood);
	nodeBodyMonsterBoss->setPosition(Vec2(mSprite->getContentSize().width / 2, mSprite->getContentSize().height / 2 - 5));
	mBossHudBlood->setPosition(Vec2(mSprite->getContentSize().width / 2 - 35, mSprite->getContentSize().height / 2 + 35));
	hud->setPosition(Vec2(mSprite->getContentSize().width / 2 - 35, mSprite->getContentSize().height / 2 + 35));
	//mPhysicsFoot->setPositionOffset(Vec2(mSprite->getContentSize().width / 2 -5, mSprite->getContentSize().height / 2 - 100));
}

void BossMonster::setDetect(bool detect)
{
	mDetect = detect;
}

bool BossMonster::getDetect()
{
	return mDetect;
}