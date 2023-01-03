#include "McSetting.h"
#include "cocos2d.h"
#include "Define.h"
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

McSetting* McSetting::sInstance = nullptr;

McSetting::McSetting()
{
	settingValue[GOLD] = 0;
	settingValue[VOLUME] = 0;
	settingValue[ENABLEMUSIC] = 0;
	settingValue[ENABLESOUND] = 0;
	settingValue[MANA_SPELL] = 0;
	settingValue[BLOOD_SPELL] = 0;
	settingValue[ARMOR] = 0;
	settingValue[PLAYER_LEVEL] = 0;
	settingValue[EXP] = 0;
	settingValue[EXP_MAX] = 0;
	settingValue[DAMAGE_PLAYER] = 0;
	settingValue[BLOOD_PLAYER] = 0;
	settingValue[SHOW_TRAILER] = 0;

}

McSetting::~McSetting()
{

}

McSetting* McSetting::getInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new McSetting();
	}
	return sInstance;
}



void McSetting::setEnableMusic(int isEnableMusic)
{
	settingValue[ENABLEMUSIC] = isEnableMusic;
}

bool McSetting::isEnableMusic()
{
	return settingValue[ENABLEMUSIC] == 1;
}

void McSetting::setEnableSound(int isEnableSound)
{
	settingValue[ENABLESOUND] = isEnableSound;
}

bool McSetting::isEnableSound()
{
	return settingValue[ENABLESOUND] == 1;
}

void McSetting::setVolume(int volume)
{
	settingValue[VOLUME] = volume;
}

int McSetting::getVolume()
{
	return settingValue[VOLUME];
}

int McSetting::getGold()
{
	return settingValue[GOLD];
}

void McSetting::setGold(int gold)
{
	settingValue[GOLD] = gold;
}

void McSetting::loadData()
{
	Document m_document;
	ssize_t size;

	char* buf = (char*)FileUtils::getInstance()->getFileData("Data/Player.json", "r", &size);
	string content(buf, size);

	//std::string content = FileUtils::getInstance()->getStringFromFile(FileUtils::getInstance()->fullPathForFilename("Data/Player.json"));

	m_document.Parse(content.c_str());

	if (!m_document.HasParseError())
	{
		if (m_document.HasMember("player"))
		{
			McSetting* mcSetting = McSetting::getInstance();
			
			mcSetting->setGold(m_document["player"]["GOLD"].GetInt());
			mcSetting->setMana(m_document["player"]["MANA_SPELL"].GetInt());
			mcSetting->setBloodSpell(m_document["player"]["BLOOD_SPELL"].GetInt());
			mcSetting->setArmor(m_document["player"]["ARMOR"].GetInt());
			mcSetting->setPlayerLevel(m_document["player"]["PLAYER_LEVEL"].GetInt());
			mcSetting->setEXP(m_document["player"]["EXP"].GetInt());
			mcSetting->setMaxEXP(m_document["player"]["EXP_MAX"].GetInt());
			mcSetting->setDamePlayer(m_document["player"]["DAMAGE_PLAYER"].GetInt());
			mcSetting->setBloodPlayer(m_document["player"]["BLOOD_PLAYER"].GetInt());

		}
	}
}


void McSetting::abcData()
{
	McSetting* mcSetting = McSetting::getInstance();

	mcSetting->setGold(500);
	mcSetting->setMana(100);
	mcSetting->setBloodSpell(0);
	mcSetting->setArmor(10);
	mcSetting->setPlayerLevel(1);
	mcSetting->setEXP(0);
	mcSetting->setMaxEXP(20);
	mcSetting->setDamePlayer(20);
	mcSetting->setBloodPlayer(200);

}

