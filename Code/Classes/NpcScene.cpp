#include "NpcScene.h"
#include "MenuScene.h"
#include "ItemShop.h"
#include "ui/CocosGUI.h"
#include "InGameScene.h"
#include "MapManager.h"
#include "SelectMapScene.h"
#include "SimpleAudioEngine.h"
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
using namespace CocosDenshion;

vector<ItemShop*> listArmor;
vector<ItemShop*> listWeapon;
vector<ItemShop*> listMedicine;
vector<ItemShop*> listBought;
vector<ItemShop*> listSelect;
vector<ui::CheckBox*> listCheck;
Label* goldText;

Size visible;
cocos2d::Node* mTutorialLayer;
cocos2d::Node* mShopLayer;
cocos2d::Node* mController;
SneakyJoystickSkinnedBase* joystick;
ui::ScrollView* scrollView;
Sprite* tutorialPanel;
Sprite* shopPanel;
Sprite* chestPanel;
Sprite* layoutChest;
Sprite* bgItem;
Sprite* avatarItem;
Node* sellNode;
ui::Button* sell;

NpcScene* NpcScene::sInstance = nullptr;

NpcScene* NpcScene::getInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new NpcScene();
		
		sInstance->loadFile();
	}
	return sInstance;
}

Scene* NpcScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vect(0.0f, 0.0f));
	auto layer = NpcScene::create();
	scene->addChild(layer);
	return scene;
}

bool NpcScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	
	visible = Director::getInstance()->getVisibleSize();
	
	MapManager::getInstance()->loadMap(this, 0, "citymap.tmx");

	//create map
	//createMap();
	
	//init physics
	//createPhysics();

	createObjects();

	createController();

	createShopPanel();

	createTutorialPanel();

	//add Listener: touch, key 
	addListener();
	//update
	schedule(CC_SCHEDULE_SELECTOR(NpcScene::updateObject), 0.5);
	scheduleOnce(CC_SCHEDULE_SELECTOR(NpcScene::updateReady_dg), 2);

	return true;
}

void NpcScene::updateReady_dg(float dt)
{
	this->getScene()->getPhysicsWorld()->setAutoStep(false);
	this->scheduleUpdate();
}

void NpcScene::update(float dt)
{
	setViewPointCenter(mPlayer->getSprite()->getPosition());
	if (mPlayer->getSprite()->getNumberOfRunningActions() == 0)
	{
		mPlayer->setState(Player::STATE_IDLE);
	}

	// Move Player by Joystick (If player attack, player will stop move)
	Point scaleVelocity = ccpMult(leftJoystick->getVelocity(), 120);
	if (scaleVelocity.x < 0 && canMove == true)
	{
		mPlayer->getSprite()->setAnchorPoint(Vec2(0.4, 0));
		Point newPosition = ccp(mPlayer->getSprite()->getPosition().x + scaleVelocity.x*dt, mPlayer->getSprite()->getPosition().y + scaleVelocity.y*dt);
		mPlayer->setFlip(false);
		mPlayer->getPhysicsFoot()->setVelocity(Vec2(scaleVelocity.x, scaleVelocity.y));
		mPlayer->setState(Player::STATE_MOVE);
		mPlayer->setMoveRight(false);
	}
	else if (scaleVelocity.x > 0 && canMove == true)
	{
		mPlayer->getSprite()->setAnchorPoint(Vec2(0.4, 0));
		Point newPosition = ccp(mPlayer->getSprite()->getPosition().x + scaleVelocity.x*dt, mPlayer->getSprite()->getPosition().y + scaleVelocity.y*dt);
		mPlayer->setFlip(true);
		mPlayer->getPhysicsFoot()->setVelocity(Vec2(scaleVelocity.x, scaleVelocity.y));
		mPlayer->setState(Player::STATE_MOVE);
		mPlayer->setMoveRight(true);
	}
	else
	{
		mPlayer->getSprite()->setAnchorPoint(Vec2(0.3, 0));
		mPlayer->getPhysicsFoot()->setVelocity(Vect(0, mPlayer->getPhysicsFoot()->getWorld()->getGravity().y));
		if (mPlayer->getCurrentState() < Player::STATE_ATTACK)
		{
			mPlayer->setState(Player::STATE_IDLE);
		}
	}

	//Set anchorpoint for Player at state attack, skill 1, skill 2
	if (mPlayer->getCurrentState() == 3
		|| mPlayer->getCurrentState() == 4
		|| mPlayer->getCurrentState() == 5)
	{
		mPlayer->getSprite()->setAnchorPoint(Vec2(0, 0));
	}
	if (mPlayer->getCurrentState() == 6)
	{

		mPlayer->getSprite()->setAnchorPoint(Vec2(0.1, 0));
		auto action = Sequence::create(
			CallFunc::create(
			[&]() {
			mPlayer->getSprite()->setColor(Color3B(255, 255, 155));
		}
			),
			DelayTime::create(10),
			CallFunc::create(
			[&]() {
			mPlayer->getSprite()->setColor(Color3B(255, 255, 255));
		}
			),
			NULL
			);

		runAction(action);
	}

	for (int i = 0; i < 6; ++i)
	{
		this->getScene()->getPhysicsWorld()->step(1 / 180.0f);
	}

}


