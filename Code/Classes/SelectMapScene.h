#ifndef _SELECTMAP_SCENE_H_
#define _SELECTMAP_SCENE_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ItemShop.h"
#include "Player.h"
#include "Model.h"
#include "MapManager.h"
#include "SoundManager.h"
#include <string>

using namespace std;

class SelectMapScene : public cocos2d::Layer
{
private:
	static SelectMapScene* sInstance;

	
public:
	static SelectMapScene* getInstance();

	static cocos2d::Scene* createScene();

	virtual bool init();


	
	CREATE_FUNC(SelectMapScene);
	string getNameMap();

private:
	void addListener();
	string mNameMap;
};

#endif // _NPC_SCENE_H_
