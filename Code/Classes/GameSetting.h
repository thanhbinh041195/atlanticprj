#ifndef _GAME_SETTING_H_
#define _GAME_SETTING_H_

class GameSetting
{
private:
	static GameSetting* sInstance;
	bool mIsEnableMusic;
	bool mIsEnableSound;
	int mVolume;
private:
	GameSetting();

public:
	~GameSetting();

	static GameSetting* getInstance();

	void setEnableMusic(bool enable);

	bool isEnableMusic();

	void setEnableSound(bool enable);

	bool isEnableSound();

	void setVolume(int volume);

	int getVolume();
};

#endif
