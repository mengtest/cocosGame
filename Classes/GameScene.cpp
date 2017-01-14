#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

GameScene::GameScene() : _targets(NULL), _bullets(NULL) {}

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

GameScene::~GameScene()
{
	if (_targets)
	{
		_targets->release();
		_targets = NULL;
	}
	if (_bullets)
	{
		_bullets->release();
		_bullets = NULL;
	}
}

bool GameScene::init()
{
	if (!LayerColor::init()){
		return false;
	}

	winSize = CCDirector::sharedDirector()->getWinSize();

	initArrays();
	initMenuButton();
	initMap();
	createPlayer();
	createGun();
	
	this->schedule(schedule_selector(GameScene::gameLogic), spawnEnemyFrequency);
	this->schedule(schedule_selector(GameScene::moveEnemies), enemySpeed);
	this->schedule(schedule_selector(GameScene::moveBullets), bulletSpeed);
	this->schedule(schedule_selector(GameScene::update));

	addKeyboardEventListener();
	addAccelarationEventListener();
	
	return true;
}

void GameScene::initArrays() {
	_targets = CCArray::create();
	_targets->retain();
	_bullets = CCArray::create();
	_bullets->retain();
}

void GameScene::initMenuButton() {

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto menuButton = MenuItemImage::create(
		"images/ui/menu_button.png",
		"images/ui/menu_button.png",
		CC_CALLBACK_1(GameScene::menuOpenMenuCallback, this));
	menuButton->setPosition(Vec2(origin.x + visibleSize.width - menuButton->getContentSize().width / 2,
		origin.y + visibleSize.height - menuButton->getContentSize().height / 2));
	auto menu = Menu::create(menuButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 40, "menuButton");
}

void GameScene::initMap() {
	cocos2d::Sprite* back = Sprite::create("images/map/map.png");
	back->setAnchorPoint(Vec2(0, 0));
	back->setPosition(Vec2(0, 0));
	this->addChild(back, 0, "back");

	cocos2d::Sprite* light = Sprite::create("images/map/light_2.png");
	light->setAnchorPoint(Vec2(0, 0));
	light->setPosition(Vec2(0, 0));
	this->addChild(light, 1, "light");

	cocos2d::Sprite* shadow = Sprite::create("images/map/shadow.png");
	shadow->setAnchorPoint(Vec2(0, 0));
	shadow->setPosition(Vec2(0, 0));
	this->addChild(shadow, 10, "shadow");
}

void GameScene::createPlayer() {

	player = Sprite::create("images/player/idle/0001.png");

	int posX = floorToPixelSize(player->getContentSize().width * 2);
	int posY = floorToPixelSize(winSize.height / 2);
	
	player->setPosition(Vec2(posX + 3, posY + 3));

	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(2);

	animFrames.pushBack(SpriteFrame::create("images/player/idle/0001.png", Rect(0, 0, 125, 125)));
	animFrames.pushBack(SpriteFrame::create("images/player/idle/0002.png", Rect(0, 0, 125, 125)));
	animFrames.pushBack(SpriteFrame::create("images/player/idle/0003.png", Rect(0, 0, 125, 125)));
	animFrames.pushBack(SpriteFrame::create("images/player/idle/0004.png", Rect(0, 0, 125, 125)));

	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.14f);
	Animate* animate = Animate::create(animation);

	player->runAction(RepeatForever::create(animate));

	std::string mapKey0 = "player";
	playerMap.insert(mapKey0, player);

	Sprite* hero_shadow = Sprite::create("images/player/hero_shadow.png");
	hero_shadow->setPosition(Vec2(playerMap.at("player")->getPosition().x, playerMap.at("player")->getPosition().y));
	playerMap.insert("hero_shadow", hero_shadow);

	this->addChild(playerMap.at("hero_shadow"), 2, "hero_shadow");
	this->addChild(playerMap.at("player"), 3, "player");
}

void GameScene::createGun() {

	Sprite* gun = Sprite::create("images/player/gun/gun.png");
	gun->setScale(1);
	gun->setPosition(Vec2(playerMap.at("player")->getPosition().x + gunOffsetX, playerMap.at("player")->getPosition().y + gunOffsetY));

	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(2);

	animFrames.pushBack(SpriteFrame::create("images/player/gun/gun.png", Rect(0, 0, 130, 100)));
	animFrames.pushBack(SpriteFrame::create("images/player/gun/gun_2.png", Rect(0, 0, 130, 100)));

	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.28f);
	Animate* animate = Animate::create(animation);

	gun->runAction(RepeatForever::create(animate));
	std::string mapKey0 = "gun";
	playerMap.insert(mapKey0, gun);

	this->addChild(playerMap.at("gun"), 3, "gun");
}

