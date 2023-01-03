#include "AnimationManager.h"

AnimationManager* AnimationManager::mInstance = 0;
AnimationManager * AnimationManager::getInstance()
{
	if (mInstance == 0)
	{
		mInstance = new AnimationManager();
	}
	return mInstance;
}
AnimationManager::AnimationManager()
{
}
AnimationManager::~AnimationManager()
{
}
Animation* AnimationManager::createAnimation(const char* prefixName, int start, int end, float delay, std::string lastFrame)
{
	Vector<SpriteFrame*> animFrames;

	for (int i = start; i <= end; i++)
	{
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat(prefixName, i)->getCString());
		if (frame)
		{
			animFrames.pushBack(frame);
		}
	}
	animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(lastFrame));
	return Animation::createWithSpriteFrames(animFrames, delay);
}
Animation* AnimationManager::createAnimation(const char* prefixName, int start, int end, float delay)
{
	Vector<SpriteFrame*> animFrames;

	for (int i = start; i <= end; i++)
	{
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat(prefixName, i)->getCString());
		if (frame)
		{
			animFrames.pushBack(frame);
		}
	}
	return Animation::createWithSpriteFrames(animFrames, delay);
}
Animation* AnimationManager::createAnimationR(const char* prefixName, int start, int end, float delay)
{
	Vector<SpriteFrame*> animFrames;

	for (int i = start; i >= end; i--)
	{
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat(prefixName, i)->getCString());
		if (frame)
		{
			animFrames.pushBack(frame);
		}
	}
	return Animation::createWithSpriteFrames(animFrames, delay);
}

