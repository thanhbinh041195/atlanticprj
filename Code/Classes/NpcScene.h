#ifndef _NPC_SCENE_H_
#define _NPC_SCENE_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ItemShop.h"
#include "Player.h"
#include "Model.h"
#include "MapManager.h"
#include "SneakyButton.h"
#include "SneakyButtonSkinnedBase.h"
#include "SneakyJoystickSkinnedBase.h"
#include "SoundManager.h"
#include <string>
#include "rapidjson/document.h"
using namespace rapidjson;
using namespace std;
class NpcScene : public cocos2d::Layer
{
private:
	static NpcScene* sInstance;
	
	//use cheat for key
	cocos2d::TMXTiledMap* mTileMap;
	cocos2d::TMXTiledMap* mTileMapFarBackground;
	cocos2d::TMXObjectGroup* objectGroup1;
	cocos2d::TMXObjectGroup* objectGroup2;
	cocos2d::TMXObjectGroup* objectGroup3;
	cocos2d::TMXObjectGroup* mObjectGroup;
	cocos2d::PhysicsBody* physicsMap1;
	cocos2d::PhysicsBody* physicsMap2;
	cocos2d::PhysicsBody* physicsMap3;
	cocos2d::TMXLayer* mPhysicsLayer;
	cocos2d::Vec2 keeperPosition;
	cocos2d::Vec2 telePosition;
	Player* mPlayer;
	bool canMove = true;
public:
	static NpcScene* getInstance();

	SneakyJoystick *leftJoystick;
	SneakyButton *jumpBtn;
	static cocos2d::Scene* createScene();

	virtual bool init();

	void loadFile();
	void loadChest();
	void saveFile();
	vector<ItemShop*> getListBought();
	void setListBought();
	void setReborn();
	virtual void update(float dt);
	bool onContactBegin(cocos2d::PhysicsContact& contact);
	void setViewPointCenter(Point position);
	CREATE_FUNC(NpcScene);

private:
	void updateReady_dg(float delta);
	void updateObject(float delta);
	void createMap();
	float rebornX, rebornY;
	vector<ItemShop*> list;
	void loadItem(string &fullpath, string &member, vector<ItemShop*> &list);
	void JsonToFile(rapidjson::Document &jsonObject, string &fullpath);
	string JsonToString(rapidjson::Document &jsonObject);
	
	void addListener();
	void createPhysics();
	void loadShop(vector<ItemShop*> list);
	void createObjects();
	void createController();
	void createShopPanel();
	void createTutorialPanel();

};

#endif // _NPC_SCENE_H_
