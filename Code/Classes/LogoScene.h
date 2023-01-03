#ifndef __LOGO_SCENE_H__
#define __LOGO_SCENE_H__

#include "cocos2d.h"
#include "NpcScene.h"
#include "ItemShop.h"
#include <string>
#include "rapidjson/document.h"
using namespace rapidjson;
using namespace std;
class LogoScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	void JsonToFile(rapidjson::Document &jsonObject, string &fullpath);
	string JsonToString(rapidjson::Document &jsonObject);
	void loadFile();
	void saveFile();
	void loadItem(string &fullpath, string &member, vector<ItemShop*> &list);
	CREATE_FUNC(LogoScene);
};

#endif // __LOGO_SCENE_H__