/*void AnimationManager::init_desert()
{

	AnimationCache::getInstance()->addAnimation(createAnimation("desertThug_0%03d.png", 89, 106, 0.05f), "Thug_death");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertThug_0%03d.png", 23, 44, 0.1f), "Thug_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertThug_0%03d.png", 45, 66, 0.1f), "Thug_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertThug_0%03d.png", 1, 22, 0.1f), "Thug_runleft");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertThug_0%03d.png", 1, 22, 0.1f), "Thug_runright");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertThug_0%03d.png", 67, 88, 0.1f), "Thug_attack");


	AnimationCache::getInstance()->addAnimation(createAnimation("desertRaider_0%03d.png", 89, 106, 0.05f), "Raider_death");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertRaider_0%03d.png", 23, 44, 0.1f), "Raider_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertRaider_0%03d.png", 45, 66, 0.1f), "Raider_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertRaider_0%03d.png", 1, 22, 0.1f), "Raider_runleft");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertRaider_0%03d.png", 1, 22, 0.1f), "Raider_runright");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertRaider_0%03d.png", 67, 88, 0.1f), "Raider_attack");


	AnimationCache::getInstance()->addAnimation(createAnimation("desertWolf_0%03d.png", 69, 86, 0.05f), "Wolf_death");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertWolf_0%03d.png", 11, 30, 0.1f), "Wolf_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertWolf_0%03d.png", 1, 10, 0.1f), "Wolf_runleft");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertWolf_0%03d.png", 1, 10, 0.1f), "Wolf_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertWolf_0%03d.png", 1, 10, 0.1f), "Wolf_runright");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertWolf_0%03d.png", 31, 43, 0.1f), "Wolf_attack");


	AnimationCache::getInstance()->addAnimation(createAnimation("desertImmortal_0%03d.png", 67, 88, 0.05f), "Immortal_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertImmortal_0%03d.png", 89, 124, 0.1f), "Immortal_death");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertImmortal_0%03d.png", 23, 44, 0.1f), "Immortal_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertImmortal_0%03d.png", 45, 66, 0.1f), "Immortal_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertImmortal_0%03d.png", 1, 22, 0.1f), "Immortal_runleft");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertImmortal_0%03d.png", 1, 22, 0.1f), "Immortal_runright");


	AnimationCache::getInstance()->addAnimation(createAnimation("fallen_0%03d.png", 48, 69, 0.05f), "Fallen_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("fallen_0%03d.png", 70, 110, 0.1f), "Fallen_death");
	AnimationCache::getInstance()->addAnimation(createAnimation("fallen_0%03d.png", 17, 32, 0.1f), "Fallen_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("fallen_0%03d.png", 32, 47, 0.1f), "Fallen_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("fallen_0%03d.png", 1, 16, 0.1f), "Fallen_runleft");
	AnimationCache::getInstance()->addAnimation(createAnimation("fallen_0%03d.png", 1, 16, 0.1f), "Fallen_runright");
	AnimationCache::getInstance()->addAnimation(createAnimation("fallen_0%03d.png", 114, 146, 0.1f), "Fallen_birth");


	AnimationCache::getInstance()->addAnimation(createAnimation("desertMunra_0%03d.png", 122, 163, 0.05f), "Munra_skill");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertMunra_0%03d.png", 73, 120, 0.05f), "Munra_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertMunra_0%03d.png", 165, 200, 0.05f), "Munra_death");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertMunra_0%03d.png", 25, 47, 0.05f), "Munra_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertMunra_0%03d.png", 49, 71, 0.05f), "Munra_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertMunra_0%03d.png", 1, 23, 0.05f), "Munra_runleft");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertMunra_0%03d.png", 1, 23, 0.05f), "Munra_runright");


	AnimationCache::getInstance()->addAnimation(createAnimation("scorpion_0%003d.png", 28, 69, 0.05f), "Scorpion_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("scorpion_0%003d.png", 70, 99, 0.1f), "Scorpion_death");
	AnimationCache::getInstance()->addAnimation(createAnimation("scorpion_0%003d.png", 10, 18, 0.1f), "Scorpion_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("scorpion_0%003d.png", 19, 27, 0.1f), "Scorpion_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("scorpion_0%003d.png", 1, 9, 0.1f), "Scorpion_runleft");
	AnimationCache::getInstance()->addAnimation(createAnimation("scorpion_0%003d.png", 1, 9, 0.1f), "Scorpion_runright");


	AnimationCache::getInstance()->addAnimation(createAnimation("wasp_hornet_0%003d.png", 43, 52, 0.05f), "WaspHornet_death");
	AnimationCache::getInstance()->addAnimation(createAnimation("wasp_hornet_0%003d.png", 15, 29, 0.1f), "WaspHornet_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("wasp_hornet_0%003d.png", 30, 42, 0.1f), "WaspHornet_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("wasp_hornet_0%003d.png", 1, 14, 0.1f), "WaspHornet_runleft");
	AnimationCache::getInstance()->addAnimation(createAnimation("wasp_hornet_0%003d.png", 1, 14, 0.1f), "WaspHornet_runright");


	AnimationCache::getInstance()->addAnimation(createAnimation("wasp_queen_0%003d.png", 43, 53, 0.1f), "WaspQueen_death");
	AnimationCache::getInstance()->addAnimation(createAnimation("wasp_queen_0%003d.png", 15, 29, 0.1f), "WaspQueen_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("wasp_queen_0%003d.png", 30, 42, 0.1f), "WaspQueen_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("wasp_queen_0%003d.png", 1, 14, 0.1f), "WaspQueen_runleft");
	AnimationCache::getInstance()->addAnimation(createAnimation("wasp_queen_0%003d.png", 1, 14, 0.1f), "WaspQueen_runright");

	AnimationCache::getInstance()->addAnimation(createAnimation("desertExecutioner_0%003d.png", 73, 98, 0.05f), "Executioner_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertExecutioner_0%003d.png", 99, 118, 0.05f), "Executioner_death");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertExecutioner_0%003d.png", 49, 72, 0.05f), "Executioner_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertExecutioner_0%003d.png", 25, 48, 0.05f), "Executioner_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertExecutioner_0%003d.png", 1, 24, 0.05f), "Executioner_runleft");
	AnimationCache::getInstance()->addAnimation(createAnimation("desertExecutioner_0%003d.png", 1, 24, 0.05f), "Executioner_runright");

	AnimationCache::getInstance()->addAnimation(createAnimation("tremor_0%03d.png", 1, 14, 0.1f), "Tremor_runleft");
	AnimationCache::getInstance()->addAnimation(createAnimation("tremor_0%03d.png", 1, 14, 0.1f), "Tremor_runright");
	AnimationCache::getInstance()->addAnimation(createAnimation("tremor_0%03d.png", 15, 28, 0.1f), "Tremor_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("tremor_0%03d.png", 29, 42, 0.1f), "Tremor_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("tremor_0%03d.png", 83, 95, 0.1f), "Tremor_death");
	AnimationCache::getInstance()->addAnimation(createAnimation("tremor_0%03d.png", 43, 61, 0.05f), "Tremor_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("tremor_0%03d.png", 64, 75, 0.1f), "Tremor_out");
	AnimationCache::getInstance()->addAnimation(createAnimation("tremor_0%03d.png", 75, 82, 0.1f), "Tremor_in");
}

/*void AnimationManager::init_jungle()
{
	AnimationCache::getInstance()->addAnimation(createAnimation("Canibal_0%03d.png", 67, 77, 0.1f), "Canibal_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("Canibal_0%03d.png", 78, 130, 0.05f), "Canibal_death");
	AnimationCache::getInstance()->addAnimation(createAnimation("Canibal_0%03d.png", 23, 44, 0.1f), "Canibal_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("Canibal_0%03d.png", 45, 66, 0.1f), "Canibal_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("Canibal_0%03d.png", 1, 22, 0.1f), "Canibal_runleft");
	AnimationCache::getInstance()->addAnimation(createAnimation("Canibal_0%03d.png", 1, 22, 0.1f), "Canibal_runright");


	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalHunter_0%03d.png", 67, 89, 0.05f), "Hunter_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalHunter_0%03d.png", 106, 138, 0.1f), "Hunter_death");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalHunter_0%03d.png", 23, 44, 0.1f), "Hunter_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalHunter_0%03d.png", 45, 66, 0.1f), "Hunter_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalHunter_0%03d.png", 1, 22, 0.1f), "Hunter_runleft");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalHunter_0%03d.png", 1, 22, 0.1f), "Hunter_runright");


	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalShamanMagic_0%03d.png", 127, 150, 0.05f), "Shaman_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalShamanMagic_0%03d.png", 171, 177, 0.1f), "Shaman_death");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalShamanMagic_0%03d.png", 23, 44, 0.1f), "Shaman_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalShamanMagic_0%03d.png", 45, 66, 0.1f), "Shaman_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalShamanMagic_0%03d.png", 1, 22, 0.1f), "Shaman_runleft");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalShamanMagic_0%03d.png", 1, 22, 0.1f), "Shaman_runright");


	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalShamanPriest_0%03d.png", 67, 150, 0.02f), "Priest_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalShamanPriest_0%03d.png", 160, 176, 0.1f), "Priest_death");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalShamanPriest_0%03d.png", 23, 44, 0.1f), "Priest_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalShamanPriest_0%03d.png", 45, 66, 0.1f), "Priest_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalShamanPriest_0%03d.png", 1, 22, 0.1f), "Priest_runleft");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalShamanPriest_0%03d.png", 1, 22, 0.1f), "Priest_runright");


	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalShamanShield_0%03d.png", 67, 90, 0.05f), "Shield_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalShamanShield_0%03d.png", 100, 118, 0.1f), "Shield_death");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalShamanShield_0%03d.png", 23, 44, 0.1f), "Shield_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalShamanShield_0%03d.png", 45, 66, 0.1f), "Shield_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalShamanShield_0%03d.png", 1, 22, 0.1f), "Shield_runleft");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalShamanShield_0%03d.png", 1, 22, 0.1f), "Shield_runright");


	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalWingRider_0%03d.png", 43, 53, 0.1f), "WingRider_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalWingRider_0%03d.png", 57, 67, 0.1f), "WingRider_death");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalWingRider_0%03d.png", 15, 28, 0.1f), "WingRider_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalWingRider_0%03d.png", 29, 42, 0.1f), "WingRider_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalWingRider_0%03d.png", 1, 14, 0.1f), "WingRider_runleft");
	AnimationCache::getInstance()->addAnimation(createAnimation("CanibalWingRider_0%03d.png", 1, 14, 0.1f), "WingRider_runright");


	AnimationCache::getInstance()->addAnimation(createAnimation("Gorilla_0%03d.png", 73, 126, 0.05f), "Gorilla_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("Gorilla_0%03d.png", 127, 160, 0.1f), "Gorilla_death");
	AnimationCache::getInstance()->addAnimation(createAnimation("Gorilla_0%03d.png", 26, 49, 0.1f), "Gorilla_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("Gorilla_0%03d.png", 50, 72, 0.1f), "Gorilla_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("Gorilla_0%03d.png", 1, 25, 0.1f), "Gorilla_runleft");
	AnimationCache::getInstance()->addAnimation(createAnimation("Gorilla_0%03d.png", 1, 25, 0.1f), "Gorilla_runright");
}*/

