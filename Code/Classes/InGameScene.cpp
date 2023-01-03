#include "InGameScene.h"
#include "MenuScene.h"
#include "NpcScene.h"
#include "MapManager.h"
#include "SimpleAudioEngine.h"
#include "SelectMapScene.h"
#include <iostream>
#include <string>

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

extern string gMapName;
extern int gMapNumber;
Size visibleSize;

cocos2d::Node* mPauseLayer;
cocos2d::ui::Button* btnPause;
cocos2d::Node* mControllerLayer;
Sprite* panel;
Sprite* winPanel;
SneakyJoystickSkinnedBase* joystickBase;
Label* levelLabel;
Label* goldLabel;
Label* armorLabel;
Label* damageLabel;

float deltaTime = 0.6f;
bool checkMove = false;

Scene* InGameScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vect(0.0f, 0.0f));
	auto layer = InGameScene::create();
	scene->addChild(layer);
	return scene;
}

// on "init" you need to initialize your instance
bool InGameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();

	//CCLOG("%d", NpcScene::getInstance()->getListBought().size());

	//create map
	//createMap();

	//init physics
	//createPhysics();

	MapManager::getInstance()->loadMap(this, gMapNumber, gMapName);

	createObjects();

	//create controller
	createController();

	//create hud
	createHud();

	//create pause layer
	createPauseLayer();

	//add Listener: touch, key 
	addListener();

	//update
	schedule(CC_SCHEDULE_SELECTOR(InGameScene::updateObject), 0.5);
	scheduleOnce(CC_SCHEDULE_SELECTOR(InGameScene::updateReady_dg), 1);
	this->scheduleUpdate();

	return true;
}

void InGameScene::updateObject(float dt)
{

	auto listCollisionObjects = MapManager::getInstance()->getCollisionObjects();
	auto listCollisionNodes = MapManager::getInstance()->getCollisionNodes();

	Rect viewVisible = Rect(-this->getPositionX(), -this->getPositionY(), visibleSize.width, visibleSize.height);

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
void InGameScene::updateReady_dg(float dt)
{
	this->getScene()->getPhysicsWorld()->setAutoStep(false);

	//this->scheduleUpdate();
}


void InGameScene::update(float dt)
{
	//update Monsters

	updateCowMonsters();
	updateArcherMonster();
	updateBossMonsters();
	updateDogMonsters();

	setViewPointCenter(mPlayer->getSprite()->getPosition());

	//update blood of Player 
	mPlayer->update(dt);

	//update blood hud of mc
	mMcHudBlood->setPercent((mPlayer->getCurrent_Blood() * 100) / mPlayer->getBlood());
	mMcHudMana->setPercent((mPlayer->getCurrent_Mana() * 100) / mPlayer->getMana());
	mMcHudExperience->setPercent((mPlayer->getCurrent_Exp() * 100) / mPlayer->getMaxExp());

	ostringstream SL;
	SL << mPlayer->getLevel();
	levelLabel->setString(SL.str());

	ostringstream SL2;
	SL2 << mPlayer->getGold();
	goldLabel->setString(SL2.str());

	ostringstream SL3;
	SL3 << mPlayer->getArmor();
	armorLabel->setString(SL3.str());

	ostringstream SL4;
	SL4 << mPlayer->getDamage();
	damageLabel->setString(SL4.str());



	if (mPlayer->getSprite()->getNumberOfRunningActions() == 0)
	{
		mPlayer->setState(Player::STATE_IDLE);
	}

	// Move Player by Joystick (If player attack, player will stop move)
	Point scaleVelocity = ccpMult(leftJoystick->getVelocity(), 120);
	if (scaleVelocity.x < 0 && mPlayer->getCurrentState() < Player::STATE_ATTACK)
	{
		mPlayer->getSprite()->setAnchorPoint(Vec2(0.4, 0));
		Point newPosition = ccp(mPlayer->getSprite()->getPosition().x + scaleVelocity.x*dt, mPlayer->getSprite()->getPosition().y + scaleVelocity.y*dt);
		mPlayer->setFlip(false);
		mPlayer->getPhysicsFoot()->setVelocity(Vec2(scaleVelocity.x, scaleVelocity.y));
		mPlayer->setState(Player::STATE_MOVE);
		mPlayer->setMoveRight(false);
	}
	else if (scaleVelocity.x > 0 && mPlayer->getCurrentState() < Player::STATE_ATTACK)
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

	// Move Player by Keyboard
	if ((mCurrentKey == EventKeyboard::KeyCode::KEY_LEFT_ARROW || mCurrentKey == EventKeyboard::KeyCode::KEY_A) && mPlayer->getCurrentState() < Player::STATE_ATTACK) //move left
	{
		Point newPosition = ccp(mPlayer->getSprite()->getPosition().x - 120 * dt, mPlayer->getSprite()->getPosition().y + 1 * dt);
		mPlayer->setFlip(false);
		mPlayer->getPhysicsFoot()->setVelocity(Vec2(-3000 * dt, 1 * dt));
		mPlayer->setState(Player::STATE_MOVE);
		mPlayer->setMoveRight(false);
	}
	if ((mCurrentKey == EventKeyboard::KeyCode::KEY_RIGHT_ARROW || mCurrentKey == EventKeyboard::KeyCode::KEY_D) && mPlayer->getCurrentState() < Player::STATE_ATTACK) //move right
	{
		Point newPosition = ccp(mPlayer->getSprite()->getPosition().x + 120 * dt, mPlayer->getSprite()->getPosition().y + 1 * dt);
		mPlayer->setFlip(true);
		mPlayer->getPhysicsFoot()->setVelocity(Vec2(3000 * dt, 1 * dt));
		mPlayer->setState(Player::STATE_MOVE);
		mPlayer->setMoveRight(true);
	}
	if ((mCurrentKey == EventKeyboard::KeyCode::KEY_UP_ARROW || mCurrentKey == EventKeyboard::KeyCode::KEY_W) && mPlayer->getCurrentState() < Player::STATE_ATTACK) //move left
	{
		Point newPosition = ccp(mPlayer->getSprite()->getPosition().x + 1 * dt, mPlayer->getSprite()->getPosition().y + 120 * dt);
		mPlayer->getPhysicsFoot()->setVelocity(Vec2(1 * dt, 3000 * dt));
		mPlayer->setState(Player::STATE_MOVE);
	}
	if ((mCurrentKey == EventKeyboard::KeyCode::KEY_DOWN_ARROW || mCurrentKey == EventKeyboard::KeyCode::KEY_S) && mPlayer->getCurrentState() < Player::STATE_ATTACK) //move right
	{
		Point newPosition = ccp(mPlayer->getSprite()->getPosition().x + 1 * dt, mPlayer->getSprite()->getPosition().y - 120 * dt);
		mPlayer->getPhysicsFoot()->setVelocity(Vec2(1 * dt, -3000 * dt));
		mPlayer->setState(Player::STATE_MOVE);
	}

	for (int i = 0; i < 6; ++i)
	{
		this->getScene()->getPhysicsWorld()->step(1 / 180.0f);
	}

	//cd skill
	if (timeskill1 > 0.02f)
	{
		timeskill1 -= dt;
		__String *text1 = __String::createWithFormat("%.1f", timeskill1);
		cdskill1->setString(text1->getCString());
	}
	if (timeskill2 > 0.02f)
	{
		timeskill2 -= dt;
		__String *text2 = __String::createWithFormat("%.1f", timeskill2);
		cdskill2->setString(text2->getCString());
	}
	if (timeskill3 > 0.02f)
	{
		timeskill3 -= dt;
		__String *text3 = __String::createWithFormat("%.1f", timeskill3);
		cdskill3->setString(text3->getCString());
	}

	if (mPlayer->isAlive() == false)
	{
		auto inGame = SelectMapScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(1, inGame, Color3B(0, 255, 255)));
		this->unscheduleUpdate();
	}

	if (mPlayer->getCurrent_Mana() < 10)
	{
		mSkill1Controller->setEnabled(false);
		mSkill2Controller->setEnabled(false);
	}
	else
	{
		if (timeskill1 <= 0.04f)
		{
			mSkill1Controller->setEnabled(true);
		}
		if (timeskill2 <= 0.04f)
		{
			mSkill2Controller->setEnabled(true);
		}
	}
	if (mPlayer->getCurrent_Mana() < 20)
	{
		mSkill3Controller->setEnabled(false);
	}
	else
	{
		if (timeskill3 <= 0.04f)
		{
			mSkill3Controller->setEnabled(true);
		}
	}
}


