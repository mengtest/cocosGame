#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

GameScene::GameScene() : _targets(NULL), _bullets(NULL), _loot(NULL) {}

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
	if (_loot)
	{
		_loot->release();
		_loot = NULL;
	}
}

bool GameScene::init()
{
	if (!LayerColor::init()){
		return false;
	}

	winSize = Director::getInstance()->getWinSize();

	initArrays();
	initUI();
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
	_loot = CCArray::create();
	_loot->retain();
}

void GameScene::initUI() {

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto menuButton = MenuItemImage::create(
		"images/ui/menu_button.png",
		"images/ui/menu_button_2.png",
		CC_CALLBACK_1(GameScene::menuOpenMenuCallback, this));
	menuButton->setPosition(Vec2(origin.x + visibleSize.width - menuButton->getContentSize().width / 2,
		origin.y + visibleSize.height - menuButton->getContentSize().height / 2));
	auto menu = Menu::create(menuButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, MENU_BUTTON_TAG, "menuButton");

	cocos2d::Sprite* coinsSpite = Sprite::create("images/ui/coin_icon.png");
	coinsSpite->setAnchorPoint(Vec2(0, 0));
	coinsSpite->setPosition(Vec2(0, winSize.height - coinsSpite->getContentSize().height));
	this->addChild(coinsSpite, 40, "coinSprite");

	coinsLabel = Label::create("1200", "fonts/Marker Felt.ttf", 50);
	coinsLabel->setAnchorPoint(Vec2(0, 0));
	coinsLabel->setPosition(Point(100, winSize.height - 96));
	char buffer1[100];
	sprintf(buffer1, "%d", coins);
	coinsLabel->setString(buffer1);
	this->addChild(coinsLabel, 40);

	//auto visibleSize = Director::getInstance()->getVisibleSize();
	//Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto switchAmmoButton = MenuItemImage::create(
		"images/ui/switch_ammo_button.png",
		"images/ui/switch_ammo_button.png",
		CC_CALLBACK_1(GameScene::switchAmmo, this));
	switchAmmoButton->setPosition(Vec2(switchAmmoButton->getContentSize().width / 2,
		switchAmmoButton->getContentSize().height / 2));
	auto menu2 = Menu::create(switchAmmoButton, NULL);
	menu2->setPosition(Vec2::ZERO);
	this->addChild(menu2, MENU_BUTTON_TAG, "switchAmmoButton");
}

void GameScene::switchAmmo(Ref* pSender) {
	if (ammo == 0) {
		playerMap.at("gun")->stopAllActions();
		Vector<SpriteFrame*> animFrames;
		animFrames.reserve(2);

		animFrames.pushBack(SpriteFrame::create("images/player/gun/gun_3.png", Rect(0, 0, 130, 100)));
		animFrames.pushBack(SpriteFrame::create("images/player/gun/gun_4.png", Rect(0, 0, 130, 100)));

		Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.28f);
		Animate* animate = Animate::create(animation);

		playerMap.at("gun")->runAction(RepeatForever::create(animate));
		ammo = 1;
		shootMode = 1;
		blockShootingTime = gunBlockShootingTime;
	}
	else {
		playerMap.at("gun")->stopAllActions();
		Vector<SpriteFrame*> animFrames;
		animFrames.reserve(2);

		animFrames.pushBack(SpriteFrame::create("images/player/gun/gun.png", Rect(0, 0, 130, 100)));
		animFrames.pushBack(SpriteFrame::create("images/player/gun/gun_2.png", Rect(0, 0, 130, 100)));

		Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.28f);
		Animate* animate = Animate::create(animation);

		playerMap.at("gun")->runAction(RepeatForever::create(animate));
		ammo = 0;
		shootMode = 0;
		blockShootingTime = pistolBlockShootingTime;
	}
}