/*void AnimationManager::init_underground()
{
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianBlazefang_0%03d.png", 1, 21, 0.05f), "Blazefang_runleft");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianBlazefang_0%03d.png", 1, 21, 0.05f), "Blazefang_runright");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianBlazefang_0%03d.png", 23, 43, 0.05f), "Blazefang_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianBlazefang_0%03d.png", 45, 65, 0.05f), "Blazefang_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianBlazefang_0%03d.png", 67, 95, 0.05f), "Blazefang_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianBlazefang_0%03d.png", 187, 203, 0.05f), "Blazefang_death");


	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianBroodguard_0%03d.png", 1, 21, 0.05f), "Broodguard_runleft");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianBroodguard_0%03d.png", 1, 21, 0.05f), "Broodguard_runright");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianBroodguard_0%03d.png", 23, 43, 0.05f), "Broodguard_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianBroodguard_0%03d.png", 45, 65, 0.05f), "Broodguard_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianBroodguard_0%03d.png", 67, 84, 0.05f), "Broodguard_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianBroodguard_0%03d.png", 97, 114, 0.05f), "Broodguard_death");


	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianElite_0%03d.png", 1, 21, 0.05f), "Elite_runleft");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianElite_0%03d.png", 1, 21, 0.05f), "Elite_runright");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianElite_0%03d.png", 23, 43, 0.05f), "Elite_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianElite_0%03d.png", 45, 65, 0.05f), "Elite_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianElite_0%03d.png", 67, 123, 0.05f), "Elite_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianElite_0%03d.png", 126, 143, 0.05f), "Elite_death");


	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianMyrmidon_0%03d.png", 1, 21, 0.05f), "Myrmidon_runleft");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianMyrmidon_0%03d.png", 1, 21, 0.05f), "Myrmidon_runright");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianMyrmidon_0%03d.png", 23, 43, 0.05f), "Myrmidon_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianMyrmidon_0%03d.png", 45, 65, 0.05f), "Myrmidon_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianMyrmidon_0%03d.png", 67, 85, 0.05f), "Myrmidon_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianMyrmidon_0%03d.png", 87, 104, 0.05f), "Myrmidon_death");


	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianNightscale_0%03d.png", 1, 21, 0.05f), "Nightscale_runleft");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianNightscale_0%03d.png", 1, 21, 0.05f), "Nightscale_runright");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianNightscale_0%03d.png", 23, 43, 0.05f), "Nightscale_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianNightscale_0%03d.png", 45, 65, 0.05f), "Nightscale_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianNightscale_0%03d.png", 67, 84, 0.05f), "Nightscale_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianNightscale_0%03d.png", 125, 144, 0.05f), "Nightscale_death");

	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianQuetzal_0%03d.png", 1, 13, 0.05), "Quetzal_runleft");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianQuetzal_0%03d.png", 1, 13, 0.05), "Quetzal_runright");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianQuetzal_0%03d.png", 15, 28, 0.05), "Quetzal_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianQuetzal_0%03d.png", 29, 41, 0.05), "Quetzal_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianQuetzal_0%03d.png", 43, 65, 0.05), "Quetzal_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("SaurianQuetzal_0%03d.png", 65, 83, 0.05), "Quetzal_death");

}*/

