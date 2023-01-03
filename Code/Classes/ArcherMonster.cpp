#include "ArcherMonster.h"

#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

Node * nodeBodyMonsterArrow;

ArcherMonster::ArcherMonster(cocos2d::Layer* layer)
{
	mDamage = 15;
	mBlood = 200;
	mCurrent_Blood = mBlood;
	mArmor = 0;
	scale = 0.85f;
	mCurrent_Exp = 10;
	mCurrentState = STATE_START;
	mIsALive = true;
	SpriteFrameCache* cache = SpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("Creep/Archer.plist", "Creep/Archer.png");
	mSprite = Sprite::createWithSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("Green_Archer_25.png"));
	mSprite->setAnchorPoint({ 0.4f, 0.4f });
	mSprite->setPosition(300, 300);
	mSprite->setScaleY(scale);
	mSprite->setScaleX(-scale);
	mSprite->setAnchorPoint(Vec2(0.5, 0));

	//physics body
	//------------PhysicFoot CowMonster----------------//
	mPhysicsFoot = PhysicsBody::createBox(Size(mSprite->getContentSize().width - 10, mSprite->getContentSize().height - 80), PhysicsMaterial(1.0f, 0.0f, 1.0f));
	mPhysicsFoot->setDynamic(true);
	mPhysicsFoot->setRotationEnable(false);
	mPhysicsFoot->setCollisionBitmask(BITMASK_MONSTER_FOOT);
	mPhysicsFoot->setContactTestBitmask(true);
	mPhysicsFoot->setPositionOffset(Vec2(-5, -30));
	mPhysicsFoot->setMass(9999);
	mPhysicsFoot->setVelocityLimit(0);
	mSprite->setPhysicsBody(mPhysicsFoot);

	//------------PhysicsBody CowMonster-----------------//
	mPhysicsBody = PhysicsBody::createBox(Size(mSprite->getContentSize().width, mSprite->getContentSize().height - 10), PhysicsMaterial(1.0f, 0.0f, 1.0f));
	mPhysicsBody->setRotationEnable(false);
	mPhysicsBody->setDynamic(false);
	mPhysicsBody->setCollisionBitmask(BITMASK_ARCHER_MONSTER);
	mPhysicsBody->setContactTestBitmask(true);
	nodeBodyMonsterArrow = Node::create();
	nodeBodyMonsterArrow->setPhysicsBody(mPhysicsBody);
	nodeBodyMonsterArrow->setPosition(Vec2(25, 45));
	mSprite->addChild(nodeBodyMonsterArrow);

	//Attack Physic of Cow Monster
	auto node = Node::create();
	mAttackBody = PhysicsBody::createBox(Size(20.0f, 60.0f));
	mAttackBody->setRotationEnable(false);
	mAttackBody->setDynamic(false);
	mAttackBody->setEnabled(false);

	mAttackBody->setCollisionBitmask(Model::BITMASK_ARCHER_ATTACK);
	mAttackBody->setContactTestBitmask(true);
	node->setPhysicsBody(mAttackBody);
	node->setPosition(Vec2(60, 45));

	//Create hud for cow
	auto hud = Sprite::create("Hud/monster_blood_bg.png");
	hud->setAnchorPoint(Vec2(0.5, 0));
	hud->setPosition(Vec2(27, 80));
	hud->setScale(0.05f, 0.05f);

	auto hudNode = Node::create();
	mArcherHudBlood = ui::LoadingBar::create("Hud/monster_blood.png");
	mArcherHudBlood->setAnchorPoint(Vec2(0.5, 0));
	mArcherHudBlood->setPosition(Vec2(27, 80));
	mArcherHudBlood->setDirection(ui::LoadingBar::Direction::LEFT);
	mArcherHudBlood->setPercent(100);
	mArcherHudBlood->setScale(0.05f, 0.05f);
	hudNode->addChild(hud);
	hudNode->addChild(mArcherHudBlood);

	mSprite->addChild(node);
	mSprite->addChild(hudNode);
	layer->addChild(mSprite);

	//create animate
	Vector<SpriteFrame*> animFrames;
	char str[100] = { 0 };
	//Fire
	for (int i = 25; i <= 31; ++i)
	{
		sprintf(str, "Green_Archer_%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	//float duration = (rand() % 3 + 12) * 1.0f / 100; //0.05f - 0.14f
	mAnimation[ANIM_FIRE] = Repeat::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.15f)), 1);

	//Attack
	animFrames.clear();
	for (int i = 77; i <= 84; ++i)
	{
		sprintf(str, "Green_Archer_%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_ATTACK] = Repeat::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.1f)), 1);

	//damage
	animFrames.clear();
	for (int i = 143; i <= 146; ++i)
	{
		sprintf(str, "Green_Archer_%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_DAMAGE] = Repeat::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.05f)), 1);

	//idle
	animFrames.clear();
	for (int i = 129; i <= 130; ++i)
	{
		sprintf(str, "Green_Archer_%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_IDLE] = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.5f)));

	//Move
	animFrames.clear();
	for (int i = 127; i <= 130; ++i)
	{
		sprintf(str, "Green_Archer_%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_MOVE] = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.2f)));

	//Die
	animFrames.clear();
	for (int i = 148; i <= 153; ++i)
	{
		sprintf(str, "Green_Archer_%d.png", i);
		SpriteFrame* frame = cache->spriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	mAnimation[ANIM_DIE] = Repeat::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.05f)), 1);


	CC_SAFE_RETAIN(mAnimation[ANIM_IDLE]);
	CC_SAFE_RETAIN(mAnimation[ANIM_DAMAGE]);
	CC_SAFE_RETAIN(mAnimation[ANIM_FIRE]);
	CC_SAFE_RETAIN(mAnimation[ANIM_ATTACK]);
	CC_SAFE_RETAIN(mAnimation[ANIM_MOVE]);
	CC_SAFE_RETAIN(mAnimation[ANIM_DIE]);

	//Arrow
	auto sprite = Sprite::create("arrow.png", Rect(0, 0, 30, 4));
	mArrow = new Arrow(layer, sprite, BITMASK_ARCHER_ARROW);
	mArrow->setArrowAlive(false);
	mArrow->setPosition(mSprite->getPosition());
}

ArcherMonster::~ArcherMonster()
{
	for (int i = 0; i < ANIM_TOTAl; i++)
	{
		if (mAnimation[i])
		{
			mAnimation[i]->autorelease();
		}
	}
	CC_SAFE_DELETE(mArrow);
}

void ArcherMonster::setState(int state)
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
	case STATE_DAMAGE:
		if (mCurrentState != STATE_DAMAGE)
		{
			mSprite->stopAllActions();
		}
		if (mCurrentState != state)
		{
			mSprite->runAction(mAnimation[ANIM_DAMAGE]);
		}
		break;
	case STATE_ATTACK:
		if (mCurrentState != state)
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
	case STATE_MOVE:
		setFlip(mIsMcLeft);
		if (mCurrentState != state)
		{
			mAttackBody->setEnabled(false);
			mSprite->stopAllActions();
			mSprite->runAction(mAnimation[ANIM_MOVE]);
		}
		break;
	case STATE_FIRE_1:
		if (mCurrentState != state)
		{
			mSprite->stopAllActions();
			mSprite->runAction(mAnimation[ANIM_FIRE]);
		}
		break;
	case STATE_FIRE_2:
		if (mArrow->isAlive() == false)
		{
			mArrow->setArrowAlive(true);
			mArrow->setPosition(Vec2(mSprite->getPosition().x, mSprite->getPosition().y + 40));
			mArrow->explode(!mIsMcLeft);
			mArrow->fire();
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
			mArrow->setArrowAlive(false);
		}
	}
	mCurrentState = state;
}

void ArcherMonster::setFlip(bool direction)
{
	if (direction)
	{
		mSprite->setScaleX(-scale);
		mArcherHudBlood->setDirection(ui::LoadingBar::Direction::RIGHT);
		mPhysicsFoot->setPositionOffset(Vec2(-5, -30));
	}
	else
	{
		mSprite->setScaleX(scale);
		mArcherHudBlood->setDirection(ui::LoadingBar::Direction::LEFT);
		mPhysicsFoot->setPositionOffset(Vec2(5, -30));
	}
}

void ArcherMonster::update(bool isMcLeft)
{
	CCLOG("Update Archer in Class");
	mIsMcLeft = isMcLeft;
	//change state
	if (mSprite->getNumberOfRunningActions() == 0)
	{
		if (mCurrentState == STATE_FIRE_2)
		{
			setState(STATE_FIRE_1);
		}
		else if (mCurrentState == STATE_FIRE_1)
		{
			setState(STATE_FIRE_2);
		}
		if (mCurrentState == STATE_ATTACK)
		{
			setState(STATE_IDLE);
		}
		else if (mCurrentState == STATE_IDLE)
		{
			setState(STATE_ATTACK);
		}
		//update orientation
		setFlip(mIsMcLeft);
		//mSprite->setScaleX(-1);
	}
	//Set blood for Cow
	mArcherHudBlood->setPercent((mCurrent_Blood * 100)/ mBlood);
}

void ArcherMonster::bulletHasCollision()
{
	mArrow->setArrowAlive(false);
}