void NpcScene::updateObject(float dt)
{

	auto listCollisionObjects = MapManager::getInstance()->getCollisionObjects();
	auto listCollisionNodes = MapManager::getInstance()->getCollisionNodes();

	Rect viewVisible = Rect(-this->getPositionX(), -this->getPositionY(), visible.width, visible.height);

	for (int i = 0; i < listCollisionObjects.size(); i++)
	{
		auto rect = listCollisionObjects[i];
		if (viewVisible.intersectsRect(rect))
		{
			if (!listCollisionNodes[i]->getPhysicsBody()->isEnabled())
			{
				listCollisionNodes[i]->getPhysicsBody()->setEnabled(true);
			}
		}
		else
		{
			if (listCollisionNodes[i]->getPhysicsBody()->isEnabled())
			{
				listCollisionNodes[i]->getPhysicsBody()->setEnabled(false);
			}
		}
	}
}

void NpcScene::addListener()
{
	//add contact event listener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(NpcScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void NpcScene::JsonToFile(rapidjson::Document &jsonObject, string &fullpath) {
	ofstream outputFile;
	outputFile.open(fullpath);
	if (outputFile.is_open()) {
		string jsonObjectData = JsonToString(jsonObject);
		outputFile << jsonObjectData;
	}
	outputFile.close();
}

string NpcScene::JsonToString(rapidjson::Document &jsonObject) {
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(buffer);
	jsonObject.Accept(jsonWriter);
	return buffer.GetString();
}

void NpcScene::loadItem(string &fullpath, string &member, vector<ItemShop*> &list)
{
	Document m_document;
	ssize_t size;

	char* buf = (char*)FileUtils::getInstance()->getFileData(fullpath, "r", &size);
	string content(buf, size);

	//std::string content = FileUtils::getInstance()->getStringFromFile(FileUtils::getInstance()->fullPathForFilename(fullpath));


	m_document.Parse(content.c_str());

	if (!m_document.HasParseError())
	{
		if (m_document.HasMember(member.c_str()))
		{
			for (int i = 0; i < m_document[member.c_str()].Size(); i++)
			{
				ItemShop* item = new ItemShop();
				item->setSprite(m_document[member.c_str()][i]["picture"].GetString());
				item->setSpriteName(m_document[member.c_str()][i]["picture"].GetString());
				item->setName(m_document[member.c_str()][i]["name"].GetString());
				item->setInfo(m_document[member.c_str()][i]["info"].GetString());
				item->setPrice(m_document[member.c_str()][i]["price"].GetInt());
				item->setType(m_document[member.c_str()][i]["type"].GetInt());
				item->setHP(m_document[member.c_str()][i]["hp"].GetInt());
				item->setMP(m_document[member.c_str()][i]["mp"].GetInt());
		
				item->setArmor(m_document[member.c_str()][i]["armor"].GetInt());
				item->setDamage(m_document[member.c_str()][i]["damage"].GetInt());
				item->setQuantity(m_document[member.c_str()][i]["quantity"].GetInt());
				item->setQuantityBought(m_document[member.c_str()][i]["quantity_bought"].GetInt());
				list.push_back(item);
			}
		}
	}
}

void NpcScene::loadFile()
{
	listMedicine.clear();
	listArmor.clear();
	listWeapon.clear();
	listBought.clear();
	string f1 = "Data/ListItem.json";
	string f2 = "Data/ListBought.json";
	string m1 = "item_medicine";
	string m2 = "item_armor"; 
	string m3 = "item_weapon";
	string m4 = "item_bought";
	loadItem(f1, m1, listMedicine);
	loadItem(f1, m2, listArmor);
	loadItem(f1, m3, listWeapon);
	loadItem(f2, m4, listBought);
}

