#include "ItemShop.h"
#include "cocos2d.h"
#include <string>
#include <iostream>

using namespace std;
USING_NS_CC;

ItemShop::ItemShop(){};
ItemShop::~ItemShop(){};

void ItemShop::setSpriteName(string spriteName)
{
	mSpriteName = spriteName;

}

string ItemShop::getSpriteName()
{
	return mSpriteName;
};

void ItemShop::setSprite(string sprite)
{
	mSprite = Sprite::create("ItemShop/" + sprite);
};

Sprite* ItemShop::getSprite()
{
	return mSprite;
};

void ItemShop::setPrice(int price)
{
	mPrice = price;
}

int ItemShop::getPrice()
{
	return mPrice;
};

void ItemShop::setType(int type)
{
	mType = type;
}

int ItemShop::getType()
{
	return mType;
};

void ItemShop::setMP(int MP)
{
	mMP = MP;
}

int ItemShop::getMP()
{
	return mMP;
};

void ItemShop::setHP(int HP)
{
	mHP = HP;
}

int ItemShop::getHP()
{
	return mHP;
};


void ItemShop::setArmor(int armor)
{
	mArmor = armor;
}

int ItemShop::getArmor()
{
	return mArmor;
};

void ItemShop::setDamage(int damage)
{
	mDamage = damage;
}

int ItemShop::getDamage()
{
	return mDamage;
};

void ItemShop::setInfo(string info)
{
	mInfo = info;
}

string ItemShop::getInfo()
{
	return mInfo;
};

void ItemShop::setName(string name)
{
	mName = name;
}

string ItemShop::getName()
{
	return mName;
};

void ItemShop::setQuantity(int quantity)
{
	mQuantity = quantity;
}
int ItemShop::getQuantity()
{
	return mQuantity;
}
void ItemShop::setQuantityBought(int quantityBought)
{
	mQuantityBought = quantityBought;
}
int ItemShop::getQuantityBought()
{
	return mQuantityBought;
}

void ItemShop::setBought(bool bought)
{
	mBought = bought;
}

bool ItemShop::getBought()
{
	return mBought;
};
