#pragma once
#pragma once

#include "cocos2d.h"
#include "GameScene.h"
class PauseScene : public cocos2d::LayerColor
{
public:
	PauseScene();
	~PauseScene();

	CREATE_FUNC(PauseScene);

	cocos2d::Sprite* darkness;
	float fadeTime = 1.0f;
	static cocos2d::Scene* createScene();

	virtual bool init() override;
	void innerGoToMainMenu();
	void menuCloseCallback(Ref* pSender);
	void goToMainMenu(Ref* pSender);

	void resumeGame();
};