void NpcScene::loadChest()
{
	//Add item was bought into chest
	layoutChest->removeAllChildren();
	sellNode->removeAllChildren();

	for (int i = listBought.size() - 1; i >= 0; i--)
	{
		if (listBought.at(i)->getQuantityBought() == 0)
		{
			listBought.erase(listBought.begin() + i);
		}

	}

	int k = 0;
	for (int i = 4; i > 0; i--)
		for (int j = 0; j < 4; j++)
		{
			if (listBought.size() == 0)
			{
				Document document;
				document.SetObject();
				rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
				rapidjson::Value array(rapidjson::kArrayType);
				document.AddMember("item_bought", array, allocator);
				string tr = "Data/ListBought.json";
				JsonToFile(document, tr);
				break;
			}
			else
				if (k >= listBought.size())
					break;
			auto avatar = ui::Button::create("avatarItem.png", "avatarItem_p.png");
			avatar->setScale(0.9);
			avatar->setPosition(Vec2(j * 32 + 22, i * 35 - 10));
			avatar->addClickEventListener([=](Ref* event) {
				sellNode->removeAllChildren();

				auto itemName = Label::createWithTTF("Item:\n" + listBought.at(k)->getName(), "fonts/comic.ttf", 15);
				itemName->setPosition(Vec2(chestPanel->getPosition().x*0.9, chestPanel->getPosition().y*0.38));
				itemName->setName("itemname");
				sellNode->addChild(itemName);

				sell->addClickEventListener([=](Ref* event) {
					if (listBought.size() != 0 && sellNode->getChildByName("itemname")!=nullptr)
					{
						listBought.at(k)->setQuantityBought(listBought.at(k)->getQuantityBought() - 1);

						McSetting::getInstance()->setGold(McSetting::getInstance()->getGold() + listBought.at(k)->getPrice());
						ostringstream OS;
						OS << McSetting::getInstance()->getGold();
						goldText->setString(OS.str());

						//Output list bought into file
						Document document;
						document.SetObject();
						rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
						rapidjson::Value array(rapidjson::kArrayType);
						for (int i = 0; i < listBought.size(); i++)
						{
							rapidjson::Value object(rapidjson::kObjectType);
							rapidjson::Value spriteName, name, info;
							object.AddMember("picture", spriteName.SetString(listBought.at(i)->getSpriteName().c_str(), allocator), allocator);
							object.AddMember("name", name.SetString(listBought.at(i)->getName().c_str(), allocator), allocator);
							object.AddMember("info", info.SetString(listBought.at(i)->getInfo().c_str(), allocator), allocator);
							object.AddMember("price", listBought.at(i)->getPrice(), allocator);
							object.AddMember("type", listBought.at(i)->getType(), allocator);
							object.AddMember("hp", listBought.at(i)->getHP(), allocator);
							object.AddMember("mp", listBought.at(i)->getMP(), allocator);
							object.AddMember("armor", listBought.at(i)->getArmor(), allocator);
							object.AddMember("damage", listBought.at(i)->getDamage(), allocator);
							object.AddMember("quantity", listBought.at(i)->getQuantity(), allocator);
							object.AddMember("quantity_bought", listBought.at(i)->getQuantityBought(), allocator);
							array.PushBack(object, allocator);

						}
						document.AddMember("item_bought", array, allocator);
						string tr = "Data/ListBought.json";
						JsonToFile(document, tr);

						loadChest();
					}
					
				});
			});
			layoutChest->addChild(avatar);

			auto item = Sprite::create("ItemShop/" + listBought.at(k)->getSpriteName());
			if (listBought.at(k)->getType() == 3)
				item->setScale(0.09);
			else
				item->setScale(0.4);
			item->setPosition(avatar->getPosition());
			layoutChest->addChild(item);

			ostringstream SL;
			SL << listBought.at(k)->getQuantityBought();
			auto number = Label::createWithTTF(SL.str(), "fonts/LifeCraft_Font.ttf", 10);
			number->setPosition(avatar->getPosition().x + 8, avatar->getPosition().y + 8);
			layoutChest->addChild(number);

			k++;
		}
}


void NpcScene::setViewPointCenter(Point position)
{
	int x = MAX(position.x, visible.width / 2);
	int y = MAX(position.y, visible.height / 2);
	x = MIN(x, (MapManager::getInstance()->getTiledMap()->getMapSize().width * MapManager::getInstance()->getTiledMap()->getTileSize().width) - visible.width / 2);
	y = MIN(y, (MapManager::getInstance()->getTiledMap()->getMapSize().height * MapManager::getInstance()->getTiledMap()->getTileSize().height) - visible.height / 2);
	Point actualPosition = ccp(x, y);

	Point centerOfView = ccp(visible.width / 2, visible.height / 2);
	Point viewPoint = ccpSub(centerOfView, actualPosition);
	this->setPosition(viewPoint);
	mController->setPosition(-viewPoint);
	mShopLayer->setPosition(-viewPoint);
}

