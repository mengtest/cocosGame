#pragma once

#include "cocos2d.h"
#include "GameScene.h"
class MainMenuScene : public cocos2d::LayerColor
{
public:
	MainMenuScene();
	~MainMenuScene();

	CREATE_FUNC(MainMenuScene);
	
	bool startingGame = false;
	bool endingGame = false;
	float fadeTime = 1.0f;
	cocos2d::Sprite* back2;
	static cocos2d::Scene* createScene();

	virtual bool init() override;

	void startNewGame(cocos2d::Ref* pSender);
	void menuCloseCallback(Ref* pSender);
	void innerStartNewGame();
	void close();
	void innerClose();
};