/*void AnimationManager::init_towers()
{
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_archer_lvl1_shooter_00%02d.png", 3, 10, 0.05f, "tower_archer_lvl1_shooter_0001.png"), "level1_shoot_down");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_archer_lvl2_shooter_00%02d.png", 3, 10, 0.05f, "tower_archer_lvl2_shooter_0001.png"), "level2_shoot_down");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_archer_lvl3_shooter_00%02d.png", 3, 10, 0.05f, "tower_archer_lvl3_shooter_0001.png"), "level3_shoot_down");

	AnimationCache::getInstance()->addAnimation(createAnimation("tower_archer_lvl3_shooter_00%02d.png", 11, 18, 0.05f, "tower_archer_lvl3_shooter_0002.png"), "level1_shoot_top");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_archer_lvl3_shooter_00%02d.png", 11, 18, 0.05f, "tower_archer_lvl3_shooter_0002.png"), "level2_shoot_top");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_archer_lvl3_shooter_00%02d.png", 11, 18, 0.05f, "tower_archer_lvl3_shooter_0002.png"), "level3_shoot_top");

	AnimationCache::getInstance()->addAnimation(createAnimation("fx_blood_splat_red_00%02d.png", 1, 9, 0.05f), "blood_red");

	AnimationCache::getInstance()->addAnimation(createAnimation("reinforce_C0_0%03d.png", 1, 6, 0.05f), "reinforce1_run");
	AnimationCache::getInstance()->addAnimation(createAnimation("reinforce_C0_0%03d.png", 7, 17, 0.05f), "reinforce1_attack");

	AnimationCache::getInstance()->addAnimation(createAnimation("mage_lvl1_00%02d.png", 1, 12, 0.05f), "level1_mage_shine");
	AnimationCache::getInstance()->addAnimation(createAnimation("mage_lvl2_00%02d.png", 1, 12, 0.05f), "level2_mage_shine");
	AnimationCache::getInstance()->addAnimation(createAnimation("mage_lvl3_00%02d.png", 1, 12, 0.05f), "level3_mage_shine");

	AnimationCache::getInstance()->addAnimation(createAnimation("mage_shooter_00%02d.png", 17, 29, 0.05f, "mage_shooter_0001.png"), "mage_shoot_up");
	AnimationCache::getInstance()->addAnimation(createAnimation("mage_shooter_00%02d.png", 3, 16, 0.05f, "mage_shooter_0001.png"), "mage_shoot_down");


	AnimationCache::getInstance()->addAnimation(createAnimation("ArchMageGuy_00%02d.png", 3, 30, 0.05f, "ArchMageGuy_0001.png"), "ArchMageGuy_up");
	AnimationCache::getInstance()->addAnimation(createAnimation("ArchMageGuy_00%02d.png", 31, 60, 0.05f, "ArchMageGuy_0002.png"), "ArchMageGuy_down");

	AnimationCache::getInstance()->addAnimation(createAnimation("ArchMageTwister_00%02d.png", 1, 9, 0.05f), "ArchMageTwister_create");
	AnimationCache::getInstance()->addAnimation(createAnimation("ArchMageTwister_00%02d.png", 9, 15, 0.05f), "ArchMageTwister_twist");
	AnimationCache::getInstance()->addAnimation(createAnimation("ArchMageTwister_00%02d.png", 16, 24, 0.05f), "ArchMageTwister_end");

	AnimationCache::getInstance()->addAnimation(createAnimation("proy_archbolt_00%02d.png", 1, 10, 0.05f), "proy_archbolt");


	AnimationCache::getInstance()->addAnimation(createAnimation("reinforce_B0_0%03d.png", 1, 6, 0.05f, "reinforce_B0_0001.png"), "reinforce2_run");
	AnimationCache::getInstance()->addAnimation(createAnimation("reinforce_B0_0%03d.png", 7, 14, 0.05f, "reinforce_B0_0001.png"), "reinforce2_attack");


	AnimationCache::getInstance()->addAnimation(createAnimation("freeze_creep_00%02d.png", 1, 7, 0.05f), "freeze_creep");
	AnimationCache::getInstance()->addAnimation(createAnimation("freeze_creep_00%02d.png", 8, 23, 0.05f), "refreeze_creep");
	AnimationCache::getInstance()->addAnimation(createAnimation("freeze_creepFlying_00%02d.png", 1, 7, 0.05f), "freeze_creepFlying");
	AnimationCache::getInstance()->addAnimation(createAnimation("freeze_creepFlying_00%02d.png", 10, 21, 0.05f), "refreeze_creepFlying");

	AnimationCache::getInstance()->addAnimation(createAnimation("small_freeze_explosion_00%02d.png", 1, 20, 0.05f), "freeze_explosion");

	AnimationCache::getInstance()->addAnimation(createAnimation("fireball_explosion_00%02d.png", 1, 15, 0.06f), "fireBall_explosion");
	AnimationCache::getInstance()->addAnimation(createAnimation("fireball_proyectile_00%02d.png", 1, 5, 0.05f), "fireBall_fire");
	AnimationCache::getInstance()->addAnimation(createAnimation("fireball_particle_00%02d.png", 1, 4, 0.05f), "fireBall_smoke");

	AnimationCache::getInstance()->addAnimation(createAnimation("effect_sellSmoke_00%02d.png", 1, 11, 0.05f), "sellSmoke");
	AnimationCache::getInstance()->addAnimation(createAnimation("effect_buildSmoke_00%02d.png", 1, 10, 0.05f), "build_smoke");

	AnimationCache::getInstance()->addAnimation(createAnimation("tower_artillery_lvl1_layer1_00%02d.png", 1, 35, 0.05f), "level1_Artillery_towerBase");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_artillery_lvl2_layer1_00%02d.png", 1, 35, 0.05f), "level2_Artillery_towerBase");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_artillery_lvl3_layer1_00%02d.png", 1, 35, 0.05f), "level3_Artillery_towerBase");

	AnimationCache::getInstance()->addAnimation(createAnimation("tower_artillery_lvl1_layer2_00%02d.png", 1, 17, 0.05f), "level1_Artillery_leftShooter_hold");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_artillery_lvl2_layer2_00%02d.png", 1, 17, 0.05f), "level2_Artillery_leftShooter_hold");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_artillery_lvl3_layer2_00%02d.png", 1, 17, 0.05f), "level3_Artillery_leftShooter_hold");

	AnimationCache::getInstance()->addAnimation(createAnimation("tower_artillery_lvl1_layer3_00%02d.png", 1, 35, 0.05f, "tower_artillery_lvl1_layer3_0001.png"), "level1_Artillery_rightShooter");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_artillery_lvl2_layer3_00%02d.png", 1, 35, 0.05f, "tower_artillery_lvl2_layer3_0001.png"), "level2_Artillery_rightShooter");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_artillery_lvl3_layer3_00%02d.png", 1, 35, 0.05f, "tower_artillery_lvl3_layer3_0001.png"), "level3_Artillery_rightShooter");

	AnimationCache::getInstance()->addAnimation(createAnimation("tower_artillery_lvl1_layer4_00%02d.png", 1, 35, 0.05f), "level1_Artillery_barrel");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_artillery_lvl2_layer4_00%02d.png", 1, 35, 0.05f), "level2_Artillery_barrel");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_artillery_lvl3_layer4_00%02d.png", 1, 35, 0.05f), "level3_Artillery_barrel");

	AnimationCache::getInstance()->addAnimation(createAnimation("tower_artillery_lvl1_layer5_00%02d.png", 1, 35, 0.05f), "level1_Artillery_smoke");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_artillery_lvl2_layer5_00%02d.png", 1, 35, 0.05f), "level2_Artillery_smoke");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_artillery_lvl3_layer5_00%02d.png", 1, 35, 0.05f), "level3_Artillery_smoke");

	AnimationCache::getInstance()->addAnimation(createAnimation("tower_artillery_lvl1_layer6_00%02d.png", 18, 35, 0.05f, "tower_artillery_lvl1_layer6_0001.png"), "level1_Artillery_c4");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_artillery_lvl2_layer6_00%02d.png", 18, 35, 0.05f, "tower_artillery_lvl2_layer6_0001.png"), "level2_Artillery_c4");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_artillery_lvl3_layer6_00%02d.png", 18, 35, 0.05f, "tower_artillery_lvl3_layer6_0001.png"), "level3_Artillery_c4");

	AnimationCache::getInstance()->addAnimation(createAnimation("tower_artillery_lvl1_layer2_00%02d.png", 18, 35, 0.05f, "tower_artillery_lvl1_layer2_0001.png"), "level1_Artillery_leftShooter_throw");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_artillery_lvl2_layer2_00%02d.png", 18, 35, 0.05f, "tower_artillery_lvl2_layer2_0001.png"), "level2_Artillery_leftShooter_throw");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_artillery_lvl3_layer2_00%02d.png", 18, 35, 0.05f, "tower_artillery_lvl3_layer2_0001.png"), "level3_Artillery_leftShooter_throw");

	AnimationCache::getInstance()->addAnimation(createAnimation("explosion_air_00%02d.png", 1, 17, 0.05f), "bomb");


	AnimationCache::getInstance()->addAnimation(createAnimation("soldier_lvl1_00%02d.png", 2, 6, 0.05f), "level1_barracksSoilder_run");
	AnimationCache::getInstance()->addAnimation(createAnimation("soldier_lvl2_00%02d.png", 2, 6, 0.05f), "level2_barracksSoilder_run");
	AnimationCache::getInstance()->addAnimation(createAnimation("soldier_lvl3_00%02d.png", 2, 6, 0.05f), "level3_barracksSoilder_run");
	AnimationCache::getInstance()->addAnimation(createAnimation("Soldier_Assassin_00%02d.png", 2, 6, 0.05f), "Assassin_run");

	AnimationCache::getInstance()->addAnimation(createAnimation("soldier_lvl1_00%02d.png", 7, 17, 0.05f), "level1_barracksSoilder_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("soldier_lvl2_00%02d.png", 7, 17, 0.05f), "level2_barracksSoilder_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("soldier_lvl3_00%02d.png", 7, 17, 0.05f), "level3_barracksSoilder_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("Soldier_Assassin_00%02d.png", 7, 17, 0.05f), "Assassin_attack");

	AnimationCache::getInstance()->addAnimation(createAnimation("soldier_lvl1_00%02d.png", 18, 23, 0.05f), "level1_barracksSoilder_dead");
	AnimationCache::getInstance()->addAnimation(createAnimation("soldier_lvl2_00%02d.png", 18, 23, 0.05f), "level2_barracksSoilder_dead");
	AnimationCache::getInstance()->addAnimation(createAnimation("soldier_lvl3_00%02d.png", 18, 23, 0.05f), "level3_barracksSoilder_dead");
	AnimationCache::getInstance()->addAnimation(createAnimation("Soldier_Assassin_00%02d.png", 69, 76, 0.05f), "Assassin_dead");

	AnimationCache::getInstance()->addAnimation(createAnimation("Soldier_Assassin_00%02d.png", 19, 43, 0.05f), "Assassin_skill1");
	AnimationCache::getInstance()->addAnimation(createAnimation("Soldier_Assassin_00%02d.png", 44, 69, 0.05f), "Assassin_skill2");

	AnimationCache::getInstance()->addAnimation(createAnimation("Templar_00%02d.png", 3, 7, 0.1f), "Templar_run");
	AnimationCache::getInstance()->addAnimation(createAnimation("Templar_00%02d.png", 7, 36, 0.05f), "Templar_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("Templar_00%02d.png", 36, 60, 0.05f), "Templar_skill2");
	AnimationCache::getInstance()->addAnimation(createAnimation("Templar_00%02d.png", 61, 94, 0.05f), "Templar_skill1");
	AnimationCache::getInstance()->addAnimation(createAnimation("Templar_00%02d.png", 94, 103, 0.05f), "Templar_dead");


	AnimationCache::getInstance()->addAnimation(createAnimation("tower_barracks_lvl1_layer2_00%02d.png", 1, 25, 0.05f), "level1_barracks_opendoor");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_barracks_lvl2_layer2_00%02d.png", 1, 25, 0.05f), "level2_barracks_opendoor");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_barracks_lvl3_layer2_00%02d.png", 1, 25, 0.05f), "level3_barracks_opendoor");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_assasins_layer2_00%02d.png", 1, 24, 0.05f), "level4_barracks_opendoor");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_templars_layer2_00%02d.png", 1, 4, 0.05f), "level5_barracks_opendoor");

	AnimationCache::getInstance()->addAnimation(createAnimation("TemplarTower_Fire_00%02d.png", 1, 11, 0.05f), "level5_barracks_fire");


	AnimationCache::getInstance()->addAnimation(createAnimation("NecomancerMage_00%02d.png", 32, 62, 0.05f, "NecomancerMage_0002.png"), "NecomancerMage_shoot_up");
	AnimationCache::getInstance()->addAnimation(createAnimation("NecomancerMage_00%02d.png", 3, 31, 0.05f, "NecomancerMage_0003.png"), "NecomancerMage_shoot_down");

	AnimationCache::getInstance()->addAnimation(createAnimation("NecromancerDeathKnight_00%02d.png", 1, 6, 0.05f, "NecromancerDeathKnight_0001.png"), "DK_run");
	AnimationCache::getInstance()->addAnimation(createAnimation("NecromancerDeathKnight_00%02d.png", 7, 23, 0.05f, "NecromancerDeathKnight_0001.png"), "DK_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("NecromancerDeathKnight_00%02d.png", 25, 46, 0.05f, "NecromancerDeathKnight_0001.png"), "DK_birth");
	AnimationCache::getInstance()->addAnimation(createAnimation("NecromancerDeathKnight_00%02d.png", 48, 63, 0.05f), "DK_death");

	AnimationCache::getInstance()->addAnimation(createAnimation("skeleton_00%02d.png", 36, 54, 0.05f), "skeleton_death");
	AnimationCache::getInstance()->addAnimation(createAnimation("skeleton_00%02d.png", 60, 92, 0.05f, "skeleton_0001.png"), "skeleton_birth");
	AnimationCache::getInstance()->addAnimation(createAnimation("skeleton_00%02d.png", 17, 33, 0.05f), "skeleton_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("skeleton_00%02d.png", 1, 16, 0.05f), "skeleton_run");

	AnimationCache::getInstance()->addAnimation(createAnimation("NecromancerTowerEffect_00%02d.png", 1, 15, 0.05f), "NecromancerTower_Effect");

	AnimationCache::getInstance()->addAnimation(createAnimation("tower_mechs_layer2_00%02d.png", 1, 45, 0.05f), "MechsTower_layer2");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_mechs_layer3_00%02d.png", 1, 8, 0.05f), "MechsTower_layer3");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_mechs_layer4_00%02d.png", 1, 8, 0.05f), "MechsTower_layer4");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_mechs_layer4_00%02d.png", 1, 27, 0.05f), "MechsTower_layer6");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_mechs_layer7_00%02d.png", 1, 27, 0.05f), "MechsTower_layer7");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_mechs_layer8_00%02d.png", 1, 42, 0.05f), "MechsTower_layer8");
	AnimationCache::getInstance()->addAnimation(createAnimation("tower_mechs_layer9_00%02d.png", 1, 23, 0.05f, "tower_mechs_layer9_0001.png"), "MechsTower_layer9");

	AnimationCache::getInstance()->addAnimation(createAnimationR("tower_mechs_layer3_00%02d.png", 8, 1, 0.05f), "MechsTower_layer3_r");
	AnimationCache::getInstance()->addAnimation(createAnimationR("tower_mechs_layer4_00%02d.png", 8, 1, 0.05f), "MechsTower_layer4_r");

	AnimationCache::getInstance()->addAnimation(createAnimation("Mecha_00%02d.png", 1, 21, 0.03f), "Mecha_walk");
	AnimationCache::getInstance()->addAnimation(createAnimation("Mecha_00%02d.png", 24, 68, 0.03f, "Mecha_0001.png"), "Mecha_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("Mecha_0%03d.png", 75, 105, 0.03f), "Mecha_shoot");
	AnimationCache::getInstance()->addAnimation(createAnimation("Mecha_0%03d.png", 106, 143, 0.03f), "Mecha_shit");


	AnimationCache::getInstance()->addAnimation(createAnimation("CossbowHunter_shooter_0%03d.png", 2, 14, 0.02f, "CossbowHunter_shooter_0001.png"), "CossbowHunter_shooter_down");
	AnimationCache::getInstance()->addAnimation(createAnimation("CossbowHunter_shooter_0%03d.png", 17, 31, 0.02f, "CossbowHunter_shooter_0017.png"), "CossbowHunter_shooter_up");

	AnimationCache::getInstance()->addAnimation(createAnimation("CossbowHunter_shooter_0%03d.png", 56, 71, 0.02f, "CossbowHunter_shooter_0017.png"), "CossbowHunter_shooter_skill_up");
	AnimationCache::getInstance()->addAnimation(createAnimation("CossbowHunter_shooter_0%03d.png", 34, 53, 0.02f), "CossbowHunter_shooter_skill_down");

	AnimationCache::getInstance()->addAnimation(createAnimation("CossbowHunter_hawk_0%03d.png", 1, 145, 0.05f), "CossbowHunter_hawk");


	AnimationCache::getInstance()->addAnimation(createAnimation("TotemTower_EyesDown_0%03d.png", 1, 25, 0.05f), "TotemTower_EyesDown");

	AnimationCache::getInstance()->addAnimation(createAnimation("TotemTower_Shooter_0%03d.png", 1, 16, 0.05f, "TotemTower_Shooter_0001.png"), "TotemTower_Shooter_down");
	AnimationCache::getInstance()->addAnimation(createAnimation("TotemTower_Shooter_0%03d.png", 23, 44, 0.05f, "TotemTower_Shooter_0023.png"), "TotemTower_Shooter_up");

	AnimationCache::getInstance()->addAnimation(createAnimation("TotemTower_RedTotem_0%03d.png", 1, 29, 0.05f), "TotemTower_RedTotem");
	AnimationCache::getInstance()->addAnimation(createAnimation("TotemTower_VioletTotem_0%03d.png", 1, 29, 0.05f), "TotemTower_VioletTotem");

}*/

