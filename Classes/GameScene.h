#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#define PAUSE_SCENE_TAG 99

#include "cocos2d.h"
#include "MainMenuScene.h"
#include "PauseScene.h"

class GameScene : public cocos2d::LayerColor
{
public:
	GameScene();
	~GameScene();
    static cocos2d::Scene* createScene();
	cocos2d::Label* acceleration;
	cocos2d::Label* label;
	cocos2d::Sprite* player;
	float eaterPointsPerSecX = 0;
	float eaterPointsPerSecY = 0;
	float startYOffset = 0;
	float startXOffset = 0;
	bool movementStarted = false;

	bool wPressed = false;
	bool sPressed = false;
	bool aPressed = false;
	bool dPressed = false;

	float spawnEnemyFrequency = 0.4;

	int playerSpeed = 3;

    virtual bool init();

	//void realInit(cocos2d::CCNode*);
	void addMonster();
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& pTouches, cocos2d::Event *pEvent);
	void gameLogic(float);
	void update(float);
	void checkTouch();
	void openMenu();
	void spriteMoveFinished(cocos2d::CCNode*);
	void onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event);
	void shoot();
	void addKeyboardEventListener();
	void addAccelarationEventListener();
	void initArrays();
	void initMenuButton();
	void createPlayer();
    
    // a selector callback
	void menuOpenMenuCallback(cocos2d::Ref* pSender);
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
	CREATE_FUNC(GameScene);
protected:
	cocos2d::CCArray *_targets;
	cocos2d::CCArray *_projectiles;
};

#endif // __GAME_SCENE_H__
