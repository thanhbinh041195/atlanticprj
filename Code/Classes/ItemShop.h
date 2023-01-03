#pragma once
#include "cocos2d.h"
#include "Player.h"
#include <iostream>
#include <string>
using namespace cocos2d;
using namespace std;
USING_NS_CC;
class ItemShop
{
public:
	ItemShop();
	void setSpriteName(string spriteName);
	string getSpriteName();
	void setSprite(string sprite);
	Sprite* getSprite();
	void setName(string name);
	string getName();
	void setInfo(string info);
	string getInfo();
	void setPrice(int price);
	int getPrice();
	void setType(int type);
	int getType();
	void setMP(int MP);
	int getMP();
	void setHP(int HP);
	int getHP();
	void setArmor(int armor);
	int getArmor();
	void setDamage(int damage);
	int getDamage();
	void setQuantity(int quantity);
	int getQuantity();
	void setQuantityBought(int quantityBought);
	int getQuantityBought();
	void setBought(bool bought);
	bool getBought();
	void buyItem(int quantum);
	~ItemShop();
protected:
	Sprite* mSprite;
	string mSpriteName;
	string mName;
	string mInfo;
	int mPrice;
	int mType;
	int mMP;
	int mHP;
	int mArmor;
	int mDamage;
	int mQuantity;
	int mQuantityBought;
	bool mBought;
private:
};