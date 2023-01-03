#include "Arrow.h"

//Size visible = Director::getInstance()->getVisibleSize();

Arrow::Arrow(Layer* layer, Sprite *sprite, int owner)
{
	mSprite = sprite;
	mIsALive = false;
	//physics body
	mPhysicsBody = PhysicsBody::createBox(Size(30, 4), PhysicsMaterial(1.0f, 0.0f, 1.0f));
	mPhysicsBody->setRotationEnable(false);
	mPhysicsBody->setDynamic(false);
	mPhysicsBody->setGravityEnable(false);
	mPhysicsBody->setCollisionBitmask(owner);
	mPhysicsBody->setContactTestBitmask(true);
	mSprite->setPhysicsBody(mPhysicsBody);
	setArrowAlive(false);
	layer->addChild(mSprite);
}

Arrow::~Arrow()
{
}

void Arrow::explode(bool isRight)
{
	mIsRunLeftToRight = isRight;
	//float impulse = 500000;
	if (isRight)
	{
		mSprite->setFlippedX(false);
		//mPhysicsBody->applyImpulse(Vec2(impulse, 0));
	}
	else
	{
		mSprite->setFlippedX(true);
		//mPhysicsBody->applyImpulse(Vec2(-impulse, 0));
	}

}

void Arrow::fire()
{
	if (isAlive() == true)
	{
		if (mIsRunLeftToRight)
		{
			auto moveBy = MoveBy::create(1, Vec2(400, 0));
			auto finish = CallFunc::create([&]()
			{
				setArrowAlive(false);
			});
			mSprite->runAction(Sequence::createWithTwoActions(moveBy, finish));
		}
		else
		{
			auto moveBy = MoveBy::create(1, Vec2(-400, 0));
			auto finish = CallFunc::create([&]()
			{
				setArrowAlive(false);
			});
			mSprite->runAction(Sequence::createWithTwoActions(moveBy, finish));
		}
		//////out of screen
	}
}
