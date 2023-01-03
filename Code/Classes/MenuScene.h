#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include <string>
#include "rapidjson/document.h"
using namespace rapidjson;
using namespace std;
class MenuScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

    virtual bool init();
	string JsonToString(rapidjson::Document &jsonObject);
	void JsonToFile(rapidjson::Document &jsonObject, string &fullpath);
	void menuCloseCallback(Ref* pSender);

	CREATE_FUNC(MenuScene);
};

#endif // __MENU_SCENE_H__