void InGameScene::addListener()
{
	//key
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(InGameScene::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(InGameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//add contact event listener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(InGameScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool InGameScene::onTouchBegan(Touch* touch, Event  *event)
{
	mCurrentTouchState = ui::Widget::TouchEventType::BEGAN;
	mCurrentTouchPoint = touch->getLocation();
	return true;
}

void InGameScene::onTouchMoved(Touch* touch, Event  *event)
{
	mCurrentTouchState = ui::Widget::TouchEventType::MOVED;
	mCurrentTouchPoint = touch->getLocation();
	log("Touch Moved (%f, %f)", touch->getLocation().x, touch->getLocation().y);
}

void InGameScene::onTouchEnded(Touch* touch, Event  *event)
{
	mCurrentTouchState = ui::Widget::TouchEventType::ENDED;
	mCurrentTouchPoint = Point(-1, -1);
}

void InGameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	mCurrentKey = keyCode;
}

void InGameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	mCurrentKey = EventKeyboard::KeyCode::KEY_NONE;
}


void InGameScene::createObjects() {


	//Create player from Tile Map
	auto playerTile = MapManager::getInstance()->getTiledMap()->getObjectGroup("Player");
	auto mc = playerTile->objectNamed("Player");
	auto x = mc.at("x").asInt();
	auto y = mc.at("y").asInt();
	mPlayer = new Player(this);
	mPlayer->getSprite()->setPosition(x, y);
	//NpcScene::getInstance();
	vector<ItemShop*> list = NpcScene::getInstance()->getListBought();
	for (int i = 0; i < list.size(); i++)
	{
		if (list.at(i)->getType() == 1)
		{
			mPlayer->setBlood(mPlayer->getBlood() + list.at(i)->getHP());
			//MP
		}
		else
			if (list.at(i)->getType() == 2)
			{
				mPlayer->setBlood(mPlayer->getBlood() + list.at(i)->getHP());
				mPlayer->setArmor(mPlayer->getArmor() + list.at(i)->getArmor());
			}
			else
				if (list.at(i)->getType() == 3)
				{
					mPlayer->setDamage(mPlayer->getDamage() + list.at(i)->getDamage());
				}
	}
	// Create monster from Tile Map
	auto objects = MapManager::getInstance()->getTiledMap()->getObjectGroup("Monster")->getObjects();
	int cow_count = 0, archer_count = 0, dog_count = 0, boss_count = 0;
	for (int i = 0; i < objects.size(); i++)
	{
		auto object = objects.at(i);
		auto properties = object.asValueMap();
		int posX = properties.at("x").asInt();
		int posY = properties.at("y").asInt();
		int type = object.asValueMap().at("type").asInt();
		if (type == Model::MODLE_TYPE_COW_MONSTER)
		{
			CowMonster *cowMonster = new CowMonster(this);
			cowMonster->setPosition(Vec2(posX, posY));
			cowMonster->setIndex(cow_count++);
			mCowMonsters.push_back(cowMonster);
		}
		else
			if (type == Model::MODLE_TYPE_Archer_MONSTER)
			{
				ArcherMonster *archerMonster = new ArcherMonster(this);
				archerMonster->setPosition(Vec2(posX, posY));
				archerMonster->setIndex(archer_count++);
				mArcherMonsters.push_back(archerMonster);
			}
			else
				if (type == Model::MODLE_TYPE_DOG_MONSTER)
				{
					DogMonster *dogMonster = new DogMonster(this);
					dogMonster->setPosition(Vec2(posX, posY));
					dogMonster->setIndex(dog_count++);
					mDogMonsters.push_back(dogMonster);
				}
				else
					if (type == Model::MODLE_TYPE_BOSS_MONSTER)
					{
						BossMonster *bossMonster = new BossMonster(this);
						bossMonster->setPosition(Vec2(posX, posY));
						bossMonster->setIndex(boss_count++);
						mBossMonsters.push_back(bossMonster);
					}
	}
}

//Create controller, include skills button and pause game button
void InGameScene::createController()
{
	Director::getInstance()->getVisibleSize();
	mControllerLayer = Node::create();
	mControllerLayer->setContentSize(visibleSize);
	addChild(mControllerLayer);

	//Joystick
	Rect joystickBaseDimensions;
	joystickBaseDimensions = Rect(0, 0, 160.0f, 160.0f);

	Point joystickBasePosition;
	joystickBasePosition = Vec2(visibleSize.width*0.16, visibleSize.height*0.15);

	joystickBase = new SneakyJoystickSkinnedBase();
	joystickBase->init();
	joystickBase->setPosition(joystickBasePosition);
	joystickBase->setBackgroundSprite(Sprite::create("res/joystick-back2.png"));
	joystickBase->setThumbSprite(Sprite::create("res/stickv.png"));
	//joystickBase->setOpacity(220);
	joystickBase->setScale(0.8);
	joystickBase->setZOrder(7);
	SneakyJoystick *aJoystick = new SneakyJoystick();
	aJoystick->initWithRect(joystickBaseDimensions);

	aJoystick->autorelease();
	joystickBase->setJoystick(aJoystick);
	joystickBase->setPosition(joystickBasePosition);
	aJoystick->setZOrder(7);
	leftJoystick = joystickBase->getJoystick();
	leftJoystick->retain();
	leftJoystick->setZOrder(7);

	mControllerLayer->addChild(joystickBase);

	//attack control
	mAttackController = ui::Button::create("at.png", "at_p.png");

	mAttackController->setOpacity(220);
	mAttackController->setPosition(Vec2(visibleSize.width - 65, 65));
	mAttackController->addTouchEventListener(CC_CALLBACK_2(InGameScene::attack, this));
	mAttackController->setScale(0.8f);
	mAttackController->setGlobalZOrder(7);
	mControllerLayer->addChild(mAttackController);
	float scaleSkill = 0.6f;

	//skill 1 control
	mSkill1Controller = ui::Button::create("skill1.png", "skill1_p.png");

	mSkill1Controller->setOpacity(220);
	mSkill1Controller->setPosition(Vec2(mAttackController->getPosition().x - 95, mAttackController->getPosition().y - 27));
	mSkill1Controller->addTouchEventListener(CC_CALLBACK_2(InGameScene::skill1, this));
	mSkill1Controller->setScale(scaleSkill);
	mControllerLayer->addChild(mSkill1Controller);
	cdskill1 = Label::createWithSystemFont("", "Arial", 25);
	cdskill1->enableShadow();
	cdskill1->setPosition(mSkill1Controller->getPosition());
	mSkill1Controller->setGlobalZOrder(7);
	cdskill1->setGlobalZOrder(8);
	mControllerLayer->addChild(cdskill1);
	cdskill1->setVisible(false);

	//skill 2 control
	mSkill2Controller = ui::Button::create("skill2.png", "skill2_p.png");

	mSkill2Controller->setOpacity(220);
	mSkill2Controller->setPosition(Vec2(mAttackController->getPosition().x - 65, mAttackController->getPosition().y + 55));
	mSkill2Controller->addTouchEventListener(CC_CALLBACK_2(InGameScene::skill2, this));
	mSkill2Controller->setScale(scaleSkill);
	mControllerLayer->addChild(mSkill2Controller);
	cdskill2 = Label::createWithSystemFont("", "Arial", 25);
	cdskill2->enableShadow();
	cdskill2->setPosition(mSkill2Controller->getPosition());
	mSkill2Controller->setGlobalZOrder(7);
	cdskill2->setGlobalZOrder(8);
	mControllerLayer->addChild(cdskill2);
	cdskill2->setVisible(false);


	//skill 3 control
	mSkill3Controller = ui::Button::create("skill3.png", "skill3_p.png");

	mSkill3Controller->setOpacity(220);
	mSkill3Controller->setPosition(Vec2(mAttackController->getPosition().x + 10, mAttackController->getPosition().y + 90));
	mSkill3Controller->addTouchEventListener(CC_CALLBACK_2(InGameScene::skill3, this));
	mSkill3Controller->setScale(scaleSkill);
	mControllerLayer->addChild(mSkill3Controller);
	cdskill3 = Label::createWithSystemFont("", "Arial", 25);
	cdskill3->enableShadow();
	cdskill3->setPosition(mSkill3Controller->getPosition());
	mSkill3Controller->setGlobalZOrder(7);
	cdskill3->setGlobalZOrder(8);
	mControllerLayer->addChild(cdskill3);
	cdskill3->setVisible(false);

	//pause on game screen
	btnPause = ui::Button::create("pause_norrmal.png", "pause_pressed.png");
	
	btnPause->setScale(1.2f);
	btnPause->setPosition(Vec2(visibleSize.width*0.92, visibleSize.height - 40));
	btnPause->addTouchEventListener(CC_CALLBACK_2(InGameScene::pause, this));
	btnPause->setGlobalZOrder(7);
	mControllerLayer->addChild(btnPause);
	mControllerLayer->setGlobalZOrder(7);

}

void InGameScene::createPauseLayer()
{
	mPauseLayer = Node::create();
	mPauseLayer->setName("mPauseLayer");
	addChild(mPauseLayer);
	mPauseLayer->setVisible(false);

	panel = Sprite::create("pausemenuabg.png");
	panel->setScale(1.3f);
	panel->setPosition(visibleSize / 2);
	panel->setGlobalZOrder(7);
	mPauseLayer->addChild(panel);

	auto btnHome = ui::Button::create("home_normal.png", "home_pressed.png");
	btnHome->setScale(0.7);
	btnHome->setPosition(Vec2(panel->getPosition().x*0.15f + 28, panel->getPosition().y*0.55f + 10));
	btnHome->addClickEventListener([&](Ref* event) {
		McSetting* mcSetting = McSetting::getInstance();
		mcSetting->setBloodPlayer(mPlayer->getBlood());
		mcSetting->setMana(mPlayer->getMana());
		mcSetting->setDamePlayer(mPlayer->getDamage());
		mcSetting->setArmor(mPlayer->getArmor());
		mcSetting->setEXP(mPlayer->getCurrent_Exp());
		mcSetting->setGold(mPlayer->getGold());
		mcSetting->setPlayerLevel(mPlayer->getLevel());
		mcSetting->saveData();

		SimpleAudioEngine::getInstance()->resumeAllEffects();
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		Director::getInstance()->resume();
		Director::getInstance()->replaceScene(MenuScene::createScene());
	});
	btnHome->setGlobalZOrder(7);
	panel->addChild(btnHome);
	
	auto btnResume = ui::Button::create("resume_normal.png", "resume_pressed.png");
	btnResume->setScale(0.7);
	btnResume->setPosition(Vec2(panel->getPosition().x*0.15f + 28, panel->getPosition().y*0.55f + 65));
	btnResume->addClickEventListener([](Ref* event) {
		SimpleAudioEngine::getInstance()->resumeAllEffects();
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		Director::getInstance()->resume();
		mPauseLayer->setVisible(false);
		mControllerLayer->setVisible(true);
	});
	btnResume->setGlobalZOrder(7);
	panel->addChild(btnResume);

	auto btnSetting = ui::Button::create("setting_normal1.png", "setting_pressed1.png");
	btnSetting->setScale(0.7);
	btnSetting->setPosition(Vec2(panel->getPosition().x*0.15f + 28, panel->getPosition().y*0.15f));
	btnSetting->addClickEventListener([=](Ref* event) {
		//mControllerLayer->setVisible(false);
		panel->setVisible(false);
		SoundManager::getInstance()->showSetting(mPauseLayer);
	});
	btnSetting->setGlobalZOrder(7);
	panel->addChild(btnSetting);
	
	
	auto btnSave = ui::Button::create("save.png", "save_p.png");
	btnSave->setScale(0.7);
	btnSave->setPosition(Vec2(panel->getPosition().x*0.15f + 28, panel->getPosition().y*0.35f + 5));
	btnSave->addClickEventListener([&](Ref* event) {
		McSetting* mcSetting = McSetting::getInstance();
		mcSetting->setBloodPlayer(mPlayer->getBlood());
		mcSetting->setMana(mPlayer->getMana());
		mcSetting->setDamePlayer(mPlayer->getDamage());
		mcSetting->setArmor(mPlayer->getArmor());
		mcSetting->setEXP(mPlayer->getCurrent_Exp());
		mcSetting->setGold(mPlayer->getGold());
		mcSetting->setPlayerLevel(mPlayer->getLevel());
		mcSetting->saveData();
		/////////
		SimpleAudioEngine::getInstance()->resumeAllEffects();
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		Director::getInstance()->resume();
		mPauseLayer->setVisible(false);
		mControllerLayer->setVisible(true);
	});
	btnSave->setGlobalZOrder(7);
	panel->addChild(btnSave);

	winPanel = Sprite::create("pausemenuabg.png");
	winPanel->setScale(1.3f);
	winPanel->setPosition(visibleSize / 2);
	winPanel->setGlobalZOrder(7);
	mPauseLayer->addChild(winPanel);

	auto winLabel = Label::createWithTTF("Win Game", "fonts/HoboStd.otf", 27);
	winLabel->setPosition(Vec2(winPanel->getPosition().x*0.15f + 28, winPanel->getPosition().y*0.55f + 30));
	winLabel->setGlobalZOrder(7);
	winPanel->addChild(winLabel);

	auto btnNewGame = ui::Button::create("home_normal.png", "home_pressed.png");
	btnNewGame->setScale(0.7);
	btnNewGame->setPosition(Vec2(winPanel->getPosition().x*0.15f + 28, winPanel->getPosition().y*0.35f + 5));
	btnNewGame->addClickEventListener([&](Ref* event) {
		McSetting* mcSetting = McSetting::getInstance();
		mcSetting->setBloodPlayer(mPlayer->getBlood());
		mcSetting->setMana(mPlayer->getMana());
		mcSetting->setDamePlayer(mPlayer->getDamage());
		mcSetting->setArmor(mPlayer->getArmor());
		mcSetting->setEXP(mPlayer->getCurrent_Exp());
		mcSetting->setGold(mPlayer->getGold());
		mcSetting->setPlayerLevel(mPlayer->getLevel());
		mcSetting->saveData();

		SimpleAudioEngine::getInstance()->resumeAllEffects();
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		Director::getInstance()->resume();
		Director::getInstance()->replaceScene(MenuScene::createScene());
	});
	btnNewGame->setGlobalZOrder(7);
	winPanel->addChild(btnNewGame);

	mPauseLayer->setGlobalZOrder(7);
}


void InGameScene::setViewPointCenter(Point position)
{
	int x = MAX(position.x, visibleSize.width / 2);
	int y = MAX(position.y, visibleSize.height / 2);
	x = MIN(x, (MapManager::getInstance()->getTiledMap()->getMapSize().width * MapManager::getInstance()->getTiledMap()->getTileSize().width) - visibleSize.width / 2);
	y = MIN(y, (MapManager::getInstance()->getTiledMap()->getMapSize().height * MapManager::getInstance()->getTiledMap()->getTileSize().height) - visibleSize.height / 2);
	Point actualPosition = ccp(x, y);

	Point centerOfView = ccp(visibleSize.width / 2, visibleSize.height / 2);
	Point viewPoint = ccpSub(centerOfView, actualPosition);
	this->setPosition(viewPoint);
	mControllerLayer->setPosition(-viewPoint);
	mPauseLayer->setPosition(-viewPoint);
}



void InGameScene::attack(Ref* sender, ui::Widget::TouchEventType type)
{
	mAttackController->setEnabled(true);
	auto action = Sequence::create(
		CallFunc::create(
		[&]()
	{
		if (SoundManager::getInstance()->getVlSound())
		{
			SoundManager::getInstance()->playButtonEffect(SOUND_NAME::BUTTON);
		}
		mPlayer->setState(Player::STATE_ATTACK);
		mAttackController->setTouchEnabled(false);
		mAttackController->setEnabled(false);
		mSkill2Controller->setTouchEnabled(false);
		mSkill1Controller->setTouchEnabled(false);
		mSkill3Controller->setTouchEnabled(false);
	}
		),
		DelayTime::create(0.6),
		CallFunc::create(
		[&]()
	{
		mSkill2Controller->setTouchEnabled(true);
		mSkill1Controller->setTouchEnabled(true);
		mSkill3Controller->setTouchEnabled(true);
		mAttackController->setTouchEnabled(true);
		mAttackController->setEnabled(true);
	}
		),
		NULL
		);
	runAction(action);
}

void InGameScene::skill1(Ref* sender, ui::Widget::TouchEventType type)
{
	SoundManager::getInstance()->playButtonEffect(SOUND_NAME::SKILL_CHOP);
	mPlayer->setState(Player::STATE_SKILL1);
	mSkill1Controller->setEnabled(true);
	auto action = Sequence::create(
		CallFunc::create(
		[&]()
	{
		if (SoundManager::getInstance()->getVlSound())
		{
			SoundManager::getInstance()->playButtonEffect(SOUND_NAME::SKILL_CHOP);
		}
		mPlayer->setState(Player::STATE_SKILL1);
		mSkill1Controller->setTouchEnabled(false);
		mSkill1Controller->setEnabled(false);
		mAttackController->setTouchEnabled(false);
		mSkill2Controller->setTouchEnabled(false);
		mSkill3Controller->setTouchEnabled(false);
		cdskill1->setVisible(true);
		timeskill1 = 2.0;
	}
		),
		DelayTime::create(0.5),
		CallFunc::create(
		[&]()
	{
		mAttackController->setTouchEnabled(true);
		mSkill2Controller->setTouchEnabled(true);
		mSkill3Controller->setTouchEnabled(true);
	}
		),
		DelayTime::create(1.5),
		CallFunc::create(
		[&]()
	{
		mSkill1Controller->setTouchEnabled(true);
		mSkill1Controller->setEnabled(true);
		cdskill1->setVisible(false);
	}
		),
		NULL
		)
		;
	runAction(action);
}


void InGameScene::skill2(Ref* sender, ui::Widget::TouchEventType type)
{
	mSkill2Controller->setEnabled(true);
	auto action = Sequence::create(
		CallFunc::create(
		[&]()
	{
		if (SoundManager::getInstance()->getVlSound())
		{
			SoundManager::getInstance()->playButtonEffect(SOUND_NAME::SKILL_SHURIKEN);
		}
		mPlayer->setState(Player::STATE_SKILL2);
		mSkill2Controller->setTouchEnabled(false);
		mSkill2Controller->setEnabled(false);
		mAttackController->setTouchEnabled(false);
		mSkill1Controller->setTouchEnabled(false);
		mSkill3Controller->setTouchEnabled(false);
		cdskill2->setVisible(true);
		timeskill2 = 5.0;
	}
		),
		DelayTime::create(0.8),
		CallFunc::create(
		[&]()
	{
		mAttackController->setTouchEnabled(true);
		mSkill1Controller->setTouchEnabled(true);
		mSkill3Controller->setTouchEnabled(true);
	}
		),
		DelayTime::create(4.2),
		CallFunc::create(
		[&]()
	{
		mSkill2Controller->setTouchEnabled(true);
		mSkill2Controller->setEnabled(true);
		cdskill2->setVisible(false);
	}
		),
		NULL
		);
	runAction(action);
}


void InGameScene::skill3(Ref* sender, ui::Widget::TouchEventType type)
{
	mSkill3Controller->setEnabled(true);
	auto action = Sequence::create(
		CallFunc::create(
		[&]()
	{
		if (SoundManager::getInstance()->getVlSound())
		{
			SoundManager::getInstance()->playButtonEffect(SOUND_NAME::SKILL_STORM);
		}
		mPlayer->setState(Player::STATE_SKILL3);
		mSkill3Controller->setTouchEnabled(false);
		mSkill3Controller->setEnabled(false);
		mSkill2Controller->setTouchEnabled(false);
		mSkill1Controller->setTouchEnabled(false);
		mAttackController->setTouchEnabled(false);
		cdskill3->setVisible(true);
		timeskill3 = 25.0;

	}
		),
		DelayTime::create(0.1),
		CallFunc::create(
		[&]()
	{
		mSkill2Controller->setTouchEnabled(true);
		mSkill1Controller->setTouchEnabled(true);
		mAttackController->setTouchEnabled(true);
	}
		),
		DelayTime::create(24.9),
		CallFunc::create(
		[&]()
	{
		mSkill3Controller->setTouchEnabled(true);
		mSkill3Controller->setEnabled(true);
		cdskill3->setVisible(false);
	}
		),
		NULL
		);
	runAction(action);
}


void InGameScene::pause(Ref* sender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case ui::Widget::TouchEventType::ENDED:
		auto funcPause = CallFunc::create([]() {
			SimpleAudioEngine::getInstance()->pauseAllEffects();
			SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
			Director::getInstance()->pause();
		});
		mControllerLayer->setVisible(false);
		mPauseLayer->setVisible(true);
		panel->setVisible(true);
		winPanel->setVisible(false);
		auto fadeIn = FadeIn::create(0.3f);
		mPauseLayer->runAction(Sequence::create(fadeIn, funcPause, nullptr));
		break;
	}
}

