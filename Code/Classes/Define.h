#ifndef _DEFINE_H_
#define _DEFINE_H_

#define GOLD 0
#define VOLUME 1
#define ENABLEMUSIC 2
#define ENABLESOUND 3
#define MANA_SPELL 4
#define BLOOD_SPELL 5
#define ARMOR 6
#define PLAYER_LEVEL 7
#define EXP 8
#define EXP_MAX 9
#define BLOOD_PLAYER 10
#define DAMAGE_PLAYER 11
#define SHOW_TRAILER 12




enum CHARACTER_DIRECTION
{
	TOP,
	RIGHT,
	DOWN,
	LEFT,
};


enum TYPE_ITEMSHOP
{
	Blood = 0,
	Shuriken = 1,
	Partical = 2,
	Sword = 3
};

enum SOUND_NAME
{
	SKILL_CHOP = 0,
	SKILL_SHURIKEN = 1,
	SKILL_FLASH = 2,
	SKILL_STORM = 3,
	BUTTON = 4
};
enum SOUND_MODEL
{
	SKILL_PLAER_COW,
	SKILL_PLAER_AECHER,
	SKILL_PLAER_BOSS,
	SKILL_PLAER_DOG


};
enum SOUND_PLAER
{
	SKILL_COW_PLAER = 0
};
#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()
#endif