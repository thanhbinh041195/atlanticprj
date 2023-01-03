#ifndef __LOGO_TEAM_H__
#define __LOGO_TEAM_H__

#include "cocos2d.h"

USING_NS_CC;


class LogoTeam : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();


	virtual bool init();
	CREATE_FUNC(LogoTeam);
};

#endif 