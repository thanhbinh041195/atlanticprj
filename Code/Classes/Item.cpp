#include "Item.h"


Sprite * Item::getSprite()
{
	return this->mSprite;
}

void Item::setSprite(Sprite * sprite)
{
	this->mSprite = sprite;
}

string Item::getName()
{
	return this->mName;
}

void Item::setName(string name)
{
	this->mName = name;
}


Item::Item(Layer * scene, string name, int idItem)
{
	this->mIDItem = idItem;
	this->mScene = scene;
	this->mName = name;
	this->mSprite = Sprite::create();
	this->addChild(this->mSprite, 2);
	this->playAction(this->mName + "_idle", 0);
};

void Item::playAction(const string actionName, int repeart)
{
	if (this->mSprite)
	{
		if (repeart == 0)
		{
			this->mSprite->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation(actionName))));
		}
		else
		{
			this->mSprite->runAction(Repeat::create(Animate::create(AnimationCache::getInstance()->getAnimation(actionName)), repeart));
		}
	}
}

int Item::getIDItem()
{
	return this->mIDItem;
}

void Item::setIDItem(int idItem)
{
	this->mIDItem = idItem;
}

void Item::initPhysics(int mask, Size size)
{
	auto physics = PhysicsBody::createBox(size, PhysicsMaterial(1, 0, 1));
	physics->setDynamic(false);
	physics->setCollisionBitmask(mask);
	physics->setContactTestBitmask(true);
	this->setPhysicsBody(physics);
}

void Item::shaking()
{
	if (this->mSprite)
	{
		this->mSprite->runAction(
			RepeatForever::create(
			Sequence::create(
			MoveBy::create(0.2, Vec2(0, 5)),
			MoveBy::create(0.2, Vec2(0, -5)),
			RotateBy::create(0.2, -15),
			RotateBy::create(0.2, 15),
			DelayTime::create(0.01),
			nullptr)
			)
			);
	}
}

Item::Item(std::string name, Sprite * sprite)
{
	this->mName = name;
	this->mSprite = sprite;
}


Item::Item()
{
};

Item::~Item()
{
};