void McSetting::saveData()
{
	//Output 
	Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

		rapidjson::Value object(rapidjson::kObjectType);
		
		object.AddMember("GOLD", settingValue[GOLD], allocator);
		object.AddMember("MANA_SPELL", settingValue[MANA_SPELL], allocator);
		object.AddMember("BLOOD_SPELL", settingValue[BLOOD_SPELL], allocator);
		object.AddMember("ARMOR", settingValue[ARMOR], allocator);
		object.AddMember("PLAYER_LEVEL", settingValue[PLAYER_LEVEL], allocator);
		object.AddMember("EXP", settingValue[EXP], allocator);
		object.AddMember("EXP_MAX", settingValue[EXP_MAX], allocator);
		object.AddMember("DAMAGE_PLAYER", settingValue[DAMAGE_PLAYER], allocator);
		object.AddMember("BLOOD_PLAYER", settingValue[BLOOD_PLAYER], allocator);

		document.AddMember("player", object, allocator);

	string tr = "Data/Player.json";
	JsonToFile(document, tr);

}

void McSetting::newData()
{
	//Output 
	Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	rapidjson::Value object(rapidjson::kObjectType);

	object.AddMember("GOLD", 200, allocator);
	object.AddMember("MANA_SPELL", 100, allocator);
	object.AddMember("BLOOD_SPELL", 0, allocator);
	object.AddMember("ARMOR", 10, allocator);
	object.AddMember("PLAYER_LEVEL", 1, allocator);
	object.AddMember("EXP", 0, allocator);
	object.AddMember("EXP_MAX", 20, allocator);
	object.AddMember("DAMAGE_PLAYER", 10, allocator);
	object.AddMember("BLOOD_PLAYER", 200, allocator);

	document.AddMember("player", object, allocator);

	string tr = "Data/Player.json";
	JsonToFile(document, tr);

}


int McSetting::getMana()
{
	return settingValue[MANA_SPELL];
}

void McSetting::setMana(int mana)
{
	settingValue[MANA_SPELL] = mana;
}

int McSetting::getBloodSpell()
{
	return settingValue[BLOOD_SPELL];
}

void McSetting::setBloodSpell(int numberSpell)
{
	settingValue[BLOOD_SPELL] = numberSpell;
}

int McSetting::getArmor()
{
	return settingValue[ARMOR];
}

void McSetting::setArmor(int armor)
{
	settingValue[ARMOR] = armor;
}

int McSetting::getPlayerLevel()
{
	return settingValue[PLAYER_LEVEL];
}

void McSetting::setPlayerLevel(int playerLevel)
{
	settingValue[PLAYER_LEVEL] = playerLevel;
}

int McSetting::getEXP()
{
	return settingValue[EXP];
}

void McSetting::setEXP(int exp)
{
	settingValue[EXP] = exp;
}

int McSetting::getMaxEXP()
{
	return settingValue[EXP_MAX];
}

void McSetting::setMaxEXP(int exp)
{
	settingValue[EXP_MAX] = exp;
}

int McSetting::getBloodPlayer()
{
	return settingValue[BLOOD_PLAYER];
}

void McSetting::setBloodPlayer(int blood)
{
	settingValue[BLOOD_PLAYER] = blood;
}

float McSetting::getDamgePlayer()
{
	return settingValue[DAMAGE_PLAYER];
}

void McSetting::setDamePlayer(float damage)
{
	settingValue[DAMAGE_PLAYER] = damage;
}

void McSetting::setIsShowTrailer(int isShow)
{
	settingValue[SHOW_TRAILER] = isShow;
}

bool McSetting::getIsShowTrailer()
{
	return settingValue[SHOW_TRAILER] == 1 ? true : false;
}

void McSetting::preLoadSounds()
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
}


void McSetting::JsonToFile(rapidjson::Document &jsonObject, string &fullpath) {
	ofstream outputFile;
	outputFile.open(fullpath);
	if (outputFile.is_open()) {
		string jsonObjectData = JsonToString(jsonObject);
		outputFile << jsonObjectData;
	}
	outputFile.close();
}

string McSetting::JsonToString(rapidjson::Document &jsonObject) {
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(buffer);
	jsonObject.Accept(jsonWriter);
	return buffer.GetString();
}