void InGameScene::enablePressedControl(bool isLeft, bool pressed)
{

}
void InGameScene::updateCowMonsters()
{
	CCLOG("Update Cow in Game");
	for (int i = 0; i < mCowMonsters.size(); i++)
	{
		if (mCowMonsters.at(i)->isAlive())
		{
			if (mCowMonsters.at(i)->getCurrent_Blood() <= 0)
			{

				mCowMonsters.at(i)->setState(CowMonster::STATE_DIE);
				mPlayer->setCurrent_Exp(mPlayer->getCurrent_Exp() + mCowMonsters.at(i)->getCurrent_Exp());
				mCowMonsters.at(i)->setAlive(false);

				SpriteFrameCache* cache = SpriteFrameCache::sharedSpriteFrameCache();
				cache->addSpriteFramesWithFile("ItemShop/Coin$.plist", "ItemShop/Coin$.png");
				auto gold = Sprite::createWithSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("coin_2.png"));
				gold->setScale(0.3);
				auto physicsBody2D = cocos2d::PhysicsBody::createBox(Size(gold->getContentSize()*0.3), PhysicsMaterial(10000.0f, 0.0f, 1.0f));
				physicsBody2D->setMass(50);
				physicsBody2D->setCollisionBitmask(Model::BITMASK_GOLD);
				physicsBody2D->setContactTestBitmask(true);
				physicsBody2D->setDynamic(false);
				physicsBody2D->setRotationEnable(false);
				gold->setPhysicsBody(physicsBody2D);
				gold->setPosition(mCowMonsters.at(i)->getPosition());
				Vector<SpriteFrame*> animFrames;
				char str[100] = { 0 };
				animFrames.clear();
				for (int i = 2; i <= 27; ++i)
				{
					sprintf(str, "coin_%d.png", i);
					SpriteFrame* frame = cache->spriteFrameByName(str);
					animFrames.pushBack(frame);
				}
				gold->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.03f))));
				addChild(gold);
			}
			//xu ly toa do 
			if (mPlayer->getPosition().y > mCowMonsters.at(i)->getSprite()->getPosition().y - 20)
			{
				mCowMonsters.at(i)->getSprite()->setGlobalZOrder(5);
				mPlayer->getSprite()->setGlobalZOrder(4);
			}
			else if (mPlayer->getPosition().y < mCowMonsters.at(i)->getSprite()->getPosition().y)
			{
				mCowMonsters.at(i)->getSprite()->setGlobalZOrder(3);
				mPlayer->getSprite()->setGlobalZOrder(4);

			}
			/*------------Monster Move------------------*/
			//Tinh khoang cach giua player voi CowMonster
			auto distance = ccpDistance(Vec2(mCowMonsters.at(i)->getPosition()), Vec2(mPlayer->getPosition()));
			//neu distance <= 200 va khong trong trang thay bi danh thi se phat hien ra player
			if (distance <= 200)
			{
				mCowMonsters.at(i)->setDetect(true);
			}
			else if (distance > 200)
			{
				mCowMonsters.at(i)->setDetect(false);
			}
			//log("%b", mCowMonsters.at(i)->getDetect());
			if (mCowMonsters.at(i)->getDetect() == true && mCowMonsters.at(i)->getCurrentState() != CowMonster::STATE_DAMAGE
				&& mCowMonsters.at(i)->getCurrentState() != CowMonster::STATE_ATTACK
				&& mCowMonsters.at(i)->getCurrentState() != CowMonster::STATE_DIE)
			{
				//CowMonster huong mat va di theo vi tri cua player
				//trai
				if ((mPlayer->getPosition().x < mCowMonsters.at(i)->getPosition().x))
				{
					//mArcher->getSprite()->setAnchorPoint(Vec2(0.5, 0));
					mCowMonsters.at(i)->setState(CowMonster::STATE_MOVE);
					mCowMonsters.at(i)->getPhysicsFoot()->setVelocity({ mPlayer->getPosition().x
						- mCowMonsters.at(i)->getPosition().x
						+ mPlayer->getContentSize().width - 3,
						mPlayer->getPosition().y
						- mCowMonsters.at(i)->getPosition().y });
					mCowMonsters.at(i)->getPhysicsFoot()->setVelocityLimit(30);
					mCowMonsters.at(i)->setFlip(true);
					//SimpleAudioEngine::getInstance()->playEffect("sounds/wolf3.mp3");
				}

				//phai
				if (mPlayer->getPosition().x > mCowMonsters.at(i)->getPosition().x)
				{
					//mArcher->getSprite()->setAnchorPoint(Vec2(0.5, 0));
					mCowMonsters.at(i)->setState(CowMonster::STATE_MOVE);
					mCowMonsters.at(i)->getPhysicsFoot()->setVelocity({ mPlayer->getPosition().x
						- mCowMonsters.at(i)->getPosition().x
						- mPlayer->getContentSize().width + 3,
						mPlayer->getPosition().y
						- mCowMonsters.at(i)->getPosition().y });
					mCowMonsters.at(i)->setFlip(false);
				}
			}
			//neu ra ngoai pham vi phat hien thi CowMonster se dung yen
			if (mCowMonsters.at(i)->getDetect() == false && mCowMonsters.at(i)->getCurrentState() != CowMonster::STATE_DIE)
			{
				//mArcher->getSprite()->setAnchorPoint(Vec2(0.5, 0));
				mCowMonsters.at(i)->getPhysicsFoot()->setVelocity({ 0, 0 });
				mCowMonsters.at(i)->setState(CowMonster::STATE_IDLE);
				//SimpleAudioEngine::getInstance()->playEffect("sounds/wolf3.mp3");
			}
			/*-------------Monster Attack----------------*/
			//neu nhan vat trong tam danh cua CowMonster thi CowMonster se phat huy don danh
			if (distance <= 60 && distance >= -60 && mCowMonsters.at(i)->getCurrentState() != CowMonster::STATE_DAMAGE
				&& mCowMonsters.at(i)->getCurrentState() != CowMonster::STATE_DIE)
			{
				//mArcher->getSprite()->setAnchorPoint(Vec2(0, 0));
				mCowMonsters.at(i)->getPhysicsFoot()->setVelocity({ 0, 0 });
				mCowMonsters.at(i)->setState(CowMonster::STATE_ATTACK);
			}
			//neu Cowmonster khong phai trang thai Attack thi huy box attack
			if (mCowMonsters.at(i)->getCurrentState() != CowMonster::STATE_ATTACK && mCowMonsters.at(i)->getCurrentState() != CowMonster::STATE_DIE)
			{
				mCowMonsters.at(i)->getPhysicsAttack()->setEnabled(false);
			}

			if (mCowMonsters.at(i)->getCurrentState() == CowMonster::STATE_DIE)
			{
				mCowMonsters.at(i)->getPhysicsFoot()->setVelocity({ 0, 0 });
			}

		}
		mCowMonsters.at(i)->update();
	}
}


