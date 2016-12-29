#pragma once

#include "cocos2d.h"
#include "HelloWorldScene.h"
class MenuScreen : public cocos2d::LayerColor
{
public:
	MenuScreen();
	~MenuScreen();

	CREATE_FUNC(MenuScreen);

	static cocos2d::Scene* createScene();

	virtual bool init() override;

	void startNewGame(cocos2d::Ref* pSender);
	void menuCloseCallback(Ref* pSender);

	void close();
};

