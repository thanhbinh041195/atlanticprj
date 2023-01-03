#include "TutorialScene.h"
#include"MenuScene.h"
#include "SimpleAudioEngine.h"
#include "GameSetting.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* TutorialScene::createScene()
{
	auto scene = Scene::create();
	auto layer = TutorialScene::create();
	scene->addChild(layer);
	return scene;
}

// on "init" you need to initialize your instance
bool TutorialScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto back4 = Sprite::create("st.png");
	back4->setContentSize(visibleSize);
	//back2->setAnchorPoint(Vec2(0.5f, 0.5f));
	back4->setPosition(visibleSize / 2);
	this->addChild(back4, 0);
	auto screenSize = Director::getInstance()->getVisibleSize();
	auto button = Sprite::create("res/tut.png");
	button->setPosition(screenSize.width / 2, screenSize.height / 2);
	button->setScale(0.7f);
	addChild(button, 1);
	//pause on game screen
	auto btnBack = ui::Button::create("home_normal.png", "home_pressed.png");
	btnBack->setPosition(Vec2(screenSize.width*0.095, screenSize.height*0.92));
	btnBack->setScale(0.8);
	btnBack->addClickEventListener([](Ref* event) {
		if (GameSetting::getInstance()->isEnableSound())
		{
			SimpleAudioEngine::getInstance()->playEffect("sounds/confirm.wav");
		}
		Director::getInstance()->replaceScene(MenuScene::createScene());
	});
	addChild(btnBack, 2);

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	return true;
}
