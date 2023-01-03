#ifndef _TESTARROW_H_
#define _TESTARROW_H_

#include "cocos2d.h"
#include "Model.h"

class Arrow : public Model
{
public:
	Arrow(cocos2d::Layer* layer, cocos2d::Sprite *sprite, int owner);
	~Arrow();
	void explode(bool isRight);
	void fire();
};
#endif