void NpcScene::createObjects() {

	//Create player from Tile Map
	auto playerTile = MapManager::getInstance()->getTiledMap()->getObjectGroup("Player");
	auto mc = playerTile->objectNamed("Player");
	auto x = mc.at("x").asInt();
	auto y = mc.at("y").asInt();
	mPlayer = new Player(this);
	mPlayer->getSprite()->setPosition(x, y);

	//PlayerReborn
	auto playerReborn = MapManager::getInstance()->getTiledMap()->getObjectGroup("PlayerReborn");
	auto rb = playerReborn->objectNamed("PlayerReborn");
	rebornX = rb.at("x").asFloat();
	rebornY = rb.at("y").asFloat();

	auto posObject = MapManager::getInstance()->getTiledMap()->getObjectGroup("NPC");
	if (!posObject)
	{
		cocos2d::log("posObject is null");
	}
	else
	{
		for (auto object : posObject->getObjects())
		{
			auto value = object.asValueMap();
			auto x = value.at("x").asInt();
			auto y = value.at("y").asInt();
			string name = object.asValueMap().at("name").asString();
			if (name == "NPC_Keeper")
			{
				keeperPosition = Vec2(x, y);
			}

		}
	}

}

void NpcScene::createController()
{
	Director::getInstance()->getVisibleSize();
	mController = Node::create();
	mController->setContentSize(visible);
	addChild(mController);

	//Joystick
	Rect joystickBaseDimensions;
	joystickBaseDimensions = Rect(0, 0, 160.0f, 160.0f);

	Point joystickBasePosition;
	joystickBasePosition = Vec2(visible.width*0.16, visible.height*0.15);

	joystick = new SneakyJoystickSkinnedBase();
	joystick->init();
	joystick->setPosition(joystickBasePosition);
	joystick->setBackgroundSprite(Sprite::create("res/joystick-back2.png"));
	joystick->setThumbSprite(Sprite::create("res/stickv.png"));
	//joystick->setOpacity(220);
	joystick->setScale(0.8);

	SneakyJoystick *aJoystick = new SneakyJoystick();
	aJoystick->initWithRect(joystickBaseDimensions);

	aJoystick->autorelease();
	joystick->setJoystick(aJoystick);
	joystick->setPosition(joystickBasePosition);

	leftJoystick = joystick->getJoystick();
	leftJoystick->retain();
	mController->addChild(joystick);

	//create skip button
	auto skipBtn = ui::Button::create("map.png", "map_p.png");
	skipBtn->setScale(0.25);
	skipBtn->setPosition(Vec2(visible.width*0.92 , visible.height*0.9 ));
	skipBtn->addClickEventListener([=](Ref* event) {
		auto selectMap = SelectMapScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, selectMap, Color3B(0, 255, 255)));
		//MapManager::getInstance()->loadMap(InGameScene, 1, "World1_MapBattle1.tmx");
	});
	//mController->addChild(skipBtn);

	auto enterBtn = ui::Button::create("entergame.png", "entergame_p.png");
	enterBtn->setScale(0.18);
	enterBtn->setGlobalZOrder(7);
	enterBtn->setOpacity(230);
	enterBtn->setPosition(Vec2(visible.width*0.9, 100));
	enterBtn->addClickEventListener([=](Ref* event) {
		auto selectMap = SelectMapScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, selectMap, Color3B(0, 255, 255)));
		//MapManager::getInstance()->loadMap(InGameScene, 1, "World1_MapBattle1.tmx");
	});
	mController->addChild(enterBtn);

	auto move1 = MoveBy::create(0.7, Vec2(10, 0));
	auto move2 = MoveBy::create(0.7, Vec2(-10, 0));
	//enterBtn->runAction(RepeatForever::create(Sequence::create(move1, move2, nullptr)));
	

	/////////
	auto btnMenu = ui::Button::create("home_normal.png", "home_pressed.png");
	btnMenu->setScale(0.8);
	btnMenu->setOpacity(230);
	btnMenu->setPosition(Vec2(visible.width*0.095, visible.height*0.92));
	btnMenu->addClickEventListener([=](Ref* event) {
		auto menu = MenuScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, menu, Color3B(0, 255, 255)));
	});
	mController->addChild(btnMenu);
}

