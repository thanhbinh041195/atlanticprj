#include "SoundManager.h"
#include "SimpleAudioEngine.h"
#include "MenuScene.h"
#include "AudioEngine.h"
#include "Define.h"
#include "InGameScene.h"

using namespace CocosDenshion;

extern cocos2d::Node* mControllerLayer;
extern cocos2d::Node* mMenuLayer;

SoundManager* SoundManager::instance = 0;
SoundManager * SoundManager::getInstance()
{
	if (instance == 0)
	{
		instance = new SoundManager();

	}
	return instance;
}

SoundManager::SoundManager()
{
	m_Vl_Music = 0.2;
	m_Vl_Sound = 0.2;
	m_showState = false;

	/*audioButton->playEffect("res/sound/effects/button_click.mp3", false, 1.0f, 0.0f, 1.0f);
	m_SkillChop->playEffect("res/sound/effects/snd_BushCut.wav", false, 1.0f, 0.0f, 1.0f);*/

}

SoundManager::~SoundManager()
{
}
void SoundManager::setVlMusic(float VolumeBackground)
{
	this->m_Vl_Music = VolumeBackground / 100;
}
float SoundManager::getVlMusic()
{
	return m_Vl_Music * 100;
}
void SoundManager::setVlSound(float VolumeSound)
{
	this->m_Vl_Sound = VolumeSound / 100;
}
float SoundManager::getVlSound()
{
	return m_Vl_Sound * 100;
}


void SoundManager::applySoundSetting(float VolumeBackground, float VolumeSound)
{
	audioBackground->setBackgroundMusicVolume(VolumeBackground / 100);
	audioButton->setEffectsVolume(VolumeSound / 100);
}
void SoundManager::playBackgroundMusic()
{
	audioBackground = SimpleAudioEngine::getInstance();

	audioBackground->playBackgroundMusic("sounds/tieungao.mp3", true);
};
void SoundManager::playmodel(int n)
{
	switch (n)
	{
	case SOUND_MODEL::SKILL_PLAER_COW:
		audioButton = SimpleAudioEngine::getInstance();

		audioButton->playEffect("sounds/SCREAM3.mp3", false, 1.0f, 0.0f, 1.0f);
		break;
	case SOUND_MODEL::SKILL_PLAER_AECHER:
		audioButton = SimpleAudioEngine::getInstance();

		audioButton->playEffect("sounds/BanCung.mp3", false, 1.0f, 0.0f, 1.0f);
		break;
	case SOUND_MODEL::SKILL_PLAER_DOG:
		audioButton = SimpleAudioEngine::getInstance();

		audioButton->playEffect("sounds/AmThanhDanhNhau-V.A-3611605.mp3", false, 1.0f, 0.0f, 1.0f);
		break;
	default:
		break;

	}
};
void SoundManager::playplayer(int n)
{
	switch (n)
	{
	case SOUND_PLAER::SKILL_COW_PLAER:
		audioButton = SimpleAudioEngine::getInstance();

		audioButton->playEffect("sounds/BiDanhTrung.mp3", false, 1.0f, 0.0f, 1.0f);
		break;
	default:
		break;
	}
};
void SoundManager::playButtonEffect(int n)
{
	switch (n)
	{
	case SOUND_NAME::BUTTON:
		audioButton = SimpleAudioEngine::getInstance();
		audioButton->playEffect("sounds/zapsplat_warfare_sword_swing_fast_whoosh_metal_004.mp3", false, 1.0f, 0.0f, 1.0f);
		break;
	case SOUND_NAME::SKILL_CHOP:
		audioButton = SimpleAudioEngine::getInstance();
		audioButton->playEffect("sounds/sword1.wav", false, 1.0f, 0.0f, 1.0f);
		break;
	case SOUND_NAME::SKILL_SHURIKEN:
		audioButton = SimpleAudioEngine::getInstance();
		audioButton->playEffect("sounds/unholy-armor.wav", false, 1.0f, 0.0f, 1.0f);
		break;
	case SOUND_NAME::SKILL_STORM:
		audioButton = SimpleAudioEngine::getInstance();
		audioButton->playEffect("sounds/sword-remove.wav", false, 1.0f, 0.0f, 1.0f);
		break;
	default:
		break;
	}
}