void InGameScene::updateArcherMonster()
{

	for (int i = 0; i < mArcherMonsters.size(); i++)
	{
		CCLOG("Update Archer in Game");
		if (mArcherMonsters.at(i)->isAlive())
		{
			CCLOG("check isalive");
			//If player is in the active radius (140) of the frog, frog will attack player
			auto distanceX = abs(mArcherMonsters.at(i)->getPhysicsBody()->getPosition().x - mPlayer->getPhysicsBody()->getPosition().x);
			auto distanceY = abs(mArcherMonsters.at(i)->getPhysicsBody()->getPosition().y - mPlayer->getPhysicsBody()->getPosition().y);
			if ((distanceX <= 300 && distanceX >= 135 && distanceY <= 50)
				&& (mArcherMonsters.at(i)->getCurrentState() != ArcherMonster::STATE_DIE))
			{
				mArcherMonsters.at(i)->getSprite()->setAnchorPoint(Vec2(0.5, 0));
				mArcherMonsters.at(i)->getPhysicsFoot()->setVelocity({ 0, 0 });
				mArcherMonsters.at(i)->setState(ArcherMonster::STATE_FIRE_1);
			}
			else if (distanceX <= 130 && distanceX >= 70 && distanceY <= 70 && (mArcherMonsters.at(i)->getCurrentState() != ArcherMonster::STATE_DIE
				|| mArcherMonsters.at(i)->getCurrentState() != ArcherMonster::STATE_IDLE || mArcherMonsters.at(i)->getCurrentState() != ArcherMonster::STATE_FIRE_1
				|| mArcherMonsters.at(i)->getCurrentState() != ArcherMonster::STATE_FIRE_2))
			{
				mArcherMonsters.at(i)->getSprite()->setAnchorPoint(Vec2(0.5, 0));
				mArcherMonsters.at(i)->setState(ArcherMonster::STATE_MOVE);
				mArcherMonsters.at(i)->getPhysicsFoot()->setVelocity({ mPlayer->getPosition().x
					- mArcherMonsters.at(i)->getPosition().x
					+ mPlayer->getContentSize().width - 3,
					mPlayer->getPosition().y
					- mArcherMonsters.at(i)->getPosition().y });
				mArcherMonsters.at(i)->getPhysicsFoot()->setVelocityLimit(30);
				//mArcher->setFlip(mPlayer->getmIsMcLeft());
			}
			else if (mArcherMonsters.at(i)->getCurrent_Blood() <= 0)
			{
				mArcherMonsters.at(i)->getPhysicsFoot()->setVelocity({ 0, 0 });
				mArcherMonsters.at(i)->setState(ArcherMonster::STATE_DIE);
				mPlayer->setCurrent_Exp(mPlayer->getCurrent_Exp() + mArcherMonsters.at(i)->getCurrent_Exp());
				mArcherMonsters.at(i)->setAlive(false);

				SpriteFrameCache* cache = SpriteFrameCache::sharedSpriteFrameCache();
				cache->addSpriteFramesWithFile("ItemShop/Coin$.plist", "ItemShop/Coin$.png");
				auto gold = Sprite::createWithSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("coin_2.png"));
				gold->setScale(0.3);
				auto physicsBody2D = cocos2d::PhysicsBody::createBox(Size(gold->getContentSize()*0.3), PhysicsMaterial(10000.0f, 0.0f, 1.0f));
				physicsBody2D->setMass(50);
				physicsBody2D->setCollisionBitmask(Model::BITMASK_GOLD);
				physicsBody2D->setContactTestBitmask(true);
				physicsBody2D->setDynamic(false);
				physicsBody2D->setRotationEnable(false);
				gold->setPhysicsBody(physicsBody2D);
				gold->setPosition(mArcherMonsters.at(i)->getPosition());
				Vector<SpriteFrame*> animFrames;
				char str[100] = { 0 };
				animFrames.clear();
				for (int i = 2; i <= 27; ++i)
				{
					sprintf(str, "coin_%d.png", i);
					SpriteFrame* frame = cache->spriteFrameByName(str);
					animFrames.pushBack(frame);
				}
				gold->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.05f))));
				addChild(gold);
			}
			else if (distanceX <= 65 && distanceY <= 70 && (mArcherMonsters.at(i)->getCurrentState() != ArcherMonster::STATE_DIE))
			{
				mArcherMonsters.at(i)->getSprite()->setAnchorPoint(Vec2(0.3, 0));
				mArcherMonsters.at(i)->getPhysicsFoot()->setVelocity({ 0, 0 });
				mArcherMonsters.at(i)->setState(ArcherMonster::STATE_ATTACK);
				
			}
			else if (mPlayer->getCurrentState() != ArcherMonster::STATE_ATTACK || mPlayer->getCurrentState() != ArcherMonster::STATE_DIE
				|| mPlayer->getCurrentState() != ArcherMonster::STATE_FIRE_1 || mPlayer->getCurrentState() != ArcherMonster::STATE_MOVE)
			{
				mArcherMonsters.at(i)->getSprite()->setAnchorPoint(Vec2(0.5, 0));
				mArcherMonsters.at(i)->setState(ArcherMonster::STATE_IDLE);
			}
			if (mPlayer->getPosition().y > mArcherMonsters.at(i)->getSprite()->getPosition().y - 20)
			{
				mArcherMonsters.at(i)->getSprite()->setGlobalZOrder(5);
				mArcherMonsters.at(i)->getSprite()->setGlobalZOrder(4);
				//CCLOG("cao hon");
			}
			else if (mPlayer->getPosition().y < mArcherMonsters.at(i)->getSprite()->getPosition().y)
			{
				mArcherMonsters.at(i)->getSprite()->setGlobalZOrder(3);
				mArcherMonsters.at(i)->getSprite()->setGlobalZOrder(4);
				//CCLOG("thap hon");
			}
		}
		mArcherMonsters.at(i)->update(mPlayer->getPosition().x < mArcherMonsters.at(i)->getPosition().x);
	}
}


void InGameScene::updateBossMonsters()
{
	for (int i = 0; i < mBossMonsters.size(); i++)
	{
		if (mBossMonsters.at(i)->isAlive())
		{
			if (mBossMonsters.at(i)->getCurrent_Blood() <= 0)
			{
				mBossMonsters.at(i)->setState(BossMonster::STATE_DIE);
				mPlayer->setCurrent_Exp(mPlayer->getCurrent_Exp() + mBossMonsters.at(i)->getCurrent_Exp());
				mBossMonsters.at(i)->setAlive(false);

				auto gold = Sprite::create("ItemShop/weapons_excalibur_sword.png");
				gold->setScale(0.2);
				auto physicsBody2D = cocos2d::PhysicsBody::createBox(Size(gold->getContentSize()*0.3), PhysicsMaterial(10000.0f, 0.0f, 1.0f));
				physicsBody2D->setMass(50);
				physicsBody2D->setCollisionBitmask(Model::BITMASK_ITEM);
				physicsBody2D->setContactTestBitmask(true);
				physicsBody2D->setDynamic(false);
				physicsBody2D->setRotationEnable(false);
				gold->setPhysicsBody(physicsBody2D);
				gold->setPosition(mBossMonsters.at(i)->getPosition());
				auto rotate1 = RotateBy::create(1, 90);
				auto rotate2 = RotateBy::create(1, -90);
				/*auto moveby1 = MoveBy::create(1, Vec2(0, 15));
				auto moveby2 = MoveBy::create(1, Vec2(0, -15));*/
				gold->runAction(RepeatForever::create(/*Spawn::create(*/Sequence::create(rotate1, rotate2, nullptr)
					/*Sequence::create(moveby1, moveby2, nullptr), nullptr))*/));
				addChild(gold);

				auto text = Label::createWithTTF("Boss was beated", "fonts/Marker Felt.ttf", 20);
				text->setColor(Color3B(233, 0, 41));
				Vec2 v = mBossMonsters.at(i)->getPosition();
				text->setPosition(Vec2(v.x, v.y + mBossMonsters.at(i)->getSprite()->getContentSize().height*0.85 + 15));
				addChild(text);
				auto moveBy = MoveBy::create(2, Vec2(0, 20));
				auto fadeOut = FadeOut::create(1);
				auto spa = Spawn::create(moveBy, fadeOut, nullptr);
				text->runAction(spa);
			}
			//xu ly toa do 
			if (mPlayer->getPosition().y > mBossMonsters.at(i)->getSprite()->getPosition().y - 20)
			{
				mBossMonsters.at(i)->getSprite()->setGlobalZOrder(5);
				mPlayer->getSprite()->setGlobalZOrder(4);
			}
			else if (mPlayer->getPosition().y < mBossMonsters.at(i)->getSprite()->getPosition().y)
			{
				mBossMonsters.at(i)->getSprite()->setGlobalZOrder(3);
				mPlayer->getSprite()->setGlobalZOrder(4);

			}
			/*------------Monster Move------------------*/
			//Tinh khoang cach giua player voi CowMonster
			auto distance = ccpDistance(Vec2(mBossMonsters.at(i)->getPhysicsBody()->getPosition()), Vec2(mPlayer->getPhysicsBody()->getPosition()));
			//neu distance <= 200 va khong trong trang thay bi danh thi se phat hien ra player
			if (distance <= 200)
			{
				mBossMonsters.at(i)->setDetect(true);
			}
			else if (distance > 200)
			{
				mBossMonsters.at(i)->setDetect(false);
			}
			//log("%b", mCowMonsters.at(i)->getDetect());
			if (mBossMonsters.at(i)->getDetect() == true && mBossMonsters.at(i)->getCurrentState() != BossMonster::STATE_DAMAGE
				&& mBossMonsters.at(i)->getCurrentState() != BossMonster::STATE_ATTACK
				&& mBossMonsters.at(i)->getCurrentState() != BossMonster::STATE_ATTACK_1
				&& mBossMonsters.at(i)->getCurrentState() != BossMonster::STATE_DIE)
			{
				//CowMonster huong mat va di theo vi tri cua player
				//trai
				if ((mPlayer->getPosition().x < mBossMonsters.at(i)->getPosition().x))
				{
					mBossMonsters.at(i)->setState(BossMonster::STATE_MOVE);
					mBossMonsters.at(i)->getPhysicsFoot()->setVelocity({ mPlayer->getPosition().x
						- mBossMonsters.at(i)->getPosition().x
						+ mPlayer->getContentSize().width - 3,
						mPlayer->getPosition().y
						- mBossMonsters.at(i)->getPosition().y });
					mBossMonsters.at(i)->getPhysicsFoot()->setVelocityLimit(30);
					mBossMonsters.at(i)->setFlip(false);
				}

				//phai
				if (mPlayer->getPosition().x > mBossMonsters.at(i)->getPosition().x)
				{
					mBossMonsters.at(i)->setState(BossMonster::STATE_MOVE);
					mBossMonsters.at(i)->getPhysicsFoot()->setVelocity({ mPlayer->getPosition().x
						- mBossMonsters.at(i)->getPosition().x
						- mPlayer->getContentSize().width + 3,
						mPlayer->getPosition().y
						- mBossMonsters.at(i)->getPosition().y });
					mBossMonsters.at(i)->setFlip(true);
				}
			}
			//neu ra ngoai pham vi phat hien thi CowMonster se dung yen
			if (mBossMonsters.at(i)->getDetect() == false && mBossMonsters.at(i)->getCurrentState() != BossMonster::STATE_DIE)
			{
				mBossMonsters.at(i)->getPhysicsFoot()->setVelocity({ 0, 0 });
				mBossMonsters.at(i)->setState(BossMonster::STATE_IDLE);
			}
			/*-------------Monster Attack----------------*/
			//neu nhan vat trong tam danh cua CowMonster thi CowMonster se phat huy don danh
			if (distance <= 90 && distance >= -90 && mBossMonsters.at(i)->getCurrentState() != BossMonster::STATE_DAMAGE
				&& mBossMonsters.at(i)->getCurrentState() != BossMonster::STATE_DIE)
			{
				mBossMonsters.at(i)->getPhysicsFoot()->setVelocity({ 0, 0 });
				if ((mBossMonsters.at(i)->getCurrent_Blood() * 100 / mBossMonsters.at(i)->getBlood()) > 51)
				{
					mBossMonsters.at(i)->setState(BossMonster::STATE_ATTACK);
				}
				else if ((mBossMonsters.at(i)->getCurrent_Blood() * 100 / mBossMonsters.at(i)->getBlood()) <= 50
					&& (mBossMonsters.at(i)->getCurrent_Blood() * 100 / mBossMonsters.at(i)->getBlood()) > 1)
				{
					mBossMonsters.at(i)->setState(BossMonster::STATE_ATTACK_1);
				}
			}
			//neu Cowmonster khong phai trang thai Attack thi huy box attack
			if (mBossMonsters.at(i)->getCurrentState() != BossMonster::STATE_ATTACK
				&& mBossMonsters.at(i)->getCurrentState() != BossMonster::STATE_ATTACK_1
				&& mBossMonsters.at(i)->getCurrentState() != BossMonster::STATE_DIE)
			{
				mBossMonsters.at(i)->getPhysicsAttack()->setEnabled(false);
			}

			if (mBossMonsters.at(i)->getCurrentState() == BossMonster::STATE_DIE)
			{
				mBossMonsters.at(i)->getPhysicsFoot()->setVelocity({ 0, 0 });
			}
		}
		mBossMonsters.at(i)->update();
	}
}

