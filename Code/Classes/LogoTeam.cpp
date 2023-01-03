#include "LogoTeam.h"
#include "LogoScene.h"


Scene* LogoTeam::createScene()
{
	//auto myLabel = Label::createWithTTF("My Label Text", "Marker Felt.ttf", 16);

	auto scene = Scene::create();
	auto layer = LogoTeam::create();
	scene->addChild(layer);
	return scene;
}

bool LogoTeam::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto screenSize = Director::getInstance()->getVisibleSize();
	auto back3 = Sprite::create("logo1.png");
	back3->setContentSize(back3->getContentSize()*0.4);
	back3->setPosition(visibleSize / 2);
	this->addChild(back3, 0);
	auto gotoNext = CallFunc::create([]() {
		Director::getInstance()->replaceScene(TransitionProgressOutIn::create(1,LogoScene::createScene()));
	});
	auto sequence = Sequence::create(DelayTime::create(2), gotoNext, nullptr);

	runAction(sequence);
	return true;
}



