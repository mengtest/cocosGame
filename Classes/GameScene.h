#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#define PAUSE_SCENE_TAG 99

#include "cocos2d.h"
#include "MainMenuScene.h"
#include "PauseScene.h"
#include "Monster.h"
#include "Weapon.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;

class GameScene : public LayerColor
{
public:
	ui::Button* uButton;
	static Scene* createScene();
	Size winSize;
	cocos2d::Sprite* darkness;
	Label* acceleration;
	Label* coinsLabel;
	Label* label;
	Sprite* player;
	Weapon* currentWeapon;
	//int ammo = 0;
	//int shootMode = 0;

	int gunIndex = 0;

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
	int RELOAD_BAR_TAG = 18;

	int pixelSize = 5;
	int coinOffsetX = 0;
	int coinOffsetY = 0;
	int playerLeftPadding = 60;
	int playerRightPadding = 60;
	int playerUpPadding = 60;
	int playerDownPadding = 20;
	int coins = 0;
	int coinsIndicatorOffsetX = 201;
	int coinsQuantityOffsetX = 88;
	int levelMonstersQuantity = 50;
	int currentMonstersSendQuantity = 0;
	int currentMonstersInGameQuantity = 0;

	int lifesOffsetX = 35;
	int lifesOffsetY = -40;

	float fadeTime = 1.0f;
	float spawnEnemyFrequency = 1.2;
	float enemySpeed = 0.06f;
	float bulletSpeed = 0.005f;
	float eaterPointsPerSecX = 0;
	float eaterPointsPerSecY = 0;
	float startYOffset = 0;
	float startXOffset = 0;
	float delayBeforeShowLevelComplete = 1.0f;
	float blockShootingTime = 0;
	float dropCoinProbability = 0.04f;

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
	bool reloading = false;
	bool pause = true;
	bool levelComplete = false;
	bool levelCompleteAndMessageShown = false;
	bool levelCompleteTapped = false;

	//void onEnter();
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
	void showReloadBar(float);
	void hideReloadBar();
	void initMap();
	void initWeapons();
	void removeCorpse(Sprite*);
	void setSpriteVisibleTrue(Monster*);
	void removeDeadMonster(Monster*);
	void fadeHideMonster(Monster*);
	void restoreEnemy(Sprite*);
	void onDamageReceived(Monster* monster);
	void runFadeActionForSprite(float delay, Sprite* sprite);
	void menuOpenMenuCallback(Ref* pSender);
	void menuCloseCallback(Ref* pSender);
	void allowShooting();
	void onKillMonster(Monster* monster);
	void dropCoin(Monster* monster, int);
	void showLevelComplete();
	void onLevelCompleteTap();
	void goToMainMenu();

	int floorToPixelSize(int number);

	virtual bool init();
	CREATE_FUNC(GameScene);
	GameScene();
	~GameScene();
protected:
	__Array *_weapons;
	__Array *_targets;
	__Array *_bullets;
	__Array *_loot;

	Map<std::string, Sprite*> playerMap;
};

#endif // __GAME_SCENE_H__