void GameScene::addKeyboardEventListener() {

	auto eventListener = EventListenerKeyboard::create();

	eventListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event){

		Vec2 loc = event->getCurrentTarget()->getPosition();
		switch (keyCode){
		case EventKeyboard::KeyCode::KEY_BACKSPACE:
			Director::getInstance()->end();
			break;
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			openMenu();
			break;
		case EventKeyboard::KeyCode::KEY_SPACE:
			shoot();
			break;
		case EventKeyboard::KeyCode::KEY_A:
			aWasPressed = true;
			aPressed = true;
			dPressed = false;
			break;
		case EventKeyboard::KeyCode::KEY_D:
			dWasPressed = true;
			dPressed = true;
			aPressed = false;
			break;
		case EventKeyboard::KeyCode::KEY_W:
			wWasPressed = true;
			wPressed = true;
			sPressed = false;
			break;
		case EventKeyboard::KeyCode::KEY_S:
			sPressed = true;
			sWasPressed = true;
			wPressed = false;
			break;
		}
	};

	eventListener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event){

		Vec2 loc = event->getCurrentTarget()->getPosition();
		switch (keyCode){
		case EventKeyboard::KeyCode::KEY_A:
			aPressed = false;
			aWasPressed = false;
			if (dWasPressed) {
				dPressed = true;
			}
			break;
		case EventKeyboard::KeyCode::KEY_D:
			dPressed = false;
			dWasPressed = false;
			if (aWasPressed) {
				aPressed = true;
			}
			break;
		case EventKeyboard::KeyCode::KEY_W:
			wPressed = false;
			wWasPressed = false;
			if (sWasPressed) {
				sPressed = true;
			}
			break;
		case EventKeyboard::KeyCode::KEY_S:
			sPressed = false;
			sWasPressed = false;
			if (wWasPressed) {
				wPressed = true;
			}
			break;
		}
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, player);
}

void GameScene::addAccelarationEventListener() {

	Device::setAccelerometerEnabled(true);
	auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(GameScene::onAcceleration, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

void GameScene::onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event){

	Director *director = Director::getInstance();
	float minSize = MIN(winSize.width, winSize.height);
	if (!movementStarted) {
		startYOffset = acc->y;
		startXOffset = acc->x;
	}
	double targetSpeedY = (acc->y - startYOffset) * minSize * 2;
	double targetSpeedX = (acc->x - startXOffset) * minSize * 2;
	eaterPointsPerSecY = (eaterPointsPerSecY * .8f) + (targetSpeedY * .2f);
	eaterPointsPerSecX = (eaterPointsPerSecX * .8f) + (targetSpeedX * .2f);

	movementStarted = true;
}

void GameScene::onTouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event){

	for (auto& touch : touches) {
		if (touch) {
			shoot();
		}
	}
}

void GameScene::shoot() {
	
	CCSprite *bullet = CCSprite::create("images/bullet.png");
	int posX = floorToPixelSize(playerMap.at("gun")->getPositionX() + bulletOffsetX);
	int posY = floorToPixelSize(playerMap.at("gun")->getPositionY() + bulletOffsetY);
	bullet->setPosition(ccp(posX, posY));
	bullet->setTag(2);
	_bullets->addObject(bullet);
	this->addChild(bullet, 2);

	auto gunFire = CCSprite::create("images/player/gun/fire.png");
	gunFire->setPosition(playerMap.at("gun")->getPosition().x + gunFireOffsetX, playerMap.at("gun")->getPosition().y + gunFireOffsetY);
	this->addChild(gunFire, 2, "gunFire");
	auto action = Sequence::create(DelayTime::create(0.07), CallFunc::create(std::bind(&GameScene::removeCorpse, this, gunFire)), NULL);
	this->runAction(action);
}