void InGameScene::updateDogMonsters()
{
	for (int i = 0; i < mDogMonsters.size(); i++)
	{
		if (mDogMonsters.at(i)->isAlive())
		{
			if (mDogMonsters.at(i)->getCurrent_Blood() <= 0)
			{
				mDogMonsters.at(i)->setState(DogMonster::STATE_DIE);
				mPlayer->setCurrent_Exp(mPlayer->getCurrent_Exp() + mDogMonsters.at(i)->getCurrent_Exp());
				mDogMonsters.at(i)->setAlive(false);

				SpriteFrameCache* cache = SpriteFrameCache::sharedSpriteFrameCache();
				cache->addSpriteFramesWithFile("ItemShop/Coin$.plist", "ItemShop/Coin$.png");
				auto gold = Sprite::createWithSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("coin_2.png"));
				gold->setScale(0.3);
				auto physicsBody2D = cocos2d::PhysicsBody::createBox(Size(gold->getContentSize()*0.3), PhysicsMaterial(10000.0f, 0.0f, 1.0f));
				physicsBody2D->setMass(50);
				physicsBody2D->setCollisionBitmask(Model::BITMASK_GOLD);
				physicsBody2D->setContactTestBitmask(true);
				physicsBody2D->setDynamic(false);
				physicsBody2D->setRotationEnable(false);
				gold->setPhysicsBody(physicsBody2D);
				gold->setPosition(mDogMonsters.at(i)->getPosition());
				Vector<SpriteFrame*> animFrames;
				char str[100] = { 0 };
				animFrames.clear();
				for (int i = 2; i <= 27; ++i)
				{
					sprintf(str, "coin_%d.png", i);
					SpriteFrame* frame = cache->spriteFrameByName(str);
					animFrames.pushBack(frame);
				}
				gold->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(animFrames, 0.05f))));
				addChild(gold);
			}
			//xu ly toa do 
			if (mPlayer->getPosition().y > mDogMonsters.at(i)->getSprite()->getPosition().y - 20)
			{
				mDogMonsters.at(i)->getSprite()->setGlobalZOrder(5);
				mPlayer->getSprite()->setGlobalZOrder(4);
			}
			else if (mPlayer->getPosition().y < mDogMonsters.at(i)->getSprite()->getPosition().y)
			{
				mDogMonsters.at(i)->getSprite()->setGlobalZOrder(3);
				mPlayer->getSprite()->setGlobalZOrder(4);

			}
			/*------------Monster Move------------------*/
			//Tinh khoang cach giua player voi CowMonster
			auto distance = ccpDistance(Vec2(mDogMonsters.at(i)->getPhysicsBody()->getPosition()), Vec2(mPlayer->getPhysicsBody()->getPosition()));
			//neu distance <= 200 va khong trong trang thay bi danh thi se phat hien ra player
			if (distance <= 200)
			{
				mDogMonsters.at(i)->setDetect(true);
			}
			else if (distance > 200)
			{
				mDogMonsters.at(i)->setDetect(false);
			}
			//log("%b", mCowMonsters.at(i)->getDetect());
			if (mDogMonsters.at(i)->getDetect() == true && mDogMonsters.at(i)->getCurrentState() != DogMonster::STATE_DAMAGE
				&& mDogMonsters.at(i)->getCurrentState() != DogMonster::STATE_ATTACK
				&& mDogMonsters.at(i)->getCurrentState() != DogMonster::STATE_DIE)
			{
				//CowMonster huong mat va di theo vi tri cua player
				//trai
				if ((mPlayer->getPosition().x < mDogMonsters.at(i)->getPosition().x))
				{
					mDogMonsters.at(i)->setState(DogMonster::STATE_MOVE);
					mDogMonsters.at(i)->getPhysicsFoot()->setVelocity({ mPlayer->getPosition().x
						- mDogMonsters.at(i)->getPosition().x
						+ mPlayer->getContentSize().width - 1,
						mPlayer->getPosition().y
						- mDogMonsters.at(i)->getPosition().y });
					mDogMonsters.at(i)->getPhysicsFoot()->setVelocityLimit(30);
					mDogMonsters.at(i)->setFlip(true);
				}

				//phai
				if (mPlayer->getPosition().x > mDogMonsters.at(i)->getPosition().x)
				{
					mDogMonsters.at(i)->setState(DogMonster::STATE_MOVE);
					mDogMonsters.at(i)->getPhysicsFoot()->setVelocity({ mPlayer->getPosition().x
						- mDogMonsters.at(i)->getPosition().x
						- mPlayer->getContentSize().width + 1,
						mPlayer->getPosition().y
						- mDogMonsters.at(i)->getPosition().y });
					mDogMonsters.at(i)->setFlip(false);
				}
			}
			//neu ra ngoai pham vi phat hien thi CowMonster se dung yen
			if (mDogMonsters.at(i)->getDetect() == false && mDogMonsters.at(i)->getCurrentState() != DogMonster::STATE_DIE)
			{
				mDogMonsters.at(i)->getPhysicsFoot()->setVelocity({ 0, 0 });
				mDogMonsters.at(i)->setState(DogMonster::STATE_IDLE);

			}
			/*-------------Monster Attack----------------*/
			//neu nhan vat trong tam danh cua CowMonster thi CowMonster se phat huy don danh
			if (distance <= 62 && distance >= -62 && mDogMonsters.at(i)->getCurrentState() != DogMonster::STATE_DAMAGE
				&& mDogMonsters.at(i)->getCurrentState() != DogMonster::STATE_DIE)
			{
				mDogMonsters.at(i)->getPhysicsFoot()->setVelocity({ 0, 0 });
				mDogMonsters.at(i)->setState(DogMonster::STATE_ATTACK);
			}
			//neu Cowmonster khong phai trang thai Attack thi huy box attack
			if (mDogMonsters.at(i)->getCurrentState() == DogMonster::STATE_DIE)
			{
				mDogMonsters.at(i)->getPhysicsFoot()->setVelocity({ 0, 0 });
			}
		}
		mDogMonsters.at(i)->update();
	}
}