/*void AnimationManager::init_player()
{
	auto delayTime = DELAY_CHOPCHOP / 7;
	AnimationCache::getInstance()->addAnimation(createAnimation("sprPlayerAttackEast_%01d.png", 0, 6, delayTime), "PlayerBoy_attack_e");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprPlayerAttackNorth_%01d.png", 0, 6, delayTime), "PlayerBoy_attack_n");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprPlayerAttackSouth_%01d.png", 0, 6, delayTime), "PlayerBoy_attack_s");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprPlayerAttackWest_%01d.png", 0, 6, delayTime), "PlayerBoy_attack_w");

	delayTime = DELAY_SUPPERSTORM / 3 / 10;
	AnimationCache::getInstance()->addAnimation(createAnimation("sprPlayerAttackSpecial_%01d.png", 0, 2, delayTime), "PlayerBoy_skill");

	AnimationCache::getInstance()->addAnimation(createAnimation("sprPlayerIdleEast_%01d.png", 0, 1, 0.25f), "PlayerBoy_idle_e");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprPlayerIdleNorth_%01d.png", 0, 1, 0.25f), "PlayerBoy_idle_n");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprPlayerIdleSouth_%01d.png", 0, 1, 0.25f), "PlayerBoy_idle_s");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprPlayerIdleWest_%01d.png", 0, 1, 0.25f), "PlayerBoy_idle_w");

	AnimationCache::getInstance()->addAnimation(createAnimation("sprPlayerStaggerEast_%01d.png", 0, 0, 0.15f), "PlayerBoy_stagger_e");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprPlayerStaggerNorth_%01d.png", 0, 0, 0.15f), "PlayerBoy_stagger_n");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprPlayerStaggerSouth_%01d.png", 0, 0, 0.15f), "PlayerBoy_stagger_s");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprPlayerStaggerWest_%01d.png", 0, 0, 0.15f), "PlayerBoy_stagger_w");

	AnimationCache::getInstance()->addAnimation(createAnimation("sprPlayerWalkEast_%01d.png", 0, 3, 0.15f), "PlayerBoy_runright");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprPlayerWalkNorth_%01d.png", 0, 3, 0.15f), "PlayerBoy_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprPlayerWalkSouth_%01d.png", 0, 3, 0.15f), "PlayerBoy_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprPlayerWalkWest_%01d.png", 0, 3, 0.15f), "PlayerBoy_runleft");

	delayTime = DELAY_CHOPCHOP / 7;
	AnimationCache::getInstance()->addAnimation(createAnimation("sprGirlAttackEast_%01d.png", 0, 6, delayTime), "PlayerGirl_attack_e");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprGirlAttackNorth_%01d.png", 0, 6, delayTime), "PlayerGirl_attack_n");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprGirlAttackSouth_%01d.png", 0, 6, delayTime), "PlayerGirl_attack_s");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprGirlAttackWest_%01d.png", 0, 6, delayTime), "PlayerGirl_attack_w");

	delayTime = DELAY_SUPPERSTORM / 3 / 10;
	AnimationCache::getInstance()->addAnimation(createAnimation("sprGirlAttackSpecial_%01d.png", 0, 2, delayTime), "PlayerGirl_skill");

	AnimationCache::getInstance()->addAnimation(createAnimation("sprGirlIdleEast_%01d.png", 0, 1, 0.25f), "PlayerGirl_idle_e");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprGirlIdleNorth_%01d.png", 0, 1, 0.25f), "PlayerGirl_idle_n");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprGirlIdleSouth_%01d.png", 0, 1, 0.25f), "PlayerGirl_idle_s");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprGirlIdleWest_%01d.png", 0, 1, 0.25f), "PlayerGirl_idle_w");

	AnimationCache::getInstance()->addAnimation(createAnimation("sprGirlStaggerEast_%01d.png", 0, 0, 0.15f), "PlayerGirl_stagger_e");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprGirlStaggerNorth_%01d.png", 0, 0, 0.15f), "PlayerGirl_stagger_n");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprGirlStaggerSouth_%01d.png", 0, 0, 0.15f), "PlayerGirl_stagger_s");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprGirlStaggerWest_%01d.png", 0, 0, 0.15f), "PlayerGirl_stagger_w");

	AnimationCache::getInstance()->addAnimation(createAnimation("sprGirlWalkEast_%01d.png", 0, 3, 0.15f), "PlayerGirl_runright");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprGirlWalkNorth_%01d.png", 0, 3, 0.15f), "PlayerGirl_runup");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprGirlWalkSouth_%01d.png", 0, 3, 0.15f), "PlayerGirl_rundown");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprGirlWalkWest_%01d.png", 0, 3, 0.15f), "PlayerGirl_runleft");


	delayTime = DELAY_CHOPCHOP / 7;
	AnimationCache::getInstance()->addAnimation(createAnimation("sprSwordGoldenEast_%01d.png", 0, 6, delayTime), "SwordGolden_attack_e");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprSwordGoldenNorth_%01d.png", 0, 6, delayTime), "SwordGolden_attack_n");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprSwordGoldenSouth_%01d.png", 0, 6, delayTime), "SwordGolden_attack_s");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprSwordGoldenWest_%01d.png", 0, 6, delayTime), "SwordGolden_attack_w");

	AnimationCache::getInstance()->addAnimation(createAnimation("sprSwordMetalEast_%01d.png", 0, 6, delayTime), "SwordMetal_attack_e");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprSwordMetalNorth_%01d.png", 0, 6, delayTime), "SwordMetal_attack_n");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprSwordMetalSouth_%01d.png", 0, 6, delayTime), "SwordMetal_attack_s");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprSwordMetalWest_%01d.png", 0, 6, delayTime), "SwordMetal_attack_w");

	AnimationCache::getInstance()->addAnimation(createAnimation("sprSwordWoodenEast_%01d.png", 0, 6, delayTime), "SwordWooden_attack_e");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprSwordWoodenNorth_%01d.png", 0, 6, delayTime), "SwordWooden_attack_n");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprSwordWoodenSouth_%01d.png", 0, 6, delayTime), "SwordWooden_attack_s");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprSwordWoodenWest_%01d.png", 0, 6, delayTime), "SwordWooden_attack_w");
}*/

