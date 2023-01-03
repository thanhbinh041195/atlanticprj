#include "LogoScene.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include<ui/CocosGUI.h>
#include "rapidjson/rapidjson.h"
#include "rapidjson/writer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/document.h"
#include <iostream>
#include <string>
#include <fstream>
#include "rapidjson/stringbuffer.h"
using namespace std;
using namespace rapidjson;
USING_NS_CC;

Scene* LogoScene::createScene()
{
	auto myLabel = Label::createWithTTF("My Label Text", "Marker Felt.ttf", 16);

	auto scene = Scene::create();
	auto layer = LogoScene::create();
	scene->addChild(layer);
	return scene;
}

bool LogoScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//loadPlayer();
	McSetting* mcSetting = McSetting::getInstance();
	mcSetting->loadData();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto screenSize = Director::getInstance()->getVisibleSize();
	auto back3 = Sprite::create("background2.png");
	back3->setContentSize(visibleSize);
	back3->setPosition(visibleSize / 2);
	this->addChild(back3, 0);
	auto gotoNext = CallFunc::create([]() {
		Director::getInstance()->replaceScene(MenuScene::createScene());
	});
	auto sequence = Sequence::create(DelayTime::create(10), gotoNext, nullptr);

	runAction(sequence);
	auto loadingBarGB = Sprite::create("loading_bar_main.png");
	loadingBarGB->setPosition(Vec2(400, 35));
	loadingBarGB->setScale(0.65);
	addChild(loadingBarGB);
	static auto loadingbar = ui::LoadingBar::create("loading_bar_bg1.png");
	loadingbar->setPercent(0);
	loadingbar->setScale(0.65);
	loadingbar->setDirection(ui::LoadingBar::Direction::LEFT);
	loadingbar->setPosition(loadingBarGB->getPosition());
	addChild(loadingbar);
	//update Loading Bar
	static int dem = 0;
	auto updateLoadingBar = CallFunc::create([]() {
		if (loadingbar->getPercent() < 100)
		{
			dem++;
			loadingbar->setPercent(loadingbar->getPercent() + 1);
		}
		if (loadingbar->getPercent() == 100)
		{
			Director::getInstance()->replaceScene(MenuScene::createScene());
		}
	});

	auto sequenceRunUpdateLoadingBar = Sequence::createWithTwoActions(updateLoadingBar, DelayTime::create(0.05f));
	auto repeat = Repeat::create(sequenceRunUpdateLoadingBar, 100);
	loadingbar->runAction(repeat);

	return true;
}