bool InGameScene::onContactBegin(PhysicsContact& contact)
{
	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();

	//foot monster with body player
	if ((a->getCollisionBitmask() == Model::BITMASK_PLAYER &&b->getCollisionBitmask() == Model::BITMASK_MONSTER_FOOT)
		|| (a->getCollisionBitmask() == Model::BITMASK_MONSTER_FOOT&&b->getCollisionBitmask() == Model::BITMASK_PLAYER))
	{
		return false;
	}
	//foot player with body monster
	if ((a->getCollisionBitmask() == Model::BITMASK_PLAYER_FOOT && b->getCollisionBitmask() == Model::BITMASK_COW_MONSTER)
		|| (a->getCollisionBitmask() == Model::BITMASK_COW_MONSTER && b->getCollisionBitmask() == Model::BITMASK_PLAYER_FOOT))
	{
		return false;
	}
	if ((a->getCollisionBitmask() == Model::BITMASK_PLAYER_FOOT && b->getCollisionBitmask() == Model::BITMASK_ARCHER_MONSTER)
		|| (a->getCollisionBitmask() == Model::BITMASK_ARCHER_MONSTER && b->getCollisionBitmask() == Model::BITMASK_PLAYER_FOOT))
	{
		return false;
	}
	if ((a->getCollisionBitmask() == Model::BITMASK_PLAYER_FOOT && b->getCollisionBitmask() == Model::BITMASK_DOG_ATTACK)
		|| (a->getCollisionBitmask() == Model::BITMASK_DOG_ATTACK && b->getCollisionBitmask() == Model::BITMASK_PLAYER_FOOT))
	{
		return false;
	}
	if ((a->getCollisionBitmask() == Model::BITMASK_PLAYER_FOOT && b->getCollisionBitmask() == Model::BITMASK_BOSS_ATTACK)
		|| (a->getCollisionBitmask() == Model::BITMASK_BOSS_ATTACK && b->getCollisionBitmask() == Model::BITMASK_PLAYER_FOOT))
	{
		return false;
	}
	//foot monster with body monster
	if ((a->getCollisionBitmask() == Model::BITMASK_COW_MONSTER && b->getCollisionBitmask() == Model::BITMASK_MONSTER_FOOT)
		|| (a->getCollisionBitmask() == Model::BITMASK_MONSTER_FOOT && b->getCollisionBitmask() == Model::BITMASK_COW_MONSTER))
		return false;
	if ((a->getCollisionBitmask() == Model::BITMASK_ARCHER_MONSTER && b->getCollisionBitmask() == Model::BITMASK_MONSTER_FOOT)
		|| (a->getCollisionBitmask() == Model::BITMASK_MONSTER_FOOT && b->getCollisionBitmask() == Model::BITMASK_ARCHER_MONSTER))
		return false;
	if ((a->getCollisionBitmask() == Model::BITMASK_DOG_MONSTER && b->getCollisionBitmask() == Model::BITMASK_MONSTER_FOOT)
		|| (a->getCollisionBitmask() == Model::BITMASK_MONSTER_FOOT && b->getCollisionBitmask() == Model::BITMASK_DOG_MONSTER))
		return false;
	if ((a->getCollisionBitmask() == Model::BITMASK_BOSS_MONSTER && b->getCollisionBitmask() == Model::BITMASK_MONSTER_FOOT)
		|| (a->getCollisionBitmask() == Model::BITMASK_MONSTER_FOOT && b->getCollisionBitmask() == Model::BITMASK_BOSS_MONSTER))
		return false;
	if ((a->getCollisionBitmask() == Model::BITMASK_PLAYER && b->getCollisionBitmask() == Model::BITMASK_PLAYER_FOOT)
		|| (a->getCollisionBitmask() == Model::BITMASK_PLAYER_FOOT && b->getCollisionBitmask() == Model::BITMASK_PLAYER))
		return false;

	//bullet with foot
	if ((a->getCollisionBitmask() == Model::BITMASK_PLAYER_ATTACK && b->getCollisionBitmask() == Model::BITMASK_MONSTER_FOOT)
		|| (a->getCollisionBitmask() == Model::BITMASK_MONSTER_FOOT && b->getCollisionBitmask() == Model::BITMASK_PLAYER_ATTACK))
		return false;
	if ((a->getCollisionBitmask() == Model::BITMASK_MONSTER_ATTACK && b->getCollisionBitmask() == Model::BITMASK_PLAYER_FOOT)
		|| (a->getCollisionBitmask() == Model::BITMASK_PLAYER_FOOT && b->getCollisionBitmask() == Model::BITMASK_MONSTER_ATTACK))
	{
		return false;
	}

	//CowMonster attack with monster foot
	if ((a->getCollisionBitmask() == Model::BITMASK_MONSTER_ATTACK && b->getCollisionBitmask() == Model::BITMASK_COW_MONSTER
		|| a->getCollisionBitmask() == Model::BITMASK_COW_MONSTER && b->getCollisionBitmask() == Model::BITMASK_MONSTER_FOOT))
	{
		return false;
	}


	if ((a->getCollisionBitmask() == Model::BITMASK_PLAYER && b->getCollisionBitmask() == Model::BITMASK_COWMONSTER_ATTACK)
		|| (a->getCollisionBitmask() == Model::BITMASK_COWMONSTER_ATTACK && b->getCollisionBitmask() == Model::BITMASK_PLAYER))
	{
		if (a->getCollisionBitmask() == Model::BITMASK_PLAYER)
		{
			//mPlayer->setState(Player::STATE_DAMAGE);
			if (b->isEnabled() == true)
			{
				if (((mCowMonsters.at(b->getGroup())->getDamage() + 10) - mPlayer->getArmor() * 0.35) > 0)
				{
					mPlayer->setCurrent_Blood(mPlayer->getCurrent_Blood() - ((mCowMonsters.at(b->getGroup())->getDamage() + 10) - mPlayer->getArmor() * 0.35));
				}
				else if (((mCowMonsters.at(b->getGroup())->getDamage() + 10) - mPlayer->getArmor() * 0.35) <= 0)
				{
					mPlayer->setCurrent_Blood(mPlayer->getCurrent_Blood() - 1);
				}
			}
			b->setEnabled(false);
		}
		if (b->getCollisionBitmask() == Model::BITMASK_PLAYER)
		{
			//mPlayer->setState(Player::STATE_DAMAGE);
			if (a->isEnabled() == true)
			{
				if (((mCowMonsters.at(a->getGroup())->getDamage() + 10) - mPlayer->getArmor() * 0.35) > 0)
				{
					mPlayer->setCurrent_Blood(mPlayer->getCurrent_Blood() - ((mCowMonsters.at(a->getGroup())->getDamage() + 10) - mPlayer->getArmor() * 0.35));
				}
				else if (((mCowMonsters.at(a->getGroup())->getDamage() + 10) - mPlayer->getArmor() * 0.35) <= 0)
				{
					mPlayer->setCurrent_Blood(mPlayer->getCurrent_Blood() - 1);
				}
			}
			a->setEnabled(false);
		}
	}
	if ((a->getCollisionBitmask() == Model::BITMASK_PLAYER && b->getCollisionBitmask() == Model::BITMASK_ARCHER_ARROW)
		|| (a->getCollisionBitmask() == Model::BITMASK_ARCHER_ARROW && b->getCollisionBitmask() == Model::BITMASK_PLAYER))
	{
		if (a->getCollisionBitmask() == Model::BITMASK_PLAYER)
		{
			//mPlayer->setState(Player::STATE_DAMAGE);
			if (b->isEnabled() == true)
			{
				if (((mArcherMonsters.at(b->getGroup())->getDamage() + 10) - mPlayer->getArmor() * 0.35) > 0)
				{
					mPlayer->setCurrent_Blood(mPlayer->getCurrent_Blood() - ((mArcherMonsters.at(b->getGroup())->getDamage() + 10) - mPlayer->getArmor() * 0.35));
				}
				else if (((mArcherMonsters.at(b->getGroup())->getDamage() + 10) - mPlayer->getArmor() * 0.35) <= 0)
				{
					mPlayer->setCurrent_Blood(mPlayer->getCurrent_Blood() - 1);
				}
			}
			b->setEnabled(false);
			b->getNode()->setVisible(false);
			//mArrow.at(b->getGroup())->getSprite()->setVisible(false);
			CCLOG("%d", mPlayer->getCurrent_Blood());
		}
		if (b->getCollisionBitmask() == Model::BITMASK_PLAYER)
		{
			//mPlayer->setState(Player::STATE_DAMAGE);
			if (((mArcherMonsters.at(a->getGroup())->getDamage() + 10) - mPlayer->getArmor() * 0.35) > 0)
			{
				mPlayer->setCurrent_Blood(mPlayer->getCurrent_Blood() - ((mArcherMonsters.at(a->getGroup())->getDamage() + 10) - mPlayer->getArmor() * 0.35));
			}
			else if (((mArcherMonsters.at(a->getGroup())->getDamage() + 10) - mPlayer->getArmor() * 0.35) <= 0)
			{
				mPlayer->setCurrent_Blood(mPlayer->getCurrent_Blood() - 1);
			}
			a->setEnabled(false);
			a->getNode()->setVisible(false);
			//mArrow.at(a->getGroup())->getSprite()->setVisible(false);
			CCLOG("%d", mPlayer->getCurrent_Blood());
		}
	}
	if ((a->getCollisionBitmask() == Model::BITMASK_PLAYER && b->getCollisionBitmask() == Model::BITMASK_ARCHER_ATTACK)
		|| (a->getCollisionBitmask() == Model::BITMASK_ARCHER_ATTACK && b->getCollisionBitmask() == Model::BITMASK_PLAYER))
	{
		if (a->getCollisionBitmask() == Model::BITMASK_PLAYER)
		{
			//mPlayer->setState(Player::STATE_DAMAGE);
			if (b->isEnabled() == true)
			{
				if (((mArcherMonsters.at(b->getGroup())->getDamage()) - mPlayer->getArmor() * 0.35) > 0)
				{
					mPlayer->setCurrent_Blood(mPlayer->getCurrent_Blood() - ((mArcherMonsters.at(b->getGroup())->getDamage()) - mPlayer->getArmor() * 0.35));
				}
				else if (((mArcherMonsters.at(b->getGroup())->getDamage()) - mPlayer->getArmor() * 0.35) <= 0)
				{
					mPlayer->setCurrent_Blood(mPlayer->getCurrent_Blood() - 1);
				}
			}
			b->setEnabled(false);
		}
		if (b->getCollisionBitmask() == Model::BITMASK_PLAYER)
		{
			//mPlayer->setState(Player::STATE_DAMAGE);
			if (a->isEnabled() == true)
			{
				if (((mArcherMonsters.at(a->getGroup())->getDamage()) - mPlayer->getArmor() * 0.35) > 0)
				{
					mPlayer->setCurrent_Blood(mPlayer->getCurrent_Blood() - ((mArcherMonsters.at(a->getGroup())->getDamage()) - mPlayer->getArmor() * 0.35));
				}
				else if (((mArcherMonsters.at(a->getGroup())->getDamage()) - mPlayer->getArmor() * 0.35) <= 0)
				{
					mPlayer->setCurrent_Blood(mPlayer->getCurrent_Blood() - 1);
				}
			}
			a->setEnabled(false);
		}
	}
	if ((a->getCollisionBitmask() == Model::BITMASK_PLAYER && b->getCollisionBitmask() == Model::BITMASK_BOSS_ATTACK)
		|| (a->getCollisionBitmask() == Model::BITMASK_BOSS_ATTACK && b->getCollisionBitmask() == Model::BITMASK_PLAYER))
	{
		if (a->getCollisionBitmask() == Model::BITMASK_PLAYER)
		{
			//mPlayer->setState(Player::STATE_DAMAGE);
			if (b->isEnabled() == true)
			{
				if ((mBossMonsters.at(b->getGroup())->getDamage() - mPlayer->getArmor() * 0.35) > 0)
				{
					mPlayer->setCurrent_Blood(mPlayer->getCurrent_Blood() - (mBossMonsters.at(b->getGroup())->getDamage() - mPlayer->getArmor() * 0.35));
				}
				else if ((mBossMonsters.at(b->getGroup())->getDamage() - mPlayer->getArmor() * 0.35) <= 0)
				{
					mPlayer->setCurrent_Blood(mPlayer->getCurrent_Blood() - 1);
				}
			}
			b->setEnabled(false);
			CCLOG("%d", mPlayer->getCurrent_Blood());
		}
		if (b->getCollisionBitmask() == Model::BITMASK_PLAYER)
		{
			//mPlayer->setState(Player::STATE_DAMAGE);
			if (a->isEnabled() == true)
			{
				if ((mBossMonsters.at(a->getGroup())->getDamage() - mPlayer->getArmor() * 0.35) > 0)
				{
					mPlayer->setCurrent_Blood(mPlayer->getCurrent_Blood() - (mBossMonsters.at(a->getGroup())->getDamage() - mPlayer->getArmor() * 0.35));
				}
				else if ((mBossMonsters.at(a->getGroup())->getDamage() - mPlayer->getArmor() * 0.35) <= 0)
				{
					mPlayer->setCurrent_Blood(mPlayer->getCurrent_Blood() - 1);
				}
			}
			a->setEnabled(false);
			CCLOG("%d", mPlayer->getCurrent_Blood());
		}
	}

	if ((a->getCollisionBitmask() == Model::BITMASK_PLAYER && b->getCollisionBitmask() == Model::BITMASK_DOG_ATTACK)
		|| (a->getCollisionBitmask() == Model::BITMASK_DOG_ATTACK && b->getCollisionBitmask() == Model::BITMASK_PLAYER))
	{
		if (a->getCollisionBitmask() == Model::BITMASK_PLAYER)
		{
			//mPlayer->setState(Player::STATE_DAMAGE);
			if (b->isEnabled() == true)
			{
				if ((mDogMonsters.at(b->getGroup())->getDamage() - mPlayer->getArmor() * 0.35) > 0)
				{
					mPlayer->setCurrent_Blood(mPlayer->getCurrent_Blood() - (mDogMonsters.at(b->getGroup())->getDamage() - mPlayer->getArmor() * 0.35));
				}
				else if ((mDogMonsters.at(b->getGroup())->getDamage() - mPlayer->getArmor() * 0.35) <= 0)
				{
					mPlayer->setCurrent_Blood(mPlayer->getCurrent_Blood() - 1);
				}
			}
			b->setEnabled(false);
			CCLOG("%d", mPlayer->getCurrent_Blood());
		}
		if (b->getCollisionBitmask() == Model::BITMASK_PLAYER)
		{
			//mPlayer->setState(Player::STATE_DAMAGE);
			if (a->isEnabled() == true)
			{
				if ((mDogMonsters.at(a->getGroup())->getDamage() - mPlayer->getArmor() * 0.35) > 0)
				{
					mPlayer->setCurrent_Blood(mPlayer->getCurrent_Blood() - (mDogMonsters.at(a->getGroup())->getDamage() - mPlayer->getArmor() * 0.35));
				}
				else if ((mDogMonsters.at(a->getGroup())->getDamage() - mPlayer->getArmor() * 0.35) <= 0)
				{
					mPlayer->setCurrent_Blood(mPlayer->getCurrent_Blood() - 1);
				}
			}
			a->setEnabled(false);
			CCLOG("%d", mPlayer->getCurrent_Blood());
		}
	}
	////Cow vs Player Attack 
	if ((a->getCollisionBitmask() == Model::BITMASK_COW_MONSTER && b->getCollisionBitmask() == Model::BITMASK_PLAYER_ATTACK)
		|| (a->getCollisionBitmask() == Model::BITMASK_PLAYER_ATTACK && b->getCollisionBitmask() == Model::BITMASK_COW_MONSTER))
	{

		auto emitter = ParticleSystemQuad::create("Particle/particle_texture.plist");

		if (a->getCollisionBitmask() == Model::BITMASK_COW_MONSTER)
		{
			mCowMonsters.at(a->getGroup())->setState(CowMonster::STATE_DAMAGE);
			if (b->isEnabled() == true)
			{
				mCowMonsters.at(a->getGroup())->setCurrent_Blood(mCowMonsters.at(a->getGroup())->getCurrent_Blood() - (mPlayer->getDamage() - mCowMonsters.at(a->getGroup())->getArmor() * 0.35));
			}
			emitter->setPosition(mCowMonsters.at(a->getGroup())->getPosition());
			ostringstream ST1;
			ST1 << mPlayer->getDamage() - mCowMonsters.at(a->getGroup())->getArmor() * 0.35;
			auto text = Label::createWithTTF("- " + ST1.str(), "fonts/Marker Felt.ttf", 12);
			text->setColor(Color3B(233, 0, 41));
			Vec2 v = mCowMonsters.at(a->getGroup())->getPosition();
			text->setPosition(Vec2(v.x, v.y + mCowMonsters.at(a->getGroup())->getSprite()->getContentSize().height*0.8 + 15));
			addChild(text);
			auto moveBy = MoveBy::create(1, Vec2(0, 15));
			auto fadeOut = FadeOut::create(1);
			auto spa = Spawn::create(moveBy, fadeOut, nullptr);
			text->runAction(spa);
		}
		if (b->getCollisionBitmask() == Model::BITMASK_COW_MONSTER)
		{
			mCowMonsters.at(b->getGroup())->setState(CowMonster::STATE_DAMAGE);
			if (a->isEnabled() == true)
			{
				mCowMonsters.at(b->getGroup())->setCurrent_Blood(mCowMonsters.at(b->getGroup())->getCurrent_Blood() - (mPlayer->getDamage() - mCowMonsters.at(b->getGroup())->getArmor() * 0.35));
			}
			emitter->setPosition(mCowMonsters.at(b->getGroup())->getPosition());
			//Display blood, exp
			ostringstream ST2;
			ST2 << mPlayer->getDamage() - mCowMonsters.at(b->getGroup())->getArmor() * 0.35;
			auto text = Label::createWithTTF("- " + ST2.str(), "fonts/Marker Felt.ttf", 18);
			text->setColor(Color3B(233, 0, 41));
			Vec2 v = mCowMonsters.at(b->getGroup())->getPosition();
			text->setPosition(Vec2(v.x, v.y + mCowMonsters.at(b->getGroup())->getSprite()->getContentSize().height*0.8 + 15));
			addChild(text);
			auto moveBy = MoveBy::create(1, Vec2(0, 15));
			auto fadeOut = FadeOut::create(1);
			auto blood = Spawn::create(moveBy, fadeOut, nullptr);
			text->runAction(blood);
		}
		addChild(emitter);
	}
	//Acher vs player attack
	if ((a->getCollisionBitmask() == Model::BITMASK_ARCHER_MONSTER && b->getCollisionBitmask() == Model::BITMASK_PLAYER_ATTACK)
		|| (a->getCollisionBitmask() == Model::BITMASK_PLAYER_ATTACK && b->getCollisionBitmask() == Model::BITMASK_ARCHER_MONSTER))
	{

		auto emitter = ParticleSystemQuad::create("Particle/particle_texture.plist");
		if (a->getCollisionBitmask() == Model::BITMASK_ARCHER_MONSTER)
		{
			//mArcher->setState(ArcherMonster::STATE_DAMAGE);
			if (b->isEnabled() == true)
			{
				mArcherMonsters.at(a->getGroup())->setCurrent_Blood(mArcherMonsters.at(a->getGroup())->getCurrent_Blood() - (mPlayer->getDamage() - mArcherMonsters.at(a->getGroup())->getArmor() * 0.35));
			}
			emitter->setPosition(mArcherMonsters.at(a->getGroup())->getPosition());
			ostringstream OS1;
			OS1 << mPlayer->getDamage() - mArcherMonsters.at(a->getGroup())->getArmor() * 0.35;
			auto text = Label::createWithTTF("- " + OS1.str(), "fonts/Marker Felt.ttf", 12);
			text->setColor(Color3B(233, 0, 41));
			Vec2 v = mArcherMonsters.at(a->getGroup())->getPosition();
			text->setPosition(Vec2(v.x, v.y + mArcherMonsters.at(a->getGroup())->getSprite()->getContentSize().height*0.85 + 15));
			addChild(text);
			auto moveBy = MoveBy::create(1, Vec2(0, 15));
			auto fadeOut = FadeOut::create(1);
			auto spa = Spawn::create(moveBy, fadeOut, nullptr);
			text->runAction(spa);
		}
		if (b->getCollisionBitmask() == Model::BITMASK_ARCHER_MONSTER)
		{
			//mArcher->setState(ArcherMonster::STATE_DAMAGE);
			if (a->isEnabled() == true)
			{
				mArcherMonsters.at(b->getGroup())->setCurrent_Blood(mArcherMonsters.at(b->getGroup())->getCurrent_Blood() - (mPlayer->getDamage() - mArcherMonsters.at(b->getGroup())->getArmor() * 0.35));
			}
			emitter->setPosition(mArcherMonsters.at(b->getGroup())->getPosition());
			ostringstream OS2;
			OS2 << mPlayer->getDamage() - mArcherMonsters.at(b->getGroup())->getArmor() * 0.35;
			auto text = Label::createWithTTF("- " + OS2.str(), "fonts/Marker Felt.ttf", 12);
			text->setColor(Color3B(233, 0, 41));
			Vec2 v = mArcherMonsters.at(b->getGroup())->getPosition();
			text->setPosition(Vec2(v.x, v.y + mArcherMonsters.at(b->getGroup())->getSprite()->getContentSize().height*0.85 + 15));
			addChild(text);
			auto moveBy = MoveBy::create(1, Vec2(0, 15));
			auto fadeOut = FadeOut::create(1);
			auto spa = Spawn::create(moveBy, fadeOut, nullptr);
			text->runAction(spa);
		}
		addChild(emitter);
	}

	//Dog vs player attack
	if ((a->getCollisionBitmask() == Model::BITMASK_DOG_MONSTER && b->getCollisionBitmask() == Model::BITMASK_PLAYER_ATTACK)
		|| (a->getCollisionBitmask() == Model::BITMASK_PLAYER_ATTACK && b->getCollisionBitmask() == Model::BITMASK_DOG_MONSTER))
	{

		auto emitter = ParticleSystemQuad::create("Particle/particle_texture.plist");
		if (a->getCollisionBitmask() == Model::BITMASK_DOG_MONSTER)
		{
			//mArcher->setState(ArcherMonster::STATE_DAMAGE);
			if (b->isEnabled() == true)
			{
				mDogMonsters.at(a->getGroup())->setCurrent_Blood(mDogMonsters.at(a->getGroup())->getCurrent_Blood() - (mPlayer->getDamage() - mDogMonsters.at(a->getGroup())->getArmor() * 0.35));
			}
			emitter->setPosition(mDogMonsters.at(a->getGroup())->getPosition());
			ostringstream OS1;
			OS1 << mPlayer->getDamage() - mDogMonsters.at(a->getGroup())->getArmor() * 0.35;
			auto text = Label::createWithTTF("- " + OS1.str(), "fonts/Marker Felt.ttf", 12);
			text->setColor(Color3B(233, 0, 41));
			Vec2 v = mDogMonsters.at(a->getGroup())->getPosition();
			text->setPosition(Vec2(v.x, v.y + mDogMonsters.at(a->getGroup())->getSprite()->getContentSize().height*0.85 + 15));
			addChild(text);
			auto moveBy = MoveBy::create(1, Vec2(0, 15));
			auto fadeOut = FadeOut::create(1);
			auto spa = Spawn::create(moveBy, fadeOut, nullptr);
			text->runAction(spa);
		}
		if (b->getCollisionBitmask() == Model::BITMASK_DOG_MONSTER)
		{
			//mArcher->setState(ArcherMonster::STATE_DAMAGE);
			if (a->isEnabled() == true)
			{
				mDogMonsters.at(b->getGroup())->setCurrent_Blood(mDogMonsters.at(b->getGroup())->getCurrent_Blood() - (mPlayer->getDamage() - mDogMonsters.at(b->getGroup())->getArmor() * 0.35));
			}
			emitter->setPosition(mDogMonsters.at(b->getGroup())->getPosition());
			ostringstream OS2;
			OS2 << mPlayer->getDamage() - mDogMonsters.at(b->getGroup())->getArmor() * 0.35;
			auto text = Label::createWithTTF("- " + OS2.str(), "fonts/Marker Felt.ttf", 12);
			text->setColor(Color3B(233, 0, 41));
			Vec2 v = mDogMonsters.at(b->getGroup())->getPosition();
			text->setPosition(Vec2(v.x, v.y + mDogMonsters.at(b->getGroup())->getSprite()->getContentSize().height*0.85 + 15));
			addChild(text);
			auto moveBy = MoveBy::create(1, Vec2(0, 15));
			auto fadeOut = FadeOut::create(1);
			auto spa = Spawn::create(moveBy, fadeOut, nullptr);
			text->runAction(spa);
		}
		addChild(emitter);
	}

	//Boss vs player attack
	if ((a->getCollisionBitmask() == Model::BITMASK_BOSS_MONSTER && b->getCollisionBitmask() == Model::BITMASK_PLAYER_ATTACK)
		|| (a->getCollisionBitmask() == Model::BITMASK_PLAYER_ATTACK && b->getCollisionBitmask() == Model::BITMASK_BOSS_MONSTER))
	{

		auto emitter = ParticleSystemQuad::create("Particle/particle_texture.plist");
		if (a->getCollisionBitmask() == Model::BITMASK_BOSS_MONSTER)
		{
			//mArcher->setState(ArcherMonster::STATE_DAMAGE);
			if (b->isEnabled() == true)
			{
				mBossMonsters.at(a->getGroup())->setCurrent_Blood(mBossMonsters.at(a->getGroup())->getCurrent_Blood() - (mPlayer->getDamage() - mBossMonsters.at(a->getGroup())->getArmor() * 0.35));
			}
			emitter->setPosition(mBossMonsters.at(a->getGroup())->getPosition());
			ostringstream OS1;
			OS1 << mPlayer->getDamage() - mBossMonsters.at(a->getGroup())->getArmor() * 0.35;
			auto text = Label::createWithTTF("- " + OS1.str(), "fonts/Marker Felt.ttf", 12);
			text->setColor(Color3B(233, 0, 41));
			Vec2 v = mBossMonsters.at(a->getGroup())->getPosition();
			text->setPosition(Vec2(v.x, v.y + mBossMonsters.at(a->getGroup())->getSprite()->getContentSize().height*0.85 + 15));
			addChild(text);
			auto moveBy = MoveBy::create(1, Vec2(0, 15));
			auto fadeOut = FadeOut::create(1);
			auto spa = Spawn::create(moveBy, fadeOut, nullptr);
			text->runAction(spa);
		}
		if (b->getCollisionBitmask() == Model::BITMASK_BOSS_MONSTER)
		{
			//mArcher->setState(ArcherMonster::STATE_DAMAGE);
			if (a->isEnabled() == true)
			{
				mBossMonsters.at(b->getGroup())->setCurrent_Blood(mBossMonsters.at(b->getGroup())->getCurrent_Blood() - (mPlayer->getDamage() - mBossMonsters.at(b->getGroup())->getArmor() * 0.35));
			}
			emitter->setPosition(mBossMonsters.at(b->getGroup())->getPosition());
			ostringstream OS2;
			OS2 << mPlayer->getDamage() - mBossMonsters.at(b->getGroup())->getArmor() * 0.35;
			auto text = Label::createWithTTF("- " + OS2.str(), "fonts/Marker Felt.ttf", 12);
			text->setColor(Color3B(233, 0, 41));
			Vec2 v = mBossMonsters.at(b->getGroup())->getPosition();
			text->setPosition(Vec2(v.x, v.y + mBossMonsters.at(b->getGroup())->getSprite()->getContentSize().height*0.85 + 15));
			addChild(text);
			auto moveBy = MoveBy::create(1, Vec2(0, 15));
			auto fadeOut = FadeOut::create(1);
			auto spa = Spawn::create(moveBy, fadeOut, nullptr);
			text->runAction(spa);
		}
		addChild(emitter);
	}

	if ((a->getCollisionBitmask() == Model::BITMASK_PLAYER && b->getCollisionBitmask() == Model::BITMASK_GOLD)
		|| (a->getCollisionBitmask() == Model::BITMASK_GOLD && b->getCollisionBitmask() == Model::BITMASK_PLAYER))
	{
		SimpleAudioEngine::getInstance()->playEffect("sounds/coin.wav");
		mPlayer->setGold(mPlayer->getGold() + 50);
		if (a->getCollisionBitmask() == Model::BITMASK_GOLD)
		{
			a->setEnabled(false);
			a->getOwner()->setVisible(false);

		}
		else
		{
			b->setEnabled(false);
			b->getOwner()->setVisible(false);
		}
	}

	if ((a->getCollisionBitmask() == Model::BITMASK_PLAYER && b->getCollisionBitmask() == Model::BITMASK_ITEM)
		|| (a->getCollisionBitmask() == Model::BITMASK_ITEM && b->getCollisionBitmask() == Model::BITMASK_PLAYER))
	{
		SimpleAudioEngine::getInstance()->playEffect("sounds/sword.wav");
		mPlayer->setDamage(mPlayer->getDamage() + 50);
		if (a->getCollisionBitmask() == Model::BITMASK_ITEM)
		{
			a->setEnabled(false);
			a->getOwner()->setVisible(false);
		}
		else
		{
			b->setEnabled(false);
			b->getOwner()->setVisible(false);
		}

		auto funcPause = CallFunc::create([]() {
			SimpleAudioEngine::getInstance()->pauseAllEffects();
			SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
			Director::getInstance()->pause();
			mControllerLayer->setVisible(false);
			mPauseLayer->setVisible(true);
			panel->setVisible(false);
			winPanel->setVisible(true);
		}); 
		auto fadeIn = FadeIn::create(0.3f);
		mPauseLayer->runAction(Sequence::create(fadeIn, funcPause, nullptr));
	}

	//
	if ((a->getCollisionBitmask() == Model::BITMASK_PLAYER && b->getCollisionBitmask() == Model::BITMASK_TELEPORT)
		|| (a->getCollisionBitmask() == Model::BITMASK_TELEPORT && b->getCollisionBitmask() == Model::BITMASK_PLAYER))
	{
		McSetting* mcSetting = McSetting::getInstance();
		mcSetting->setBloodPlayer(mPlayer->getBlood());
		mcSetting->setMana(mPlayer->getMana());
		mcSetting->setDamePlayer(mPlayer->getDamage());
		mcSetting->setArmor(mPlayer->getArmor());
		mcSetting->setEXP(mPlayer->getCurrent_Exp());
		mcSetting->setGold(mPlayer->getGold());
		mcSetting->setPlayerLevel(mPlayer->getLevel());
		mcSetting->saveData();
		///////////////////
		ostringstream OS;
		/*if (gMapNumber == 2)
		gMapNumber = 1;*/
		OS << gMapNumber + 1;
		gMapName = "World1_MapBattle" + OS.str() + ".tmx";
		//auto inGame = InGameScene::createScene();
		MapManager::getInstance()->setDone(true);
		auto map = SelectMapScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(1, map, Color3B(0, 255, 255)));
		this->unscheduleUpdate();
	}

	return true;
}

