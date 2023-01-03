#include "AboutScene.h"
#include "SimpleAudioEngine.h"
#include"MenuScene.h"
#include "GameSetting.h"

USING_NS_CC;
extern Size visible;
cocos2d::Node* mAboutLayer;
ui::ScrollView* scroll;
using namespace CocosDenshion;
Scene* AboutScene::createScene()
{
	auto scene = Scene::create();
	auto layer = AboutScene::create();
	scene->addChild(layer);
	return scene;
}

// on "init" you need to initialize your instance
bool AboutScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	//Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto back4 = Sprite::create("st.png");
	back4->setContentSize(visibleSize);
	//back2->setAnchorPoint(Vec2(0.5f, 0.5f));
	back4->setPosition(visibleSize / 2);
	this->addChild(back4, 0);
	auto screenSize = Director::getInstance()->getVisibleSize();
	auto button = Sprite::create("res/about.png");
	button->setPosition(screenSize/2);
	button->setScale(0.7f);
	addChild(button, 1);
	
	mAboutLayer = Node::create();
	mAboutLayer->setContentSize(visible);
	addChild(mAboutLayer);
	mAboutLayer->setVisible(false);
	
	auto btnBack = ui::Button::create("home_normal.png", "home_pressed.png");
	btnBack->setPosition(Vec2(visibleSize.width*0.095, visibleSize.height*0.92));
	btnBack->setScale(0.8);
	btnBack->addClickEventListener([](Ref* event) {
		if (GameSetting::getInstance()->isEnableSound())
		{
			SimpleAudioEngine::getInstance()->playEffect("sounds/confirm.wav");
		}
		Director::getInstance()->replaceScene(MenuScene::createScene());
	});
	addChild(btnBack, 3);

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	return true;
}

