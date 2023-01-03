#include "SelectMapScene.h"
#include "MenuScene.h"
#include "NpcScene.h"
#include "ItemShop.h"
#include "ui/CocosGUI.h"
#include "InGameScene.h"
#include "SimpleAudioEngine.h"
#include <iostream>
#include <string>
using namespace std;
using namespace rapidjson;

USING_NS_CC;
using namespace CocosDenshion;

string gMapName;
int gMapNumber;

Size visibles;

SelectMapScene* SelectMapScene::sInstance = nullptr;

SelectMapScene* SelectMapScene::getInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new SelectMapScene();
	}
	return sInstance;
}

Scene* SelectMapScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vect(0.0f, 0.0f));
	auto layer = SelectMapScene::create();
	scene->addChild(layer);
	return scene;
}
string SelectMapScene::getNameMap()
{
	return mNameMap;
}
bool SelectMapScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibles = Director::getInstance()->getVisibleSize();

	auto bg = Sprite::create("bgsl.png");
	bg->setPosition(visibles / 2);
	bg->setContentSize(visibles);
	addChild(bg);

	auto btnMenu = ui::Button::create("shopicon.png", "shopicon_p.png");
	btnMenu->setScale(0.35);
	btnMenu->setPosition(Vec2(40, visibles.height - 35));
	btnMenu->addClickEventListener([=](Ref* event) {
		auto npc = NpcScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, npc, Color3B(0, 255, 255)));
	});
	btnMenu->setGlobalZOrder(3);
	addChild(btnMenu);

	auto scrollView = cocos2d::ui::ScrollView::create();
	scrollView->setAnchorPoint(Vec2(0.5, 0.5));
	scrollView->setPosition(Vec2(visibles.width/2,visibles.height/2-45));
	scrollView->setContentSize(Size(visibles.width * 0.7, visibles.height*0.7));
	scrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
	scrollView->setBounceEnabled(true);
	scrollView->setTouchEnabled(true);
	scrollView->setInnerContainerSize(Size(1700,visibles.height*0.4));
	addChild(scrollView);

	auto map1 = ui::Button::create("m1.png", "m1.png");
	map1->setScale(0.7);
	map1->setPosition(Vec2(250, scrollView->getPosition().y));
	map1->addClickEventListener([=](Ref* event) {
		gMapNumber = 1;
		gMapName = "World1_MapBattle1.tmx";
		auto inGame = InGameScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, inGame, Color3B(0, 255, 255)));

	});
	scrollView->addChild(map1);

	auto map2 = ui::Button::create("m2.png", "m2.png");
	map2->setScale(0.7);
	map2->setPosition(Vec2(650, scrollView->getPosition().y));
	map2->addClickEventListener([&](Ref* event) {
		gMapNumber = 2;
		gMapName = "World1_MapBattle2.tmx";
		auto inGame = InGameScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, inGame, Color3B(0, 255, 255)));

	});
	scrollView->addChild(map2);

	auto map3 = ui::Button::create("m3.png", "m3.png");
	map3->setScale(0.7);
	map3->setPosition(Vec2(1050, scrollView->getPosition().y));
	map3->addClickEventListener([&](Ref* event) {
		gMapNumber = 3;
		gMapName = "World1_MapBattle3.tmx";
		auto inGame = InGameScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, inGame, Color3B(0, 255, 255)));

	});
	scrollView->addChild(map3);

	auto map4 = ui::Button::create("m4.png", "m4.png");
	map4->setScale(0.7);
	map4->setPosition(Vec2(1450, scrollView->getPosition().y));
	map4->addClickEventListener([&](Ref* event) {
		gMapNumber = 4;
		gMapName = "World1_MapBoss_Fafnir.tmx";
		auto inGame = InGameScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, inGame, Color3B(0, 255, 255)));

	});
	scrollView->addChild(map4);

	return true;
}