void GameScene::addMonster(){

	Sprite *monster = Sprite::create("images/mobs/demon.png");
	Sprite *monster_shadow = Sprite::create("images/mobs/demon_shadow.png");
	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(2);
	animFrames.pushBack(SpriteFrame::create("images/mobs/walk/1.png", Rect(0, 0, 125, 125)));
	animFrames.pushBack(SpriteFrame::create("images/mobs/walk/2.png", Rect(0, 0, 125, 125)));
	animFrames.pushBack(SpriteFrame::create("images/mobs/walk/3.png", Rect(0, 0, 125, 125)));
	animFrames.pushBack(SpriteFrame::create("images/mobs/walk/4.png", Rect(0, 0, 125, 125)));
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.28f);
	Animate* animate = Animate::create(animation);
	monster -> runAction(RepeatForever::create(animate));
	int monsterHeight = monster->getContentSize().height;
	int monsterWidth = monster->getContentSize().width;
	int minY = monsterHeight / 2;
	int maxY = winSize.height - monsterHeight / 2;
	int rangeY = maxY - minY;
	int actualY = floorToPixelSize((rand() % rangeY) + minY);
	actualY += 2;
	int actualX = floorToPixelSize(winSize.width + monsterWidth / 2);
	actualX += 2;
	monster->setPosition(Vec2(winSize.width + monsterWidth / 2, actualY));
	monster_shadow->setPosition(Vec2(monster->getPositionX(), monster->getPositionY()));
	monster->setTag(1);
	Monster *monsterObj = new Monster();
	monsterObj->spritesMap.insert("monster", monster);
	monsterObj->spritesMap.insert("monster_shadow", monster_shadow);
	_targets->addObject(monsterObj);
	this->addChild(monster_shadow, 1);
	this->addChild(monster, 1);
}

void GameScene::gameLogic(float dt){
	addMonster();
}

void GameScene::moveEnemies(float dt){
	CCObject* jt = NULL;
	CCARRAY_FOREACH(_targets, jt)
	{
		Monster *currentMonster = dynamic_cast<Monster*>(jt);	
		for ( auto key : currentMonster->spritesMap.keys() ) {
			CCSprite *target = currentMonster->spritesMap.at(key);
			target->setPositionX(target->getPosition().x - pixelSize);
		}
	}
}

void GameScene::moveBullets(float dt){
	CCObject* jt = NULL;
	CCARRAY_FOREACH(_bullets, jt)
	{
		CCSprite *target = dynamic_cast<CCSprite*>(jt);
		target->setPositionX(target->getPosition().x + pixelSize);
	}
}

void GameScene::movePlayer(float dt) {
	int newX = 0;
	int newY = 0;
	if (movementStarted) {
		float diffY = (eaterPointsPerSecY * dt)*1.3f;
		float diffX = (eaterPointsPerSecX * dt)*1.3f;
		int intDiffX = 0;
		int intDiffY = 0;
		if (diffX < 0) {
			intDiffX = int(diffX - 4);
		}
		else {
			intDiffX = int(diffX + 4);
		}
		intDiffX = floorToPixelSize(intDiffX);
		if (diffY < 0) {
			intDiffY = int(diffY - 4);
		}
		else {
			intDiffY = int(diffY + 4);
		}
		intDiffY = floorToPixelSize(intDiffY);
		newX = player->getPosition().x + intDiffX;
		newY = player->getPosition().y + intDiffY;
	}
	if (wPressed) {
		newY = player->getPosition().y + pixelSize;
	}
	if (sPressed) {
		newY = player->getPosition().y - pixelSize;
	}
	if (dPressed) {
		newX = player->getPosition().x + pixelSize;
	}
	if (aPressed) {
		newX = player->getPosition().x - pixelSize;
	}

	if (newY < player->getContentSize().height / 2 || newY > winSize.height - player->getContentSize().height / 2) {
		newY = player->getPosition().y;
	}
	if (newX < player->getContentSize().width / 2 || newX > winSize.width - player->getContentSize().width / 2) {
		newX = player->getPosition().x;
	}

	playerMap.at("player")->setPosition(newX, newY);
	playerMap.at("gun")->setPosition(newX + gunOffsetX, newY + gunOffsetY);
	playerMap.at("hero_shadow")->setPosition(newX, newY);
	if (this->getChildByName("gunFire")) {
		this->getChildByName("gunFire")->setPosition(playerMap.at("gun")->getPosition().x + gunFireOffsetX, playerMap.at("gun")->getPosition().y + gunFireOffsetY);
	}
}