void NpcScene::createTutorialPanel()
{
	mTutorialLayer = Node::create();
	mTutorialLayer->setContentSize(visible);
	addChild(mTutorialLayer);
	mTutorialLayer->setVisible(false);

	tutorialPanel = Sprite::create("shoppanel.png");
	tutorialPanel->setScale(1.2);
	tutorialPanel->setPosition(visible.width / 4 * 1.4, visible.height / 2);
	mTutorialLayer->addChild(tutorialPanel);

	auto scrollView = cocos2d::ui::ScrollView::create();
	scrollView->setAnchorPoint(Vec2(0.5, 0.5));
	scrollView->setContentSize(Size(tutorialPanel->getContentSize().width * tutorialPanel->getScale(), tutorialPanel->getContentSize().height * tutorialPanel->getScale() * 0.6));
	scrollView->setInnerContainerSize(Size(scrollView->getContentSize().width, 100 + 27));
	scrollView->setPosition(Vec2(tutorialPanel->getPosition().x, tutorialPanel->getPosition().y - 30));
	scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
	scrollView->setBounceEnabled(true);
	scrollView->setTouchEnabled(true);
	mTutorialLayer->addChild(scrollView);

	/*auto tutorialLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 18);
	tutorialLabel->setPosition(Vec2(goldPanel->getPosition().x + 10, goldPanel->getPosition().y));
	mShopLayer->addChild(goldText);*/

	auto closeBtn = ui::Button::create("closeshoppanel.png", "closeshoppanel_p.png");
	//closeBtn->setScale(0.9);
	closeBtn->setPosition(Vec2(tutorialPanel->getPosition().x, tutorialPanel->getPosition().y*0.25));
	closeBtn->addClickEventListener([=](Ref* event) {
		mTutorialLayer->setVisible(false);
		mController->setVisible(true);
		mPlayer->getSprite()->setVisible(true);
		mPlayer->getPhysicsBody()->setEnabled(true);
		mPlayer->getPhysicsFoot()->setEnabled(true);
		canMove = true;
	});
	mTutorialLayer->addChild(closeBtn);
}

void NpcScene::createShopPanel()
{
	mShopLayer = Node::create();
	mShopLayer->setContentSize(visible);
	addChild(mShopLayer);
	mShopLayer->setVisible(false);

	shopPanel = Sprite::create("shoppanel.png");
	shopPanel->setScale(1.2);
	shopPanel->setPosition(visible.width/4*1.4,visible.height/2);
	mShopLayer->addChild(shopPanel);
	
	//Template
	bgItem = Sprite::create("bgItem.png");
	bgItem->setScale(shopPanel->getScale());

	avatarItem = Sprite::create("avatarItem.png");
	avatarItem->setScale(shopPanel->getScale());

	scrollView = cocos2d::ui::ScrollView::create();
	scrollView->setAnchorPoint(Vec2(0.5, 0.5));
	scrollView->setContentSize(Size(shopPanel->getContentSize().width * shopPanel->getScale(), shopPanel->getContentSize().height * shopPanel->getScale() * 0.6));
	scrollView->setPosition(Vec2(shopPanel->getPosition().x, shopPanel->getPosition().y - 30));
	scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
	scrollView->setBounceEnabled(true);
	scrollView->setTouchEnabled(true);
	mShopLayer->addChild(scrollView);
	
	//Chest of item
	chestPanel = Sprite::create("chest.png");
	chestPanel->setScale(1.5);
	chestPanel->setPosition(visible.width * 3 / 4 + 30, visible.height / 2-20);
	mShopLayer->addChild(chestPanel);

	layoutChest = Sprite::create("layout_chest.png");
	layoutChest->setScale(1.5);
	layoutChest->setPosition(chestPanel->getPosition());
	mShopLayer->addChild(layoutChest);

	sellNode = Node::create();
	mShopLayer->addChild(sellNode);

	sell = ui::Button::create("sell.png", "sell_p.png");
	//sell->setScale(0.9);
	sell->setPosition(Vec2(chestPanel->getPosition().x*1.08, chestPanel->getPosition().y*0.37));
	mShopLayer->addChild(sell);

	auto goldPanel = Sprite::create("GoldPanel.png");
	goldPanel->setScale(0.5);
	goldPanel->setOpacity(220);
	goldPanel->setPosition(Vec2(chestPanel->getPosition().x, chestPanel->getPosition().y+200));
	mShopLayer->addChild(goldPanel);

	auto gold = Sprite::create("coinitem.png");
	gold->setScale(0.19);
	gold->setPosition(Vec2(goldPanel->getPosition().x - 20, goldPanel->getPosition().y));
	mShopLayer->addChild(gold);

	goldText = Label::createWithTTF("", "fonts/Marker Felt.ttf", 18);
	goldText->setPosition(Vec2(goldPanel->getPosition().x + 10, goldPanel->getPosition().y));
	mShopLayer->addChild(goldText);

	ostringstream OS;
	OS << McSetting::getInstance()->getGold();
	goldText->setString(OS.str());

	loadChest();
}

