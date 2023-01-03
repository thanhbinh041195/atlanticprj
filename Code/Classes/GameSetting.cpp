#include "GameSetting.h"

GameSetting* GameSetting::sInstance = nullptr;

GameSetting::GameSetting() :
	mIsEnableMusic(true),
	mIsEnableSound(true),
	mVolume(100)
{

}

GameSetting::~GameSetting()
{

}

GameSetting* GameSetting::getInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new GameSetting();
	}
	return sInstance;
}

void GameSetting::setEnableMusic(bool enable)
{
	mIsEnableMusic = enable;
}

bool GameSetting::isEnableMusic()
{
	return mIsEnableMusic;
}

void GameSetting::setEnableSound(bool enable)
{
	mIsEnableSound = enable;
}

bool GameSetting::isEnableSound()
{
	return mIsEnableSound;
}

void GameSetting::setVolume(int volume)
{
	mVolume = volume;
}

int GameSetting::getVolume()
{
	return mVolume;
}