//Create blood for player
void InGameScene::createHud()
{
	float scaleHUD = 0.3;
	auto hud = Sprite::create("Hud/hud.png");
	hud->setAnchorPoint(Vec2(1, 0.5));
	hud->setPosition(Vec2(visibleSize.width / 3 - 30, visibleSize.height - 40));
	hud->setScale(scaleHUD);
	hud->setGlobalZOrder(7);
	ccBlendFunc blnd = { (int)GL_SRC_ALPHA, (int)GL_ONE_MINUS_SRC_ALPHA };
	mControllerLayer->addChild(hud);

	mMcHudBlood = ui::LoadingBar::create("Hud/hud_blood.png");
	mMcHudBlood->setAnchorPoint(Vec2(1, 0.5));
	mMcHudBlood->setPosition(hud->getPosition());
	mMcHudBlood->setDirection(ui::LoadingBar::Direction::LEFT);
	mMcHudBlood->setPercent(100);
	mMcHudBlood->setScale(scaleHUD);
	mMcHudBlood->setGlobalZOrder(7);
	mControllerLayer->addChild(mMcHudBlood);

	mMcHudMana = ui::LoadingBar::create("Hud/hud_mana.png");
	mMcHudMana->setAnchorPoint(Vec2(1, 0.5));
	mMcHudMana->setPosition(hud->getPosition());
	mMcHudMana->setDirection(ui::LoadingBar::Direction::LEFT);
	mMcHudMana->setPercent(100);
	mMcHudMana->setScale(scaleHUD);
	mMcHudMana->setGlobalZOrder(7);
	mControllerLayer->addChild(mMcHudMana);

	auto experienceBg = Sprite::create("Hud/experience.png");
	experienceBg->setAnchorPoint(Vec2(1, 0.5));
	experienceBg->setPosition(Vec2(hud->getPosition().x, hud->getPosition().y - 40));
	experienceBg->setScale(scaleHUD);
	experienceBg->setGlobalZOrder(7);
	mControllerLayer->addChild(experienceBg);

	mMcHudExperience = ui::LoadingBar::create("Hud/hud_experience.png");
	mMcHudExperience->setAnchorPoint(Vec2(1, 0.5));
	mMcHudExperience->setPosition(Vec2(hud->getPosition().x, hud->getPosition().y - 40));
	mMcHudExperience->setDirection(ui::LoadingBar::Direction::LEFT);
	mMcHudExperience->setPercent(0);
	mMcHudExperience->setScale(scaleHUD);
	mMcHudExperience->setGlobalZOrder(7);
	mControllerLayer->addChild(mMcHudExperience);


	auto levelPanel = Sprite::create("LevelPanel.png");
	levelPanel->setScale(0.3);
	levelPanel->setPosition(Vec2(mMcHudExperience->getPosition().x - mMcHudExperience->getContentSize().width*mMcHudExperience->getScale() - 20, mMcHudExperience->getPosition().y));
	levelPanel->setGlobalZOrder(7);
	mControllerLayer->addChild(levelPanel);

	levelLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 15);
	//levelLabel->setColor(Color3B(32, 90, 167));
	levelLabel->setPosition(levelPanel->getPosition());
	levelLabel->setGlobalZOrder(7);
	mControllerLayer->addChild(levelLabel);

	auto goldPanel = Sprite::create("GoldPanel.png");
	goldPanel->setScale(0.5);
	goldPanel->setOpacity(220);
	goldPanel->setPosition(Vec2(visibleSize.width / 2 - 50 + 70, visibleSize.height - 40));
	goldPanel->setGlobalZOrder(7);
	mControllerLayer->addChild(goldPanel);

	auto gold = Sprite::create("coinitem.png");
	gold->setScale(0.17);
	gold->setPosition(Vec2(goldPanel->getPosition().x - 20, goldPanel->getPosition().y));
	gold->setGlobalZOrder(7);
	mControllerLayer->addChild(gold);

	goldLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 18);
	goldLabel->setPosition(Vec2(goldPanel->getPosition().x + 10, goldPanel->getPosition().y));
	goldLabel->setGlobalZOrder(7);
	mControllerLayer->addChild(goldLabel);

	auto armorPanel = Sprite::create("ArmorPanel.png");
	armorPanel->setScale(0.5);
	armorPanel->setOpacity(220);
	armorPanel->setPosition(Vec2(visibleSize.width / 2 + 150 + 70, visibleSize.height - 40));
	armorPanel->setGlobalZOrder(7);
	mControllerLayer->addChild(armorPanel);

	auto armor = Sprite::create("armor.png");
	armor->setScale(0.07);
	armor->setPosition(Vec2(armorPanel->getPosition().x - 20, armorPanel->getPosition().y));
	armor->setGlobalZOrder(7);
	mControllerLayer->addChild(armor);

	armorLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 18);
	armorLabel->setPosition(Vec2(armorPanel->getPosition().x + 10, armorPanel->getPosition().y));
	armorLabel->setGlobalZOrder(7);
	mControllerLayer->addChild(armorLabel);

	auto damagePanel = Sprite::create("DamagePanel.png");
	damagePanel->setScale(0.5);
	damagePanel->setOpacity(220);
	damagePanel->setPosition(Vec2(visibleSize.width / 2 + 50 + 70, visibleSize.height - 40));
	damagePanel->setGlobalZOrder(7);
	mControllerLayer->addChild(damagePanel);

	auto damage = Sprite::create("damage.png");
	damage->setScale(0.06);
	damage->setPosition(Vec2(damagePanel->getPosition().x - 20, damagePanel->getPosition().y));
	damage->setGlobalZOrder(7);
	mControllerLayer->addChild(damage);

	damageLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 18);
	damageLabel->setPosition(Vec2(damagePanel->getPosition().x + 10, damagePanel->getPosition().y));
	damageLabel->setGlobalZOrder(7);
	mControllerLayer->addChild(damageLabel);

	auto shopIcon = ui::Button::create("shopicon.png", "shopicon_p.png");
	shopIcon->setScale(0.4);
	shopIcon->setPosition(Vec2(visibleSize.width / 2 - 80, visibleSize.height - 40));
	shopIcon->setOpacity(220);
	shopIcon->addClickEventListener([&](Ref* event) {
		McSetting* mcSetting = McSetting::getInstance();
		mcSetting->setBloodPlayer(mPlayer->getBlood());
		mcSetting->setMana(mPlayer->getMana());
		mcSetting->setDamePlayer(mPlayer->getDamage());
		mcSetting->setArmor(mPlayer->getArmor());
		mcSetting->setEXP(mPlayer->getCurrent_Exp());
		mcSetting->setGold(mPlayer->getGold());
		mcSetting->setPlayerLevel(mPlayer->getLevel());
		mcSetting->saveData();
		/////////////////
		auto npc = NpcScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(1, npc, Color3B(0, 255, 255)));
	});
	shopIcon->setGlobalZOrder(7);
	//mControllerLayer->addChild(shopIcon);
	mControllerLayer->setGlobalZOrder(7);
}