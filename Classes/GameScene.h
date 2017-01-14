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
	ui::Button* uButton;
	static Scene* createScene();
	Size winSize;
	Label* acceleration;
	Label* label;
	Sprite* player;

	int pixelSize = 5;
	int gunOffsetX = 102;
	int gunOffsetY = 2;
	int gunFireOffsetX = 100;
	int gunFireOffsetY = 10;
	int bulletOffsetX = 35;
	int bulletOffsetY = 15;

	float spawnEnemyFrequency = 1;
	float enemySpeed = 0.05;
	float bulletSpeed = 0.005;
	float eaterPointsPerSecX = 0;
	float eaterPointsPerSecY = 0;
	float startYOffset = 0;
	float startXOffset = 0;

	bool movementStarted = false;
	bool wPressed = false;
	bool sPressed = false;
	bool aPressed = false;
	bool dPressed = false;
	bool wWasPressed = false;
	bool sWasPressed = false;
	bool aWasPressed = false;
	bool dWasPressed = false;

	void addMonster();
	void onTouchesEnded(const std::vector<Touch*>& pTouches, Event *pEvent);
	void gameLogic(float);
	void checkIntersections();
	void moveEnemies(float);
	void moveBullets(float);
	void update(float);
	void checkTouch();
	void openMenu();
	void onAcceleration(Acceleration *acc, Event *event);
	void shoot();
	void addKeyboardEventListener();
	void addAccelarationEventListener();
	void initArrays();
	void initMenuButton();
	void createPlayer();
	void movePlayer(float);
	void createGun();
	void initMap();
	void removeCorpse(Sprite*);
	void restoreEnemy(Sprite*);
	void runFadeActionForSprite(float delay, Sprite* sprite);
	void menuOpenMenuCallback(Ref* pSender);
	void menuCloseCallback(Ref* pSender);

	int floorToPixelSize(int number);

	virtual bool init();
	CREATE_FUNC(GameScene);
	GameScene();
	~GameScene();
protected:
	CCArray *_targets;
	CCArray *_bullets;

	Map<std::string, Sprite*> playerMap;
};

#endif // __GAME_SCENE_H__
