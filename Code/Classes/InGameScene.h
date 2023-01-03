#ifndef _INGAME_SCENE_H_
#define _INGAME_SCENE_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Player.h"
#include "Model.h"
#include "CowMonster.h"
#include "ArcherMonster.h"
#include "DogMonster.h"
#include "BossMonster.h"
#include "SneakyButton.h"
#include "SneakyButtonSkinnedBase.h"
#include "SneakyJoystickSkinnedBase.h"
#include "SoundManager.h"
#include "ArcherMonster.h"
#include "McSetting.h"

class InGameScene : public cocos2d::Layer
{
private:
	//use cheat for key
	cocos2d::EventKeyboard::KeyCode mCurrentKey;

	cocos2d::ui::Widget::TouchEventType mCurrentTouchState;

	cocos2d::Point mCurrentTouchPoint;

	cocos2d::Label* cdskill1;
	cocos2d::Label* cdskill2;
	cocos2d::Label* cdskill3;

	cocos2d::Sprite* mMoveLeftController;
	cocos2d::Sprite* mMoveLeftControllerPressed;
	cocos2d::Sprite* mMoveRightController;
	cocos2d::Sprite* mMoveRightControllerPressed;

	cocos2d::ui::Button* mAttackController;
	cocos2d::ui::Button* mSkill1Controller;
	cocos2d::ui::Button* mSkill2Controller;
	cocos2d::ui::Button* mSkill3Controller;

	cocos2d::TMXTiledMap* mTileMap;
	cocos2d::TMXTiledMap* mTileMapFarBackground;
	cocos2d::TMXObjectGroup* objectGroup1;
	cocos2d::TMXObjectGroup* objectGroup2;
	cocos2d::TMXObjectGroup* objectGroup3;
	cocos2d::TMXObjectGroup* mObjectGroup;
	cocos2d::PhysicsBody* physicsMap1;
	cocos2d::PhysicsBody* physicsMap2;
	cocos2d::PhysicsBody* physicsMap3;
	cocos2d::TMXLayer* mPhysicsLayer;

	//cd skill
	float timeskill1 = 0.0f;
	float timeskill2 = 0.0f;
	float timeskill3 = 0.0f;

	//progress's blood
	cocos2d::ui::LoadingBar *mMcHudBlood;
	cocos2d::ui::LoadingBar *mMcHudMana;
	cocos2d::ui::LoadingBar *mMcHudExperience;

	Player *mPlayer;
	std::vector<CowMonster*> mCowMonsters;
	std::vector<ArcherMonster*> mArcherMonsters;
	std::vector<DogMonster*> mDogMonsters;
	std::vector<BossMonster*> mBossMonsters;
	//ArcherMonster *mArcher;

public:
	SneakyJoystick *leftJoystick;
	SneakyButton *jumpBtn;

	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual void update(float dt);

	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event  *event);

	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event  *event);

	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event  *event);

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	bool onContactBegin(cocos2d::PhysicsContact& contact);

	void setViewPointCenter(Point position);
	void setButtonPointCenter(Point position);
	CREATE_FUNC(InGameScene);

private:
	void createMap();

	void createPhysics();

	void createObjects();

	void createController();

	void createHud();

	void createPauseLayer();

	void updateReady_dg(float delta);

	void updateObject(float delta);

	void updateController();

	void addListener();

	void setCenterView(bool isMoveRight);

	void moveLeft();

	void moveRight();

	void moveUp();

	void moveDown();

	void attack(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void skill1(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void skill2(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void skill3(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void pause(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void enablePressedControl(bool isLeft, bool pressed);

	void updateArcherMonster();
	void updateCowMonsters();
	void updateBossMonsters();
	void updateDogMonsters();
};

#endif // _InGame_SCENE_H_
