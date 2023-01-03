#pragma once
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "InGameScene.h"

USING_NS_CC;
class SoundManager
{
public:
	static SoundManager* getInstance();
	void setVlMusic(float);
	float getVlMusic();
	void setVlSound(float);
	float getVlSound();
	void applySoundSetting(float, float);
	void playBackgroundMusic();
	void playButtonEffect(int);
	void playmodel(int);
	void playplayer(int);
	void playChestEffect(int);
	void showSetting(cocos2d::Node* Scene);
	bool getShowState();
	void setShowState(bool state);
private:
	static SoundManager* instance;
	CocosDenshion::SimpleAudioEngine* audioBackground;
	CocosDenshion::SimpleAudioEngine* audioButton;
	CocosDenshion::SimpleAudioEngine* m_SkillChop;
	CocosDenshion::SimpleAudioEngine* m_SkillFlash;
	CocosDenshion::SimpleAudioEngine* m_SkillStorm;
	CocosDenshion::SimpleAudioEngine* m_SkillShuriken;
	CocosDenshion::SimpleAudioEngine* m_Pot;
	CocosDenshion::SimpleAudioEngine* m_ChestSmall;
	CocosDenshion::SimpleAudioEngine* m_Gem;
	CocosDenshion::SimpleAudioEngine* m_Model;
	CocosDenshion::SimpleAudioEngine* m_Model1;

	float m_Vl_Music;
	float m_Vl_Sound;
	SoundManager();
	~SoundManager();
	bool m_showState;
	cocos2d::Sprite* settingBackground;
};