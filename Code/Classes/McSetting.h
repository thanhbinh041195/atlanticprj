#ifndef _GAME_SETTING_H_
#define _GAME_SETTING_H_

#include "SimpleAudioEngine.h"
#include "ItemShop.h"
#include <string>
#include "rapidjson/document.h"
using namespace rapidjson;
using namespace std;
class McSetting
{
private:
	static McSetting* sInstance;

	int settingValue[12];

	McSetting();

public:

	int back;

	~McSetting();

	static McSetting* getInstance();


	void setEnableMusic(int isEnableMusic);

	bool isEnableMusic();

	void setEnableSound(int isEnableSound);

	bool isEnableSound();

	void setVolume(int volume);

	int getVolume();

	int getGold();

	void setGold(int gold);

	void loadData();

	void saveData();

	void abcData();

	void newData();

	int getMana();

	void setMana(int mana);

	int getBloodSpell();

	void setBloodSpell(int numberSpell);

	int getArmor();

	void setArmor(int armor);

	int getPlayerLevel();

	void setPlayerLevel(int playerLevel);

	int getEXP();

	void setEXP(int exp);

	int getMaxEXP();

	void setMaxEXP(int exp);

	int getBloodPlayer();

	void setBloodPlayer(int blood);

	float getDamgePlayer();

	void setDamePlayer(float damage);

	void setIsShowTrailer(int isShow);

	bool getIsShowTrailer();

	void preLoadSounds();

	void JsonToFile(rapidjson::Document &jsonObject, string &fullpath);
	string JsonToString(rapidjson::Document &jsonObject);
};

#endif