void NpcScene::loadShop(vector<ItemShop*> list)
{
	bgItem = Sprite::create("bgItem.png");
	bgItem->setScale(shopPanel->getScale());

	scrollView->removeAllChildren();
	scrollView->setInnerContainerSize(Size(scrollView->getContentSize().width, list.size() * (bgItem->getContentSize().height * bgItem->getScale() + 27)));

	for (int i = 0; i < list.size(); i++)
	{
		auto backgroundItem = ui::CheckBox::create("bgItem.png", "bgItem_p.png", "bgItem_p.png", "bgItem.png", "bgItem.png");
		backgroundItem->setSelected(false);
		backgroundItem->addClickEventListener([=](Ref* event) {
			//log("%d", list.size());
		});
		backgroundItem->setScale(1.3);
		backgroundItem->setPosition(Vec2(scrollView->getPosition().x - 90, 70 * i + 70));
		scrollView->addChild(backgroundItem);
		listCheck.push_back(backgroundItem);

		auto avatarItem = Sprite::create("avatarItem.png");
		avatarItem->setScale(1.3);
		avatarItem->setPosition(backgroundItem->getPosition().x - 105, backgroundItem->getPosition().y);
		scrollView->addChild(avatarItem);

		//auto sprite = list.at(i)->getSprite();
		//auto text = sprite->getTexture();
		auto item = Sprite::create("ItemShop/" + list.at(i)->getSpriteName());
		if (list.at(i)->getType() == 3)
			item->setScale(0.1);
		else
			item->setScale(0.5);
		item->setPosition(avatarItem->getPosition());
		scrollView->addChild(item);

		auto nameItem = Label::createWithTTF(list.at(i)->getName(), "fonts/Starcraft Normal.ttf", 17);
		nameItem->setPosition(backgroundItem->getPosition().x, backgroundItem->getPosition().y + 15);
		scrollView->addChild(nameItem);

		ostringstream SL2;
		SL2 << list.at(i)->getPrice();
		auto priceItem = Label::createWithTTF(SL2.str() + " $", "fonts/Starcraft Normal.ttf", 13);
		priceItem->setColor(Color3B(249, 244, 0));
		priceItem->setPosition(backgroundItem->getPosition().x + 100, backgroundItem->getPosition().y - 18);
		scrollView->addChild(priceItem);

		if (list.at(i)->getType() == 1)
		{
			ostringstream MP;
			MP << list.at(i)->getMP();
			auto mpItem = Label::createWithTTF("MP: " + MP.str(), "fonts/Starcraft Normal.ttf", 13);
			mpItem->setColor(Color3B(66, 110, 180));
			mpItem->setPosition(backgroundItem->getPosition().x - 30, backgroundItem->getPosition().y - 7);
			scrollView->addChild(mpItem);

			ostringstream HP;
			HP << list.at(i)->getHP();
			auto hpItem = Label::createWithTTF("HP: " + HP.str(), "fonts/Starcraft Normal.ttf", 13);
			hpItem->setColor(Color3B(223, 0, 41));
			hpItem->setPosition(backgroundItem->getPosition().x + 50, backgroundItem->getPosition().y - 7);
			scrollView->addChild(hpItem);
		}
		else
			if (list.at(i)->getType() == 2)
			{
				ostringstream HP;
				HP << list.at(i)->getHP();
				auto hpItem = Label::createWithTTF("HP: " + HP.str(), "fonts/Starcraft Normal.ttf", 13);
				hpItem->setColor(Color3B(223, 0, 41));
				hpItem->setPosition(backgroundItem->getPosition().x - 30, backgroundItem->getPosition().y - 7);
				scrollView->addChild(hpItem);

				ostringstream AR;
				AR << list.at(i)->getArmor();
				auto armorItem = Label::createWithTTF("ARMOR: " + AR.str(), "fonts/Starcraft Normal.ttf", 13);
				armorItem->setColor(Color3B(0, 160, 107));
				armorItem->setPosition(backgroundItem->getPosition().x + 50, backgroundItem->getPosition().y - 7);
				scrollView->addChild(armorItem);
			}
			else
			{
				ostringstream DAMAGE;
				DAMAGE << list.at(i)->getDamage();
				auto damageItem = Label::createWithTTF("DAMAGE: " + DAMAGE.str(), "fonts/Starcraft Normal.ttf", 13);
				damageItem->setColor(Color3B(208, 119, 11));
				damageItem->setPosition(backgroundItem->getPosition().x, backgroundItem->getPosition().y - 7);
				scrollView->addChild(damageItem);
			}

	}

	auto commitBtn = ui::Button::create("commitshoppanel.png", "commitshoppanel_p.png");
	//commitBtn->setScale(0.9);
	commitBtn->setPosition(Vec2(shopPanel->getPosition().x*1.3, shopPanel->getPosition().y*0.25));
	commitBtn->addClickEventListener([=](Ref* event) {
		layoutChest->removeAllChildren();

		for (int k = 0; k < listCheck.size(); k++)
		{
			if (listCheck.at(k)->isSelected())
			{
				listSelect.push_back(list.at(k));
			}
		}
		if (listSelect.size() != 0)
		{
			int sumGold = 0;
			for (int p = 0; p < listSelect.size(); p++)
			{
				sumGold = sumGold + listSelect.at(p)->getPrice();
			}
			if (sumGold <= McSetting::getInstance()->getGold())
			{
				for (int p = 0; p < listSelect.size(); p++)
				{
					bool same = false;
					for (int q = 0; q < listBought.size(); q++)
					{
						if (listSelect.at(p)->getName().compare(listBought.at(q)->getName()) == 0)
						{
							listBought.at(q)->setQuantityBought(listBought.at(q)->getQuantityBought() + 1);
							same = true;
						}
					}
					if (same == false)
					{
						listSelect.at(p)->setQuantityBought(listSelect.at(p)->getQuantityBought() + 1);
						listBought.push_back(listSelect.at(p));
					}
					McSetting::getInstance()->setGold(McSetting::getInstance()->getGold() - sumGold);
					log("%d", McSetting::getInstance()->getGold());
					ostringstream OS;
					OS << McSetting::getInstance()->getGold();
					goldText->setString(OS.str());
				}
			}
			
		}

		loadChest();

		//Output list bought into file
		Document document;
		document.SetObject();

		// must pass an allocator when the object may need to allocate memory
		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
		rapidjson::Value array(rapidjson::kArrayType);
		for (int i = 0; i < listBought.size(); i++)
		{
			rapidjson::Value object(rapidjson::kObjectType);
			rapidjson::Value spriteName, name, info;
			object.AddMember("picture", spriteName.SetString(listBought.at(i)->getSpriteName().c_str(), allocator), allocator);
			object.AddMember("name", name.SetString(listBought.at(i)->getName().c_str(), allocator), allocator);
			object.AddMember("info", info.SetString(listBought.at(i)->getInfo().c_str(), allocator), allocator);
			object.AddMember("price", listBought.at(i)->getPrice(), allocator);
			object.AddMember("type", listBought.at(i)->getType(), allocator);
			object.AddMember("hp", listBought.at(i)->getHP(), allocator);
			object.AddMember("mp", listBought.at(i)->getMP(), allocator);
			object.AddMember("armor", listBought.at(i)->getArmor(), allocator);
			object.AddMember("damage", listBought.at(i)->getDamage(), allocator);
			object.AddMember("quantity", listBought.at(i)->getQuantity(), allocator);
			object.AddMember("quantity_bought", listBought.at(i)->getQuantityBought(), allocator);
			array.PushBack(object, allocator);

		}
		document.AddMember("item_bought", array, allocator);

		string tr = "Data/ListBought.json";
		JsonToFile(document, tr);

		listSelect.clear();
		listCheck.clear();
		loadShop(list);
		
	});
	mShopLayer->addChild(commitBtn);

	auto closeBtn = ui::Button::create("closeshoppanel.png", "closeshoppanel_p.png");
	//closeBtn->setScale(0.9);
	closeBtn->setPosition(Vec2(shopPanel->getPosition().x*0.8, shopPanel->getPosition().y*0.25));
	closeBtn->addClickEventListener([=](Ref* event) {
		mShopLayer->setVisible(false);
		mController->setVisible(true);
		mPlayer->getSprite()->setVisible(true);
		mPlayer->getPhysicsBody()->setEnabled(true);
		mPlayer->getPhysicsFoot()->setEnabled(true);
		canMove = true;
		listSelect.clear();
		listCheck.clear();
		McSetting::getInstance()->saveData();
		log("%d", McSetting::getInstance()->getGold());
	});
	mShopLayer->addChild(closeBtn);
}