/*void AnimationManager::init_item()
{
	AnimationCache::getInstance()->addAnimation(createAnimation("SmallBlood.png", 0, 0, 1), "SmallHealthPotion_idle");
	AnimationCache::getInstance()->addAnimation(createAnimation("MediumBlood.png", 0, 0, 1), "MediumHealthPotion_idle");
	AnimationCache::getInstance()->addAnimation(createAnimation("BigBlood.png", 0, 0, 1), "LargeHealthPotion_idle");

	AnimationCache::getInstance()->addAnimation(createAnimation("shuriken_%01d.png", 1, 1, 1), "shuriken_1_idle");
	AnimationCache::getInstance()->addAnimation(createAnimation("shuriken_%01d.png", 2, 2, 1), "shuriken_2_idle");
	AnimationCache::getInstance()->addAnimation(createAnimation("shuriken_%01d.png", 3, 3, 1), "shuriken_3_idle");

	AnimationCache::getInstance()->addAnimation(createAnimation("sprPot_%01d.png", 0, 0, 1), "Pot_idle");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprPotSmash_%01d.png", 0, 4, 0.1f), "Pot_open");

	AnimationCache::getInstance()->addAnimation(createAnimation("sprChestSmall_%01d.png", 0, 0, 1), "ChestSmall_idle");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprChestSmall_%01d.png", 1, 3, 0.1f), "ChestSmall_open");

	AnimationCache::getInstance()->addAnimation(createAnimation("sprChestLarge_%01d.png", 0, 0, 1), "ChestLarge_idle");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprChestLarge_%01d.png", 1, 1, 0.1f), "ChestLarge_open");

	AnimationCache::getInstance()->addAnimation(createAnimation("sprCactus_%01d.png", 0, 0, 1), "Cactus_idle");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprBushCutDesert_%01d.png", 0, 4, 0.1f), "Cactus_open");

	AnimationCache::getInstance()->addAnimation(createAnimation("sprBlueGem_%01d.png", 0, 5, 0.1), "BlueGem_idle");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprGreenGem_%01d.png", 0, 5, 0.1), "GreenGem_idle");
	AnimationCache::getInstance()->addAnimation(createAnimation("sprPurpleGem_%01d.png", 0, 5, 0.1), "PurpleGem_idle");

	AnimationCache::getInstance()->addAnimation(createAnimation("sprIconLifePotion_%01d.png", 0, 0, 1), "HealthPotion_idle");
	AnimationCache::getInstance()->addAnimation(createAnimation("key_%01d.png", 0, 0, 1), "Key_idle");

}*/