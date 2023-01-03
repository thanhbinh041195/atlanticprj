#pragma once
#include "Player.h"
#include "cocos2d.h"
#include "Define.h"
#include "MapManager.h"
#include <vector>

using namespace cocos2d;
using namespace std;
class Item : public Node
{
public:
	Item(Layer * scene, string name, int idItem = -1);
	void playAction(const string actionName, int repeart = 1);
	int getIDItem();
	void setIDItem(int idItem);
	Sprite* getSprite();
	void setSprite(Sprite* sprite);
	string getName();
	void setName(string name);
	void initPhysics(int mask, Size size = Size(20, 20));
	void shaking();
	Item();
	Item(std::string name, Sprite *sprite);
	~Item();
protected:
	int mIDItem;
	Layer* mScene;
	Sprite* mSprite;
	string mName;
	
};