bool NpcScene::onContactBegin(PhysicsContact& contact)
{
	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();

	
	if ((a->getCollisionBitmask() == Model::BITMASK_PLAYER && b->getCollisionBitmask() == Model::BITMASK_PLAYER_FOOT)
		|| (a->getCollisionBitmask() == Model::BITMASK_PLAYER_FOOT && b->getCollisionBitmask() == Model::BITMASK_PLAYER))
		return false;

	/////////
	if ((a->getCollisionBitmask() == Model::BITMASK_PLAYER_FOOT && b->getCollisionBitmask() == Model::BITMASK_TalkNPC_Keeper)
		|| (a->getCollisionBitmask() == Model::BITMASK_TalkNPC_Keeper && b->getCollisionBitmask() == Model::BITMASK_PLAYER_FOOT))
	{
		auto funcMove = CallFunc::create([&]() {
			canMove = false;
			mPlayer->getSprite()->setPosition(Vec2(mPlayer->getSprite()->getPosition().x, mPlayer->getSprite()->getPosition().y - 100));
			mPlayer->getSprite()->setVisible(false);
			mPlayer->getPhysicsBody()->setEnabled(false);
			mPlayer->getPhysicsFoot()->setEnabled(false);
		});
		auto funcDisplay = CallFunc::create([&]() {
			mController->setVisible(false);
			mTutorialLayer->setVisible(true);
		});
		runAction(Sequence::create(funcMove, funcDisplay, nullptr));
	}
	///////////

	if ((a->getCollisionBitmask() == Model::BITMASK_PLAYER_FOOT && b->getCollisionBitmask() == Model::BITMASK_TalkNPC_ArmorSeller)
		|| (a->getCollisionBitmask() == Model::BITMASK_TalkNPC_ArmorSeller && b->getCollisionBitmask() == Model::BITMASK_PLAYER_FOOT))
	{
		auto funcMove = CallFunc::create([&]() {
			canMove = false;
			mPlayer->getSprite()->setPosition(Vec2(mPlayer->getSprite()->getPosition().x, mPlayer->getSprite()->getPosition().y - 100));
			mPlayer->getSprite()->setVisible(false);
			mPlayer->getPhysicsBody()->setEnabled(false);
			mPlayer->getPhysicsFoot()->setEnabled(false);
		});
		auto funcDisplay = CallFunc::create([&]() {
			mController->setVisible(false);
			mShopLayer->setVisible(true);
			loadShop(listArmor);
		});
		runAction(Sequence::create( funcMove, funcDisplay, nullptr));
	}

	if ((a->getCollisionBitmask() == Model::BITMASK_PLAYER_FOOT && b->getCollisionBitmask() == Model::BITMASK_TalkNPC_Medicine)
		|| (a->getCollisionBitmask() == Model::BITMASK_TalkNPC_Medicine && b->getCollisionBitmask() == Model::BITMASK_PLAYER_FOOT))
	{
		auto funcMove = CallFunc::create([&]() {
			canMove = false;
			mPlayer->getSprite()->setPosition(Vec2(mPlayer->getSprite()->getPosition().x, mPlayer->getSprite()->getPosition().y - 100));
			mPlayer->getSprite()->setVisible(false);
			mPlayer->getPhysicsBody()->setEnabled(false);
			mPlayer->getPhysicsFoot()->setEnabled(false);
		});
		auto funcDisplay = CallFunc::create([&]() {
			mController->setVisible(false);
			mShopLayer->setVisible(true);
			loadShop(listMedicine);
		});
		runAction(Sequence::create(funcMove, funcDisplay, nullptr));
	}

	if ((a->getCollisionBitmask() == Model::BITMASK_PLAYER_FOOT && b->getCollisionBitmask() == Model::BITMASK_TalkNPC_ArmsSeller)
		|| (a->getCollisionBitmask() == Model::BITMASK_TalkNPC_ArmsSeller && b->getCollisionBitmask() == Model::BITMASK_PLAYER_FOOT))
	{
		auto funcMove = CallFunc::create([&]() {
			canMove = false;
			mPlayer->getSprite()->setPosition(Vec2(mPlayer->getSprite()->getPosition().x, mPlayer->getSprite()->getPosition().y - 100));
			mPlayer->getSprite()->setVisible(false);
			mPlayer->getPhysicsBody()->setEnabled(false);
			mPlayer->getPhysicsFoot()->setEnabled(false);
		});
		auto funcDisplay = CallFunc::create([&]() {
			mController->setVisible(false);
			mShopLayer->setVisible(true);
			loadShop(listWeapon);
		});
		runAction(Sequence::create(funcMove, funcDisplay, nullptr));
	}

	/*if ((a->getCollisionBitmask() == Model::BITMASK_PLAYER_FOOT && b->getCollisionBitmask() == Model::BITMASK_TELEPORT)
		|| (a->getCollisionBitmask() == Model::BITMASK_TELEPORT && b->getCollisionBitmask() == Model::BITMASK_PLAYER_FOOT))
	{
		auto selectMap = SelectMapScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, selectMap, Color3B(0, 255, 255)));
	}*/

	return true;
}

vector<ItemShop*> NpcScene::getListBought()
{
	list = listBought;
	return list;
}

void NpcScene::setListBought()
{
	listBought.clear();
}

void NpcScene::setReborn()
{
	/*log("%d, %d", mPlayer->getSprite()->getPosition().x, mPlayer->getSprite()->getPosition().y);
	mPlayer->getSprite()->setPosition(rebornX,rebornY);*/
}
