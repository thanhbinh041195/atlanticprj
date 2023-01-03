#include "InGameScene.h"
#include "MenuScene.h"
#include "AboutScene.h"
#include "SettingScene.h"
#include "SelectMapScene.h"
#include "NpcScene.h"
#include "TutorialScene.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "SoundManager.h"
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

cocos2d::Node* mMenuLayer;
cocos2d::Node* mSelectLayer;


Scene* MenuScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MenuScene::create();
	scene->addChild(layer);
	return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
   
	if (!Layer::init())
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	SoundManager::getInstance()->playBackgroundMusic();

	this->setName("mMenuLayer");
	auto bg = Sprite::create("bg.png");
	bg->setContentSize(visibleSize);
	bg->setPosition(visibleSize / 2);
	addChild(bg);

	auto emitter = ParticleFire::create();
	emitter->setPosition(visibleSize.width / 2, visibleSize.height / 3 * 2);
	addChild(emitter);

	auto gameNameLabel = Label::createWithTTF("Atlantis", "fonts/Starcraft Normal.ttf",65);
	gameNameLabel->enableOutline(Color4B::BLACK, 2);
	gameNameLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 97));
	addChild(gameNameLabel);

	auto gameTitleLabel = Label::createWithTTF("THE RETURN OF THE KING", "fonts/comicbd.ttf", 23);
	//gameTitleLabel->enableBold();
	gameTitleLabel->setColor(Color3B::ORANGE);
	gameTitleLabel->enableOutline(Color4B::BLACK, 2);
	gameTitleLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 42));
	addChild(gameTitleLabel);
	
	float scale = 0.6f;

	mMenuLayer = Node::create();
	addChild(mMenuLayer);

	mSelectLayer = Node::create();
	addChild(mSelectLayer);

	auto newGame = ui::Button::create("newgame.png", "newgame.png");
	newGame->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 3 * 2 - 30));
	newGame->setScale(scale);
	newGame->addClickEventListener([&](Ref* event) {
		/*McSetting* mcSetting = McSetting::getInstance();
		mcSetting->abcData();
		mcSetting->saveData();
		NpcScene::getInstance()->setListBought();
		//////////////////
		Document document;
		document.SetObject();
		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
		rapidjson::Value array(rapidjson::kArrayType);
		document.AddMember("item_bought", array, allocator);
		string tr = "Data/ListBought.json";
		JsonToFile(document, tr);*/
		///////////////////////
		auto npc = NpcScene::getInstance()->createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, npc, Color3B(0, 255, 255)));
	});
	mSelectLayer->addChild(newGame);

	auto loadGame = ui::Button::create("loadgame.png", "loadgame.png");
	loadGame->setPosition(Vec2(visibleSize.width / 2, newGame->getPosition().y - newGame->getContentSize().height * scale));
	loadGame->setScale(scale);
	loadGame->addClickEventListener([&](Ref* event) {
		/*McSetting* mcSetting = McSetting::getInstance();
		log("%d", McSetting::getInstance()->getGold());
		mcSetting->loadData();
		log("%d", McSetting::getInstance()->getGold());*/
		auto npc = NpcScene::getInstance()->createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, npc, Color3B(0, 255, 255)));
	});
	mSelectLayer->addChild(loadGame);

	auto backMenu = ui::Button::create("backmenu.png", "backmenu.png");
	backMenu->setPosition(Vec2(visibleSize.width / 2, loadGame->getPosition().y - loadGame->getContentSize().height * scale));
	backMenu->setScale(scale);
	backMenu->addClickEventListener([&](Ref* event) {
		mSelectLayer->setVisible(false);
		mMenuLayer->setVisible(true);
	});
	mSelectLayer->addChild(backMenu);

	mSelectLayer->setVisible(false);

	auto startItem = MenuItemImage::create("startItem.png", "startItem.png", [](Ref* sender) {
		/*auto npc = NpcScene::getInstance()->createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, npc, Color3B(0, 255, 255)));*/
		mMenuLayer->setVisible(false);
		mSelectLayer->setVisible(true);
	});
	startItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 3 * 2 - 20));
	startItem->setScale(scale);

	auto settingItem = MenuItemImage::create("settingItem.png", "settingItem.png", [&](Ref* sender) {
		//auto setting = SettingScene::createScene();
		//Director::getInstance()->replaceScene(TransitionFade::create(0.5, setting, Color3B(0, 255, 255)));
		mMenuLayer->setVisible(false);
		SoundManager::getInstance()->showSetting(this);
	});
	settingItem->setPosition(Vec2(visibleSize.width / 2, startItem->getPosition().y - startItem->getContentSize().height * scale));
	settingItem->setScale(scale);

	auto tutorialItem = MenuItemImage::create("tutorialItem.png", "tutorialItem.png", [](Ref* sender) {
		auto tutorial = TutorialScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, tutorial, Color3B(0, 255, 255)));
	});
	tutorialItem->setPosition(Vec2(visibleSize.width / 2, settingItem->getPosition().y - startItem->getContentSize().height * scale));
	tutorialItem->setScale(scale);

	auto aboutItem = MenuItemImage::create("aboutItem.png", "aboutItem.png", [](Ref* sender) {
		auto about = AboutScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, about, Color3B(0, 255, 255)));
	});
	aboutItem->setPosition(Vec2(visibleSize.width / 2, tutorialItem->getPosition().y - startItem->getContentSize().height * scale));
	aboutItem->setScale(scale);

	auto exitItem = MenuItemImage::create("exitItem.png", "exitItem.png", CC_CALLBACK_1(MenuScene::menuCloseCallback, this));
	exitItem->setPosition(Vec2(visibleSize.width / 2, aboutItem->getPosition().y - startItem->getContentSize().height * scale));
	exitItem->setScale(scale);

	

	// create menu, it's an autorelease object
	auto menu = Menu::create(startItem, settingItem, tutorialItem, aboutItem, exitItem, NULL);
	menu->setPosition(Vec2::ZERO);
	mMenuLayer->addChild(menu, 1);
	//SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/Socapex - Dark ambiance.mp3", true);

	
    return true;
}

void MenuScene::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

}


void MenuScene::JsonToFile(rapidjson::Document &jsonObject, string &fullpath) {
	ofstream outputFile;
	outputFile.open(fullpath);
	if (outputFile.is_open()) {
		string jsonObjectData = JsonToString(jsonObject);
		outputFile << jsonObjectData;
	}
	outputFile.close();
}


string MenuScene::JsonToString(rapidjson::Document &jsonObject) {
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(buffer);
	jsonObject.Accept(jsonWriter);
	return buffer.GetString();
}