void GameScene::initMap() {
	cocos2d::Sprite* back = Sprite::create("images/map/map.png");
	back->setAnchorPoint(Vec2(0, 0));
	back->setPosition(Vec2(0, 0));
	this->addChild(back, MAP_TAG, "back");

	cocos2d::Sprite* light = Sprite::create("images/map/light_2.png");
	light->setAnchorPoint(Vec2(0, 0));
	light->setPosition(Vec2(0, 0));
	this->addChild(light, MAP_LIGHT_TAG, "light");

	cocos2d::Sprite* shadow = Sprite::create("images/map/shadow.png");
	shadow->setAnchorPoint(Vec2(0, 0));
	shadow->setPosition(Vec2(0, 0));
	this->addChild(shadow, MAP_SHADOW_TAG, "shadow");
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

	this->addChild(playerMap.at("hero_shadow"), PLAYER_SHADOW_TAG, "hero_shadow");
	this->addChild(playerMap.at("player"), PLAYER_TAG, "player");
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

	this->addChild(playerMap.at("gun"), PLAYER_GUN_TAG, "gun");
}

void GameScene::addKeyboardEventListener() {

	auto eventListener = EventListenerKeyboard::create();

	eventListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event){

		Vec2 loc = event->getCurrentTarget()->getPosition();
		switch (keyCode){
		case EventKeyboard::KeyCode::KEY_R:
			switchAmmo(this);
			break;
		case EventKeyboard::KeyCode::KEY_BACKSPACE:
			Director::getInstance()->end();
			break;
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			openMenu();
			break;
		case EventKeyboard::KeyCode::KEY_SPACE:
			firePressed = true;
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
		case EventKeyboard::KeyCode::KEY_SPACE:
			firePressed = false;
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
	if (!movementStarted || acceleratorOffsetUpdateRequired) {
		startYOffset = acc->y;
		startXOffset = acc->x;
		acceleratorOffsetUpdateRequired = false;
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
			firePressed = false;
		}
	}
}

void GameScene::onTouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event){

	for (auto& touch : touches) {
		if (touch) {
			//shoot();
			firePressed = true;
		}
	}
}

void GameScene::shoot() {
	if (!blockShooting) {
		Sprite *bullet = Sprite::create("images/bullet.png");
		if (ammo == 1) {
			bullet->setTexture("images/player/gun/bullet_3.png");
		}
		int posX = floorToPixelSize(playerMap.at("gun")->getPositionX() + bulletOffsetX);
		int posY = floorToPixelSize(playerMap.at("gun")->getPositionY() + bulletOffsetY);
		if (ammo == 1) {
			posX = floorToPixelSize(playerMap.at("gun")->getPositionX() + gunBulletOffsetX);
			posY = floorToPixelSize(playerMap.at("gun")->getPositionY() + gunBulletOffsetY);
		}
		bullet->setPosition(Vec2(posX, posY));
		bullet->setTag(2);
		_bullets->addObject(bullet);
		this->addChild(bullet, BULLET_TAG);

		auto gunFire = Sprite::create("images/player/gun/fire.png");
		if (ammo == 1) {
			gunFire->setTexture("images/player/gun/fire_2.png");
			gunFire->setPosition(playerMap.at("gun")->getPosition().x + gunFireOffsetX, playerMap.at("gun")->getPosition().y + gunFireOffsetY);
			this->addChild(gunFire, GUN_FIRE_TAG, "gunFire");
			auto action = Sequence::create(DelayTime::create(0.05f), CallFunc::create(std::bind(&GameScene::removeCorpse, this, gunFire)), NULL);
			this->runAction(action);
		}
		else {
			gunFire->setPosition(playerMap.at("gun")->getPosition().x + pistolFireOffsetX, playerMap.at("gun")->getPosition().y + pistolFireOffsetY);
			this->addChild(gunFire, GUN_FIRE_TAG, "gunFire");
			auto action = Sequence::create(DelayTime::create(0.08f), CallFunc::create(std::bind(&GameScene::removeCorpse, this, gunFire)), NULL);
			this->runAction(action);
		}
		
		blockShooting = true;
		auto allowShootingAction = Sequence::create(DelayTime::create(blockShootingTime), CallFunc::create(std::bind(&GameScene::allowShooting, this)), NULL);
		this->runAction(allowShootingAction);
	}
}

void GameScene::allowShooting() {
	blockShooting = false;
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
	monster->setPosition(Vec2(actualX, actualY));
	monster_shadow->setPosition(Vec2(monster->getPositionX(), monster->getPositionY()));
	monster->setTag(1);
	Monster *monsterObj = new Monster();
	monsterObj->spritesMap.insert("monster", monster);
	monsterObj->spritesMap.insert("monster_shadow", monster_shadow);
	_targets->addObject(monsterObj);
	this->addChild(monster_shadow, MONSTER_SHADOW_TAG);
	this->addChild(monster, MONSTER_TAG);
}

