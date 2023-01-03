#include "CowMonster.h"
Node * nodeBodyMonsterCow;

#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

CowMonster::CowMonster(cocos2d::Layer* layer)
{
	//Monster stats
	mDamage = 20;
	mBlood = 200;
	mCurrent_Blood = mBlood;
	mArmor = 0;
	mCurrent_Exp = 5;
	mCurrentState = STATE_START;
	mIsMcLeft = true;
	mIsALive = true;
	SpriteFrameCache* cache = SpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("Creep/Buffalo_Monster1.plist", "Creep/Buffalo_Monster1.png");
	mSprite = Sprite::createWithSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("Buffalo_Monster_1.png"));
	mSprite->setScale(COW_SCALE);
	mSprite->setAnchorPoint(Vec2(0.5, 0));
	//this->mSprite->setAnchorPoint({ 0.5f, 0 });

	//------------PhysicFoot CowMonster----------------//
	mPhysicsFoot = PhysicsBody::createBox(Size(mSprite->getContentSize().width - 30, mSprite->getContentSize().height - 80), PhysicsMaterial(1.0f, 0.0f, 1.0f));
	mPhysicsFoot->setDynamic(true);
	mPhysicsFoot->setRotationEnable(false);
	mPhysicsFoot->setCollisionBitmask(BITMASK_MONSTER_FOOT);
	mPhysicsFoot->setContactTestBitmask(true);
	mPhysicsFoot->setPositionOffset(Vec2(0, -30));
	mPhysicsFoot->setMass(9999);
	mPhysicsFoot->setVelocityLimit(0);
	mSprite->setPhysicsBody(mPhysicsFoot);

	//------------PhysicsBody CowMonster-----------------//
	mPhysicsBody = PhysicsBody::createBox(Size(mSprite->getContentSize().width, mSprite->getContentSize().height - 10), PhysicsMaterial(1.0f, 0.0f, 1.0f));
	mPhysicsBody->setRotationEnable(false);
	mPhysicsBody->setDynamic(false);
	mPhysicsBody->setCollisionBitmask(BITMASK_COW_MONSTER);
	mPhysicsBody->setContactTestBitmask(true);
	nodeBodyMonsterCow = Node::create();
	//nodeBodyMonsterCow->setAnchorPoint({ 0, 0 });
	nodeBodyMonsterCow->setPhysicsBody(mPhysicsBody);
	nodeBodyMonsterCow->setPosition(Vec2(mSprite->getContentSize().width / 2, mSprite->getContentSize().height / 2));
	mSprite->addChild(nodeBodyMonsterCow);
	mSprite->setFlippedX(mIsMcLeft);
	//setMoveRight(false);

	//Create hud for cow
	auto hud = Sprite::create("Hud/monster_blood_bg.png");
	hud->setAnchorPoint(Vec2(0, 0));
	hud->setPosition(Vec2(mSprite->getPosition().x, mSprite->getPosition().y + mSprite->getContentSize().height / 2 + 45));
	hud->setScale(0.06f, 0.06f);

	auto hudNode = Node::create();
	mCowHudBlood = ui::LoadingBar::create("Hud/monster_blood.png");
	mCowHudBlood->setAnchorPoint(Vec2(0, 0));
	mCowHudBlood->setPosition(Vec2(mSprite->getPosition().x, mSprite->getPosition().y + mSprite->getContentSize().height / 2 + 45));
	mCowHudBlood->setDirection(ui::LoadingBar::Direction::LEFT);
	mCowHudBlood->setPercent(100);
	mCowHudBlood->setScale(0.06f, 0.06f);
	hudNode->addChild(hud);
	hudNode->addChild(mCowHudBlood);


	//Attack Physic of Cow Monster
	auto node = Node::create();
	mAttackBody = PhysicsBody::createBox(Size(20.0f, 35.0f));
	mAttackBody->setRotationEnable(false);
	mAttackBody->setDynamic(false);
	mAttackBody->setEnabled(false);

	mAttackBody->setCollisionBitmask(Model::BITMASK_COWMONSTER_ATTACK);
	mAttackBody->setContactTestBitmask(true);
	node->setPhysicsBody(mAttackBody);
	node->setPosition(Vec2(-8, 24));

	mSprite->addChild(node);
	mSprite->addChild(hudNode);
	layer->addChild(mSprite);

	Vector<SpriteFrame*> animFrames;

	//idle
	char str[100] = { 0 };
	animFrames.clear();
	for (int i = 1; i <= 14; ++i)
	{
		sprintf(str, "Buffalo_Monster_%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_IDLE] = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.2f)));

	//move
	animFrames.clear();
	for (int i = 16; i <= 24; ++i)
	{
		sprintf(str, "Buffalo_Monster_%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_MOVE] = Repeat::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.2f)), 1);

	//attack
	animFrames.clear();
	for (int i = 25; i <= 30; ++i)
	{
		sprintf(str, "Buffalo_Monster_%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_ATTACK] = Repeat::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.15f)), 1);

	//damage
	animFrames.clear();
	for (int i = 31; i <= 36; ++i)
	{
		sprintf(str, "Buffalo_Monster_%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	//for (int i = 34; i >= 31; --i)
	//{
	//	sprintf(str, "Buffalo_Monster_%d.png", i);
	//	SpriteFrame* frame = cache->spriteFrameByName(str);
	//	animFrames.pushBack(frame);
	//}
	mAnimation[ANIM_DAMAGE] = Repeat::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.03f)), 1);

	//die
	animFrames.clear();
	for (int i = 37; i <= 44; i++)
	{
		sprintf(str, "Buffalo_Monster_%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_DIE] = Repeat::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.1f)), 1);

	CC_SAFE_RETAIN(mAnimation[ANIM_MOVE]);
	CC_SAFE_RETAIN(mAnimation[ANIM_IDLE]);
	CC_SAFE_RETAIN(mAnimation[ANIM_ATTACK]);
	CC_SAFE_RETAIN(mAnimation[ANIM_DAMAGE]);
	CC_SAFE_RETAIN(mAnimation[ANIM_DIE]);
	setState(STATE_IDLE);
}


void CowMonster::setState(int nextState)
{
	//if (!((mCurrentState == STATE_ATTACK || mCurrentState == STATE_SKILL1 || mCurrentState == STATE_SKILL2 || mCurrentState == STATE_SKILL3 || mCurrentState == STATE_DAMAGE) && mSprite->getNumberOfRunningActions() > 0))
	//{
	switch (nextState) {
	case STATE_START:
		mIsRunLeftToRight = true;
		mCurrent_Blood = mBlood;
		setAlive(true);
		setState(STATE_IDLE);
		break;
	case STATE_IDLE:

		if (nextState != mCurrentState)
		{
			mSprite->stopAllActions();
			mSprite->runAction(mAnimation[ANIM_IDLE]);
			mAttackBody->setEnabled(false);
		}
		else if (mAnimation[ANIM_IDLE]->isDone())//alway set if action finished
		{
			mSprite->runAction(mAnimation[ANIM_IDLE]);
			mAttackBody->setEnabled(false);
		}
		break;
	case STATE_MOVE:

		if (nextState != mCurrentState)
		{
			mSprite->stopAllActions();
			mSprite->runAction(mAnimation[ANIM_MOVE]);
		}
		else if (mAnimation[ANIM_MOVE]->isDone())//alway set if action finished
		{

			mSprite->runAction(mAnimation[ANIM_MOVE]);
			//mSprite->stopAllActions();
		}
		break;
	case STATE_ATTACK:
		if (nextState != mCurrentState)
		{
			mSprite->stopAllActions();
			auto anim = CallFunc::create([&]()
			{
				mSprite->runAction(mAnimation[ANIM_ATTACK]);
			});
			auto attack = CallFunc::create([&]()
			{
				mAttackBody->setEnabled(true);
			});
			auto finish = CallFunc::create([&]()
			{
				//mSprite->setAnchorPoint(Vec2(0.5, 0.5));
				mAttackBody->setEnabled(false);
			});
			mSprite->runAction(Sequence::create(anim, DelayTime::create(0.5f), attack, DelayTime::create(0.1f), finish, nullptr));
		}
		break;
	case STATE_DAMAGE:
		SimpleAudioEngine::getInstance()->playEffect("sounds/SCREAM3.mp3");
		if (mCurrentState != STATE_DAMAGE)
		{
			mSprite->stopAllActions();
			mSprite->runAction(mAnimation[ANIM_DAMAGE]);
		}
		else if (mAnimation[ANIM_DAMAGE]->isDone())//alway set if action finished
		{
			mSprite->runAction(mAnimation[ANIM_DAMAGE]);
		}
		break;

	case STATE_DIE:
		if (mCurrentState != STATE_DIE)
		{
			mSprite->stopAllActions();
			mPhysicsBody->setEnabled(false);
			mSprite->setOpacity(130);
			mSprite->runAction(Spawn::create(FadeOut::create(2.0f), mAnimation[ANIM_DIE], nullptr));
		}
		else if (mAnimation[ANIM_DIE]->isDone())
		{
			setAlive(false);
		}
		break;
	}
	mCurrentState = nextState;
}

CowMonster::~CowMonster()
{

}


void CowMonster::update()
{
	CCLOG("Update Cow in Class");
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
	}
	/*if (mCurrent_Blood <= 0)
	{
		setState(STATE_DIE);
	}*/
	nodeBodyMonsterCow->setPosition(Vec2(mSprite->getContentSize().width / 2, mSprite->getContentSize().height / 2));
	this->mSprite->setAnchorPoint({ 0.5f, 0 });
	//Set blood for Cow
	mCowHudBlood->setPercent((mCurrent_Blood * 100)/mBlood);
}

void CowMonster::setFlip(bool direction)
{
	if (direction)
	{
		mCowHudBlood->setDirection(ui::LoadingBar::Direction::LEFT);
		mSprite->setScaleX(COW_SCALE);
		mPhysicsFoot->setPositionOffset(Vec2(-0, -30));
	}
	else
	{
		mCowHudBlood->setDirection(ui::LoadingBar::Direction::RIGHT);
		mSprite->setScaleX(-COW_SCALE);
		mPhysicsFoot->setPositionOffset(Vec2(0, -30));
	}
}

/*Get/Set*/
void CowMonster::setDetect(bool detect)
{
	mDetect = detect;
}

bool CowMonster::getDetect()
{
	return mDetect;
}