void GameScene::checkIntersections() {

	CCArray *bulletsToDelete = CCArray::create();
	bulletsToDelete->retain();
	CCArray* targetsToDelete = CCArray::create();
	targetsToDelete->retain();
	CCObject* it = NULL;
	CCObject* jt = NULL;

	CCARRAY_FOREACH(_bullets, it)
	{
		CCSprite *projectile = dynamic_cast<CCSprite*>(it);
		CCRect projectileRect = CCRectMake(
			projectile->getPosition().x - (projectile->getContentSize().width / 2),
			projectile->getPosition().y - (projectile->getContentSize().height / 2),
			projectile->getContentSize().width,
			projectile->getContentSize().height);

		CCARRAY_FOREACH(_targets, jt)
		{
			Monster *currentMonster = dynamic_cast<Monster*>(jt);
			CCSprite *target = currentMonster->spritesMap.at("monster");
			CCRect targetRect = CCRectMake(
				target->getPosition().x - (target->getContentSize().width / 2) + currentMonster->hitBoxXOffset,
				target->getPosition().y - (target->getContentSize().height / 2) + currentMonster->hitBoxYOffset,
				target->getContentSize().width - currentMonster->hitBoxXOffset,
				target->getContentSize().height - 2 * currentMonster->hitBoxYOffset);

			if (projectileRect.intersectsRect(targetRect))
			{
				targetsToDelete->addObject(currentMonster);
				bulletsToDelete->addObject(projectile);
			}
		}
	}

	CCARRAY_FOREACH(targetsToDelete, jt)
	{
		Monster *currentMonster = dynamic_cast<Monster*>(jt);
		currentMonster->hitsToShot--;
		CCSprite *target = currentMonster->spritesMap.at("monster");
		if (currentMonster->hitsToShot < 1) {
			CCSprite *monster_shadow = currentMonster->spritesMap.at("monster_shadow");

			for (auto key : currentMonster->spritesMap.keys()) {
				auto action = Sequence::create(DelayTime::create(2), CallFunc::create(std::bind(&GameScene::removeCorpse, this, currentMonster->spritesMap.at(key))), NULL);
				this->runAction(action);
			}

			_targets->removeObject(currentMonster);

			target->setTexture(currentMonster->getDeadTexture());

			target->stopAllActions();

			runFadeActionForSprite(1.0f, target);
			runFadeActionForSprite(1.0f, monster_shadow);

		}
		else {
			target->setColor(ccc3(0, 0, 0));
			auto action = Sequence::create(DelayTime::create(0.1), CallFunc::create(std::bind(&GameScene::restoreEnemy, this, target)), NULL);
			this->runAction(action);
		}
	}

	CCARRAY_FOREACH(bulletsToDelete, it)
	{
		CCSprite* projectile = dynamic_cast<CCSprite*>(it);
		_bullets->removeObject(projectile);
		this->removeChild(projectile, true);
	}

	bulletsToDelete->release();
	targetsToDelete->release();

	CCARRAY_FOREACH(_targets, it) {
		Monster *currentMonster = dynamic_cast<Monster*>(it);
		CCSprite *target = currentMonster->spritesMap.at("monster");
		if (currentMonster->spritesMap.at("monster")->getPositionX() < 0) {
			CCSprite *monster_shadow = currentMonster->spritesMap.at("monster_shadow");

			for (auto key : currentMonster->spritesMap.keys()) {
				removeCorpse(currentMonster->spritesMap.at(key));
			}

			_targets->removeObject(currentMonster);
		}
	}

	CCARRAY_FOREACH(_bullets, it) {
		Sprite *bullet = dynamic_cast<CCSprite*>(it);
		if (bullet->getPositionX() > winSize.width + bullet->getContentSize().width) {
			_bullets->removeObject(bullet);
			this->removeChild(bullet);
		}
	}
}

void GameScene::update(float dt)
{
	checkTouch();
	movePlayer(dt);
	checkIntersections();
}

void GameScene::restoreEnemy(Sprite* target) {
	target->setColor(ccc3(255, 255, 255));
}

void GameScene::removeCorpse(Sprite* target) {
	this->removeChild(target, true);
}

void GameScene::menuOpenMenuCallback(Ref* pSender) {
	openMenu();
}

void GameScene::checkTouch() {

	if (!this->isTouchEnabled()) {
		this->setTouchEnabled(true);
		this->setKeyboardEnabled(true);
		this->_eventDispatcher->resumeEventListenersForTarget(this->getChildByName("menuButton"));
		this->_eventDispatcher->resumeEventListenersForTarget(this->getChildByName("player"));
		removeChildByTag(PAUSE_SCENE_TAG);
	}
}

void GameScene::openMenu() {

	Director::getInstance()->pause();
	this->setTouchEnabled(false);
	this->setKeyboardEnabled(false);
	this->_eventDispatcher->pauseEventListenersForTarget(this->getChildByName("menuButton"));
	this->_eventDispatcher->pauseEventListenersForTarget(this->getChildByName("player"));
	auto scene = PauseScene::createScene();
	scene->setTag(PAUSE_SCENE_TAG);
	addChild(scene, 50);
}

void GameScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

}

int GameScene::floorToPixelSize(int number) {
	int res = number / pixelSize;
	return res * pixelSize;
}

void GameScene::runFadeActionForSprite(float delay, Sprite* sprite) {
	auto fadeOutAction = FadeOut::create(delay);
	auto fadeOut = Sequence::create(DelayTime::create(delay), fadeOutAction, NULL);
	sprite->runAction(fadeOut);
}