#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#define PAUSE_SCENE_TAG 99

#include "cocos2d.h"
#include "MainMenuScene.h"
#include "PauseScene.h"
#include "Monster.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;

class GameScene : public LayerColor
{
public:
	GameScene();
	~GameScene();
	static Scene* createScene();
	Label* acceleration;
	Label* label;
	Sprite* player;
	int pixelSize = 5;
	float eaterPointsPerSecX = 0;
	float eaterPointsPerSecY = 0;
	float startYOffset = 0;
	float startXOffset = 0;
	bool movementStarted = false;

	int gunOffsetX = 102;
	int gunOffsetY = 2;
	int gunFireOffsetX = 100;
	int gunFireOffsetY = 10;

	int bulletOffsetX = 35;
	int bulletOffsetY = 15;

	bool wPressed = false;
	bool sPressed = false;
	bool aPressed = false;
	bool dPressed = false;

	float spawnEnemyFrequency = 1;

	float enemySpeed = 0.05;
	float bulletSpeed = 0.005;

	/*int playerSpeed = 4;*/

	virtual bool init();

	//void realInit(cocos2d::CCNode*);
	void addMonster();
	void onTouchesEnded(const std::vector<Touch*>& pTouches, Event *pEvent);
	void gameLogic(float);
	void moveEnemies(float);
	void moveBullets(float);
	void update(float);
	void checkTouch();
	void openMenu();
	void spriteMoveFinished(CCNode*);
	void onAcceleration(Acceleration *acc, Event *event);
	void shoot();
	void addKeyboardEventListener();
	void addAccelarationEventListener();
	void initArrays();
	void initMenuButton();
	void createPlayer();
	void createGun();
	void initMap();
	void removeCorpse(Sprite*);
	void restoreEnemy(Sprite*);

	ui::Button* uButton;

	// a selector callback
	void menuOpenMenuCallback(Ref* pSender);
	void menuCloseCallback(Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);
protected:
	CCArray *_targets;
	CCArray *_deadTargets;
	CCArray *_projectiles;

	Map<std::string, Sprite*> playerMap;
};

#endif // __GAME_SCENE_H__