void GameScene::gameLogic(float dt){
	addMonster();
}

void GameScene::moveEnemies(float dt){
	Ref* jt = NULL;
	CCARRAY_FOREACH(_targets, jt)
	{
		Monster *currentMonster = dynamic_cast<Monster*>(jt);	
		if (!currentMonster->dead) {
			for (auto key : currentMonster->spritesMap.keys()) {
				Sprite *target = currentMonster->spritesMap.at(key);
				target->setPositionX(target->getPosition().x - pixelSize);
			}
		}
	}
}

void GameScene::moveBullets(float dt){
	Ref* jt = NULL;
	CCARRAY_FOREACH(_bullets, jt)
	{
		Sprite *target = dynamic_cast<Sprite*>(jt);
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
		if (ammo == 0) {
			this->getChildByName("gunFire")->setPosition(playerMap.at("gun")->getPosition().x + pistolFireOffsetX, playerMap.at("gun")->getPosition().y + pistolFireOffsetY);
		}
		else {
			this->getChildByName("gunFire")->setPosition(playerMap.at("gun")->getPosition().x + gunFireOffsetX, playerMap.at("gun")->getPosition().y + gunFireOffsetY);
		}
		
	}
}

void GameScene::checkIntersections() {

	__Array *bulletsToDelete = __Array::create();
	bulletsToDelete->retain();
	__Array* targetsToDelete = __Array::create();
	targetsToDelete->retain();
	__Array* lootToDelete = __Array::create();
	lootToDelete->retain();
	Ref* it = NULL;
	Ref* jt = NULL;

	CCARRAY_FOREACH(_bullets, it)
	{
		Sprite *projectile = dynamic_cast<Sprite*>(it);
		Rect projectileRect = Rect(
			projectile->getPosition().x - (projectile->getContentSize().width / 2),
			projectile->getPosition().y - (projectile->getContentSize().height / 2),
			projectile->getContentSize().width,
			projectile->getContentSize().height);

		CCARRAY_FOREACH(_targets, jt)
		{
			Monster *currentMonster = dynamic_cast<Monster*>(jt);
			if (!currentMonster->dead) {
				Sprite *target = currentMonster->spritesMap.at("monster");
				Rect targetRect = Rect(
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
	}

	Rect playerRect = Rect(
		player->getPosition().x - (player->getContentSize().width / 2) + playerLeftPadding,
		player->getPosition().y - (player->getContentSize().height / 2) + playerUpPadding,
		player->getContentSize().width - playerLeftPadding - playerRightPadding,
		player->getContentSize().height - playerUpPadding - playerDownPadding);

	CCARRAY_FOREACH(_loot, it) {
		Sprite *loot = dynamic_cast<Sprite*>(it);
		Rect lootRect = Rect(
			loot->getPosition().x - (loot->getContentSize().width / 2),
			loot->getPosition().y - (loot->getContentSize().height / 2),
			loot->getContentSize().width,
			loot->getContentSize().height);
		if (lootRect.intersectsRect(playerRect))
		{
			lootToDelete->addObject(loot);
		}
	}

	CCARRAY_FOREACH(targetsToDelete, jt)
	{
		Monster *currentMonster = dynamic_cast<Monster*>(jt);
		if (ammo == 0) {
			currentMonster->hitsToShot -= 50;
		}
		else {
			currentMonster->hitsToShot -= 40;
		}
		
		onDamageReceived(currentMonster);
		if (currentMonster->hitsToShot < 1) {
			onKillMonster(currentMonster);
		}
	}

	CCARRAY_FOREACH(bulletsToDelete, it)
	{
		Sprite* bullet = dynamic_cast<Sprite*>(it);
		_bullets->removeObject(bullet);
		this->removeChild(bullet, true);
	}

	CCARRAY_FOREACH(lootToDelete, it)
	{
		Sprite* loot = dynamic_cast<Sprite*>(it);
		auto removeAction = Sequence::create(DelayTime::create(0.3f), CallFunc::create(std::bind(&GameScene::removeCorpse, this, loot)), NULL);
		this->runAction(removeAction);
		_loot->removeObject(loot);
		auto jump = JumpBy::create(0.5, Vec2(0, 0), 140, 1);
		auto seq = Sequence::create(jump, nullptr);
		loot->stopAllActionsByTag(666);
		loot->runAction(seq);

		coins += 100;
		char buffer1[100];
		sprintf(buffer1, "%d", coins);
		coinsLabel->setString(buffer1);
	}

	bulletsToDelete->release();
	targetsToDelete->release();
	lootToDelete->release();

	CCARRAY_FOREACH(_targets, it) {
		Monster *currentMonster = dynamic_cast<Monster*>(it);
		Sprite *target = currentMonster->spritesMap.at("monster");
		if (currentMonster->spritesMap.at("monster")->getPositionX() < 0) {
			Sprite *monster_shadow = currentMonster->spritesMap.at("monster_shadow");
			for (auto key : currentMonster->spritesMap.keys()) {
				removeCorpse(currentMonster->spritesMap.at(key));
			}
			_targets->removeObject(currentMonster);
		}
	}

	CCARRAY_FOREACH(_bullets, it) {
		Sprite *bullet = dynamic_cast<Sprite*>(it);
		if (bullet->getPositionX() > winSize.width + bullet->getContentSize().width) {
			_bullets->removeObject(bullet);
			this->removeChild(bullet);
		}
	}
}

void GameScene::onKillMonster(Monster* currentMonster) {
	Sprite *target = currentMonster->spritesMap.at("monster");
	Sprite *monster_blood = Sprite::create("images/mobs/blood/1.png");
	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(2);
	animFrames.pushBack(SpriteFrame::create("images/mobs/blood/1.png", Rect(0, 0, 175, 125)));
	animFrames.pushBack(SpriteFrame::create("images/mobs/blood/2.png", Rect(0, 0, 175, 125)));
	animFrames.pushBack(SpriteFrame::create("images/mobs/blood/3.png", Rect(0, 0, 175, 125)));
	animFrames.pushBack(SpriteFrame::create("images/mobs/blood/4.png", Rect(0, 0, 175, 125)));
	animFrames.pushBack(SpriteFrame::create("images/mobs/blood/5.png", Rect(0, 0, 175, 125)));
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.14f);
	Animate* animate = Animate::create(animation);
	monster_blood->runAction(Repeat::create(animate, 1));
	monster_blood->setPosition(Vec2(floorToPixelSize(currentMonster->spritesMap.at("monster")->getPositionX() - 50) + 3,
		floorToPixelSize(currentMonster->spritesMap.at("monster")->getPositionY()) + 2));

	currentMonster->spritesMap.insert("monster_blood", monster_blood);

	this->addChild(monster_blood, MONSTER_BLOOD_TAG);

	currentMonster->dead = true;
	auto removeAction = Sequence::create(DelayTime::create(4), CallFunc::create(std::bind(&GameScene::removeDeadMonster, this, currentMonster)), NULL);
	this->runAction(removeAction);
	auto fadeAction = Sequence::create(DelayTime::create(2), CallFunc::create(std::bind(&GameScene::fadeHideMonster, this, currentMonster)), NULL);
	this->runAction(fadeAction);

	Vector<SpriteFrame*> animFrames2;
	animFrames2.reserve(2);
	animFrames2.pushBack(SpriteFrame::create("images/mobs/walk/1.png", Rect(0, 0, 125, 125)));
	animFrames2.pushBack(SpriteFrame::create("images/mobs/walk/2.png", Rect(0, 0, 125, 125)));
	animFrames2.pushBack(SpriteFrame::create("images/mobs/walk/3.png", Rect(0, 0, 125, 125)));
	animFrames2.pushBack(SpriteFrame::create("images/mobs/demon_dead_2.png", Rect(0, 0, 154, 125)));
	Animation* animation2 = Animation::createWithSpriteFrames(animFrames2, 0.14f);
	Animate* animate2 = Animate::create(animation2);
	target->stopAllActions();	
	target->runAction(Repeat::create(animate2, 1));

	if (CCRANDOM_0_1() < dropCoinProbability) {
		dropCoin(currentMonster);
	}
}

void GameScene::dropCoin(Monster* monster) {
	int randomTen = RandomHelper::random_int(1, 10);
	int coinsNumber;
	if (randomTen > 9) {
		coinsNumber = 3;
	}
	else if (randomTen > 6) {
		coinsNumber = 2;
	}
	else {
		coinsNumber = 1;
	}
	
	for (int i = 0; i < coinsNumber; i++) {
		Sprite *coin = Sprite::create("images/coin/1.png");
		int posX = floorToPixelSize(monster->spritesMap.at("monster")->getPositionX() + coinOffsetX);
		int posY = floorToPixelSize(monster->spritesMap.at("monster")->getPositionY() + coinOffsetY);
		coin->setPosition(Vec2(posX, posY));
		coin->setTag(5);
		_loot->addObject(coin);
		this->addChild(coin, COIN_TAG);

		Vector<SpriteFrame*> animFrames;
		animFrames.reserve(2);
		animFrames.pushBack(SpriteFrame::create("images/coin/1.png", Rect(0, 0, 100, 100)));
		animFrames.pushBack(SpriteFrame::create("images/coin/2.png", Rect(0, 0, 100, 100)));
		animFrames.pushBack(SpriteFrame::create("images/coin/3.png", Rect(0, 0, 100, 100)));
		animFrames.pushBack(SpriteFrame::create("images/coin/4.png", Rect(0, 0, 100, 100)));
		animFrames.pushBack(SpriteFrame::create("images/coin/5.png", Rect(0, 0, 100, 100)));
		animFrames.pushBack(SpriteFrame::create("images/coin/6.png", Rect(0, 0, 100, 100)));
		Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.14f);
		Animate* animate = Animate::create(animation);
		coin->runAction(RepeatForever::create(animate));
		int destY = RandomHelper::random_int(-5, 5) * pixelSize;
		int destX = 80 - i * 80;
		if (i == 1) {
			if (coinsNumber == 2) {
				destX -= 80;
			}
			else {
				destY += 50;
			}
		}
		auto jump = JumpBy::create(0.5, Vec2(destX, destY), 90, 1);
		auto seq = Sequence::create(jump, nullptr);
		seq->setTag(666);
		coin->runAction(seq);
	}
}

void GameScene::fadeHideMonster(Monster* monster) {
	for (auto key : monster->spritesMap.keys()) {
		runFadeActionForSprite(1.0f, monster->spritesMap.at(key));
	}
}

void GameScene::update(float dt)
{
	checkTouch();
	movePlayer(dt);
	checkIntersections();

	if (!blockShooting && firePressed && ammo == 1 & shootMode == 1) {
		shoot();
		blockShooting = true;
	}
}

void GameScene::onDamageReceived(Monster* currentMonster) {
	Sprite *target = currentMonster->spritesMap.at("monster");
	target->setColor(Color3B(0, 0, 0));
	auto action = Sequence::create(DelayTime::create(0.07f), CallFunc::create(std::bind(&GameScene::restoreEnemy, this, target)), NULL);
	this->runAction(action);
}

void GameScene::restoreEnemy(Sprite* target) {
	target->setColor(Color3B(255, 255, 255));
}

void GameScene::removeCorpse(Sprite* target) {
	this->removeChild(target, true);
}

void GameScene::removeDeadMonster(Monster* monster) {
	for (auto key : monster->spritesMap.keys()) {
		removeCorpse(monster->spritesMap.at(key));
	}
	_targets->removeObject(monster);
}

void GameScene::menuOpenMenuCallback(Ref* pSender) {
	openMenu();
}

void GameScene::checkTouch() {

	if (!this->isTouchEnabled()) {
		acceleratorOffsetUpdateRequired = true;
		this->setTouchEnabled(true);
		this->_eventDispatcher->resumeEventListenersForTarget(this->getChildByName("menuButton"));
		this->_eventDispatcher->resumeEventListenersForTarget(this->getChildByName("player"));
		removeChildByTag(PAUSE_SCENE_TAG);
	}
}

void GameScene::openMenu() {

	Director::getInstance()->pause();
	this->setTouchEnabled(false);
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