bool SoundManager::getShowState()
{
	return m_showState;
}
void SoundManager::setShowState(bool state)
{
	m_showState = state;
}
void SoundManager::showSetting(Node* scene)
{
	SoundManager::getInstance()->setShowState(false);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	SoundManager::getInstance()->playButtonEffect(SOUND_NAME::BUTTON);
	static ui::Slider* slVolumeMusic;
	static ui::Slider* slVolumeSound;
	settingBackground = Sprite::create("background_Setting.png");

	settingBackground->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 20);
	settingBackground->setScale(0.5);
	settingBackground->setGlobalZOrder(99);
	scene->addChild(settingBackground, 5);

	slVolumeMusic = ui::Slider::create();
	slVolumeMusic->setAnchorPoint(Vec2(0, 0.5));
	slVolumeMusic->setScale(1.9f);
	slVolumeMusic->loadBarTexture("slider_bar_bg.png");
	slVolumeMusic->loadSlidBallTextures("slider_ball_normal.png", "slider_ball_pressed.png", "slider_ball_disable.png");
	slVolumeMusic->loadProgressBarTexture("slider_bar_pressed.png");
	slVolumeMusic->setGlobalZOrder(100);
	slVolumeMusic->setPosition(settingBackground->getPosition() + Vec2(-slVolumeMusic->getContentSize().width / 15, settingBackground->getContentSize().height / 2.5) + Vec2(-330, 0));

	slVolumeMusic->setPercent(SoundManager::getInstance()->getVlMusic());

	slVolumeMusic->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			SoundManager::getInstance()->playButtonEffect(SOUND_NAME::BUTTON);
			break;
		case ui::Widget::TouchEventType::ENDED:
			SoundManager::getInstance()->playButtonEffect(SOUND_NAME::BUTTON);
			SoundManager::getInstance()->applySoundSetting(slVolumeMusic->getPercent(), slVolumeSound->getPercent());
			break;
		};
	});

	settingBackground->addChild(slVolumeMusic);



	slVolumeSound = ui::Slider::create();
	slVolumeSound->setAnchorPoint(Vec2(0, 0.5));
	slVolumeSound->setScale(1.9f);
	slVolumeSound->loadBarTexture("slider_bar_bg.png");
	slVolumeSound->loadSlidBallTextures("slider_ball_normal.png", "slider_ball_pressed.png", "slider_ball_disable.png");
	slVolumeSound->loadProgressBarTexture("slider_bar_pressed.png");
	slVolumeSound->setGlobalZOrder(100);
	slVolumeSound->setPosition(Vec2(settingBackground->getPosition().x - slVolumeMusic->getContentSize().width / 15, slVolumeMusic->getPosition().y - 200) + Vec2(-330, 0));
	slVolumeSound->setPercent(SoundManager::getInstance()->getVlSound());
	slVolumeSound->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			SoundManager::getInstance()->playButtonEffect(SOUND_NAME::BUTTON);
			break;
		case ui::Widget::TouchEventType::ENDED:
			SoundManager::getInstance()->applySoundSetting(slVolumeMusic->getPercent(), slVolumeSound->getPercent());
			break;
		};
	});

	settingBackground->addChild(slVolumeSound);


	auto label_Music = Label::createWithSystemFont("Music", "fonts / Marker Felt.ttf", 48);
	//label_Music->setColor(Color3B(0, 0, 0));
	label_Music->setPosition(slVolumeMusic->getPosition() - Vec2(label_Music->getContentSize().width - 300, -100));
	settingBackground->setGlobalZOrder(100);
	settingBackground->addChild(label_Music);


	auto label_Sound = Label::createWithSystemFont("Sound", "fonts/HoboStd.otf", 50);
	//label_Sound->setColor(Color3B(0, 0, 0));
	label_Sound->setPosition(slVolumeSound->getPosition() - Vec2(label_Sound->getContentSize().width - 300, -50));
	label_Sound->setGlobalZOrder(100);
	settingBackground->addChild(label_Sound);




	auto btn_apply = ui::Button::create("applySetting.png");
	btn_apply->setScale(2.3);
	btn_apply->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
	{	
		SimpleAudioEngine::getInstance()->resumeAllEffects();
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		Director::getInstance()->resume();
		if (scene->getName() == "mPauseLayer")
		{
			mControllerLayer->setVisible(true);
		}
		else
			if (scene->getName() == "mMenuLayer")
			{
				mMenuLayer->setVisible(true);
			}
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			SoundManager::getInstance()->playButtonEffect(SOUND_NAME::BUTTON);
			SoundManager::getInstance()->setShowState(true);
			break;
		case ui::Widget::TouchEventType::ENDED:
			SoundManager::getInstance()->setVlMusic(slVolumeMusic->getPercent());
			SoundManager::getInstance()->setVlSound(slVolumeSound->getPercent());
			SoundManager::getInstance()->applySoundSetting(SoundManager::getInstance()->getVlMusic(), SoundManager::getInstance()->getVlSound());


			scene->removeChild(settingBackground);
			break;

		};
	});
	btn_apply->setGlobalZOrder(100);
	settingBackground->addChild(btn_apply, 5);
	btn_apply->setPosition(label_Sound->getPosition() + Vec2(btn_apply->getContentSize().width - 170, -btn_apply->getContentSize().height - 250));

	auto btn_cancel = ui::Button::create("cancelSetting.png");
	btn_cancel->setScale(2.3);
	btn_cancel->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
	{
		SimpleAudioEngine::getInstance()->resumeAllEffects();
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		Director::getInstance()->resume();
		if (scene->getName() == "mPauseLayer")
		{
			mControllerLayer->setVisible(true);
		}
		else
			if (scene->getName() == "mMenuLayer")
			{
				mMenuLayer->setVisible(true);
			}
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			SoundManager::getInstance()->playButtonEffect(SOUND_NAME::BUTTON);
			SoundManager::getInstance()->setShowState(true);
			break;
		case ui::Widget::TouchEventType::ENDED:
			SoundManager::getInstance()->applySoundSetting(SoundManager::getInstance()->getVlMusic(), SoundManager::getInstance()->getVlSound());
			scene->removeChild(settingBackground);
			break;
		};
	});
	settingBackground->addChild(btn_cancel, 5);
	btn_cancel->setGlobalZOrder(100);
	btn_cancel->setPosition(btn_apply->getPosition() + Vec2(btn_apply->getContentSize().width + 110, 0));
};