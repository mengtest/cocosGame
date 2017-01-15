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
	Label* coinsLabel;
	Label* label;
	Sprite* player;

	int ammo = 0;
	int shootMode = 0;

	int MENU_BUTTON_TAG = 40;
	int MAP_TAG = 0;
	int MAP_LIGHT_TAG = 1;
	int MAP_SHADOW_TAG = 20;
	int PLAYER_TAG = 17;
	int PLAYER_SHADOW_TAG = 16;
	int PLAYER_GUN_TAG = 17;
	int BULLET_TAG = 13;
	int GUN_FIRE_TAG = 17;
	int MONSTER_TAG = 10;
	int MONSTER_SHADOW_TAG = 10;
	int MONSTER_BLOOD_TAG = 9;
	int COIN_TAG = 9;

	int pixelSize = 5;
	int gunOffsetX = 102;
	int gunOffsetY = 2;
	int gunFireOffsetX = 110;
	int gunFireOffsetY = 10;
	int pistolFireOffsetX = 90;
	int pistolFireOffsetY = 10;
	int bulletOffsetX = 35;
	int bulletOffsetY = 15;
	int gunBulletOffsetX = 55;
	int gunBulletOffsetY = 10;
	int coinOffsetX = 0;
	int coinOffsetY = 0;
	int playerLeftPadding = 60;
	int playerRightPadding = 60;
	int playerUpPadding = 60;
	int playerDownPadding = 20;
	int coins = 0;

	float spawnEnemyFrequency = 1.2;
	float enemySpeed = 0.06f;
	float bulletSpeed = 0.005f;
	float eaterPointsPerSecX = 0;
	float eaterPointsPerSecY = 0;
	float startYOffset = 0;
	float startXOffset = 0;
	float gunBlockShootingTime = 0.13f;
	float pistolBlockShootingTime = 0.18f;
	float blockShootingTime = pistolBlockShootingTime;
	float dropCoinProbability = 0.1f;

	bool firePressed = false;
	bool acceleratorOffsetUpdateRequired = false;
	bool movementStarted = false;
	bool wPressed = false;
	bool sPressed = false;
	bool aPressed = false;
	bool dPressed = false;
	bool wWasPressed = false;
	bool sWasPressed = false;
	bool aWasPressed = false;
	bool dWasPressed = false;
	bool blockShooting = false;

	void addMonster();
	void onTouchesEnded(const std::vector<Touch*>& pTouches, Event *pEvent);
	void onTouchesBegan(const std::vector<Touch*>& pTouches, Event *pEvent);
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
	void initUI();
	void createPlayer();
	void switchAmmo(Ref* pSender);
	void movePlayer(float);
	void createGun();
	void initMap();
	void removeCorpse(Sprite*);
	void removeDeadMonster(Monster*);
	void fadeHideMonster(Monster*);
	void restoreEnemy(Sprite*);
	void onDamageReceived(Monster* monster);
	void runFadeActionForSprite(float delay, Sprite* sprite);
	void menuOpenMenuCallback(Ref* pSender);
	void menuCloseCallback(Ref* pSender);
	void allowShooting();
	void onKillMonster(Monster* monster);
	void dropCoin(Monster* monster);

	int floorToPixelSize(int number);

	virtual bool init();
	CREATE_FUNC(GameScene);
	GameScene();
	~GameScene();
protected:
	__Array *_targets;
	__Array *_bullets;
	__Array *_loot;

	Map<std::string, Sprite*> playerMap;
};

#endif // __GAME_SCENE_H__
